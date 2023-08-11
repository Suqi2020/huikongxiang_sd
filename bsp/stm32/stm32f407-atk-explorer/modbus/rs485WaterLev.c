#include "board.h"
//#include "stmflash.h"
//ˮλĬ�ϵ�ַ02  ������9600  ����0~49��
//��ȡˮλ
const static char sign[]="[ˮλ]";

//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //���������õ���������buf����

 float waterDepth[WATERDEPTH_485_NUM];
static uint8_t respStat[WATERDEPTH_485_NUM];
//������ڷ��� 
static void waterDepthUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.waterDepth[num].useUartNum,buf, len);
}
//����ˮλ��ͨѶ״̬ true--ͨѶ�ɹ� false--ͨѶʧ��
int waterDepthState(int i)
{
		return respStat[i];
}
static  bool alarmFLag=false;

//ˮλ�������Ƚ���ֵ��������Ӧ��flag���
static void waterLevCheckSetFlag(int num)
{
	  alarmFLag=false;
		if(sheet.modbusWaterDepth[num].depthUpLimit!=0){
			if(waterDepth[num]>=sheet.modbusWaterDepth[num].depthUpLimit)
					inpoutpFlag.modbusWaterDepth[num].depthUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusWaterDepth[num].depthLowLimit!=0){
			if(waterDepth[num]<=sheet.modbusWaterDepth[num].depthLowLimit)
					inpoutpFlag.modbusWaterDepth[num].depthLowFlag=true;alarmFLag=true;
		}
}




//�� 1A 04 00 01 00 02 23 E0
//�� 1A 04 04 0B 1B 00 1C 23 6F
//��ȡˮλ
static void readWaterDepth(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadReg(sheet.waterDepth[num].slaveAddr,0X0002,READ_04,2,buf);
//		rt_mutex_take(uartDev[sheet.waterDepth[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		waterDepthUartSend(num,buf,len);
	  rt_kprintf("%swater send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,MODBUS_LENTH);
		

		len=  rs485UartRec(sheet.waterDepth[num].useUartNum,buf,500);
		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//��ȡ����ֵ ��һ���ж�crc �ڶ�����ȡ
		int ret2=modbusRespCheck(sheet.waterDepth[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//ˢ�¶�ȡ����ֵ
        uint32_t read	=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
        extern float write_hex_to_float(uint32_t number);
			  float waterDepth_p= write_hex_to_float(read);
        waterDepth[num]=(float)(waterDepth_p*10/98);
			  rt_kprintf("%sˮ��:%0.4f��\n",sign,waterDepth[num]);  
			  respStat[num]=1;
				waterLevCheckSetFlag(num);
		} 
		else{//��������0
				if(ret2==2){
						//rt_kprintf("%sERR:����485���߻��߹���\r\n",sign);
				}
				respStat[num]=0;
			  waterDepth[num]	=0;
			  rt_kprintf("%s read fail\n",sign);
		}
		//waterLevCheckSetFlag(num);
//	  rt_mutex_release(uartDev[sheet.waterDepth[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;

}


extern char   sdData[DATA_LEN];
//ˮλֵ�����json��ʽ
//���� respFlag Ϊtrue���ǻ�Ӧ
//              Ϊfalse����report����
uint16_t waterDepthJsonPack(bool respFlag)
{
//		char *sprinBuf=RT_NULL;
//		sprinBuf=rt_malloc(20);//20���ַ������� ������
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
		cJSON_AddStringToObject(root, "identifier","water_level_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
//	  cJSON_AddItemToObject(root,"acuId",cJSON_CreateString(packFlash.acuId));
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������	
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < WATERDEPTH_485_NUM; i++)
		{		
			if(sheet.waterDepth[i].workFlag==RT_TRUE){
				sdData[0]=0;
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.waterDepth[i].ID));
				//sprintf(sprinBuf,"%d",respStat[i]);
				cJSON_AddNumberToObject(nodeobj,"responseStatus",respStat[i]);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
				sprintf(sprinBuf,"%02f",waterDepth[i]);
				cJSON_AddItemToObject(nodeobj_p,"depth",cJSON_CreateString(sprinBuf));strcat(sdData,sprinBuf);strcat(sdData,"  ");

				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
				if(strlen(sdData)>=(sizeof(sdData)-2)){
						rt_kprintf("err:sdData is not enough\n");
				}
//				extern void shuiWeiSaveSD(char *id,char *data);
//				shuiWeiSaveSD(sheet.waterDepth[i].ID,sdData);
				modbusTxtSaveSD(sheet.waterDepth[i].ID,sdData,WATERDEPTH);

			}
		}
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
    int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);
		return len;
}

void resetWaterLevWarnFlag()
{
		for (int i = 0; i < WATERDEPTH_485_NUM; i++)
		{		
				inpoutpFlag.modbusWaterDepth[i].depthLowFlag =false;
				inpoutpFlag.modbusWaterDepth[i].depthUpFlag=false;
		}
}





//ģ���¶Ⱥ�ʪ��ֵ��ȡ�Լ������json��ʽ  ����true �и澯 false �޸澯
bool modPWaterLevWarn2Send()
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
		cJSON_AddStringToObject(root, "identifier","water_level_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < WATERDEPTH_485_NUM; i++)
				{		
						if(sheet.waterDepth[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.waterDepth[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
							cJSON_AddNumberToObject(nodeobj_p,"depth_high_alarm",inpoutpFlag.modbusWaterDepth[i].depthUpFlag);//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"depth_low_alarm",inpoutpFlag.modbusWaterDepth[i].depthLowFlag);
//							cJSON_AddNumberToObject(nodeobj_p,"height_low_alarm",inpoutpFlag.modbusPreSettl[i].tempLowFlag);
//							cJSON_AddNumberToObject(nodeobj_p,"height_high_alarm",inpoutpFlag.modbusPreSettl[i].tempUpFlag);		
							
							sprintf(sprinBuf,"%llu",utcTime_ms());
							cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
						}
				}
		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
    int len=jsonPackMqttTcp(&out,&root,&sprinBuf,false);
		return true;
}

//ˮλֵ�Ķ�ȡ�ʹ������  ��������װ ����ĺ���������
void waterDepthRead2Send(rt_bool_t netStat,bool respFlag)
{
	 int workFlag=RT_FALSE;

			for(int i=0;i<WATERDEPTH_485_NUM;i++){
					if(sheet.waterDepth[i].workFlag==RT_TRUE){
							readWaterDepth(i);
							workFlag=RT_TRUE;
					}
			}
			if(workFlag==RT_TRUE){
					rt_kprintf("%s����ɼ���waterdepth����\r\n",sign);
					waterDepthJsonPack(respFlag);
					if(netStat==RT_TRUE)
							rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
					
					rt_thread_mdelay(500);
					if(modPWaterLevWarn2Send()==true){
							resetWaterLevWarnFlag();//ÿ���жϺ�λwarnflag״ֵ̬
							//rt_thread_mdelay(500);
							if(netStat==RT_TRUE)
									rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
					}
			}
}

