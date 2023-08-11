
#include "board.h"
#include "mqttTask.h"
#include "transport.h"

/*
{"clientId":"a1S9kgxkc8w.humi_protect1|securemode=2,signmethod=hmacsha256,timestamp=1656814731352|",
 "username":"humi_protect1&a1S9kgxkc8w",
 "mqttHostUrl":"a1S9kgxkc8w.iot-as-mqtt.cn-shanghai.aliyuncs.com",
 "passwd":"652b0dc2b786a08a7ab11612246731f87f50e32b78cc6bf9ed3a3a5041c7635e",
 "port":1883}
*/
static char task[]="[mqttDataPh]";
//extern uint8_t  NetRxBuffer[TX_RX_MAX_BUF_SIZE];
static char sendBuf[1024];//����mqtt״̬ά������  ��Ҫ�ƻ�NetTxBuffer������
static char sendBufLen;
//extern uint16_t netRxBufLen;
extern rt_sem_t      MQTTDiscon_semp;
extern rt_sem_t      wifiTcpOK_semp;
MQTTDataPacktStru  MQTTDataPackt[MQTT_PACK_CNT]={NULL};
char deviceName[]="temp_0714_001";					
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;


static bool MQTTState=false;// true con   false nocon

#define  BROKER_RESP_TIMES    300

void offLineGetTimeStart(void);
int gu32PingFailTimes=0;
bool mqttState(void)
{
		return MQTTState;
}

//�����Ͻ� ��Ҫ�����ź�������
void mqttStateSet(bool state)
{
	  MQTTState=state;

}
//����ע���topic���
static int  mqttFindTopic(char *topic)
{
	    int i;
	    if(topic==NULL)
					return -1;
	    for( i=0;i<MQTT_PACK_CNT;i++){
						if(MQTTDataPackt[i].topic!=NULL){
							  if(strncmp(MQTTDataPackt[i].topic,topic,strlen(MQTTDataPackt[i].topic))==0){
										rt_kprintf("%stopic has regist %d\r\n",task,i);
										return i;
								}
						}
			}
			return -2;
}
//ע�ᶩ�ĵ�topic �Լ��ص�����
static void  mqttRegstTopicCallbak(char *topic,int (*fun)(char *buf))
{
			int i;
	    rt_kprintf("%smqttRegstTopic\r\n",task);
	    if(topic==0)
					return;
			if(mqttFindTopic(topic)>=0){//���˵��Ѿ�ע�����
				  rt_kprintf("%stopic have been regist\r\n",task);
					return;
			}
	    for( i=0;i<MQTT_PACK_CNT;i++){
						if(MQTTDataPackt[i].topic==NULL){
								MQTTDataPackt[i].topic=rt_malloc(strlen(topic)+1);
								MQTTDataPackt[i].fun=fun;
							  if(fun!=NULL){
										rt_kprintf("%sregist callback succ\r\n",task);
								}
								strcpy(MQTTDataPackt[i].topic,topic);
							  rt_kprintf("%stopic  regist %d len=%d\r\n",task,i,strlen(topic));
								break;
						}
			}
}
//�����topic�ͷ��ڴ�
static void  mqttCleanTopic()
{
	    int i;
	    rt_kprintf("%smqttCleanTopic begin\r\n",task);
	    for( i=0;i<MQTT_PACK_CNT;i++){
						if(MQTTDataPackt[i].topic!=NULL){
								rt_kprintf("%sfree %d st topic\r\n",task, i);
								rt_free(MQTTDataPackt[i].topic);
							  MQTTDataPackt[i].topic=NULL;
						}
			}
			rt_kprintf("%smqttCleanTopic end\n\r",task);
}




//�������õĲ�������ʾ  /sys/a1X8eraA4gE/%s/thing/service/property/set
//{"method":"thing.service.property.set","id":"1475268842","params":{"CurrentHumidity":80.8,"CurrentTemperature":81.8},"version":"1.0.0"}
//static int getSetJsoin(char *buf)
//{
//		rt_kprintf("%sgetsetJson\n\r",task);				
//		cJSON  *Json=NULL;
//		Json = cJSON_Parse(buf);
//		cJSON  *params   =cJSON_GetObjectItem(Json,"params");
//		char *out =cJSON_Print(params);
//		  if(out!=NULL){
//				rt_free(out);
//		}
//		rt_kprintf("%sJson %s\n\r",task,out);
//		double  humi     = cJSON_GetObjectItem(params,"CurrentHumidity")->valuedouble;
//		double  temp     = cJSON_GetObjectItem(params,"CurrentTemperature")->valuedouble;
//	  if(Json!=RT_NULL){
//				cJSON_Delete(Json);
//		}
//		rt_kprintf("%ssethumi[%lf] settemp[%lf]\n\r",task,humi,temp);	
//		return 1;
//}




uint32_t e;
extern struct rt_event mqttAckEvent;

//ע���˻ص�����
int  dataPhrase(char *json)
{
	rt_mutex_take(read485_mutex,RT_WAITING_FOREVER);
	extern void AllDownPhrase(char *data,int lenth);
	AllDownPhrase(json,0);//����û���õ�
	rt_mutex_release(read485_mutex);
	return 1;
}
//int mqttRet=-1;
//mqtt��ʵ�ֶ��Ĺ���



static void mqttSubFun()
{
	/* subscribe */
		MQTTString topicString = MQTTString_initializer;
		int req_qos = 0;
   // int i=0;
		char *topic=rt_malloc(100);

		rt_sprintf(topic,"/acu/%s/down",packFlash.acuId);
		mqttRegstTopicCallbak(topic,dataPhrase);
//	  rt_sprintf(topic,"/sys/a1X8eraA4gE/%s/thing/service/property/set",deviceName);
//	  mqttRegstTopicCallbak(topic,getSetJsoin);
//		rt_sprintf(topic,"/ext/ntp/a1X8eraA4gE/%s/response",deviceName);
//	  mqttRegstTopicCallbak(topic,NULL);//ע�� topic
	  rt_free(topic);
	  topic=NULL;
				
	  for(int i=0;i<MQTT_PACK_CNT;i++){
			  if(MQTTDataPackt[i].topic!=NULL){
						topicString.cstring = MQTTDataPackt[i].topic;
					  rt_kprintf("%smqttSub %s  %s\n\r",task,topicString.cstring,MQTTDataPackt[i].topic);

						rt_kprintf("%smqttSub %d topic\n\r",task,i);
						sendBufLen = MQTTSerialize_subscribe((unsigned char*)sendBuf+PACK_HEAD_LEN, sizeof(sendBuf), 0, 0, 1, &topicString, &req_qos);
         
						sendBuf[0]=(uint8_t)(sendBufLen>>24);
						sendBuf[1]=(uint8_t)(sendBufLen>>16);
						sendBuf[2]=(uint8_t)(sendBufLen>>8);
						sendBuf[3]=(uint8_t)sendBufLen;
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&sendBuf,RT_WAITING_FOREVER); 
					  if (rt_event_recv(&mqttAckEvent, (EVENT_SUBACK),
                       RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
                      BROKER_RESP_TIMES, &e) == RT_EOK)
						{
								//rt_kprintf("thread1: OR recv event 0x%x\n", e);
							  rt_kprintf("%smqttSub ack\r\n",task);
						}
				}
		}
}



//mqtt�յ����ĵ���Ϣ����
bool  mqttpubRead(uint8_t *rxbuf,int len)
{

					unsigned char dup;
					int qos;
					unsigned char retained;
					unsigned short msgid;
					int payload_in_len;
					unsigned char* payload_in;
          uint8_t *rxbufP=rxbuf;
	        int rxLen=len;
					MQTTString receivedTopic;
					int rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
							&payload_in, &payload_in_len, (uint8_t *)rxbufP,  rxLen);
					rt_kprintf("%srec topic: %.*s [%d]\r\n",task,receivedTopic.lenstring.len,receivedTopic.lenstring.data,receivedTopic.lenstring.len);
					int topicNum = mqttFindTopic(receivedTopic.lenstring.data);
				  if(topicNum>=0){
						  rt_kprintf("%stopicNum =%d  payloadlen=%d\r\n",task,topicNum,payload_in_len);
							MQTTDataPackt[topicNum].msglen  = payload_in_len;
							MQTTDataPackt[topicNum].msg			= rt_malloc(payload_in_len+1);//�����1   ���ַ���
							//strcpy(MQTTDataPackt[topicNum].msg,(char *)payload_in);//�˾���ܵ���(MEM_POOL(&small_mem->heap_ptr[mem->next]) == small_mem) assertion failed at function:rt_smem_free, line number:542 
							strncpy(MQTTDataPackt[topicNum].msg,(char *)payload_in,payload_in_len);
							MQTTDataPackt[topicNum].msg[payload_in_len]=0;//�˾���ܵ���
						  rt_kprintf("%stopicNum %d\r\n",task,topicNum);	
						  if(MQTTDataPackt[topicNum].fun!=NULL){
									MQTTDataPackt[topicNum].fun(MQTTDataPackt[topicNum].msg);//ִ�лص�����
							}
							else{
									rt_kprintf("%stopic fun is NULL\r\n",task);
							}
							rt_kprintf("%srec message: %.*s \r\n",task, MQTTDataPackt[topicNum].msglen, MQTTDataPackt[topicNum].msg);//
//							if(MQTTDataPackt[topicNum].msg==NULL)
//							{
//								rt_kprintf("msg is enpty\n");
//							}
//							else{
//								rt_kprintf("msg is not enpty\n");
//							}
//					  	rt_kprintf("test1\r\n");
						  rt_free(MQTTDataPackt[topicNum].msg);//free ����  suqi
//rt_kprintf("test2\r\n");
						  MQTTDataPackt[topicNum].msg=NULL;
						  MQTTDataPackt[topicNum].msglen=0;
//rt_kprintf("test3\r\n");
					}
					else
							rt_kprintf("%snot find topic message arrived %.*s   topic:%s  %.*s [%d]\r\n",task, payload_in_len, payload_in,receivedTopic.cstring,receivedTopic.lenstring.len,receivedTopic.lenstring.data,receivedTopic.lenstring.len);

			return false;
}


static MQTTEnum MQTTstep=conMQTT_enum; 

static uint32_t respTime=0 ;
void getMqttRespTime()
{
	respTime = rt_tick_get();
}

extern rt_bool_t gbNetState;
//mqtt״̬��ά�� �����շ�
int ping2Times=0;
int pingRspTimes=0;
int pingUnrspTimes=0;
//bool  mqttConStat=false;
int  mqttLoopData(void)
{

	  static int ret=0;
	  static int conTimes=0;
	  static uint8_t pingTimes=0;
	  int  tep=0;
		static bool reg2Flag=false;
		switch(MQTTstep){
			case  conMQTT_enum:
				    //�豸  humi_protect1

#ifdef USE_ALIYUN
			 //������ip 47.103.184.125 port 1883
						connectData.clientID.lenstring.data	=	"a1S9kgxkc8w.humi_protect1|securemode=2,signmethod=hmacsha256,timestamp=1656924907170|";
						connectData.clientID.lenstring.len	=	strlen(connectData.clientID.lenstring.data);
						connectData.username.lenstring.data	=	"humi_protect1&a1S9kgxkc8w";
						connectData.username.lenstring.len	=	strlen(connectData.username.lenstring.data);
						connectData.password.lenstring.data	=	"d7810ceaaa1370606f58d64d443182af6de70c6daff8297b34bf08081e0fdde2";
						connectData.password.lenstring.len	=	strlen(connectData.password.lenstring.data);
			      //�豸 temp_test001
#else  //������ ������͸  115.236.153.174  15260
			
			      memcpy(connectData.clientID.lenstring.data,packFlash.acuId,strlen(packFlash.acuId));
						connectData.clientID.lenstring.data	=	"fa19d97c0364436bb166e92979ce5408suqi";//���ڲ��ø�ʽ�������ַ�����  ��ֹ���ֶ��clientid��ͬ��� error
						connectData.clientID.lenstring.len	=	strlen(connectData.clientID.lenstring.data);
						connectData.username.lenstring.data	=	"admin";
						connectData.username.lenstring.len	=	strlen(connectData.username.lenstring.data);
						connectData.password.lenstring.data	=	"suqi201301";
						connectData.password.lenstring.len	=	strlen(connectData.password.lenstring.data);
#endif
						connectData.keepAliveInterval = MQTTKEEPALIVE_TIME_S;
						if((sendBufLen=MQTTSerialize_connect((unsigned char *)sendBuf+PACK_HEAD_LEN, sizeof(sendBuf), &connectData))!=0){
								rt_kprintf("%stry to con [%d]\r\n",task,sendBufLen);
								sendBuf[0]=(uint8_t)(sendBufLen>>24);
								sendBuf[1]=(uint8_t)(sendBufLen>>16);
								sendBuf[2]=(uint8_t)(sendBufLen>>8);
								sendBuf[3]=(uint8_t)sendBufLen;
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&sendBuf,RT_WAITING_FOREVER); 
						}
						if (rt_event_recv(&mqttAckEvent,EVENT_CONNACK,RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR, BROKER_RESP_TIMES, &e) == RT_EOK)
						{//qos=0 
								rt_kprintf("%sPUB reading CONNACK\r\n",task);//����3 7 6 6�İ�
							  MQTTstep	=	subMQTT_enum;
							  mqttStateSet(true);
							  conTimes=0;
								extern void LCDDispNetOffline();
								LCDDispNetOffline();
						}
						else{

							  rt_kprintf("%sno connack\r\n",task);//����3 7 6 6�İ�
							  if(++conTimes>=3){//  dis conn ��  con��3�β�����
										MQTTstep	=	resetMQTT_enum;
										tep= -1;
									  conTimes=0;
								}
								rt_thread_delay(1000);
						}
				break;
			case 	subMQTT_enum:
						mqttSubFun();
			      MQTTstep	=	pubGetUtcMqtt_enum;
				break;
			case pubGetUtcMqtt_enum:
						MQTTstep=pubMQTT_enum;
			      

			case  pubMQTT_enum:
						MQTTstep	=	dealwithMQTT_enum;
			case  pingMQTT_enum:
						ping2Times++;
				    rt_kprintf("%sping \r\n",task);
						sendBufLen=MQTTSerialize_pingreq((uint8_t *)sendBuf+PACK_HEAD_LEN, sizeof(sendBuf));			
						rt_kprintf("%stry to ping [%d]\r\n",task,sendBufLen);	
						sendBuf[0]=(uint8_t)(sendBufLen>>24);
						sendBuf[1]=(uint8_t)(sendBufLen>>16);
						sendBuf[2]=(uint8_t)(sendBufLen>>8);
						sendBuf[3]=(uint8_t)sendBufLen;
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&sendBuf,RT_WAITING_FOREVER); 			
						if (rt_event_recv(&mqttAckEvent, (EVENT_PINGRESP),
                       RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,
                      BROKER_RESP_TIMES, &e) == RT_EOK){
								pingRspTimes++;
								rt_kprintf("%sPong\r\n",task);
							  pingTimes=0;
								MQTTstep=dealwithMQTT_enum;
								getMqttRespTime();
//							  respTime=rt_tick_get();
						}
						else {
								pingUnrspTimes++;
								rt_kprintf("%sOOPS %d\r\n",task,ret);
								gu32PingFailTimes++;
							  if(pingTimes++>=2){//���жϺ�++
//									  MQTTstep	=	resetMQTT_enum;
//										mqttStateSet(false);
									
										extern void rstMqttStep();
										rstMqttStep();
			
										if(gbNetState==RT_TRUE){
												gbNetState = RT_FALSE;
			
										}
								}
						}
						rt_kprintf("ping[%d],pingrsp[%d],pingUnrsp[%d]\n",ping2Times,pingRspTimes,pingUnrspTimes);
						
				break;
			case  dealwithMQTT_enum://
						if((rt_tick_get()-respTime)>(MQTTKEEPALIVE_TIME_S)*1000){
								MQTTstep=pingMQTT_enum;
							  break;
						}
						rt_thread_delay(1000);
						if(reg2Flag==false){
								reg2Flag=true;//������ֻע��һ��  �����ɶ�ʱ��ʵ�ַ���ע��
								extern uint16_t devRegJsonPack();
								devRegJsonPack();//devRegJsonPack();
							  rt_kprintf("%s devRegJsonPack\n",task);
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
				  	}
				break;
			case  resetMQTT_enum:
				    mqttCleanTopic();
						rt_kprintf("%sresetMQTT_enum 1\r\n",task);		
				    sendBufLen = (unsigned int)MQTTSerialize_disconnect((unsigned char *)(sendBuf+PACK_HEAD_LEN),sizeof(sendBuf));
						rt_kprintf("%sresetMQTT_enum 2\r\n",task);	
						if(sendBufLen !=0){
								sendBuf[0]=(uint8_t)(sendBufLen>>24);
								sendBuf[1]=(uint8_t)(sendBufLen>>16);
								sendBuf[2]=(uint8_t)(sendBufLen>>8);
								sendBuf[3]=(uint8_t)sendBufLen;
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&sendBuf,RT_WAITING_FOREVER); 
							  rt_kprintf("%sdis conn\r\n",task);	
						}
						else
							  rt_kprintf("%sdis conn err\r\n");
						MQTTstep=conMQTT_enum;
//						rt_event_send(&WDTEvent,EVENT_WDT_MQTTTASK);
						mqttStateSet(false);;
					//	rt_thread_delay(100);
				break;
		}
		return tep;
}	

void rstMqttStep()
{
		if(mqttState()==true){

			mqttStateSet(false);
		}
		if(MQTTstep!=conMQTT_enum)//����������״̬ ����malloc��  ��Ҫ����resetȥfree��
			MQTTstep=resetMQTT_enum;
}


extern struct rt_mutex  test_mutex;





