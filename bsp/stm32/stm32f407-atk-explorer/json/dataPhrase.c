#include   "dataPhrase.h"
#include   "cJSON.h"
//https://blog.csdn.net/woody218/article/details/119634171  json解析参考
extern uint16_t RTU_CRC(uint8_t *puchMsg , uint16_t usDataLen);
void  readModbusDataResp(char *monitor);
void  readAnaDataResp(char *monitor);
void  senseTHGetJsonResp(cJSON   *Json,bool modbusFlag);
void senseTimeReadJsonResp(char *string,bool  modbusFlag);
void senseTHSetJsonResp(cJSON   *Json,bool  modbusFlag);
void senseTimeJsonSet(cJSON   *Json,bool  modbusFlag);
extern uint16_t digitalInputGetResp(cJSON *Json);
uint16_t digitalOutputSetResp(cJSON *Json,char *identify);
uint16_t digitalInputGetResp(cJSON *Json);
uint16_t digitalOutputGetResp(cJSON *Json,char *identify);
uint16_t resetDeviceResp(cJSON *Json,char *identify);
uint16_t resetMcuResp(cJSON *Json);
uint16_t saveMcuResp();
uint16_t logCrtlReadResp(cJSON *Json);
uint16_t logCrtlAddResp(cJSON *Json);
uint16_t logCtrlDel(cJSON *Json);
extern  void coverJsonOnOff(cJSON   *Json);
const static char sign[]="[dataPhrs]";
uint32_t  respMid=0;
//数据校验 头尾 校验和 是否正确
//rt_TRUE 正确 rt_FALSE 错误

rt_bool_t tcpDataCheck(char *data,int lenth)
{
//	1、解析头尾校验 不对丢弃
//	2、提取packettype,分别校验
	  if(lenth<=8)
				return RT_FALSE; //头尾校验至少9个字节
		uint16_t jsonBodyCrc=RTU_CRC((uint8_t *)data+HEAD_LEN+LENTH_LEN,lenth-HEAD_LEN-LENTH_LEN-TAIL_LEN-CRC_LEN);
	  uint16_t dataCrc=(uint16_t)(data[lenth-4]<<8)+data[lenth-3];
	  if(((data[0]<<8)+data[1])!=HEAD){
				rt_kprintf("%shead err  %02x %02x\r\n",sign,data[0],data[1]);
				return RT_FALSE;		
		}
		if(((data[lenth-2]<<8)+data[lenth-1])!=TAIL){
				rt_kprintf("%stail err\r\n",sign);
				return RT_FALSE;		
		}
	  if(lenth!=((data[2]<<8)+data[3]+HEAD_LEN+LENTH_LEN+TAIL_LEN+CRC_LEN)){
				rt_kprintf("%slenth err %d %d\r\n",sign,lenth,((data[2]<<8)+data[3]+HEAD_LEN+LENTH_LEN+TAIL_LEN+CRC_LEN));
				return RT_FALSE;		
		}
	  if(jsonBodyCrc!=dataCrc){
			  rt_kprintf("%scrc err r:0x%04x c:0x%04x\r\n",sign,dataCrc,jsonBodyCrc);
				return RT_FALSE;
		}
		return RT_TRUE;
}

//分别找出下行数据的类型并分类    
packTypeEnum  downLinkPackTpyeGet(cJSON  *TYPE)
{
	  int size =sizeof(typeHeadDown)/sizeof(typeHeadDown[0]);
	
	  for(int i=0;i<size;i++){
				if(rt_strcmp(TYPE->valuestring,typeHeadDown[i])==0){
						return (packTypeEnum)i;
				}
		}
		rt_kprintf("%serr:type head [%s] listsize=%d\n",sign,TYPE->valuestring,size);	
		return ERRRESP;
}


//需要判断devid 和消息ID一致才认为心跳发送成功
rt_bool_t timeSetFun(cJSON  *Json)
{
		uint64_t  nowTime=utcTime_ms();
		cJSON  *time =cJSON_GetObjectItem(Json,"timestamp");
	  
	  rt_kprintf("%stime:%s\n\r",sign,time->valuestring);

		cJSON  *msg =cJSON_GetObjectItem(Json,"msg");
		rt_kprintf("%smsg \"%s\"\r\n",sign,msg->valuestring);

		static uint64_t u64getTick_p;
		u64getTick_p =atoll(time->valuestring);
		rt_kprintf("%stime:[%lu]s \r\n",sign, (uint32_t)((u64getTick_p)/1000));
		rt_kprintf("%stime:[%lu]ms\r\n",sign, (uint32_t)(u64getTick_p)%1000);
	  extern void  subTimeStampSet(uint64_t time);
	  if(abs(nowTime-u64getTick_p)>=30000){
        subTimeStampSet(u64getTick_p);
			  correctLcdTime(u64getTick_p/1000);
			  rt_kprintf("%stime:RTC 误差大于30秒 校时\r\n",sign);
		}
		cJSON  *mid =cJSON_GetObjectItem(Json,"mid");
    if(mcu.upHeartMessID != mid->valueint){
				rt_kprintf("%sheart resp messID err %d %d\r\n",sign,mcu.upHeartMessID,mid->valueint);
			  return RT_FALSE;
			
		}
		cJSON  *code =cJSON_GetObjectItem(Json,"code");
		if(code->valueint!=0){
			  rt_kprintf("%sheart code err %d\r\n",sign,code->valueint);
				return RT_FALSE;
		}
		cJSON  *devid =cJSON_GetObjectItem(Json,"acuId");
		if(strcmp(packFlash.acuId,devid->valuestring)!=0){
				rt_kprintf("%sheart resp acuId err %s\r\n",sign,devid->valuestring);
			  return RT_FALSE;
		}
		return RT_TRUE;
}



//需要判断devid 和消息ID一致才认为注册成功
rt_bool_t comRespFun(cJSON  *Json,uint32_t mesgID)
{
	#if 0
		cJSON  *time =cJSON_GetObjectItem(Json,"timestamp");
	  rt_kprintf("%stime:%s\n\r",sign,time->valuestring);
		static uint64_t u64getTick_p;
		u64getTick_p =atoll(time->valuestring);
	  extern void  subTimeStampSet(uint64_t time);
	  if(utcTime_ms()-u64getTick_p>=3000){
        subTimeStampSet(u64getTick_p);
			  correctLcdTime(u64getTick_p/1000);
			  rt_kprintf("%stime:RTC 误差大于3秒 校时\r\n",sign);
		}
	#endif   //suqi  只用心跳回应来校时
		cJSON  *msg =cJSON_GetObjectItem(Json,"msg");
		rt_kprintf("%sdown msg \"%s\"\r\n",sign,msg->valuestring);
		//timeSetFun(Json);
		cJSON  *mid =cJSON_GetObjectItem(Json,"mid");
    if(mesgID!= mid->valueint){
				rt_kprintf("%sreg resp messID err %d %d\r\n",sign,mid->valueint,mesgID);
			  return RT_FALSE;
		}
		cJSON  *code =cJSON_GetObjectItem(Json,"code");
		rt_kprintf("%sreg code  %d\r\n",sign,code->valueint);
		if(code->valueint==1){
			  rt_kprintf("%sreg code err\r\n",sign);
				return RT_FALSE;
		}
		return RT_TRUE;
}
//下行数据解析
rt_bool_t gbNetResp=RT_FALSE;
void AllDownPhrase(char *data,int lenth)
{
//		rt_kprintf("%sphrase len:%d\r\n",sign);
		
		
		char *Buffer=NULL;
		if(USE_MQTT){
			Buffer=data;		
		}
		else{

			if(tcpDataCheck(data,lenth)==RT_FALSE){
				
					return;
			}
			gbNetResp=RT_TRUE;
			char *buf=data+HEAD_LEN+LENTH_LEN;//偏移后是真实的json数据
			int  len=lenth-HEAD_LEN-LENTH_LEN-TAIL_LEN-CRC_LEN;//获取真实的json数据长度
			Buffer=(char *)rt_malloc(len+1);
			rt_strncpy(Buffer,buf,len);
			Buffer[len]=0;
		}	
		
//		for(int i=0;i<len;i++)
//		rt_kprintf("%c",Buffer[i]);
//		rt_kprintf("\r\n");

		//开始解析json
//		rt_kprintf("%sgetJson:%s  \r\n",sign,Buffer);	
		//rt_kprintf("getJson:%.*s  %d\r\n",len,Buffer,len);			
		cJSON  *Json=NULL;
		Json = cJSON_Parse(Buffer);
		
		
		if(Json!=RT_NULL){//解析json数据
				cJSON  *pkType = cJSON_GetObjectItem(Json,"packetType");
			  cJSON  *pkIdentf = cJSON_GetObjectItem(Json,"identifier");
			  cJSON  *acuId = cJSON_GetObjectItem(Json,"acuId");
			  if(strcmp(acuId->valuestring,packFlash.acuId)!=0){
					rt_kprintf("%sERR:acuid   %s\n",sign,acuId->valuestring);
				}
				cJSON  *mid =cJSON_GetObjectItem(Json,"mid");
			  respMid = mid->valueint;
		 
			  switch(downLinkPackTpyeGet(pkType)){

					case	PROPERTIES_HEART_RESP:
						if(!USE_MQTT){
							if(RT_TRUE==timeSetFun(Json)){//收到心跳回应 怎么通知发送层
									rt_kprintf("%sheart resp succ\r\n",sign);
							}
						}
						break;
					case	PROPERTIES_REG_RESP:
						if(RT_TRUE==comRespFun(Json,mcu.devRegMessID)){//收到注册回应 怎么通知发送层
								rt_kprintf("%sreg dev succ\r\n",sign);
							  extern rt_bool_t gbRegFlag;
							  gbRegFlag = RT_TRUE;
						}
						break;
					case	PROPERTIES_485DATA_REP_RESP:
						if(RT_TRUE==comRespFun(Json,mcu.repDataMessID)){//收到数据包回应 怎么通知发送层
								rt_kprintf("%sreport data succ\r\n",sign);
						}
						break;
					case	PROPERTIES_485DATA_GET:
					  readModbusDataResp(pkIdentf->valuestring);
						break;
					case  PROPERTIES_485DATA_SET:
						coverJsonOnOff(Json);
						break;
					case	PROPERTIES_ANADATA_REP_RESP:
						break;
					case	PROPERTIES_ANADATA_GET:
#ifndef     ANA_MASK
					  readAnaDataResp(pkIdentf->valuestring);
#endif
						break;
					case	PROPERTIES_485TIM_GET:
						senseTimeReadJsonResp(pkIdentf->valuestring,true);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_485TIM_SET:
						senseTimeJsonSet(Json,true);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_ANATIM_GET:
						senseTimeReadJsonResp(pkIdentf->valuestring,false);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_ANATIM_SET:
						senseTimeJsonSet(Json,false);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_485TH_GET:
						senseTHGetJsonResp(Json,true);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_485TH_SET:
						senseTHSetJsonResp(Json,true);
					  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_ANATH_GET:
						senseTHGetJsonResp(Json,false);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_ANATH_SET:
						senseTHSetJsonResp(Json,false);
					  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_INPUT_REP_RESP:
						rt_kprintf("%sdi rep response\r\n",sign);
						break;
					case	PROPERTIES_INPUT_GET:
					  digitalInputGetResp(Json);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_OUTPUT_REP_RESP:
						rt_kprintf("%sdo rep response\r\n",sign);
						break;
					case	PROPERTIES_OUTPUT_GET:
						
						digitalOutputGetResp(Json,pkIdentf->valuestring);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	PROPERTIES_OUTPUT_SET:
					  digitalOutputSetResp(Json,pkIdentf->valuestring);
					  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	SERVICES_CTRLCFG_READ:
						logCrtlReadResp(Json);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	SERVICES_CTRLCFG_ADD:
						logCrtlAddResp(Json);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	SERVICES_CTRLCFG_DEL:
					  logCtrlDel(Json);
					 	rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	SERVICES_DEV_REBOOT:
					  resetDeviceResp(Json,pkIdentf->valuestring);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						break;
					case	SERVICES_ACU_REBOOT:
						resetMcuResp(Json);
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
						rt_thread_delay(500);//1秒后重启
						rt_hw_cpu_reset();
	
						break;
					case	SERVICES_HISTORY_READ:
						break;
					case	SERVICES_OTA_WRITE:
						break;
					case	SERVICES_SAVE:
					  saveMcuResp();
//						stm32_flash_erase(FLASH_IP_SAVE_ADDR, sizeof(packFlash));//每次擦除128k字节数据 存储时候需要一起存储
//						stm32_flash_write(FLASH_IP_SAVE_ADDR,(uint8_t*)&packFlash,sizeof(packFlash));
//						stm32_flash_write(FLASH_MODBUS_SAVE_ADDR,(uint8_t*)&sheet,sizeof(sheet));
						rt_kprintf("%sflash save success\n",sign);
						break;
					case	EVENTS_485_ALARM_RESP:
						break;
					case	EVENTS_ANA_ALARM_RESP:
						break;
					case  ERRRESP:
						break;
				}
		}
		else{
			rt_kprintf("%serr:json cannot phrase\r\n",sign);	
		}
		//rt_kprintf("%serr:test1\r\n",sign);	
		cJSON_Delete(Json);
	//	rt_kprintf("%serr:test2\r\n",sign);	
	  if(!USE_MQTT){
			rt_free(Buffer);
			//rt_kprintf("%serr:test3\r\n",sign);	
			Buffer =RT_NULL;
		}
}




//接收到的网络数据解析头部 判断类别
void netRecSendEvent(uint8_t *recBuf,int len)
{
//	   extern uint8_t  NetRxBuffer[TX_RX_MAX_BUF_SIZE];
	   uint8_t *bufp=recBuf;
	   int rxLen=len;
	   extern struct rt_event mqttAckEvent;
	   char headBuf[10]={0};	//根据手册得知剩余长度从第二个直接开始最大字段 4个字节  5个字节buf足够
	   memcpy(headBuf,bufp,sizeof(headBuf));
	//根据手册得知剩余长度从第二个直接开始最大字段 4个字节
   
	   int ret =MQTTPacket_read((uint8_t *)headBuf, sizeof(headBuf), transport_getdata);//作用确定头部以及剩余最大数量
	
	   switch(ret)
		 {
			 case CONNACK:
				 rt_event_send(&mqttAckEvent, EVENT_CONNACK);
			   rt_kprintf("%seVent CONNACK\r\n",sign);
					break;
			 case SUBACK:{
					unsigned short submsgid;
					int subcount;
					int granted_qos;
				//	rt_kprintf("%smqttSub ack\r\n",sign);
					MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos,(uint8_t *)bufp, rxLen);
					if (granted_qos != 0)
					{
							rt_kprintf("%sERROR:granted qos != 0, 0x%02x\r\n", sign,granted_qos);
					}
				 rt_event_send(&mqttAckEvent, EVENT_SUBACK);
					rt_kprintf("%seVent SUBACK\r\n",sign);
				}
				 break;
			 case PINGRESP:
				 rt_event_send(&mqttAckEvent, EVENT_PINGRESP);
			   rt_kprintf("%sEVENT PINGRESP\r\n",sign);
				 break;
			 case PUBLISH:{
				 extern bool mqttpubRead(uint8_t *rxbuf,int len);
				 rt_kprintf("%smqttPub ack begin\r\n",sign);
				 mqttpubRead(recBuf,len);
				 extern void getMqttRespTime();
			   getMqttRespTime();
				 rt_kprintf("%smqttPub ack end\r\n",sign);
			  }
				 break;
			 default:
				 rt_kprintf("%sphrase mqtthead err %d\n",sign,ret);
			   
				 break;
			 
		 }
		 
}



