#include  "board.h"
#include  "cJSON.h"
const static char sign[]="[heart]";
//heartUpStru  heartUp;
extern uint16_t RTU_CRC(uint8_t *puchMsg ,uint16_t usDataLen);
//uint8_t   NetTxBuffer[TX_RX_MAX_BUF_SIZE*4]__attribute__((at(0x10000000)));  //��net����buff��Сһ��  ͨ�����䴫�ݸ�NetTxBuffer ���з��ͳ�ȥ
mcuParazStru mcu ={0};

//rs485ParaStru devi[MODBUS_NUM];//Ŀǰ4·485�豸
//����messageID����  ÿ�δ��������1
uint32_t upMessIdAdd()
{
	  rt_kprintf("%smessID:%d\r\n",sign,mcu.upMessID);
		return mcu.upMessID++;
}


static uint64_t subTimeStamp=1577808000000;//2020 1-1 0:0:0

//��ȡ��������ʱ�����ֵ
uint64_t subTimeStampGet()
{
		return subTimeStamp;
}
//�洢��������ʱ�����ֵ  
void  subTimeStampSet(uint64_t time)
{
	  if(time>=rt_tick_get())
				subTimeStamp=time-rt_tick_get();//������rtcֵ-��ǰtickֵ
		else
				subTimeStamp = 0;
}
//��ȡ��ǰ��utcʱ��8���ֽڳ��� ��ȷ��ms
uint64_t  utcTime_ms()
{
	return (uint64_t)rt_tick_get()+(uint64_t)subTimeStampGet();
}
//��ȷ��s
uint64_t  utcTime_s()
{
	return (uint64_t)(rt_tick_get()/1000)+(uint64_t)(subTimeStampGet()/1000);
}



//�������ݴ��
uint16_t heartUpJsonPack()
{
		char* out = NULL;
		//��������

		// ����JSON Object  
		cJSON* root = NULL;

		root = cJSON_CreateObject();
		if (root == NULL) return 0;
		// ����ڵ㣨��ֵ�ԣ�
		cJSON_AddNumberToObject(root, "mid",mcu.upMessID);
		cJSON_AddStringToObject(root, "packetType","PROPERTIES_HEART");
	  cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20���ַ������� ������
		
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		//nodeobj = cJSON_CreateObject();
	  cJSON_AddStringToObject(root, "identifier","area_control_unit");


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
			root=NULL;
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
		mcu.upHeartMessID =mcu.upMessID;
		//mcu.devRegMessID =mcu.upMessID;
		upMessIdAdd();
		rt_kprintf("%s len:%d\r\n",sign,len);
		rt_kprintf("\r\n%slen��%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",sign,len,NetTxBuffer[0],NetTxBuffer[1],NetTxBuffer[2],NetTxBuffer[3]);

		rt_free(sprinBuf);
		sprinBuf=RT_NULL;

		return len;
}


//���������� ע����Ϣ����һ��task  ����ά��
//1������������������ÿ�η��ͺ�������ʱ�� ����ղ�����Ӧ��һֱ�ط�  ���5��
//2�������÷������� 
//3�����ͷ�2�� �������͵�  �ͻظ��ģ���һ�Σ�
//  1��task


//extern struct rt_mailbox mbNetSendData;
//void netSendQueue()
//{
//	rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
//}

//����ע�����ݴ��
/*
{    
		"mid": 1234,
	    "packetType": "CMD_DEVICE_REGISTER",
	    "params":     [{        
			"model": "GY280",
			"name": "�������",
			"deviceId": "GYNJLXSD000000457",
			"ip": "192.168.1.108",
			"port": "",
			"type": "1"
		},
		{        
			"model": "GY281",
			"name": "�ӵػ���",
			"deviceId": "GYNJLXSD000000458",
			"ip": "192.168.1.108",
			"port": "",
			"type": "2"
		}
		],
		"timestamp": "1655172531937"
}
*/
/*
{
	"mid": 0,
	"packetType": "CMD_DEVICE_REGISTER",
	"params": [{
		"model": "GY280",
		"name": "????",
		"deviceId": "A000000000001",
		"ip": "",
		"port": "",
		"type": "1"
	}, {
		"model": "GY281",
		"name": "????",
		"deviceId": "A000000000002",
		"ip": "",
		"port": "",
		"type": "2"
	}, {
		"model": "GY282",
		"name": "???",
		"deviceId": "A000000000003",
		"ip": "",
		"port": "",
		"type": "3"
	}, {
		"model": "GY283",
		"name": "?????",
		"deviceId": "A000000000004",
		"ip": "",
		"port": "",
		"type": "4"
	}],
	"timestamp": "17152"
}*/
#if 0 //����Ŀ�õ�utf8��ʽ�ĺ���
uint16_t devRegJsonPack()
{
	char* out = NULL;
	//��������
	cJSON* Array = NULL;
	// ����JSON Object  
	cJSON* root = NULL;
	cJSON* nodeobj = NULL;
//	cJSON* nodeobj_p = NULL;
	root = cJSON_CreateObject();
	if (root == NULL) return 0;
	// ����ڵ㣨��ֵ�ԣ�
	cJSON_AddNumberToObject(root, "mid",mcu.upMessID);
	cJSON_AddStringToObject(root, "packetType","CMD_DEVICE_REGISTER");
	cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
	char *sprinBuf=RT_NULL;
	sprinBuf=rt_malloc(20);//20���ַ������� ������
	{
			Array = cJSON_CreateArray();
			if (Array == NULL) return 0;
			cJSON_AddItemToObject(root, "params", Array);
			for(int i=0;i<MODBUS_NUM;i++){
			switch(i)
			{
				case CIRCULA:
					for(int j=0;j<CIRCULA_485_NUM;j++){//�˶���û�����ù�
							if(sheet.cirCula[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.cirCula[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.cirCula[j].ID));
							}
					}
				break;
				case PARTDISCHAG:
					for(int j=0;j<PARTDISCHAG_485_NUM;j++){//�˶���û�����ù�
							if(sheet.partDischag[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.partDischag[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.partDischag[j].ID));
							}
					}
				break;
				case PRESSSETTL:
					for(int j=0;j<PRESSSETTL_485_NUM;j++){//�˶���û�����ù�
							if(sheet.pressSetl[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.pressSetl[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.pressSetl[j].ID));
							}
					}
				break;
				case THREEAXIS:
					for(int j=0;j<THREEAXIS_485_NUM;j++){//�˶���û�����ù�
							if(sheet.threeAxiss[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.threeAxiss[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.threeAxiss[j].ID));
							}
					}
				break;
#ifdef USE_4GAS
				case CH4:
					for(int j=0;j<CH4_485_NUM;j++){//�˶���û�����ù�
							if(sheet.ch4[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.ch4[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.ch4[j].ID));
							}
					}
				break;
				case O2:
					for(int j=0;j<O2_485_NUM;j++){//�˶���û�����ù�
							if(sheet.o2[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.o2[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.o2[j].ID));
							}
					}
				break;
				case H2S:
					for(int j=0;j<H2S_485_NUM;j++){//�˶���û�����ù�
							if(sheet.h2s[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.h2s[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.h2s[j].ID));
							}
					}
				break;
				case CO:
					for(int j=0;j<CO_485_NUM;j++){//�˶���û�����ù�
							if(sheet.co[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.co[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.co[j].ID));
							}
					}
				break;
#endif
				case TEMPHUM:
					for(int j=0;j<TEMPHUM_485_NUM;j++){//�˶���û�����ù�
							if(sheet.tempHum[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.tempHum[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.tempHum[j].ID));
							}
					}
				break;
				case WATERDEPTH:
					for(int j=0;j<WATERDEPTH_485_NUM;j++){//�˶���û�����ù�
							if(sheet.waterDepth[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
									cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.waterDepth[j].model));
									cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(modbusName_utf8[i]));
									cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.waterDepth[j].ID));
							}
					}
				break;
				default:
				break;
			}	
			extern uint8_t analogTemChanl;
			//����ģ����ʪ�ȴ�����
			if(sheet.analog[analogTemChanl].workFlag==RT_TRUE){
					nodeobj = cJSON_CreateObject();
					cJSON_AddItemToArray(Array, nodeobj);
					cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.analog[analogTemChanl].model));
					cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(sheet.analog[analogTemChanl].name));
					cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.analog[analogTemChanl].ID));
			}
		}
	}
	sprintf(sprinBuf,"%llu",utcTime_ms());
	cJSON_AddStringToObject(root,"timestamp",sprinBuf);
	rt_free(sprinBuf);
	sprinBuf=RT_NULL;
	// ��ӡJSON���ݰ�  
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
		
		
//					for(int i=0;i<rt_strlen(out);i++)
//					rt_kprintf("%02X",out[i]);
//			rt_kprintf("\n");
			rt_free(out);
			out=NULL;
	}
	if(root!=NULL){
		cJSON_Delete(root);
		root=NULL;
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
		
		mcu.devRegMessID =mcu.upMessID;
		upMessIdAdd();
		rt_kprintf("%sreg len:%d\r\n",sign,len);
		
//		for(int i=0;i<len;i++)
//				rt_kprintf("%02x",NetTxBuffer[i]);
		rt_kprintf("\r\n%slen��%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",sign,len,NetTxBuffer[0],NetTxBuffer[1],NetTxBuffer[2],NetTxBuffer[3]);
		return len;
}
#endif


//char  test[9000]="1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//	1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//	1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm1234567890qwertyuiopasdfghjklzxcvbnm\
//12345789asdfghj";
//�豸ע��json��ʽ���
#if 1
//����sprintf��ʽ��Լram ���ÿ⺯��
uint16_t devRegJsonPack()
{

	char spBuf[100],spLen;
	// ����ڵ㣨��ֵ�ԣ�
	int len =HEAD_LEN+LENTH_LEN;
	NetTxBuffer[0]='1';//����0����  ����ʹ��strcat
	NetTxBuffer[1]='1';
	NetTxBuffer[2]='1';
	NetTxBuffer[3]='1';
	NetTxBuffer[len]=0;
	spLen=sprintf(spBuf,"{\"mid\":%d,",mcu.upMessID);
	strcat((char *)NetTxBuffer,spBuf);
	strcat((char *)NetTxBuffer,"\"packetType\":\"PROPERTIES_REG\",");
	sprintf(spBuf,"\"acuId\":\"%s\",",packFlash.acuId);
	strcat((char *)NetTxBuffer,spBuf);
	strcat((char *)NetTxBuffer,"\"params\":[");
	
	for(int j=0;j<CIRCULA_485_NUM;j++){//�˶���û�����ù�
		if(sheet.cirCula[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.cirCula[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[CIRCULA]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.cirCula[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",GROUNDING_CURRENT);
				strcat((char *)NetTxBuffer,spBuf);

		}
	}
	for(int j=0;j<PARTDISCHAG_485_NUM;j++){//�˶���û�����ù�
		if(sheet.partDischag[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.partDischag[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[PARTDISCHAG]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.partDischag[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",DISCHARGE);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
	

	
	
	for(int j=0;j<PRESSSETTL_485_NUM;j++){//�˶���û�����ù�
		if(sheet.pressSetl[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.pressSetl[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[PRESSSETTL]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.pressSetl[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",SETTLEMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
	for(int j=0;j<THREEAXIS_485_NUM;j++){//�˶���û�����ù�
		if(sheet.threeAxiss[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.threeAxiss[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[THREEAXIS]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.threeAxiss[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",VIBRATION);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
#ifdef USE_4GAS

 	for(int j=0;j<CH4_485_NUM;j++){//�˶���û�����ù�
		if(sheet.ch4[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.ch4[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[CH4]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.ch4[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",AIR_ENVIRONMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
	for(int j=0;j<O2_485_NUM;j++){//�˶���û�����ù�
		if(sheet.o2[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.o2[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[O2]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.o2[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",AIR_ENVIRONMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
	for(int j=0;j<H2S_485_NUM;j++){//�˶���û�����ù�
		if(sheet.h2s[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.h2s[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[H2S]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.h2s[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",AIR_ENVIRONMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
	for(int j=0;j<CO_485_NUM;j++){//�˶���û�����ù�
		if(sheet.co[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.co[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[CO]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.co[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",AIR_ENVIRONMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
#endif
 	for(int j=0;j<TEMPHUM_485_NUM;j++){//�˶���û�����ù�
		if(sheet.tempHum[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.tempHum[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[TEMPHUM]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.tempHum[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",AIR_ENVIRONMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
 	for(int j=0;j<WATERDEPTH_485_NUM;j++){//�˶���û�����ù�
		if(sheet.waterDepth[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.waterDepth[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[WATERDEPTH]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.waterDepth[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",AIR_ENVIRONMENT);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
 	for(int j=0;j<CRACKMETER_485_NUM;j++){//�˶���û�����ù�
		if(sheet.crackMeter[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.crackMeter[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[CRACKMETER]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.crackMeter[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",CRACK_TESTER);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
 	for(int j=0;j<COVER_485_NUM;j++){//�˶���û�����ù�
		if(sheet.cover[j].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",sheet.cover[j].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",modbusName[COVER]);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",sheet.cover[j].ID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",MANHOLE_COVER);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
 	for(int i=0;i<DI_NUM;i++){//�˶���û�����ù�
		if(packFlash.diginput[i].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",packFlash.diginput[i].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",packFlash.diginput[i].name);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",packFlash.diginput[i].devID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",MANHOLE_COVER);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
 	for(int i=0;i<DO_NUM;i++){//�˶���û�����ù�
		if(packFlash.digoutput[i].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",packFlash.digoutput[i].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",packFlash.digoutput[i].name);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",packFlash.digoutput[i].devID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",SWITCH_CONTROL);
				strcat((char *)NetTxBuffer,spBuf);
		}
	} 	
	for(int i=0;i<SWITCH_NUM;i++){//�˶���û�����ù�
		if(packFlash.switchoutput[i].workFlag==RT_TRUE){
				strcat((char *)NetTxBuffer,"{\"device\":{");
				sprintf(spBuf,"\"model\":\"%s\",",packFlash.switchoutput[i].model);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"name\":\"%s\",",packFlash.switchoutput[i].name);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"deviceId\":\"%s\",",packFlash.switchoutput[i].devID);
				strcat((char *)NetTxBuffer,spBuf);
				sprintf(spBuf,"\"type\":%d}},",SWITCH_CONTROL);
				strcat((char *)NetTxBuffer,spBuf);
		}
	}
	NetTxBuffer[strlen((char *)NetTxBuffer)-1]=0;//ȥ�����һ��,��
	
	
	sprintf(spBuf,"],\"timestamp\":\"%llu\"}",utcTime_ms());
	strcat((char *)NetTxBuffer,spBuf);
	
	// ��ӡJSON���ݰ�  
#endif
	len=rt_strlen((char *)NetTxBuffer);
				for(int i=4;i<len;i++)
					rt_kprintf("%c",NetTxBuffer[i]);
			rt_kprintf("\n");
  if(USE_MQTT){

		NetTxBuffer[0]=0xff;
		NetTxBuffer[1]=0xff;
		NetTxBuffer[2]=0xff;
		NetTxBuffer[3]=0xff;

	  NetTxBuffer[len]=0;

		rt_kprintf("\n");
		extern int packMqtt();
		packMqtt();
	}
	else{
	
	// �ͷ��ڴ�  
			for(int i=4;i<len;i++)
					rt_kprintf("%c",NetTxBuffer[i]);
			rt_kprintf("\n");

		NetTxBuffer[0]= (uint8_t)(HEAD>>8); 
		NetTxBuffer[1]= (uint8_t)(HEAD);    


		//lenth
	  NetTxBuffer[2]=(uint8_t)((len-HEAD_LEN-LENTH_LEN)>>8);//����json����
	  NetTxBuffer[3]=(uint8_t)(len-HEAD_LEN-LENTH_LEN);
	  uint16_t jsonBodyCrc=RTU_CRC(NetTxBuffer+HEAD_LEN+LENTH_LEN,len-HEAD_LEN-LENTH_LEN);
	  //crc
	  NetTxBuffer[len]=(uint8_t)(jsonBodyCrc>>8); len++;//����crc
	  NetTxBuffer[len]=(uint8_t)(jsonBodyCrc);    len++;

		//tail
		NetTxBuffer[len]=(uint8_t)(TAIL>>8); len++;
		NetTxBuffer[len]=(uint8_t)(TAIL);    len++;
		NetTxBuffer[len]=0;//len++;//��β ��0
	}
		mcu.devRegMessID =mcu.upMessID;
		upMessIdAdd();
		rt_kprintf("%sreg len:%d\r\n",sign,len);
//		for(int i=0;i<len;i++)
//				rt_kprintf("%02x",NetTxBuffer[i]);
		rt_kprintf("\r\n%slen��%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",sign,len,NetTxBuffer[0],NetTxBuffer[1],NetTxBuffer[2],NetTxBuffer[3]);
		return len;
}


//�豸ע��json��ʽ���
#if 0
uint16_t devRegJsonPack()
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
	cJSON_AddNumberToObject(root, "mid",mcu.upMessID);
	cJSON_AddStringToObject(root, "packetType","PROPERTIES_REG");
	cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
	char *sprinBuf=RT_NULL;
	sprinBuf=rt_malloc(20);//20���ַ������� ������
	{
			Array = cJSON_CreateArray();
			if (Array == NULL) return 0;
			cJSON_AddItemToObject(root, "params", Array);
////////////////////////modbus��ʽ�豸ע����////////////////////////		
			for(int i=0;i<MODBUS_NUM;i++){
			switch(i)
			{

#if  1
				case CIRCULA:
					for(int j=0;j<CIRCULA_485_NUM;j++){//�˶���û�����ù�
							if(sheet.cirCula[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
                  nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);

									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.cirCula[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.cirCula[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",GROUNDING_CURRENT);
							}
					}
				break;
				case PARTDISCHAG:
					for(int j=0;j<PARTDISCHAG_485_NUM;j++){//�˶���û�����ù�
							if(sheet.partDischag[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								  nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.partDischag[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.partDischag[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",DISCHARGE);
							}
					}
				break;
				case PRESSSETTL:
					for(int j=0;j<20;j++){//�˶���û�����ù�PRESSSETTL_485_NUM
							if(sheet.pressSetl[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								  nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.pressSetl[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.pressSetl[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type", SETTLEMENT);
							}
					}
				break;
				case THREEAXIS:
					for(int j=0;j<20;j++){//�˶���û�����ù�THREEAXIS_485_NUM
							if(sheet.threeAxiss[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.threeAxiss[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.threeAxiss[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",VIBRATION);
							}
					}
				break;
#ifdef USE_4GAS
				case CH4:
					for(int j=0;j<CH4_485_NUM;j++){//�˶���û�����ù�
							if(sheet.ch4[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.ch4[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.ch4[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",AIR_ENVIRONMENT);
							}
					}
				break;
				case O2:
					for(int j=0;j<O2_485_NUM;j++){//�˶���û�����ù�
							if(sheet.o2[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.o2[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.o2[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",AIR_ENVIRONMENT);
							}
					}
				break;
				case H2S:
					for(int j=0;j<H2S_485_NUM;j++){//�˶���û�����ù�
							if(sheet.h2s[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.h2s[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.h2s[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",AIR_ENVIRONMENT);
							}
					}
				break;
				case CO:
					for(int j=0;j<CO_485_NUM;j++){//�˶���û�����ù�
							if(sheet.co[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.co[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.co[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",AIR_ENVIRONMENT);
							}
					}
				break;
#endif
				case TEMPHUM:
					for(int j=0;j<TEMPHUM_485_NUM;j++){//�˶���û�����ù�
							if(sheet.tempHum[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.tempHum[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.tempHum[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",AIR_ENVIRONMENT);
							}
					}
				break;
				case WATERDEPTH:
					for(int j=0;j<WATERDEPTH_485_NUM;j++){//�˶���û�����ù�
							if(sheet.waterDepth[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.waterDepth[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.waterDepth[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",WATER_LEVEL);
							}
					}
				break;
				case CRACKMETER:
					for(int j=0;j<CRACKMETER_485_NUM;j++){//�˶���û�����ù�
							if(sheet.crackMeter[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.crackMeter[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.crackMeter[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",CRACK_TESTER);
							}
					}
				break;
					

				case COVER:
					for(int j=0;j<COVER_485_NUM;j++){//�˶���û�����ù�
							if(sheet.cover[j].workFlag==RT_TRUE){
									nodeobj = cJSON_CreateObject();
									cJSON_AddItemToArray(Array, nodeobj);
								nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
									cJSON_AddItemToObject(nodeobj_p,"model",cJSON_CreateString(sheet.cover[j].model));
									cJSON_AddItemToObject(nodeobj_p,"name",cJSON_CreateString(modbusName[i]));
									cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(sheet.cover[j].ID));
								  cJSON_AddNumberToObject(nodeobj_p,"type",MANHOLE_COVER);
							}
					}
				break;
				default:
				break;
#endif
			}	
		}
//		extern uint8_t analogTemChanl;
//	 //����ģ����ʪ�ȴ�����  �ֶ������
//		if(sheet.analog[analogTemChanl].workFlag==RT_TRUE){
//				nodeobj = cJSON_CreateObject();
//				cJSON_AddItemToArray(Array, nodeobj);
//				cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.analog[analogTemChanl].model));
//				cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(sheet.analog[analogTemChanl].name));
//				cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.analog[analogTemChanl].ID));
//		}
		int i;
//		bool  add=true;
////////////////////////////////////////////////ģ�����ݴ��////////////////////////////////////////////////////
#ifndef     ANA_MASK
		for(i=0;i<ANALOG_NUM;i++){
			  add=true;
				if(i>=1){
						for(j=0;j<i;j++){
								if(rt_strcmp(sheet.analog[i].ID,sheet.analog[j].ID)==0){
										add=false;//�ҵ����ظ��� �����˴�i�ļ���
									  break;
								}
						}
				}
				if(add==false){
					  continue;//�����˴�ѭ�� ִ����һ��
				}
				if(sheet.analog[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
						cJSON_AddItemToObject(nodeobj,"model",cJSON_CreateString(sheet.analog[i].model));
						cJSON_AddItemToObject(nodeobj,"name",cJSON_CreateString(sheet.analog[i].name));
						cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.analog[i].ID));
				}
		 }
#endif
////////////////////////////////////////////////����������////////////////////////////////////////////////////
		 for(i=0;i<DI_NUM;i++){
//			  add=true;
//				if(i>=1){
//						for(j=0;j<i;j++){
//								if(rt_strcmp(packFlash.diginput[i].devID,packFlash.diginput[j].devID)==0){
//										add=false;//�ҵ����ظ��� �����˴�i�ļ���
//									  break;
//								}
//						}
//				}
//				if(add==false){
//					  continue;//�����˴�ѭ�� ִ����һ��
//				}
				if(packFlash.diginput[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
					nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
						cJSON_AddItemToObject(nodeobj_p,"model",  cJSON_CreateString(packFlash.diginput[i].model));
						cJSON_AddItemToObject(nodeobj_p,"name",   cJSON_CreateString(packFlash.diginput[i].name));
						cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(packFlash.diginput[i].devID));
					  cJSON_AddNumberToObject(nodeobj_p,"type",SWITCH_CONTROL);
				}
		 }
////////////////////////////////////////////////����������////////////////////////////////////////////////////
		 for(i=0;i<DO_NUM;i++){
//			  add=true;
//				if(i>=1){
//						for(j=0;j<i;j++){
//								if(rt_strcmp(packFlash.digoutput[i].devID,packFlash.digoutput[j].devID)==0){
//										add=false;//�ҵ����ظ��� �����˴�i�ļ���
//									  break;
//								}
//						}
//				}
//				if(add==false){
//					  continue;//�����˴�ѭ�� ִ����һ��
//				}
				if(packFlash.digoutput[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
					nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
						cJSON_AddItemToObject(nodeobj_p,"model",   cJSON_CreateString(packFlash.digoutput[i].model));
						cJSON_AddItemToObject(nodeobj_p,"name",    cJSON_CreateString(packFlash.digoutput[i].name));
						cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(packFlash.digoutput[i].devID));
					  cJSON_AddNumberToObject(nodeobj_p,"type",SWITCH_CONTROL);
				}
		 }
////////////////////////////////////////////////���3V3���////////////////////////////////////////////////////
		 for(i=0;i<V33O_NUM;i++){
//			  add=true;
//				if(i>=1){
//						for(j=0;j<i;j++){
//								if(rt_strcmp(packFlash.v33output[i].devID,packFlash.v33output[j].devID)==0){
//										add=false;//�ҵ����ظ��� �����˴�i�ļ���
//									  break;
//								}
//						}
//				}
//				if(add==false){
//					  continue;//�����˴�ѭ�� ִ����һ��
//				}
				if(packFlash.v33output[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
					nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
						cJSON_AddItemToObject(nodeobj_p,"model",   cJSON_CreateString(packFlash.v33output[i].model));
						cJSON_AddItemToObject(nodeobj_p,"name",    cJSON_CreateString(packFlash.v33output[i].name));
						cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(packFlash.v33output[i].devID));
					  cJSON_AddNumberToObject(nodeobj_p,"type",SWITCH_CONTROL);
				}
		 }
////////////////////////////////////////////////���5Vע����////////////////////////////////////////////////////
		 for(i=0;i<V5O_NUM;i++){
//			  add=true;
//				if(i>=1){
//						for(j=0;j<i;j++){
//								if(rt_strcmp(packFlash.v5output[i].devID,packFlash.v5output[j].devID)==0){
//										add=false;//�ҵ����ظ��� �����˴�i�ļ���
//									  break;
//								}
//						}
//				}
//				if(add==false){
//					  continue;//�����˴�ѭ�� ִ����һ��
//				}
				if(packFlash.v5output[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
					nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
						cJSON_AddItemToObject(nodeobj_p,"model",   cJSON_CreateString(packFlash.v5output[i].model));
						cJSON_AddItemToObject(nodeobj_p,"name",    cJSON_CreateString(packFlash.v5output[i].name));
						cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(packFlash.v5output[i].devID));
					  cJSON_AddNumberToObject(nodeobj_p,"type",SWITCH_CONTROL);
				}
		 }
////////////////////////////////////////////////���12Vע����////////////////////////////////////////////////////
		 for(i=0;i<SWITCH_NUM;i++){
//			  add=true;
//				if(i>=1){
//						for(j=0;j<i;j++){
//								if(rt_strcmp(packFlash.switchoutput[i].devID,packFlash.switchoutput[j].devID)==0){
//										add=false;//�ҵ����ظ��� �����˴�i�ļ���
//									  break;
//								}
//						}
//				}
//				if(add==false){
//					  continue;//�����˴�ѭ�� ִ����һ��
//				}
				if(packFlash.switchoutput[i].workFlag==RT_TRUE){
						nodeobj = cJSON_CreateObject();
						cJSON_AddItemToArray(Array, nodeobj);
					nodeobj_p= cJSON_CreateObject();
									cJSON_AddItemToObject(nodeobj, "device", nodeobj_p);
						cJSON_AddItemToObject(nodeobj_p,"model",   cJSON_CreateString(packFlash.switchoutput[i].model));
						cJSON_AddItemToObject(nodeobj_p,"name",    cJSON_CreateString(packFlash.switchoutput[i].name));
						cJSON_AddItemToObject(nodeobj_p,"deviceId",cJSON_CreateString(packFlash.switchoutput[i].devID));
					  cJSON_AddNumberToObject(nodeobj_p,"type",SWITCH_CONTROL);
				}
		 }
	}
	sprintf(sprinBuf,"%llu",utcTime_ms());
	cJSON_AddStringToObject(root,"timestamp",sprinBuf);
	rt_free(sprinBuf);
	sprinBuf=RT_NULL;
	// ��ӡJSON���ݰ�  
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
		rt_kprintf("reg len %d\n",rt_strlen(out));
		
//					for(int i=0;i<rt_strlen(out);i++)
//					rt_kprintf("%02X",out[i]);
//			rt_kprintf("\n");
			rt_free(out);
			out=NULL;
	}
	if(root!=NULL){
		cJSON_Delete(root);
		root=NULL;
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
		
		mcu.devRegMessID =mcu.upMessID;
		upMessIdAdd();
		rt_kprintf("%sreg len:%d\r\n",sign,len);
		
//		for(int i=0;i<len;i++)
//				rt_kprintf("%02x",NetTxBuffer[i]);
		rt_kprintf("\r\n%slen��%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",sign,len,NetTxBuffer[0],NetTxBuffer[1],NetTxBuffer[2],NetTxBuffer[3]);
		return len;
}

#endif