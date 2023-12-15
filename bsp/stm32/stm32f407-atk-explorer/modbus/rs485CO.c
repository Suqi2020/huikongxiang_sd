#include "board.h"
//#include "stmflash.h"
//һ����̼Ĭ�ϵ�ַ04 ������9600
//04 03 00 02 00 02 65 9E
//04 03 04 00 00 00 00 AF 33 
//ÿ������co��ȡ��ʱ������ȡ4�����岢ʵ���ϴ� 
//4��������һ���ȡ ����ǰ������ʹ�� ֻ��CO��ʱ���ط���ȡ���������  �ر�ʱ��ֻ��Ҫ�ر�CO�Ϳ��԰���������ȫ���ر�
const static char sign[]="[һ����̼]";

//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //���������õ���������buf����


float co[CO_485_NUM];
static uint8_t respStat[CO_485_NUM];
bool gasAlarmFlag=false;
//������ڷ��� 
static void coUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.co[num].useUartNum,buf, len);
}
//����co��ͨѶ״̬ true--ͨѶ�ɹ� false--ͨѶʧ��
int coState(int i)
{
		return respStat[i];
}

//co�Ƚ���ֵ��������Ӧ��flag���
static void coCheckSetFlag(int num)
{
		gasAlarmFlag=false;
		if(sheet.modbusCo[num].coUpLimit!=0){
			if(co[num]>=sheet.modbusCo[num].coUpLimit){
					inpoutpFlag.modbusCo[num].coUpFlag=true;gasAlarmFlag=true;
			}
		}
		if(sheet.modbusCo[num].coLowLimit!=0){
			if(co[num]<=sheet.modbusCo[num].coLowLimit){
					inpoutpFlag.modbusCo[num].coLowFlag=true;gasAlarmFlag=true;
			}
		}
}
//�� 1A 04 00 01 00 02 23 E0
//�� 1A 04 04 0B 1B 00 1C 23 6F
//��ȡcoֵ
void readCO(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadReg(sheet.co[num].slaveAddr,0X0002,READ_03,2,buf);
//		rt_mutex_take(uartDev[sheet.co[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		coUartSend(num,buf,len);
	  rt_kprintf("%sCO send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,MODBUS_LENTH);
		
		len=  rs485UartRec(sheet.co[num].useUartNum,buf,1000);

		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//��ȡ����ֵ ��һ���ж�crc �ڶ�����ȡ

		int ret2=modbusRespCheck(sheet.co[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//ˢ�¶�ȡ����ֵ
        int value	=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
        co[num]=(float)((float)value	/1000);
			  respStat[num]=1;
			  rt_kprintf("%sŨ��ֵ:%0.2fmol/Lread ok\n",sign,co[num]);  
			
			
				coCheckSetFlag(num);
		} 
		else{//��������0
			  co[num]	=0;
			  respStat[num]=0;
			  rt_kprintf("%s read fail\n",sign);
		}
//		coCheckSetFlag(num);
//	  rt_mutex_release(uartDev[sheet.co[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;
}


extern char   sdData[DATA_LEN];
//4��������
//���� respFlag Ϊtrue���ǻ�Ӧ
//              Ϊfalse����report����
#if 1
static uint16_t coPack(bool respFlag)
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
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		// ����ڵ㣨��ֵ�ԣ�
		memset(sdData,0,DATA_LEN);
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
		cJSON_AddStringToObject(root, "identifier","co_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < CO_485_NUM; i++)
		{		

			if(sheet.co[i].workFlag==RT_TRUE){
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.co[i].ID));
				cJSON_AddNumberToObject(nodeobj,"responseStatus",respStat[i]);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);

					sprintf(sprinBuf,"%.2f",co[i]);
					cJSON_AddItemToObject(nodeobj_p,"monoxide",cJSON_CreateString(sprinBuf));strcat(sdData,sprinBuf);strcat(sdData,"  ");


			
				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
				if(strlen(sdData)>=(sizeof(sdData)-2)){
						rt_kprintf("err:sdData is not enough\n");
				}
//				extern void coSaveSD(char *id,char *data);
//				coSaveSD(sheet.co[i].ID,sdData);
				modbusTxtSaveSD(sheet.co[i].ID,sdData,CO);

			}
			}
		//}
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  
		//���
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);

		return len;
}



//��λ��ʪ�ȵ�warn״ֵ̬
void resetCoWarnFlag()
{
		for (int i = 0; i < CO_485_NUM; i++)
		{		
				inpoutpFlag.modbusCo[i].coLowFlag =false;
			  inpoutpFlag.modbusCo[i].coUpFlag  =false;
				inpoutpFlag.modbusO2[i].o2LowFlag =false;
			  inpoutpFlag.modbusO2[i].o2UpFlag  =false;
				inpoutpFlag.modbusH2s[i].h2sLowFlag =false;
			  inpoutpFlag.modbusH2s[i].h2sUpFlag  =false;
				inpoutpFlag.modbusCh4[i].ch4LowFlag =false;
			  inpoutpFlag.modbusCh4[i].ch4UpFlag  =false;
		}
}





//ģ���¶Ⱥ�ʪ��ֵ��ȡ�Լ������json��ʽ  ����true �и澯 false �޸澯
bool modCoWarn2Send()
{
		if(gasAlarmFlag==false)//TEST
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
		cJSON_AddStringToObject(root, "identifier","co_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < CO_485_NUM; i++)
				{		
						if(sheet.co[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.co[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
		
//							cJSON_AddNumberToObject(nodeobj_p,"oxy_low_alarm",inpoutpFlag.modbusO2[i].o2LowFlag );//cJSON_CreateNumber("10"));
//							cJSON_AddNumberToObject(nodeobj_p,"oxy_high_alarm",inpoutpFlag.modbusO2[i].o2UpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"monoxide_low_alarm",inpoutpFlag.modbusCo[i].coLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"monoxide_high_alarm",inpoutpFlag.modbusCo[i].coUpFlag );		
//							cJSON_AddNumberToObject(nodeobj_p,"hydrogenSulfide_low_alarm",inpoutpFlag.modbusH2s[i].h2sLowFlag);
//							cJSON_AddNumberToObject(nodeobj_p,"hydrogenSulfide_high_alarm",inpoutpFlag.modbusH2s[i].h2sUpFlag);		
//							cJSON_AddNumberToObject(nodeobj_p,"methane_low_alarm",inpoutpFlag.modbusCh4[i].ch4LowFlag );//cJSON_CreateNumber("10"));
//							cJSON_AddNumberToObject(nodeobj_p,"methane_high_alar",inpoutpFlag.modbusCh4[i].ch4UpFlag);
														
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
extern int dispCOTotlNum;
//co����json����Ķ��η�װ
void  coRead2Send(rt_bool_t netStat,bool respFlag)
{
		rt_kprintf("%s����ɼ���co����\r\n",sign);
		 int workFlag=RT_FALSE;
	 dispCOTotlNum=0;
	 for(int i=0;i<CO_485_NUM;i++){
		if(sheet.co[i].workFlag==RT_TRUE){
					readCO(i);
					workFlag=RT_TRUE;
			    dispCOTotlNum++;
			}
	}
	if(workFlag==RT_TRUE){
				coPack(respFlag);
			if(netStat==RT_TRUE)
				rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				rt_thread_mdelay(500);return;
				if(modCoWarn2Send()==true){
						resetCoWarnFlag();//ÿ���жϺ�λwarnflag״ֵ̬
						if(netStat==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				}
			}

}

#endif

