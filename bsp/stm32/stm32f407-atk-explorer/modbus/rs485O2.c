#include "board.h"
//#include "stmflash.h"
//����Ĭ�ϵ�ַ02  ������9600
//02 03 00 02 00 02 65 F8
//02 03 04 00 00 52 1C F5 9A 00 
#ifdef USE_4GAS
const static char sign[]="[����]";

//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //���������õ���������buf����


float  o2[O2_485_NUM];
static uint8_t respStat[O2_485_NUM];

extern bool gasAlarmFlag;

static void o2CheckSetFlag(int num)
{
	  gasAlarmFlag=false;
		if(sheet.modbusO2[num].o2UpLimit!=0){
			if(o2[num]>=sheet.modbusO2[num].o2UpLimit){
					inpoutpFlag.modbusO2[num].o2UpFlag=true;
				  gasAlarmFlag=true;
			}
		}
		if(sheet.modbusO2[num].o2LowLimit!=0){
			if(o2[num]<=sheet.modbusO2[num].o2LowLimit){
					inpoutpFlag.modbusO2[num].o2LowFlag=true;
					gasAlarmFlag=true;
			}
		}
}


//����O2��ͨѶ״̬ true--ͨѶ�ɹ� false--ͨѶʧ��
int o2State(int i)
{
		return respStat[i];
}
//������ڷ��� 
static void o2UartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.o2[num].useUartNum,buf, len);
}



//�� 1A 04 00 01 00 02 23 E0
//�� 1A 04 04 0B 1B 00 1C 23 6F
//��ȡo2�ĺ���
void readO2(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadReg(sheet.o2[num].slaveAddr,0X0002,READ_03,2,buf);
//		rt_mutex_take(uartDev[sheet.o2[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		o2UartSend(num,buf,len);
	  rt_kprintf("%so2 send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,MODBUS_LENTH);
		len=  rs485UartRec(sheet.o2[num].useUartNum,buf,1000);

		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//��ȡ����ֵ ��һ���ж�crc �ڶ�����ȡ
//		uartDev[modbusFlash[O2].useUartNum].offline=RT_FALSE;
		int ret2=modbusRespCheck(sheet.o2[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//ˢ�¶�ȡ����ֵ
        uint32_t val	=((uint32_t)buf[offset]<<24)+((uint32_t)buf[offset+1]<<16)+((uint32_t)buf[offset+2]<<8)+buf[offset+3];offset+=4;

        o2[num]=(float)((float)val	/1000);
			  respStat[num]=1;
				o2CheckSetFlag(num);
			  rt_kprintf("%sŨ��ֵ:%0.2fmol/Lread ok\n",sign,o2[num]);  
		} 
		else{//��������0
				if(ret2==2){
						//rt_kprintf("%sERR:����485���߻��߹���\r\n",sign);
//					  uartDev[modbusFlash[O2].useUartNum].offline=RT_TRUE;
				}
				respStat[num]=0;
			  o2[num]	=0;
			  rt_kprintf("%s read fail\n",sign);
		}
		//o2CheckSetFlag(num);
//	  rt_mutex_release(uartDev[sheet.o2[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;

}




extern char   sdData[DATA_LEN];
//4��������
//���� respFlag Ϊtrue���ǻ�Ӧ
//              Ϊfalse����report����
#if 1
static uint16_t o2Pack(bool respFlag)
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
		//memset(sdData,0,DATA_LEN);
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
		cJSON_AddStringToObject(root, "identifier","o2_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < O2_485_NUM; i++)
		{		

			if(sheet.o2[i].workFlag==RT_TRUE){
				sdData[0]=0;
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.o2[i].ID));
				cJSON_AddNumberToObject(nodeobj,"responseStatus",respStat[i]);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);

					sprintf(sprinBuf,"%.2f",o2[i]);
					cJSON_AddItemToObject(nodeobj_p,"oxygen",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");

			
				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
				if(strlen(sdData)>=(sizeof(sdData)-2)){
						rt_kprintf("err:sdData is not enough\n");
				}
//				extern void o2SaveSD(char *id,char *data);
//				o2SaveSD(sheet.o2[i].ID,sdData);
				modbusTxtSaveSD(sheet.o2[i].ID,sdData,PRESSSETTL);

			}
			}
		
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  
		//���
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);
		return len;
}

#endif

//��λ��ʪ�ȵ�warn״ֵ̬
void resetO2WarnFlag()
{
		for (int i = 0; i < O2_485_NUM; i++)
		{		
//				inpoutpFlag.modbusCo[i].coLowFlag =false;
//			  inpoutpFlag.modbusCo[i].coUpFlag  =false;
				inpoutpFlag.modbusO2[i].o2LowFlag =false;
			  inpoutpFlag.modbusO2[i].o2UpFlag  =false;
//				inpoutpFlag.modbusH2s[i].h2sLowFlag =false;
//			  inpoutpFlag.modbusH2s[i].h2sUpFlag  =false;
//				inpoutpFlag.modbusCh4[i].ch4LowFlag =false;
//			  inpoutpFlag.modbusCh4[i].ch4UpFlag  =false;
		}
}





//ģ���¶Ⱥ�ʪ��ֵ��ȡ�Լ������json��ʽ  ����true �и澯 false �޸澯
bool modO2Warn2Send()
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
		cJSON_AddStringToObject(root, "identifier","o2_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < O2_485_NUM; i++)
				{		
						if(sheet.o2[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.o2[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
		
							cJSON_AddNumberToObject(nodeobj_p,"oxy_low_alarm",inpoutpFlag.modbusO2[i].o2LowFlag );//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"oxy_high_alarm",inpoutpFlag.modbusO2[i].o2UpFlag);
														
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
//4������json����Ķ��η�װ

extern int dispO2TotlNum;
//co����json����Ķ��η�װ
void  o2Read2Send(rt_bool_t netStat,bool respFlag)
{
		rt_kprintf("%s����ɼ���o2����\r\n",sign);
		 int workFlag=RT_FALSE;
	 dispO2TotlNum=0;
	 for(int i=0;i<O2_485_NUM;i++){
		if(sheet.o2[i].workFlag==RT_TRUE){
					readO2(i);
					workFlag=RT_TRUE;
			    dispO2TotlNum++;
			}
	}
	if(workFlag==RT_TRUE){
				o2Pack(respFlag);
			if(netStat==RT_TRUE)
				rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				rt_thread_mdelay(500);return;
				if(modO2Warn2Send()==true){
						resetO2WarnFlag();//ÿ���жϺ�λwarnflag״ֵ̬
						if(netStat==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				}
			}

}



#endif

