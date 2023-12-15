#include "board.h"
//#include "stmflash.h"
//<<ѹ��ʽ������ GY-STC-2000>> Ĭ�ϲ�����9600  modbus��ַ 0xb1(����) �������ǩ��2λ   Э���ĵ�����
/*
������֤�̵�ַ��04 00 01 00 04

ѹ����֤�̵�ַ��04 00 01 00 02
�����ʸ�Ϊ9600��ѹ�������ö̵�ַ�����붼��һ���ģ�FF FF 03 0A���豸�����ĳ���ַ��01���̵�ַ
*/
//�޸��豸��ַ FF FF 03 0A���豸�����ĳ���ַ��01���̵�ַ 
//   FF FF 03 0A 6E 01 20 21 09 08 00 B1 01 01 
//  24+��ɫ��24-��ɫ��A+��ɫ��B-��ɫ
// �� 01 04 00 01 00 04 A0 09 
// �� 01 04 08 0B CA FE 8D 00 03 03 80 C7 23 
// ��                   01 04 00 01 00 04 A0 09 
//[������]threeAxis send:9  4  0  1  0  4 a1 41 
// ��         01 04 08 0B CA FE 8D 00 03 03 80 C7 23 
//[������]rec: 9  4  6  b 73  0 9a  0 17 fc 32 69 b4

const static char sign[]="[������]";
threeAxisStru threeAxisp[THREEAXIS_485_NUM];
//���������ͨѶ״̬ true--ͨѶ�ɹ� false--ͨѶʧ��
int threeAxisState(int i)
{
		return threeAxisp[i].respStat;
}
//#define   SLAVE_ADDR     0X02 
//#define   LENTH          50  //���������õ���������buf����

static  bool alarmFLag=false;
//������ڷ��� 
static void threeAxisUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.threeAxiss[num].useUartNum,buf, len);
}

///////////////////////////////////////��д�Ĵ�����ز���////////////////////////////////////////

//��ѹ��ʽ����������һ��������
extern uint8_t tongHeModbusRead(uint16_t slavAddr,uint16_t regAddr,uint16_t len,uint8_t * out);

//����Ƚ���ֵ��������Ӧ��flag���
static void threeAccCheckSetFlag(int num)
{
	  alarmFLag=false;
	  if(sheet.modbusThreAxis[num].tempUpLimit!=0){
			if(threeAxisp[num].temp>=sheet.modbusThreAxis[num].tempUpLimit){
					inpoutpFlag.modbusThreAxis[num].tempUpFlag=true;alarmFLag=true;
			}
		}
		if(sheet.modbusThreAxis[num].tempLowLimit!=0){
			if(threeAxisp[num].temp<=sheet.modbusThreAxis[num].tempLowLimit){
					inpoutpFlag.modbusThreAxis[num].tempLowFlag=true;alarmFLag=true;
			}
		}

		if(sheet.modbusThreAxis[num].accXUpLimit!=0){
			if(threeAxisp[num].acclrationX>=sheet.modbusThreAxis[num].accXUpLimit){
					inpoutpFlag.modbusThreAxis[num].accXUpFlag=true;alarmFLag=true;
			}
		}
		if(sheet.modbusThreAxis[num].accXLowLimit!=0){
			if(threeAxisp[num].acclrationX<=sheet.modbusThreAxis[num].accXLowLimit){
					inpoutpFlag.modbusThreAxis[num].accXLowFlag=true;alarmFLag=true;
			}
		}

		if(sheet.modbusThreAxis[num].accYUpLimit!=0){
			if(threeAxisp[num].acclrationY>=sheet.modbusThreAxis[num].accYUpLimit){
					inpoutpFlag.modbusThreAxis[num].accYUpFlag=true;alarmFLag=true;
			}
		}
		if(sheet.modbusThreAxis[num].accYLowLimit!=0){
			if(threeAxisp[num].acclrationY<=sheet.modbusThreAxis[num].accYLowLimit){
					inpoutpFlag.modbusThreAxis[num].accYLowFlag=true;alarmFLag=true;
			}
		}

		if(sheet.modbusThreAxis[num].accZUpLimit!=0){
			if(threeAxisp[num].acclrationZ>=sheet.modbusThreAxis[num].accZUpLimit){
					inpoutpFlag.modbusThreAxis[num].accZUpFlag=true;alarmFLag=true;
			}
		}
    if(sheet.modbusThreAxis[num].accZLowLimit!=0){
			if(threeAxisp[num].acclrationZ<=sheet.modbusThreAxis[num].accZLowLimit){
					inpoutpFlag.modbusThreAxis[num].accZLowFlag=true;alarmFLag=true;
			}
		}

}
//�� 1A 04 00 01 00 02 23 E0
//�� 1A 04 04 0B 1B 00 1C 23 6F
//ͨ��485�ӿڶ�ȡ�����ֵ
void readThreeTempAcc(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = tongHeModbusRead(sheet.threeAxiss[num].slaveAddr,0X0001,4,buf);
//		rt_mutex_take(uartDev[sheet.threeAxiss[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		threeAxisUartSend(num,buf,len);
	  rt_kprintf("%sthreeAxis send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,MODBUS_LENTH);
		len=  rs485UartRec(sheet.threeAxiss[num].useUartNum,buf,1000);

		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
//		uartDev[modbusFlash[THREEAXIS].useUartNum].offline=RT_FALSE;
		//��ȡ����ֵ ��һ���ж�crc �ڶ�����ȡ
		int ret2=modbusRespCheck(sheet.threeAxiss[num].slaveAddr,buf,len,RT_TRUE);
		if(0 ==  ret2){//ˢ�¶�ȡ����ֵ
        int temp	=(buf[offset]<<8)+buf[offset+1];offset+=2;
			  threeAxisp[num].acclrationX = (buf[offset]<<8)+buf[offset+1];offset+=2;
				threeAxisp[num].acclrationY = (buf[offset]<<8)+buf[offset+1];offset+=2;
				threeAxisp[num].acclrationZ = (buf[offset]<<8)+buf[offset+1];offset+=2;
        threeAxisp[num].temp=(float)temp/100; 
			  threeAxisp[num].respStat=1;
				threeAccCheckSetFlag(num);
			  rt_kprintf("%stemp:%0.2f*C ACC:X%dmg Y%dmg Z%dmg ok\n",sign,threeAxisp[num].temp,threeAxisp[num].acclrationX,threeAxisp[num].acclrationY,threeAxisp[num].acclrationZ);  
		} 
		else{//��������0
				if(ret2==2){
//					  uartDev[modbusFlash[THREEAXIS].useUartNum].offline=RT_TRUE;
				}
				threeAxisp[num].respStat=0;
				threeAxisp[num].temp=0;
			  threeAxisp[num].acclrationX	= 0;
			  threeAxisp[num].acclrationY = 0;
			  threeAxisp[num].acclrationY = 0;
			  rt_kprintf("%stemp height read fail\n",sign);
		}
//		threeAccCheckSetFlag(num);
//	  rt_mutex_release(uartDev[sheet.threeAxiss[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;

		
		

		
}

/////////////////////////////////////////JSON��ʽ���//////////////////////////////////////////
//�¶ȸ߶�ֵ���

/*
{
    "mid":1234,
    "packetType�":"CMD_REPORTDATA",  
    "param":
    {
        "identifier":" vibration_meter_monitor",
        "acuId":"100000000000001",
        "deviceId":"1000000000004",�
        "data":
        {
            "temp":"22.75", //?
            "accelerationX":"1234",//mg
"accelerationY":"1234",//mg
"accelerationZ":"1234",//mg
            "monitoringTime":"1655172531937"
        }
    },
    "timestamp":"1655172531937"
}
*/
//�������ֵͨ��json���



extern char   sdData[DATA_LEN];

//���� respFlag Ϊtrue���ǻ�Ӧ
//              Ϊfalse����report����
static uint16_t threeAxisJsonPack(bool respFlag)
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
		
		
	// memset(sdData,0,DATA_LEN);
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
		cJSON_AddStringToObject(root, "identifier","vibration_monitor");
	  cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < THREEAXIS_485_NUM; i++)
		{		

			if(sheet.threeAxiss[i].workFlag==RT_TRUE){
				sdData[0]=0;
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.threeAxiss[i].ID));
				
			//	sprintf(sprinBuf,"%d",threeAxisp[i].respStat);
				cJSON_AddNumberToObject(nodeobj,"responseStatus",threeAxisp[i].respStat);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
				
				sprintf(sprinBuf,"%.2f",threeAxisp[i].temp);
				cJSON_AddItemToObject(nodeobj_p,"temperature",cJSON_CreateString(sprinBuf));   strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",threeAxisp[i].acclrationX);
				cJSON_AddItemToObject(nodeobj_p,"accelerationX",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",threeAxisp[i].acclrationY);
				cJSON_AddItemToObject(nodeobj_p,"accelerationY",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",threeAxisp[i].acclrationZ);
				cJSON_AddItemToObject(nodeobj_p,"accelerationZ",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");

				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
				if(strlen(sdData)>=(sizeof(sdData)-2)){
					rt_kprintf("err:sdData is not enough\n");
				}
//				extern void fWaiPoSaveSD(char *id,char *data);
//				fWaiPoSaveSD(sheet.threeAxiss[i].ID,sdData);
				modbusTxtSaveSD(sheet.threeAxiss[i].ID,sdData,THREEAXIS);

				rt_kprintf("%sSD sdData��%s",sign,sdData);//�Դ�����
			}
		}
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  
    int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);

		return len;
}
//��ʪ�ȴ�������ȡֵ����ֵ�Ƚϲ�����flag
 void threeAxisCheckSetFlag(int num)
{
	  alarmFLag=false;
		
		if(sheet.modbusThreAxis[num].tempUpLimit!=0){
			  if(threeAxisp[num].temp>=sheet.modbusThreAxis[num].tempUpLimit){
					inpoutpFlag.modbusThreAxis[num].tempUpFlag=true;
					alarmFLag=true;
				}
		}
		
		if(sheet.modbusThreAxis[num].tempLowLimit!=0){
				if(threeAxisp[num].temp<=sheet.modbusThreAxis[num].tempLowLimit){
					inpoutpFlag.modbusThreAxis[num].tempLowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusThreAxis[num].accXUpLimit!=0){
				if(threeAxisp[num].acclrationX>=sheet.modbusThreAxis[num].accXUpLimit){
					inpoutpFlag.modbusThreAxis[num].accXUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusThreAxis[num].accXLowLimit!=0){
				if(threeAxisp[num].temp<=sheet.modbusThreAxis[num].accXLowLimit){
					inpoutpFlag.modbusThreAxis[num].accXLowFlag=true;
					alarmFLag=true;
				}
		}

		if(sheet.modbusThreAxis[num].accYUpLimit!=0){
				if(threeAxisp[num].acclrationY>=sheet.modbusThreAxis[num].accYUpLimit){
					inpoutpFlag.modbusThreAxis[num].accYUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusThreAxis[num].accYLowLimit!=0){
				if(threeAxisp[num].temp<=sheet.modbusThreAxis[num].accYLowLimit){
					inpoutpFlag.modbusThreAxis[num].accYLowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusThreAxis[num].accZUpLimit!=0){
				if(threeAxisp[num].acclrationX>=sheet.modbusThreAxis[num].accZUpLimit){
					inpoutpFlag.modbusThreAxis[num].accZUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusThreAxis[num].accZLowLimit!=0){
				if(threeAxisp[num].temp<=sheet.modbusThreAxis[num].accZLowLimit){
					inpoutpFlag.modbusThreAxis[num].accZLowFlag=true;
					alarmFLag=true;
				}
		}
}
//��λ��ʪ�ȵ�warn״ֵ̬
void resetThreeAxisWarnFlag()
{
		for (int i = 0; i < THREEAXIS_485_NUM; i++)
		{		
				inpoutpFlag.modbusThreAxis[i].tempUpFlag =false;
				inpoutpFlag.modbusThreAxis[i].tempLowFlag=false;
				inpoutpFlag.modbusThreAxis[i].accXLowFlag=false;
				inpoutpFlag.modbusThreAxis[i].accXUpFlag =false;
				inpoutpFlag.modbusThreAxis[i].accYLowFlag=false;
				inpoutpFlag.modbusThreAxis[i].accYUpFlag =false;
				inpoutpFlag.modbusThreAxis[i].accZLowFlag=false;
				inpoutpFlag.modbusThreAxis[i].accZUpFlag =false;
		}
}





//ģ���¶Ⱥ�ʪ��ֵ��ȡ�Լ������json��ʽ  ����true �и澯 false �޸澯
bool modThreeAxisWarn2Send()
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
		cJSON_AddStringToObject(root, "identifier","vibration_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < THREEAXIS_485_NUM; i++)
				{		
						if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.threeAxiss[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
							cJSON_AddNumberToObject(nodeobj_p,"temperature_low_alarm",inpoutpFlag.modbusThreAxis[i].tempLowFlag);//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"temperature_high_alarm",inpoutpFlag.modbusThreAxis[i].tempUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"accelerationX_low_alarm",inpoutpFlag.modbusThreAxis[i].accXLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"accelerationX_high_alarm",inpoutpFlag.modbusThreAxis[i].accXUpFlag);		
							cJSON_AddNumberToObject(nodeobj_p,"accelerationY_low_alarm",inpoutpFlag.modbusThreAxis[i].accYLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"accelerationY_high_alarm",inpoutpFlag.modbusThreAxis[i].accYUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"accelerationZ_low_alarm",inpoutpFlag.modbusThreAxis[i].accZLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"accelerationZ_high_alarm",inpoutpFlag.modbusThreAxis[i].accZUpFlag);			
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




//�����ȡmodbus���ݲ�������� ��������������
void threeAxisRead2Send(rt_bool_t netStat,bool respFlag)
{					
		int workFlag=RT_FALSE;
		for(int i=0;i<THREEAXIS_485_NUM;i++){
				if(sheet.threeAxiss[i].workFlag==RT_TRUE){
							readThreeTempAcc(i);
							workFlag=RT_TRUE;
					}
			}
			if(workFlag==RT_TRUE){
					rt_kprintf("%s����ɼ���THREEAXIS����\r\n",sign);
					threeAxisJsonPack(respFlag);//circulaJsonPack();
					if(netStat==RT_TRUE)
							rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
					rt_thread_mdelay(500);return;
					if(modThreeAxisWarn2Send()==true){
							resetThreeAxisWarnFlag();//ÿ���жϺ�λwarnflag״ֵ̬
							if(netStat==RT_TRUE)
									rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
					}
			}
}
