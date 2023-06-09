#include "board.h"
//#include "stmflash.h"
//����Ĭ�ϵ�ַ03  ������9600
//03 03 00 02 00 02 64 29
//04 03 04 00 00 00 00 AF 33 

const static char sign[]="[����]";

//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //���������õ���������buf����

 float h2s[H2S_485_NUM];
static uint8_t respStat[H2S_485_NUM];
extern bool gasAlarmFlag;
//������ڷ��� 
static void h2sUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.h2s[num].useUartNum,buf, len);
}
//����h2s��ͨѶ״̬ true--ͨѶ�ɹ� false--ͨѶʧ��
int h2sState(int i)
{
		return respStat[i];
}

//h2s�Ƚ���ֵ��������Ӧ��flag���
static void h2sCheckSetFlag(int num)
{
	  gasAlarmFlag=false;
		if(sheet.modbusH2s[num].h2sUpLimit!=0){
			if(h2s[num]>=sheet.modbusH2s[num].h2sUpLimit){
					inpoutpFlag.modbusH2s[num].h2sUpFlag=true;
					gasAlarmFlag=true;
			}
		}
		if(sheet.modbusH2s[num].h2sLowLimit!=0){
			if(h2s[num]<=sheet.modbusH2s[num].h2sLowLimit){
					inpoutpFlag.modbusH2s[num].h2sLowFlag=true;
					gasAlarmFlag=true;
			}
		}
}

//�� 1A 04 00 01 00 02 23 E0
//�� 1A 04 04 0B 1B 00 1C 23 6F
//��ȡh2sֵ
void readH2S(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(LENTH);
	  uint16_t len = modbusReadReg(sheet.h2s[num].slaveAddr,0X0002,READ_03,2,buf);
//		rt_mutex_take(uartDev[sheet.h2s[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485����buf  len  �ȴ�modbus��Ӧ
		h2sUartSend(num,buf,len);
	  rt_kprintf("%sh2s send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,LENTH);

		while(rt_mq_recv(&uartmque[sheet.h2s[num].useUartNum], buf+len, 1, 500) == RT_EOK){//115200 ������1ms 10������
				len++;
		}
		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//��ȡ����ֵ ��һ���ж�crc �ڶ�����ȡ
		int ret2=modbusRespCheck(sheet.h2s[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//ˢ�¶�ȡ����ֵ
        int val	=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;

        h2s[num]	=(float)((float)val	/1000);
			  respStat[num]=1;
				h2sCheckSetFlag(num);
			  rt_kprintf("%sŨ��ֵ:%0.2fmol/Lread ok\n",sign,h2s[num]);  
		} 
		else{//��������0
				if(ret2==2){
						//rt_kprintf("%sERR:����485���߻��߹���\r\n",sign);
				}
				respStat[num]=0;
			  h2s[num]	=0;
			  rt_kprintf("%s read fail\n",sign);
		}
		//h2sCheckSetFlag(num);
//	  rt_mutex_release(uartDev[sheet.h2s[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;

}


//4��������
//���� respFlag Ϊtrue���ǻ�Ӧ
//              Ϊfalse����report����
#if 1
static uint16_t h2sPack(bool respFlag)
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
		cJSON_AddStringToObject(root, "identifier","h2s_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < H2S_485_NUM; i++)
		{		

			if(sheet.h2s[i].workFlag==RT_TRUE){
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.h2s[i].ID));
				cJSON_AddNumberToObject(nodeobj,"responseStatus",respStat[i]);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);

					sprintf(sprinBuf,"%02f",h2s[i]);
					cJSON_AddItemToObject(nodeobj_p,"hydrogenSulfide",cJSON_CreateString(sprinBuf));
			
				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
			}
			}
		
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// ��ӡJSON���ݰ�  
		//���
		int len=0;
		NetTxBuffer[len]= (uint8_t)(HEAD>>8); len++;
		NetTxBuffer[len]= (uint8_t)(HEAD);    len++;
		len+=LENTH_LEN;//json�����������д
		
		// �ͷ��ڴ�  
		out = cJSON_Print(root);
		rt_strcpy((char *)NetTxBuffer+len,out);
		len+=rt_strlen(out);
		if(out!=NULL){
				for(int i=0;i<rt_strlen(out);i++)
						rt_kprintf("%c",out[i]);
				rt_kprintf("\n");
				rt_free(out);
				out=NULL;
		}
		if(root!=NULL){
			cJSON_Delete(root);
			out=NULL;
		}

		//lenth
	  NetTxBuffer[2]=(uint8_t)((len-LENTH_LEN-HEAD_LEN)>>8);//����json����
	  NetTxBuffer[3]=(uint8_t)(len-LENTH_LEN-HEAD_LEN);
	  uint16_t jsonBodyCrc=RTU_CRC(NetTxBuffer+HEAD_LEN+LENTH_LEN,len-HEAD_LEN-LENTH_LEN);
	  //crc
	  NetTxBuffer[len]=(uint8_t)(jsonBodyCrc>>8); len++;//����crc
	  NetTxBuffer[len]=(uint8_t)(jsonBodyCrc);    len++;

		//tail
		NetTxBuffer[len]=(uint8_t)(TAIL>>8); len++;
		NetTxBuffer[len]=(uint8_t)(TAIL);    len++;
		NetTxBuffer[len]=0;//len++;//��β ��0
		if(respFlag==false){
				mcu.repDataMessID =mcu.upMessID;
				//mcu.devRegMessID =mcu.upMessID;
				upMessIdAdd();
		}
		rt_kprintf("%s len:%d\r\n",sign,len);
		rt_kprintf("\r\n%slen��%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",sign,len,NetTxBuffer[0],NetTxBuffer[1],NetTxBuffer[2],NetTxBuffer[3]);

		rt_free(sprinBuf);
		sprinBuf=RT_NULL;
		return len;
}

#endif

//��λ��ʪ�ȵ�warn״ֵ̬
void resetH2sWarnFlag()
{
		for (int i = 0; i < H2S_485_NUM; i++)
		{		
				inpoutpFlag.modbusH2s[i].h2sLowFlag =false;
			  inpoutpFlag.modbusH2s[i].h2sUpFlag  =false;
		}
}





//ģ���¶Ⱥ�ʪ��ֵ��ȡ�Լ������json��ʽ  ����true �и澯 false �޸澯
bool modH2sWarn2Send()
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
		cJSON_AddStringToObject(root, "identifier","h2s_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < H2S_485_NUM; i++)
				{		
						if(sheet.h2s[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.h2s[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);

							cJSON_AddNumberToObject(nodeobj_p,"monoxide_low_alarm",inpoutpFlag.modbusH2s[i].h2sLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"monoxide_high_alarm",inpoutpFlag.modbusH2s[i].h2sUpFlag );		
											
							sprintf(sprinBuf,"%llu",utcTime_ms());
							cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
						}
				}
		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		//���
		int len=0;
		NetTxBuffer[len]= (uint8_t)(HEAD>>8); len++;
		NetTxBuffer[len]= (uint8_t)(HEAD);    len++;
		len+=LENTH_LEN;//json�����������д
		// �ͷ��ڴ�  
		out = cJSON_Print(root);
		rt_strcpy((char *)NetTxBuffer+len,out);
		len+=rt_strlen(out);
		if(out!=NULL){
				for(int i=0;i<rt_strlen(out);i++)
						rt_kprintf("%c",out[i]);
				rt_kprintf("\n");
				rt_free(out);
				out=NULL;
		}
		if(root!=NULL){
			cJSON_Delete(root);
			out=NULL;
		}
		//lenth
	  NetTxBuffer[2]=(uint8_t)((len-LENTH_LEN-HEAD_LEN)>>8);//����json����
	  NetTxBuffer[3]=(uint8_t)(len-LENTH_LEN-HEAD_LEN);
	  uint16_t jsonBodyCrc=RTU_CRC(NetTxBuffer+HEAD_LEN+LENTH_LEN,len-HEAD_LEN-LENTH_LEN);
	  //crc
	  NetTxBuffer[len]=(uint8_t)(jsonBodyCrc>>8); len++;//����crc
	  NetTxBuffer[len]=(uint8_t)(jsonBodyCrc);    len++;
		//tail
		NetTxBuffer[len]=(uint8_t)(TAIL>>8); len++;
		NetTxBuffer[len]=(uint8_t)(TAIL);    len++;
		NetTxBuffer[len]=0;//len++;//��β ��0
		mcu.repDataMessID =mcu.upMessID;
		//mcu.devRegMessID =mcu.upMessID;
		upMessIdAdd();
		rt_free(sprinBuf);
		sprinBuf=RT_NULL;
		return true;
}
extern int dispH2STotlNum;
//co����json����Ķ��η�װ
void  h2sRead2Send(rt_bool_t netStat,bool respFlag)
{
		rt_kprintf("%s����ɼ���h2s����\r\n",sign);
		 int workFlag=RT_FALSE;
	  dispH2STotlNum=0;
	 for(int i=0;i<H2S_485_NUM;i++){
		if(sheet.h2s[i].workFlag==RT_TRUE){
					readH2S(i);
					workFlag=RT_TRUE;
			    dispH2STotlNum++;
			}
	}
	if(workFlag==RT_TRUE){
				h2sPack(respFlag);
			if(netStat==RT_TRUE)
				rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				rt_thread_mdelay(500);
				if(modH2sWarn2Send()==true){
						resetH2sWarnFlag();//ÿ���жϺ�λwarnflag״ֵ̬
						if(netStat==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				}
			}

}
