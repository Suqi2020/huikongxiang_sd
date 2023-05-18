#include "board.h"
//#include "stmflash.h"
//水位默认地址02  波特率9600  量程0~49米

const static char sign[]="[井盖]";
#if 1
//#define   SLAVE_ADDR     0X01 
//#define   LENTH          50  //工作环流用到的最大接收buf长度
static bool alarmFLag=false;
typedef struct{
	uint8_t incline;
	uint8_t switchP;
	uint8_t	vibration;
	uint8_t respStat;
}thStru;
static thStru cover[COVER_485_NUM];

//float temp[TEMPHUM_485_NUM];
//float hum[TEMPHUM_485_NUM]; 
//打包串口发送 
static void coverUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.cover[num].useUartNum,buf, len);
}
//返回485井盖的响应状态
int coverState(int i)
{
		return cover[i].respStat;
}


//井盖读取值与阈值比较并设置flag
//与其他传感器不同  井盖阈值为二值  需要判断是否相等
 void coverCheckSetFlag(int num)
{
	  alarmFLag=false;
		if(sheet.modbusCover[num].inclineUpLimit!=0){//启用
				if(cover[num].incline==1){
					inpoutpFlag.modbusCover[num].inclineUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].inclineLowLimit!=0){//启用
				if(cover[num].incline==0){
					inpoutpFlag.modbusCover[num].inclineLowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].switchUpLimit!=0){//启用
				if(cover[num].switchP==1){
					inpoutpFlag.modbusCover[num].switchUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].switchLowLimit!=0){//启用
				if(cover[num].switchP==0){
					inpoutpFlag.modbusCover[num].switchLowFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].vibrationUpLimit!=0){//启用
				if(cover[num].vibration==1){
					inpoutpFlag.modbusCover[num].vibrationUpFlag=true;
					alarmFLag=true;
				}
		}
		if(sheet.modbusCover[num].vibrationLowLimit!=0){//启用
				if(cover[num].vibration==0){
					inpoutpFlag.modbusCover[num].vibrationLowFlag=true;
					alarmFLag=true;
				}
		}
}
//复位温湿度的warn状态值
void resetCoverWarnFlag()
{
		for (int i = 0; i < COVER_485_NUM; i++)
		{		
				inpoutpFlag.modbusCover[i].inclineUpFlag	  =false;
				inpoutpFlag.modbusCover[i].inclineLowFlag		=false;
				inpoutpFlag.modbusCover[i].switchUpFlag  		=false;
				inpoutpFlag.modbusCover[i].switchLowFlag 		=false;
				inpoutpFlag.modbusCover[i].vibrationUpFlag  =false;
				inpoutpFlag.modbusCover[i].vibrationLowFlag =false;
		}
}
/*
读井盖状态 示例报文：
Tx: 01 03 0B B8 00 04 C6 08
Rx: 01 03 08 00 10 00 00 00 01 00 00 D5 D6
接收报文解析：
版本号：V1.0
倾斜状态：正常（未倾斜）
井盖状态：开启
振动报警：正常
*/
//通过485接口读取温湿度
void readCover(int num)
{
	  
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(LENTH);
	  uint16_t len = modbusReadReg(sheet.cover[num].slaveAddr,0X0BB8,READ_03,4,buf);
//		rt_mutex_take(uartDev[sheet.tempHum[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485发送buf  len  等待modbus回应
		coverUartSend(num,(uint8_t *)buf,len);
	  rt_kprintf("%scover send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
    len=0;
		memset(buf,0,LENTH);

		while(rt_mq_recv(&uartmque[sheet.cover[num].useUartNum], buf+len, 1, 500) == RT_EOK){//115200 波特率1ms 10个数据
				len++;
		}
		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		//提取环流值 第一步判断crc 第二部提取
		//uartDev[modbusFlash[TEMPHUM].useUartNum].offline=RT_FALSE;
		int ret2=modbusRespCheck(sheet.cover[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//刷新读取到的值
			  cover[num].incline = buf[offset+3];
			  cover[num].switchP = buf[offset+5];
			  cover[num].vibration = buf[offset+7];


			  rt_kprintf("%s倾斜:%d 开关:%d 震动:%d\n",sign,cover[num].incline,cover[num].switchP,cover[num].vibration);  
        cover[num].respStat=1;			
				coverCheckSetFlag(num);

		} 
		else{//读不到给0
				if(ret2==2){
				}
			  cover[num].incline = 0;
			  cover[num].switchP = 0;
			  cover[num].vibration = 0;
				cover[num].respStat  = 0;	
			  rt_kprintf("%s read fail\n",sign);
		}

		rt_free(buf);
	  buf=RT_NULL;				
}


//温湿度值通过json格式打包
//输入 respFlag 为true就是回应
//              为false就是report数据
static uint16_t coverJsonPack(bool respFlag)
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
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < COVER_485_NUM; i++)
		{		
			if(sheet.cover[i].workFlag==RT_TRUE){
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.cover[i].ID));
				//sprintf(sprinBuf,"%d",thum[i].respStat);
				cJSON_AddNumberToObject(nodeobj,"responseStatus",cover[i].respStat);
				
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
				
				cJSON_AddNumberToObject(nodeobj_p,"incline"  ,cover[i].incline);
				cJSON_AddNumberToObject(nodeobj_p,"switch"   ,cover[i].switchP);
				cJSON_AddNumberToObject(nodeobj_p,"vibration",cover[i].vibration);
				sprintf(sprinBuf,"%llu",utcTime());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
			}
		}
		}
	
		sprintf(sprinBuf,"%llu",utcTime());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// 打印JSON数据包  

		//打包
		int len=0;
		packBuf[len]= (uint8_t)(HEAD>>8); len++;
		packBuf[len]= (uint8_t)(HEAD);    len++;
		len+=LENTH_LEN;//json长度最后再填写
		
		// 释放内存  
		out = cJSON_Print(root);
		rt_strcpy((char *)packBuf+len,out);
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
	  packBuf[2]=(uint8_t)((len-LENTH_LEN-HEAD_LEN)>>8);//更新json长度
	  packBuf[3]=(uint8_t)(len-LENTH_LEN-HEAD_LEN);
	  uint16_t jsonBodyCrc=RTU_CRC(packBuf+HEAD_LEN+LENTH_LEN,len-HEAD_LEN-LENTH_LEN);
	  //crc
	  packBuf[len]=(uint8_t)(jsonBodyCrc>>8); len++;//更新crc
	  packBuf[len]=(uint8_t)(jsonBodyCrc);    len++;

		//tail
		packBuf[len]=(uint8_t)(TAIL>>8); len++;
		packBuf[len]=(uint8_t)(TAIL);    len++;
		packBuf[len]=0;//len++;//结尾 补0
		if(respFlag==false){
				mcu.repDataMessID =mcu.upMessID;
				//mcu.devRegMessID =mcu.upMessID;
				upMessIdAdd();
		}
		rt_kprintf("%s len:%d\r\n",sign,len);
		rt_kprintf("\r\n%slen：%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",sign,len,packBuf[0],packBuf[1],packBuf[2],packBuf[3]);

		rt_free(sprinBuf);
		sprinBuf=RT_NULL;

		return len;
}





//模拟温度和湿度值读取以及打包成json格式  返回true 有告警 false 无告警
bool modCoverWarn2Send()
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
		cJSON_AddStringToObject(root, "identifier","cover_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
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
							cJSON_AddNumberToObject(nodeobj_p,"switch_low_alarm",    inpoutpFlag.modbusCover[i].switchLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"switch_high_alarm",   inpoutpFlag.modbusCover[i].switchUpFlag);
						//	cJSON_AddNumberToObject(nodeobj_p,"vibration_low_alarm", inpoutpFlag.modbusCover[i].vibrationLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"vibration_high_alarm",inpoutpFlag.modbusCover[i].vibrationUpFlag);
							sprintf(sprinBuf,"%llu",utcTime());
							cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
						}
				}
		}
		sprintf(sprinBuf,"%llu",utcTime());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		//打包
		int len=0;
		packBuf[len]= (uint8_t)(HEAD>>8); len++;
		packBuf[len]= (uint8_t)(HEAD);    len++;
		len+=LENTH_LEN;//json长度最后再填写
		// 释放内存  
		out = cJSON_Print(root);
		rt_strcpy((char *)packBuf+len,out);
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
	  packBuf[2]=(uint8_t)((len-LENTH_LEN-HEAD_LEN)>>8);//更新json长度
	  packBuf[3]=(uint8_t)(len-LENTH_LEN-HEAD_LEN);
	  uint16_t jsonBodyCrc=RTU_CRC(packBuf+HEAD_LEN+LENTH_LEN,len-HEAD_LEN-LENTH_LEN);
	  //crc
	  packBuf[len]=(uint8_t)(jsonBodyCrc>>8); len++;//更新crc
	  packBuf[len]=(uint8_t)(jsonBodyCrc);    len++;
		//tail
		packBuf[len]=(uint8_t)(TAIL>>8); len++;
		packBuf[len]=(uint8_t)(TAIL);    len++;
		packBuf[len]=0;//len++;//结尾 补0
		mcu.repDataMessID =mcu.upMessID;
		//mcu.devRegMessID =mcu.upMessID;
		upMessIdAdd();
		rt_free(sprinBuf);
		sprinBuf=RT_NULL;
		return true;
}




//井盖状态读取并打包json格式
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
				rt_kprintf("%s打包采集的cover数据\r\n",sign);
				coverJsonPack(respFlag);
				if(netStat==RT_TRUE)
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&packBuf,RT_WAITING_FOREVER);
						rt_thread_mdelay(500);
				if(modCoverWarn2Send()==true){
						resetCoverWarnFlag();//每次判断后复位warnflag状态值
						if(netStat==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&packBuf,RT_WAITING_FOREVER);
				}
		}
}

//发送开锁或者关锁命令给ID对应的井盖  onOffFlag=true 打开  onOffFlag=false 关闭
//远程开锁或者LCD开锁调用
bool  coverUartOnOff(char *ID,bool onOffFlag)
{
	  #define LEN_COVER  8
	  uint8_t sendBuf[LEN_COVER];
	  uint8_t recBuf[LEN_COVER];

	  int num=0;
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
	  //485发送buf  len  等待modbus回应
		coverUartSend(num,(uint8_t *)sendBuf,len);
	  rt_kprintf("%scover send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",sendBuf[j]);
		}
		rt_kprintf("\n");
    len=0;

		while(rt_mq_recv(&uartmque[sheet.cover[num].useUartNum], recBuf+len, 1, 500) == RT_EOK){//115200 波特率1ms 10个数据
				len++;
			  if(len==LEN_COVER)
					break;
		}
		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",recBuf[j]);
				}
				rt_kprintf("\n");
		}
    for(int z=0;z<LEN_COVER;z++){
			 if(sendBuf[z]!=recBuf[z]){
				 rt_kprintf("%s开关井盖失败\n",sign);
				 return false;
			 }
		}
		return true;
}


//井盖远程开关锁

void coverJsonOnOff(cJSON   *Json)
{
		char* out = NULL;
		//创建数组
//		cJSON* Array = NULL;
		// 创建JSON Object  
		cJSON* root = NULL;
//		cJSON* nodeobj = NULL;
//		cJSON* nodeobj_p = NULL;
		root = cJSON_CreateObject();
		if (root == NULL) return;

	  cJSON_AddNumberToObject(root,"mid",respMid);

	  cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);

		cJSON_AddStringToObject(root, "identifier","cover_monitor");
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了

			
		cJSON_AddNumberToObject(root, "code",0);
		cJSON   *array=cJSON_GetObjectItem(Json,"params");
//	  cJSON  *pkIdentf = cJSON_GetObjectItem(Json,"identifier");
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

			  cJSON  *switchp=cJSON_GetObjectItem(item,"switch");

			
				cJSON_AddStringToObject(nodeobj,"deviceId",id->string);
			  int ret=0;
			  if((ret=coverUartOnOff(id->string,switchp->valueint))==true){
						cJSON_AddNumberToObject(nodeobj,"result",1);
				}
				else
						cJSON_AddNumberToObject(nodeobj,"result",0);

		}

		sprintf(sprinBuf,"%llu",utcTime());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		
		
		
		
		//打包
		int len=0;
		packBuf[len]= (uint8_t)(HEAD>>8); len++;
		packBuf[len]= (uint8_t)(HEAD);    len++;
		len+=LENTH_LEN;//json长度最后再填写
		
		// 释放内存  
		out = cJSON_Print(root);
		rt_strcpy((char *)packBuf+len,out);
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
	  packBuf[2]=(uint8_t)((len-LENTH_LEN-HEAD_LEN)>>8);//更新json长度
	  packBuf[3]=(uint8_t)(len-LENTH_LEN-HEAD_LEN);
	  uint16_t jsonBodyCrc=RTU_CRC(packBuf+HEAD_LEN+LENTH_LEN,len-HEAD_LEN-LENTH_LEN);
	  //crc
	  packBuf[len]=(uint8_t)(jsonBodyCrc>>8); len++;//更新crc
	  packBuf[len]=(uint8_t)(jsonBodyCrc);    len++;

		//tail
		packBuf[len]=(uint8_t)(TAIL>>8); len++;
		packBuf[len]=(uint8_t)(TAIL);    len++;
		packBuf[len]=0;//len++;//结尾 补0
		rt_free(sprinBuf);
		sprinBuf=RT_NULL;
}
#endif

