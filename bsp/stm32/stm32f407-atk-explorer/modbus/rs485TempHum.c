#include "board.h"
//#include "stmflash.h"
//水位默认地址02  波特率9600  量程0~49米

const static char sign[]="[温湿度]";

//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //工作环流用到的最大接收buf长度
static bool alarmFLag=false;

thStru thum[TEMPHUM_485_NUM];

//float temp[TEMPHUM_485_NUM];
//float hum[TEMPHUM_485_NUM]; 
//打包串口发送 
static void tempHumUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.tempHum[num].useUartNum,buf, len);
}
//返回485温湿度传感器的响应状态
int tempHumState(int i)
{
		return thum[i].respStat;
}


//温湿度传感器读取值与阈值比较并设置flag
 void tempHumCheckSetFlag(int num)
{
	  alarmFLag=false;
		if(sheet.modbusTempHum[num].tempUpLimit!=0){
				if(thum[num].temp>=sheet.modbusTempHum[num].tempUpLimit){
					inpoutpFlag.modbusTempHum[num].tempUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusTempHum[num].tempLowLimit!=0){
				if(thum[num].temp<=sheet.modbusTempHum[num].tempLowLimit){
					inpoutpFlag.modbusTempHum[num].tempLowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusTempHum[num].humUpLimit!=0){
				if(thum[num].hum>=sheet.modbusTempHum[num].humUpLimit){
					inpoutpFlag.modbusTempHum[num].humUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusTempHum[num].humLowLimit!=0){
				if(thum[num].temp<=sheet.modbusTempHum[num].humLowLimit){	
					inpoutpFlag.modbusTempHum[num].humLowFlag=true;
					alarmFLag=true;
				}
		}
}
//复位温湿度的warn状态值
void resetTempHumWarnFlag()
{
		for (int i = 0; i < TEMPHUM_485_NUM; i++)
		{		
				inpoutpFlag.modbusTempHum[i].tempUpFlag =false;
				inpoutpFlag.modbusTempHum[i].tempLowFlag=false;
				inpoutpFlag.modbusTempHum[i].humUpFlag  =false;
				inpoutpFlag.modbusTempHum[i].humLowFlag =false;
		}
}

//发 1A 04 00 01 00 02 23 E0
//收 1A 04 04 0B 1B 00 1C 23 6F
//通过485接口读取温湿度
void readTempHum(int num)
{
	  
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadReg(sheet.tempHum[num].slaveAddr,0X0000,READ_04,2,buf);
//		rt_mutex_take(uartDev[sheet.tempHum[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485发送buf  len  等待modbus回应
		tempHumUartSend(num,(uint8_t *)buf,len);
	  rt_kprintf("%stemphum send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,MODBUS_LENTH);
		len=  rs485UartRec(sheet.tempHum[num].useUartNum,buf,1000);

		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//提取环流值 第一步判断crc 第二部提取
		//uartDev[modbusFlash[TEMPHUM].useUartNum].offline=RT_FALSE;
		int ret2=modbusRespCheck(sheet.tempHum[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//刷新读取到的值
        uint16_t read_temp	=(buf[offset]<<8)+buf[offset+1];offset+=2;
			  uint16_t read_hum	=(buf[offset]<<8)+buf[offset+1];offset+=2;
			  rt_bool_t sig=RT_FALSE;
        if(read_temp&0x8000){//负数
						read_temp=read_temp&0x7fff;
						read_temp=0xffff-read_temp+1;
					  sig=RT_TRUE;
				}
				thum[num].temp=(float)read_temp/10;
				thum[num].hum=(float)read_hum/10;
				thum[num].respStat=1;
				if(sig==RT_TRUE)
					rt_kprintf("%s温度:-%0.1fC 湿度:%0.1f\n",sign,thum[num].temp,thum[num].hum); 
				else
					rt_kprintf("%s温度:%0.1fC 湿度:%0.1f\n",sign,thum[num].temp,thum[num].hum);   
				tempHumCheckSetFlag(num);

		} 
		else{//读不到给0
				if(ret2==2){
						//rt_kprintf("%sERR:请检查485接线或者供电\r\n",sign);
//					  uartDev[modbusFlash[TEMPHUM].useUartNum].offline=RT_TRUE;
				}
				thum[num].respStat=0;
				thum[num].temp=0;
				thum[num].hum=0;
			  rt_kprintf("%s read fail\n",sign);
		}
	//	tempHumCheckSetFlag(num);//TEST ONLY
//	  rt_mutex_release(uartDev[sheet.tempHum[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;				
}

extern char   sdData[DATA_LEN];
//温湿度值通过json格式打包
//输入 respFlag 为true就是回应
//              为false就是report数据
static uint16_t tempHumJsonPack(bool respFlag)
{
		char* out = NULL;
		//创建数组
		cJSON* Array = NULL;
		// 创建JSON Object  
		cJSON* root = NULL;
		cJSON* nodeobj = NULL;
		cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return 0;
		// 加入节点（键值对）
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
		cJSON_AddStringToObject(root, "identifier","temperature_and_humidity_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < TEMPHUM_485_NUM; i++)
		{		
			if(sheet.tempHum[i].workFlag==RT_TRUE){
				sdData[0]=0;
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.tempHum[i].ID));
				//sprintf(sprinBuf,"%d",thum[i].respStat);
				cJSON_AddNumberToObject(nodeobj,"responseStatus",thum[i].respStat);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
				sprintf(sprinBuf,"%.2f",thum[i].temp);
				cJSON_AddItemToObject(nodeobj_p,"temperature",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");

				sprintf(sprinBuf,"%.2f",thum[i].hum );
				cJSON_AddItemToObject(nodeobj_p,"humidity",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
//				extern void wenShiDuSaveSD(char *id,char *data);
//				wenShiDuSaveSD(sheet.tempHum[i].ID,sdData);//suqi
				modbusTxtSaveSD(sheet.tempHum[i].ID,sdData,TEMPHUM);

				if(strlen(sdData)>=(sizeof(sdData)-2)){
					rt_kprintf("err:sdData is not enough\n");
				}
				rt_kprintf("%sSD data：%s",sign,sdData);//自带换行
			}
		}
		}
	
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// 打印JSON数据包  
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);


		return len;
}





//模拟温度和湿度值读取以及打包成json格式  返回true 有告警 false 无告警
bool modTempHumWarn2Send()
{
		if(alarmFLag==false)//TEST
			return false;
		char* out = NULL;
		//创建数组
		cJSON* Array = NULL;
		// 创建JSON Object  
		cJSON* root = NULL;
		cJSON* nodeobj = NULL;
		cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return false;
		// 加入节点（键值对）
		cJSON_AddNumberToObject(root, "mid",mcu.upMessID);
		cJSON_AddStringToObject(root, "packetType","EVENTS_485_ALARM");
		cJSON_AddStringToObject(root, "identifier","temperature_and_humidity_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < TEMPHUM_485_NUM; i++)
				{		
						if(sheet.tempHum[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.tempHum[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
							cJSON_AddNumberToObject(nodeobj_p,"temperature_low_alarm",inpoutpFlag.modbusTempHum[i].tempLowFlag);//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"temperature_high_alarm",inpoutpFlag.modbusTempHum[i].tempUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"humidity_low_alarm",inpoutpFlag.modbusTempHum[i].humLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"humidity_high_alarm",inpoutpFlag.modbusTempHum[i].humUpFlag);
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




//温湿度值读取并打包json格式
void tempHumRead2Send(rt_bool_t netStat,bool respFlag)
{
	 int workFlag=RT_FALSE;

	 for(int i=0;i<TEMPHUM_485_NUM;i++){
		if(sheet.tempHum[i].workFlag==RT_TRUE){
					readTempHum(i);
					workFlag=RT_TRUE;

			}
	}
	if(workFlag==RT_TRUE){
			rt_kprintf("%s打包采集的temphum数据\r\n",sign);
			tempHumJsonPack(respFlag);
			if(netStat==RT_TRUE)
					rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
					rt_thread_mdelay(500);return;
			if(modTempHumWarn2Send()==true){
					resetTempHumWarnFlag();//每次判断后复位warnflag状态值
					if(netStat==RT_TRUE)
							rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
			}
	}
}


