#include "board.h"
//#include "stmflash.h"
//ˮλĬ�ϵ�ַ02  ������9600  ����0~49��

const static char sign[]="[����]";
#if 1
//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //���������õ���������buf����
static bool alarmFLag=false;

coverStru cover[COVER_485_NUM];

//float temp[TEMPHUM_485_NUM];
//float hum[TEMPHUM_485_NUM]; 
//������ڷ��� 
static void coverUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.cover[num].useUartNum,buf, len);
}
//����485���ǵ���Ӧ״̬
int coverState(int i)
{
		return cover[i].respStat;
}


//���Ƕ�ȡֵ����ֵ�Ƚϲ�����flag
//��������������ͬ  ������ֵΪ��ֵ  ��Ҫ�ж��Ƿ����
 void coverCheckSetFlag(int num)
{
	  alarmFLag=false;
		if(sheet.modbusCover[num].inclineUpLimit!=0){//����
				if(cover[num].incline==1){
					inpoutpFlag.modbusCover[num].inclineUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].inclineLowLimit!=0){//����
				if(cover[num].incline==0){
					inpoutpFlag.modbusCover[num].inclineLowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].switch1UpLimit!=0){//����
				if(cover[num].switch1p==1){
					inpoutpFlag.modbusCover[num].switch1UpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].switch1LowLimit!=0){//����
				if(cover[num].switch1p==0){
					inpoutpFlag.modbusCover[num].switch1LowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].switch2UpLimit!=0){//����
				if(cover[num].switch2p==1){
					inpoutpFlag.modbusCover[num].switch2UpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].switch2LowLimit!=0){//����
				if(cover[num].switch2p==0){
					inpoutpFlag.modbusCover[num].switch2LowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].vibrationUpLimit!=0){//����
				if(cover[num].vibration==1){
					inpoutpFlag.modbusCover[num].vibrationUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].vibrationLowLimit!=0){//����
				if(cover[num].vibration==0){
					inpoutpFlag.modbusCover[num].vibrationLowFlag=true;
					alarmFLag=true;
				}
		}
}
//��λ��ʪ�ȵ�warn״ֵ̬
void resetCoverWarnFlag()
{
		for (int i = 0; i < COVER_485_NUM; i++)
		{		
				inpoutpFlag.modbusCover[i].inclineUpFlag	  =false;
				inpoutpFlag.modbusCover[i].inclineLowFlag		=false;
				inpoutpFlag.modbusCover[i].switch1UpFlag  		=false;
				inpoutpFlag.modbusCover[i].switch1LowFlag 		=false;
				inpoutpFlag.modbusCover[i].vibrationUpFlag  =false;
				inpoutpFlag.modbusCover[i].vibrationLowFlag =false;
				inpoutpFlag.modbusCover[i].switch2UpFlag  		=false;
				inpoutpFlag.modbusCover[i].switch2LowFlag 		=false;
		}
}
/*
������״̬ ʾ�����ģ�
Tx: 01 03 0B B8 00 04 C6 08
Rx: 01 03 08 00 10 00 00 00 01 00 00 D5 D6
���ձ��Ľ�����
�汾�ţ�V1.0
��б״̬��������δ��б��
����״̬������
�񶯱���������
*/
//ͨ��485�ӿڶ�ȡ��ʪ��
void readCover(int num)
{
	  
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadReg(sheet.cover[num].slaveAddr,0X0BB8,READ_03,5,buf);
//		rt_mutex_take(uartDev[sheet.tempHum[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		coverUartSend(num,(uint8_t *)buf,len);
	  rt_kprintf("%scover send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,MODBUS_LENTH);
		len=  rs485UartRec(sheet.cover[num].useUartNum,buf,500);

		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//��ȡ����ֵ ��һ���ж�crc �ڶ�����ȡ
		//uartDev[modbusFlash[TEMPHUM].useUartNum].offline=RT_FALSE;
		int ret2=modbusRespCheck(sheet.cover[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//ˢ�¶�ȡ����ֵ
			  cover[num].incline = buf[offset+3];
			  cover[num].switch2p = buf[offset+5];
			  cover[num].vibration = buf[offset+7];
			  cover[num].switch1p = buf[offset+9];

			  rt_kprintf("%s��б:%d ����1:%d ��:%d ����2:%d\n",sign,cover[num].incline,cover[num].switch1p,cover[num].vibration,cover[num].switch2p);  
        cover[num].respStat=1;			
				coverCheckSetFlag(num);

		} 
		else{//��������0
				if(ret2==2){
				}
			  cover[num].incline = 0;
			  cover[num].switch1p = 0;
			  cover[num].vibration = 0;
				cover[num].switch2p = 0;
				cover[num].respStat  = 0;	
			  rt_kprintf("%s read fail\n",sign);
		}

		rt_free(buf);
	  buf=RT_NULL;				
}

extern char   sdData[DATA_LEN];
//��ʪ��ֵͨ��json��ʽ���
//���� respFlag Ϊtrue���ǻ�Ӧ
//              Ϊfalse����report����
static uint16_t coverJsonPack(bool respFlag)
{
		char* out = NULL;
		//��������
		cJSON* Array = NULL;
		// ����JSON Object  
		cJSON* root = NULL;
		cJSON* nodeobj = NULL;
		cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return 0;
		// ����ڵ㣨��ֵ�ԣ�
//		memset(sdData,0,DATA_LEN);
	  if(respFlag==true){
				cJSON_AddNumberToObject(root, "mid",respMid);
				cJSON_AddStringToObject(root, "packetType","PROPERTIES_485DATA_GET_RESP");
				cJSON_AddNumberToObject(root, "code",0);
		}
		else
		{
				cJSON_AddNumberToObject(root, "mid",mcu.upMessID);
				cJSON_AddStringToObject(root, "packetType","PROPERTIES_485DATA_REP");
		}
		cJSON_AddStringToObject(root, "identifier","cover_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < COVER_485_NUM; i++)
		{		
			if(sheet.cover[i].workFlag==RT_TRUE){
				sdData[0]=0;
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.cover[i].ID));
				//sprintf(sprinBuf,"%d",thum[i].respStat);
				cJSON_AddNumberToObject(nodeobj,"responseStatus",cover[i].respStat);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
				
				cJSON_AddNumberToObject(nodeobj_p,"incline"  ,cover[i].incline);      sprintf(sprinBuf,"%d",cover[i].incline);  strcat(sdData,sprinBuf);strcat(sdData,"  ");
				cJSON_AddNumberToObject(nodeobj_p,"switch2"   ,cover[i].switch2p);  sprintf(sprinBuf,"%d",cover[i].switch2p);  strcat(sdData,sprinBuf);strcat(sdData,"  ");
  
				cJSON_AddNumberToObject(nodeobj_p,"vibration",cover[i].vibration);    sprintf(sprinBuf,"%d",cover[i].vibration);  strcat(sdData,sprinBuf);strcat(sdData,"  ");

				cJSON_AddNumberToObject(nodeobj_p,"switch1"   ,cover[i].switch1p);   sprintf(sprinBuf,"%d",cover[i].switch1p);  strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
					
				extern void jingGaiSaveSD(char *id,char *data);
				jingGaiSaveSD(sheet.cover[i].ID,sdData);;
				if(strlen(sdData)>=(sizeof(sdData)-2)){
						rt_kprintf("err:sdData is not enough\n");
				}
				rt_kprintf("%sSD data��%s",sign,sdData);//�Դ�����
			}
		}
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  

		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);

		return len;
}





//ģ���¶Ⱥ�ʪ��ֵ��ȡ�Լ������json��ʽ  ����true �и澯 false �޸澯
bool modCoverWarn2Send()
{
		if(alarmFLag==false)//TEST
			return false;
		char* out = NULL;
		//��������
		cJSON* Array = NULL;
		// ����JSON Object  
		cJSON* root = NULL;
		cJSON* nodeobj = NULL;
		cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return false;
		// ����ڵ㣨��ֵ�ԣ�
		cJSON_AddNumberToObject(root, "mid",mcu.upMessID);
		cJSON_AddStringToObject(root, "packetType","EVENTS_485_ALARM");
		cJSON_AddStringToObject(root, "identifier","cover_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < COVER_485_NUM; i++)
				{		
						if(sheet.cover[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.cover[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
						//	cJSON_AddNumberToObject(nodeobj_p,"incline_low_alarm",   inpoutpFlag.modbusCover[i].inclineLowFlag);//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"incline_high_alarm",  inpoutpFlag.modbusCover[i].inclineUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"switch2_low_alarm",    inpoutpFlag.modbusCover[i].switch2LowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"switch2_high_alarm",   inpoutpFlag.modbusCover[i].switch2UpFlag);
						//	cJSON_AddNumberToObject(nodeobj_p,"vibration_low_alarm", inpoutpFlag.modbusCover[i].vibrationLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"vibration_high_alarm",inpoutpFlag.modbusCover[i].vibrationUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"switch1_low_alarm",    inpoutpFlag.modbusCover[i].switch1LowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"switch1_high_alarm",   inpoutpFlag.modbusCover[i].switch1UpFlag);
							sprintf(sprinBuf,"%llu",utcTime_ms());
							cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
						}
				}
		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		//���
	int len=jsonPackMqttTcp(&out,&root,&sprinBuf,false);
		return true;
}



//����״̬��ȡ�����json��ʽ
void coverRead2Send(rt_bool_t netStat,bool respFlag)
{
	 int workFlag=RT_FALSE;
	 for(int i=0;i<COVER_485_NUM;i++){
		if(sheet.cover[i].workFlag==RT_TRUE){
					readCover(i);
					workFlag=RT_TRUE;
			}
		}
		if(workFlag==RT_TRUE){
				rt_kprintf("%s����ɼ���cover����\r\n",sign);
				coverJsonPack(respFlag);
				if(netStat==RT_TRUE)
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
						rt_thread_mdelay(500);
				if(modCoverWarn2Send()==true){
						resetCoverWarnFlag();//ÿ���жϺ�λwarnflag״ֵ̬
						if(netStat==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				}
		}
}

//���Ϳ������߹��������ID��Ӧ�ľ���  onOffFlag=true ��  onOffFlag=false �ر�
//Զ�̿�������LCD��������
bool  coverUartOnOff(char *ID,bool onOffFlag)
{
	  #define LEN_COVER  8
	  uint8_t sendBuf[LEN_COVER];
	 // uint8_t recBuf[LEN_COVER*20];
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
  	int num=0;
	  bool  ret=true;
	  for(int i=0;i<COVER_485_NUM;i++){
				if(strcmp(sheet.cover[i].ID,ID)==0){
					  num=i;
						break;
				}
		}
		uint16_t len;
		if(onOffFlag==true)
				len = modbusReadReg(sheet.cover[num].slaveAddr,0X0CE4,WRITE,1,sendBuf);
		else 
			  len = modbusReadReg(sheet.cover[num].slaveAddr,0X0CE4,WRITE,2,sendBuf);
//		rt_mutex_take(uartDev[sheet.tempHum[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		coverUartSend(num,(uint8_t *)sendBuf,len);
	  rt_kprintf("%scover send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",sendBuf[j]);
		}
		rt_kprintf("\n");
 
		len=  rs485UartRec(sheet.cover[num].useUartNum,buf,500);

		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
    for(int z=0;z<LEN_COVER;z++){
			 if(sendBuf[z]!=buf[z]){
				 rt_kprintf("%s���ؾ���ʧ��\n",sign);
				 ret= false;
				 break;
			 }
		}
		rt_free(buf);
		buf=RT_NULL;
		return ret;
		
}


//����Զ�̿�����

void coverJsonOnOff(cJSON   *Json)
{
		char* out = NULL;
		//��������
		cJSON* root = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return;
	  cJSON_AddNumberToObject(root,"mid",respMid);
	  cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		cJSON_AddStringToObject(root, "identifier","cover_monitor");
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		cJSON_AddNumberToObject(root, "code",0);
		cJSON   *array=cJSON_GetObjectItem(Json,"params");
		int array_size = cJSON_GetArraySize(array);
		cJSON  *Arrayadd = cJSON_CreateArray();
		if (Arrayadd == NULL) return ;
		cJSON_AddItemToObject(root, "params", Arrayadd);
		
		cJSON  *nodeobj = cJSON_CreateObject();
		cJSON_AddItemToArray(Arrayadd, nodeobj);
		for(int i=0;i<array_size;i++)
		{
				cJSON *item=cJSON_GetArrayItem(array,i);
				cJSON  *id =cJSON_GetObjectItem(item,"deviceId");
			  cJSON  *switchp=cJSON_GetObjectItem(item,"switch2");
				cJSON_AddStringToObject(nodeobj,"deviceId",id->string);
			  if((coverUartOnOff(id->string,switchp->valueint))==true){
						cJSON_AddNumberToObject(nodeobj,"result",1);
				}
				else
						cJSON_AddNumberToObject(nodeobj,"result",0);
		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);

		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,false);
}
#endif

