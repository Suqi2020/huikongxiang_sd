#include <board.h>
//#include "rs485PartDischag.h"
//#include "stmflash.h"
//<<局放在线检测 --GY-JF100-C01>>  GZPD-1000电缆局放
//响应时间不确定 最长1.7秒 有时候长 有时候短
//           默认波特率115200



const static char sign[]="[局放]";
partDischargeStru partDiscStru_p[PARTDISCHAG_485_NUM]; 
static bool alarmFLag=false;


//局放比较阈值并设置相应的flag标记
static void partDischCheckSetFlag(int num)
{
	  alarmFLag=false;
	  //high
	  if(sheet.modbusPartDisChg[num].amplitudeAUpLimit!=0){
			if(partDiscStru_p[num].amplitudeA>=sheet.modbusPartDisChg[num].amplitudeAUpLimit)
					inpoutpFlag.modbusPartDisChg[num].amplitudeAUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].amplitudeBUpLimit!=0){
			if(partDiscStru_p[num].amplitudeB>=sheet.modbusPartDisChg[num].amplitudeBUpLimit)
					inpoutpFlag.modbusPartDisChg[num].amplitudeBUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].amplitudeCUpLimit!=0){
			if(partDiscStru_p[num].amplitudeC>=sheet.modbusPartDisChg[num].amplitudeCUpLimit)
					inpoutpFlag.modbusPartDisChg[num].amplitudeCUpFlag=true;alarmFLag=true;
		}
		//low
		if(sheet.modbusPartDisChg[num].amplitudeALowLimit!=0){
			if(partDiscStru_p[num].amplitudeA<=sheet.modbusPartDisChg[num].amplitudeALowLimit)
					inpoutpFlag.modbusPartDisChg[num].amplitudeALowFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].amplitudeBLowLimit!=0){
			if(partDiscStru_p[num].amplitudeB<=sheet.modbusPartDisChg[num].amplitudeBLowLimit)
					inpoutpFlag.modbusPartDisChg[num].amplitudeBLowFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].amplitudeCLowLimit!=0){
			if(partDiscStru_p[num].amplitudeC<=sheet.modbusPartDisChg[num].amplitudeCLowLimit)
					inpoutpFlag.modbusPartDisChg[num].amplitudeCLowFlag=true;alarmFLag=true;
		}
		//high
		if(sheet.modbusPartDisChg[num].dischargeAUpLimit!=0){
			if(partDiscStru_p[num].dischargeA>=sheet.modbusPartDisChg[num].dischargeAUpLimit)
					inpoutpFlag.modbusPartDisChg[num].dischargeAUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].dischargeBUpLimit!=0){
			if(partDiscStru_p[num].dischargeB>=sheet.modbusPartDisChg[num].dischargeBUpLimit)
					inpoutpFlag.modbusPartDisChg[num].dischargeBUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].dischargeCUpLimit!=0){
			if(partDiscStru_p[num].dischargeC>=sheet.modbusPartDisChg[num].dischargeCUpLimit)
					inpoutpFlag.modbusPartDisChg[num].dischargeCUpFlag=true;alarmFLag=true;
		}
		//low
		if(sheet.modbusPartDisChg[num].dischargeALowLimit!=0){
			if(partDiscStru_p[num].dischargeA<=sheet.modbusPartDisChg[num].dischargeALowLimit)
					inpoutpFlag.modbusPartDisChg[num].dischargeALowFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].dischargeBLowLimit!=0){
			if(partDiscStru_p[num].dischargeB<=sheet.modbusPartDisChg[num].dischargeBLowLimit)
					inpoutpFlag.modbusPartDisChg[num].dischargeBLowFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].dischargeCLowLimit!=0){
			if(partDiscStru_p[num].dischargeC<=sheet.modbusPartDisChg[num].dischargeCLowLimit)
					inpoutpFlag.modbusPartDisChg[num].dischargeCLowFlag=true;alarmFLag=true;
		}
		
		
		//high
		if(sheet.modbusPartDisChg[num].freqAUpLimit!=0){
			if(partDiscStru_p[num].freqA>=sheet.modbusPartDisChg[num].freqAUpLimit)
					inpoutpFlag.modbusPartDisChg[num].freqAUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].freqBUpLimit!=0){
			if(partDiscStru_p[num].freqB>=sheet.modbusPartDisChg[num].freqBUpLimit)
					inpoutpFlag.modbusPartDisChg[num].freqBUpFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].freqCUpLimit!=0){
			if(partDiscStru_p[num].freqC>=sheet.modbusPartDisChg[num].freqCUpLimit)
					inpoutpFlag.modbusPartDisChg[num].freqCUpFlag=true;alarmFLag=true;
		}
		
				//low
		if(sheet.modbusPartDisChg[num].freqALowLimit!=0){
			if(partDiscStru_p[num].freqA<=sheet.modbusPartDisChg[num].freqALowLimit)
					inpoutpFlag.modbusPartDisChg[num].freqALowFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].freqBLowLimit!=0){
			if(partDiscStru_p[num].freqB<=sheet.modbusPartDisChg[num].freqBLowLimit)
					inpoutpFlag.modbusPartDisChg[num].freqBLowFlag=true;alarmFLag=true;
		}
		if(sheet.modbusPartDisChg[num].freqCLowLimit!=0){
			if(partDiscStru_p[num].freqC<=sheet.modbusPartDisChg[num].freqCLowLimit)
					inpoutpFlag.modbusPartDisChg[num].freqCLowFlag=true;alarmFLag=true;
		}
}
//返回局放的通讯状态 true--通讯成功 false--通讯失败
int partDisState(int i)
{
		return partDiscStru_p[i].respStat;
}
//#define   SLAVE_ADDR     0X01
////#define   LENTH          1024  //工作环流用到的最大接收buf长度

//局放读取数据的发送  调用485串口
static void partDischagUartSend(int num,uint8_t *buf,int len)
{
		rs485UartSend(sheet.partDischag[num].useUartNum,buf, len);
	
}

//读取幅值 频率 放电总能量
//01 03 0300 0006 C58C
//01 03 24 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 7B A1 
//读取局放的幅值 频率
void readPdFreqDischarge(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadReg(sheet.partDischag[num].slaveAddr,0x0300,READ_03,18,buf);
//		rt_mutex_take(uartDev[sheet.partDischag[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485发送buf  len  等待modbus回应
		partDischagUartSend(num,buf,len);
	  rt_kprintf("%sPdFreqDiacg send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
		memset(buf,0,MODBUS_LENTH);
    len=0;
		len=  rs485UartRec(sheet.partDischag[num].useUartNum,buf,2000);
		if(len!=0){
				rt_kprintf("%srec:",sign);
				for(int j=0;j<len;j++){
						rt_kprintf("%x ",buf[j]);
				}
				rt_kprintf("\n");
		}
		int ret2= modbusRespCheck(sheet.partDischag[num].slaveAddr,buf,len,RT_TRUE);
		if(0 ==  ret2){//刷新读取到的值
			
				partDiscStru_p[num].amplitudeA=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
				partDiscStru_p[num].freqA     =(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
			  partDiscStru_p[num].dischargeA=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
			
				partDiscStru_p[num].amplitudeB=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
				partDiscStru_p[num].freqB     =(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
			  partDiscStru_p[num].dischargeB=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
			
				partDiscStru_p[num].amplitudeC=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
				partDiscStru_p[num].freqC     =(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
			  partDiscStru_p[num].dischargeC=(buf[offset]<<24)+(buf[offset+1]<<16)+(buf[offset+2]<<8)+buf[offset+3];offset+=4;
			  rt_kprintf("%sPdFreqDiach read ok\n",sign);
			  partDiscStru_p[num].respStat=1;
				partDischCheckSetFlag(num);
		} 
		else{//读不到给0
				if(ret2==2){
						//rt_kprintf("%sERR:请检查485接线或者供电\r\n",sign);
				}
				partDiscStru_p[num].respStat=0;
				partDiscStru_p[num].amplitudeA=0;
				partDiscStru_p[num].freqA     =0;
			  partDiscStru_p[num].dischargeA=0;
			
				partDiscStru_p[num].amplitudeB=0;
				partDiscStru_p[num].freqB     =0;
			  partDiscStru_p[num].dischargeB=0;
			
				partDiscStru_p[num].amplitudeC=0;
				partDiscStru_p[num].freqC     =0;
			  partDiscStru_p[num].dischargeC=0;
			  rt_kprintf("%sPdFreqDiach read fail\n",sign);
		}
    //partDischCheckSetFlag(num);//test only
//	  rt_mutex_release(uartDev[sheet.partDischag[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;

}

//读取局放报警状态 
//发 01 01 0001 0008 6C0C
//收 01 01 01 00 51 88 

rt_bool_t readPartDischgWarning(int num)
{
	  uint8_t offset=3;//add+regadd+len
	  uint8_t  *buf = RT_NULL;
		buf = rt_malloc(MODBUS_LENTH);
	  uint16_t len = modbusReadBitReg(sheet.partDischag[num].slaveAddr,0x0001,8,buf);//读取8个bit
//		rt_mutex_take(uartDev[sheet.partDischag[num].useUartNum].uartMutex,RT_WAITING_FOREVER);
	  //485发送buf  len  等待modbus回应
		partDischagUartSend(num,buf,len);
	  rt_kprintf("%sreadPd send:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
		memset(buf,0,MODBUS_LENTH);
    len=0;
		len=  rs485UartRec(sheet.partDischag[num].useUartNum,buf,3000);
		rt_kprintf("%srec:",sign);
		for(int j=0;j<len;j++){
				rt_kprintf("%x ",buf[j]);
		}
		rt_kprintf("\n");
//		uartDev[modbusFlash[PARTDISCHAG].useUartNum].offline=RT_FALSE;
		//提取环流值 第一步判断crc 第二部提取
		int ret2=modbusRespCheck(sheet.partDischag[num].slaveAddr,buf,len,RT_TRUE);
		if(0 == ret2){//刷新读取到的值
     
			  partDiscStru_p[num].alarm.a=(buf[offset]>>0)&0x01;
				partDiscStru_p[num].alarm.b=(buf[offset]>>1)&0x01;
			  partDiscStru_p[num].alarm.c=(buf[offset]>>2)&0x01;
			  rt_kprintf("%s提取alarm OK\r\n",sign);
			  partDiscStru_p[num].respStat=1;
		} 
		else{
				if(ret2==2){
				}
				partDiscStru_p[num].respStat=0;
				partDiscStru_p[num].alarm.a=0;
				partDiscStru_p[num].alarm.b=0;
				partDiscStru_p[num].alarm.c=0;
			  rt_kprintf("%s提取alarm fail\r\n",sign);
		}
   
//	  rt_mutex_release(uartDev[sheet.partDischag[num].useUartNum].uartMutex);
		rt_free(buf);
	  buf=RT_NULL;
		if(partDiscStru_p[num].alarm.a||partDiscStru_p[num].alarm.b||partDiscStru_p[num].alarm.c)
				return RT_TRUE;
		else 
			  return RT_FALSE;
}

void  partDisWaringEventPack()
{
		rt_kprintf("%slater add \n\r",sign);
		
}


//局放的json格式打包
//输入 respFlag 为true就是回应
//              为false就是report数据

typedef enum{
		A=0,
	  B,
	  C,
}phaseEnum;

//输入依次为 相位 设备地址 寄存器地址 寄存器长度
uint8_t  partDischgAtlasRead(phaseEnum X,uint8_t slavAddr,uint16_t regAddr,uint8_t regLen,uint8_t *out)
{

		int i=0;
	  out[i]=slavAddr;					 			i++;
	  out[i]=0x14;      				     	i++;//读取记录号
	  out[i]=7;   										i++;
	  out[i]=6;      								  i++;
		out[i]=(uint8_t)(X>>8);      	  i++;
	  out[i]=(uint8_t) X;       			i++;
	
		out[i]=(uint8_t)(regAddr>>8);      	  i++;
	  out[i]=(uint8_t) regAddr;       			i++;
	
		out[i]=(uint8_t)(regLen>>8);      	  i++;
	  out[i]=(uint8_t) regLen;       			i++;
	  uint16_t crcRet=RTU_CRC(out ,i);
	  out[i]=(uint8_t)(crcRet>>8);    i++;
	  out[i]=crcRet;       						i++;
    return i;

}






//复位温湿度的warn状态值
void resetPartDischagWarnFlag()
{
		for (int i = 0; i < PARTDISCHAG_485_NUM; i++)
		{		
				inpoutpFlag.modbusPartDisChg[i].amplitudeALowFlag =false;
				inpoutpFlag.modbusPartDisChg[i].amplitudeAUpFlag=false;
				inpoutpFlag.modbusPartDisChg[i].dischargeALowFlag=false;
				inpoutpFlag.modbusPartDisChg[i].dischargeAUpFlag =false;
				inpoutpFlag.modbusPartDisChg[i].freqALowFlag=false;
				inpoutpFlag.modbusPartDisChg[i].freqAUpFlag =false;

				inpoutpFlag.modbusPartDisChg[i].amplitudeBLowFlag =false;
				inpoutpFlag.modbusPartDisChg[i].amplitudeBUpFlag=false;
				inpoutpFlag.modbusPartDisChg[i].dischargeBLowFlag=false;
				inpoutpFlag.modbusPartDisChg[i].dischargeBUpFlag =false;
				inpoutpFlag.modbusPartDisChg[i].freqBLowFlag=false;
				inpoutpFlag.modbusPartDisChg[i].freqBUpFlag =false;
			
				inpoutpFlag.modbusPartDisChg[i].amplitudeCLowFlag =false;
				inpoutpFlag.modbusPartDisChg[i].amplitudeCUpFlag=false;
				inpoutpFlag.modbusPartDisChg[i].dischargeCLowFlag=false;
				inpoutpFlag.modbusPartDisChg[i].dischargeCUpFlag =false;
				inpoutpFlag.modbusPartDisChg[i].freqCLowFlag=false;
				inpoutpFlag.modbusPartDisChg[i].freqCUpFlag =false;
		}
}





//模拟温度和湿度值读取以及打包成json格式  返回true 有告警 false 无告警
bool modPartDischagWarn2Send()
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
		cJSON_AddStringToObject(root, "identifier","partial_discharge_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		{
				Array = cJSON_CreateArray();
				if (Array == NULL) return false;
				cJSON_AddItemToObject(root, "params", Array);
				for (int i = 0; i < PARTDISCHAG_485_NUM; i++)
				{		
						if(sheet.partDischag[i].workFlag==RT_TRUE){
							nodeobj = cJSON_CreateObject();
							cJSON_AddItemToArray(Array, nodeobj);
							cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.partDischag[i].ID));
							cJSON_AddNumberToObject(nodeobj,"alarmStatus",1);
							nodeobj_p= cJSON_CreateObject();
							cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);

							cJSON_AddNumberToObject(nodeobj_p,"pdA_low_alarm",inpoutpFlag.modbusPartDisChg[i].amplitudeALowFlag );//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"pdA_high_alarm",inpoutpFlag.modbusPartDisChg[i].amplitudeAUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"freqA_low_alarm",inpoutpFlag.modbusPartDisChg[i].freqALowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"freqA_high_alarm",inpoutpFlag.modbusPartDisChg[i].freqAUpFlag);		
							cJSON_AddNumberToObject(nodeobj_p,"dischargeDateA_low_alarm",inpoutpFlag.modbusPartDisChg[i].dischargeALowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"dischargeDateA_high_alarm",inpoutpFlag.modbusPartDisChg[i].dischargeAUpFlag);		
							
							cJSON_AddNumberToObject(nodeobj_p,"pdB_low_alarm",inpoutpFlag.modbusPartDisChg[i].amplitudeBLowFlag );//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"pdB_high_alarm",inpoutpFlag.modbusPartDisChg[i].amplitudeBUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"freqB_low_alarm",inpoutpFlag.modbusPartDisChg[i].freqBLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"freqB_high_alarm",inpoutpFlag.modbusPartDisChg[i].freqBUpFlag);		
							cJSON_AddNumberToObject(nodeobj_p,"dischargeDateB_low_alarm",inpoutpFlag.modbusPartDisChg[i].dischargeBLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"dischargeDateB_high_alarm",inpoutpFlag.modbusPartDisChg[i].dischargeBUpFlag);	

							cJSON_AddNumberToObject(nodeobj_p,"pdC_low_alarm",inpoutpFlag.modbusPartDisChg[i].amplitudeCLowFlag );//cJSON_CreateNumber("10"));
							cJSON_AddNumberToObject(nodeobj_p,"pdC_high_alarm",inpoutpFlag.modbusPartDisChg[i].amplitudeCUpFlag);
							cJSON_AddNumberToObject(nodeobj_p,"freqC_low_alarm",inpoutpFlag.modbusPartDisChg[i].freqCLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"freqC_high_alarm",inpoutpFlag.modbusPartDisChg[i].freqCUpFlag);		
							cJSON_AddNumberToObject(nodeobj_p,"dischargeDateC_low_alarm",inpoutpFlag.modbusPartDisChg[i].dischargeCLowFlag);
							cJSON_AddNumberToObject(nodeobj_p,"dischargeDateC_high_alarm",inpoutpFlag.modbusPartDisChg[i].dischargeCUpFlag);	
							sprintf(sprinBuf,"%llu",utcTime_ms());
							cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
						}
				}
		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		//打包
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,false);
		return true;
}




//局放的json格式打包
//输入 respFlag 为true就是回应
//              为false就是report数据


extern char   sdData[DATA_LEN];
#if 1
uint16_t partDischagJsonPack(bool respFlag)
{
//		char *data=NULL;
//		data =rt_malloc(200); // suqi 根据实际长度来调整
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
		cJSON_AddStringToObject(root, "identifier","partial_discharge_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		#if 1
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);
		for (int i = 0; i < PARTDISCHAG_485_NUM; i++)//		for (int i = 0; i < PARTDISCHAG_485_NUM; i++)
		{		
			
			if(sheet.partDischag[i].workFlag==RT_TRUE){
				sdData[0]=0;
				nodeobj = cJSON_CreateObject();
				cJSON_AddItemToArray(Array, nodeobj);
			  cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.partDischag[i].ID));
				cJSON_AddNumberToObject(nodeobj,"responseStatus",partDiscStru_p[i].respStat);
				nodeobj_p= cJSON_CreateObject();
				cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
				
				sprintf(sprinBuf,"%d",partDiscStru_p[i].amplitudeA);
				cJSON_AddItemToObject(nodeobj_p,"pdA",cJSON_CreateString(sprinBuf));   strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",partDiscStru_p[i].freqA);
				cJSON_AddItemToObject(nodeobj_p,"freqA",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",partDiscStru_p[i].dischargeA);
				cJSON_AddItemToObject(nodeobj_p,"dischargesdDataA",cJSON_CreateString(sprinBuf)); strcat(sdData,sprinBuf);strcat(sdData,"  ");

				sprintf(sprinBuf,"%d",partDiscStru_p[i].amplitudeB);
				cJSON_AddItemToObject(nodeobj_p,"pdB",cJSON_CreateString(sprinBuf));		strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",partDiscStru_p[i].freqB);
				cJSON_AddItemToObject(nodeobj_p,"freqB",cJSON_CreateString(sprinBuf));	strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",partDiscStru_p[i].dischargeB);
				cJSON_AddItemToObject(nodeobj_p,"dischargesdDataB",cJSON_CreateString(sprinBuf));	strcat(sdData,sprinBuf);strcat(sdData,"  ");
//				cJSON_AddItemToObject(nodeobj_p,"prpdsdDataB",cJSON_CreateString(""));
//				cJSON_AddItemToObject(nodeobj_p,"prpssdDataB",cJSON_CreateString(""));
				
				sprintf(sprinBuf,"%d",partDiscStru_p[i].amplitudeC);
				cJSON_AddItemToObject(nodeobj_p,"pdC",cJSON_CreateString(sprinBuf));	strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",partDiscStru_p[i].freqC);
				cJSON_AddItemToObject(nodeobj_p,"freqC",cJSON_CreateString(sprinBuf));	strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%d",partDiscStru_p[i].dischargeC);
				cJSON_AddItemToObject(nodeobj_p,"dischargesdDataC",cJSON_CreateString(sprinBuf));	strcat(sdData,sprinBuf);strcat(sdData,"  ");
				sprintf(sprinBuf,"%llu",utcTime_ms());
				cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf)); 	strcat(sdData,sprinBuf);strcat(sdData,"\r\n");
				if(strlen(sdData)>=(sizeof(sdData)-2)){
					rt_kprintf("err:sdData is not enough\n");
				}
//				extern void juFangTxtSaveSD(char *id,char *data);
//				juFangTxtSaveSD(sheet.partDischag[i].ID,sdData);
				modbusTxtSaveSD(sheet.partDischag[i].ID,sdData,PARTDISCHAG);

//					rt_thread_delay(10);
				rt_kprintf("%sSD sdData：%s",sign,sdData);//自带换行

			}
		}
		}
		#endif
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);  

		// 打印JSON数据包  
		
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,respFlag);

		return 1;
}

#endif

///局放的读取和发送  供其他函数来调用
void partDischagRead2Send(rt_bool_t netStat,bool respFlag)
{
		int workFlag=RT_FALSE;
		for(int i=0;i<PARTDISCHAG_485_NUM;i++){
			 if(sheet.partDischag[i].workFlag==RT_TRUE){
				    rt_thread_mdelay(200);
						readPdFreqDischarge(i);
						workFlag=RT_TRUE;
				}
		}
		if(workFlag==RT_TRUE){
				rt_kprintf("%s打包采集的PARTDISCHAG数据\r\n",sign);
				partDischagJsonPack(respFlag);//后期加入
				if(netStat==RT_TRUE)
						rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				rt_thread_mdelay(500);return;
				if(modPartDischagWarn2Send()==true){
						resetPartDischagWarnFlag();//每次判断后复位warnflag状态值
						//rt_thread_mdelay(500);
						if(netStat==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
				}
		}
}
/////////////////////////////////////////////////////////////读取////////////////////////////////////////////////////////////////


//#define ATLAS 5400
char pdBuf[5400*2]   CCMRAM;//读取modbus图谱数据放到这里
//__attribute__((at(0x10000000+TX_RX_MAX_BUF_SIZE*2)));//使用STM32F4中的CCM的另外64K内存  记录好偏移地址
//根据手册“9、	规定可读取的最大记录号为5400，即0x1518。超过该范围将不做应答。” 最大存储5400个数据
//输入相位号  第几个设备 相位频次 
//返回 buffer实际数据长度  最大 10800 最小5400
#define READ_LEN 100 //每次最多读取的寄存器个数  返回寄存器数据为 READ_LEN*2
//调用本函数前必须调用readPdFreqDischarge
//记录号不能是单数 
static int partDischagChanlRead(phaseEnum X,int num)
{
		int rdTotalNum =0;
	  //char *bufP=buf;
	  uint8_t *buf=rt_malloc(MODBUS_LENTH);
		if(X==A)
		   rdTotalNum=  partDiscStru_p[num].freqA;
		else if(X==B)
		   rdTotalNum=  partDiscStru_p[num].freqB;
		else if(X==C)
		   rdTotalNum=  partDiscStru_p[num].freqC;
	  memset(pdBuf,0,sizeof(pdBuf));//清buf
	  uint8_t readTimes   = rdTotalNum/READ_LEN;
	  uint8_t lastTimeLen = rdTotalNum%READ_LEN;
	  uint8_t readTimesP=0,len,singlReadLen;
		uint32_t offset=0;
	  while(1){
			    singlReadLen =((readTimesP==readTimes)?lastTimeLen:READ_LEN);
					len = partDischgAtlasRead(X,sheet.partDischag[num].slaveAddr,READ_LEN*readTimesP,singlReadLen,buf);//读取最后一包
					partDischagUartSend(num,buf,len);
					rt_kprintf("%sreadPd2 send:",sign);
					for(int j=0;j<len;j++){
							rt_kprintf("%x ",buf[j]);
					}
					rt_kprintf("\n");
					memset(buf,0,MODBUS_LENTH);
					len=0;
		      len=  rs485UartRec(sheet.partDischag[num].useUartNum,buf,2000);

					if(len==0){
							rt_kprintf("%s partDis read atlas not resp ,break \n",sign);
							break;
					}
					rt_kprintf("%srec:",sign);
					for(int j=0;j<len;j++){
							rt_kprintf("%x ",buf[j]);
					}
					rt_kprintf("\n");
			//		uartDev[modbusFlash[PARTDISCHAG].useUartNum].offline=RT_FALSE;
					//提取环流值 第一步判断crc 第二部提取
					int ret2=modbusRespCheck(sheet.partDischag[num].slaveAddr,buf,len,RT_TRUE);
					if(0 == ret2){//刷新读取到的值
							rt_kprintf("%satlas OK\r\n",sign);
							partDiscStru_p[num].respStat=1;
					} 
					else{
							rt_kprintf("%sERR  atlas read\r\n",sign);
					}
					if(buf[3]!=(singlReadLen*2+1)){
							rt_kprintf("%sERR sendlen:%d reclen:%d\r\n",singlReadLen,buf[3]);
					}
					else{
						  memcpy(pdBuf+offset,buf+3,singlReadLen*2);
						  offset+=singlReadLen*2;
						  if(offset>=sizeof(pdBuf)){
									rt_kprintf ("%sERR atlas len too long %d>%d",sign,offset,sizeof(pdBuf));
							}
					}
					if(readTimesP==readTimes){
							break;
					}
					readTimesP++; 
		}
		rt_free(buf);
		buf=NULL;
		return offset;
}

static  int atlasJsonNodataPack(phaseEnum X,int num)
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


		cJSON_AddNumberToObject(root, "mid",respMid);
		cJSON_AddStringToObject(root, "packetType","PROPERTIES_485DATA_GET_RESP");
		cJSON_AddNumberToObject(root, "code",0);

		cJSON_AddStringToObject(root, "identifier","partial_discharge_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);

		nodeobj = cJSON_CreateObject();
		cJSON_AddItemToArray(Array, nodeobj);
		cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.partDischag[num].ID));
		cJSON_AddNumberToObject(nodeobj,"responseStatus",partDiscStru_p[num].respStat);
		nodeobj_p= cJSON_CreateObject();
		cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
		if(X==A){
				sprintf(sprinBuf,"%d",partDiscStru_p[num].amplitudeA);
				cJSON_AddItemToObject(nodeobj_p,"pdA",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].freqA);
				cJSON_AddItemToObject(nodeobj_p,"freqA",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].dischargeA);
				cJSON_AddItemToObject(nodeobj_p,"dischargeDataA",cJSON_CreateString(sprinBuf));

				cJSON_AddItemToObject(nodeobj_p,"atlasPdA",cJSON_CreateString(""));
				cJSON_AddItemToObject(nodeobj_p,"atlasFreqA",cJSON_CreateString(""));
				cJSON_AddNumberToObject(nodeobj_p,"totalNum",0);
				cJSON_AddNumberToObject(nodeobj_p,"currentNum",0);
		}
		else if(X==B){
				sprintf(sprinBuf,"%d",partDiscStru_p[num].amplitudeB);
				cJSON_AddItemToObject(nodeobj_p,"pdB",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].freqB);
				cJSON_AddItemToObject(nodeobj_p,"freqB",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].dischargeB);
				cJSON_AddItemToObject(nodeobj_p,"dischargeDataB",cJSON_CreateString(sprinBuf));

				cJSON_AddItemToObject(nodeobj_p,"atlasPdB",cJSON_CreateString(""));
				cJSON_AddItemToObject(nodeobj_p,"atlasFreqB",cJSON_CreateString(""));
				cJSON_AddNumberToObject(nodeobj_p,"totalNum",0);
				cJSON_AddNumberToObject(nodeobj_p,"currentNum",0);
		}
		else if(X==C){
				sprintf(sprinBuf,"%d",partDiscStru_p[num].amplitudeC);
				cJSON_AddItemToObject(nodeobj_p,"pdC",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].freqC);
				cJSON_AddItemToObject(nodeobj_p,"freqC",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].dischargeC);
				cJSON_AddItemToObject(nodeobj_p,"dischargeDataC",cJSON_CreateString(sprinBuf));

				cJSON_AddItemToObject(nodeobj_p,"atlasPdC",cJSON_CreateString(""));
				cJSON_AddItemToObject(nodeobj_p,"atlasFreqC",cJSON_CreateString(""));
				cJSON_AddNumberToObject(nodeobj_p,"totalNum",0);
				cJSON_AddNumberToObject(nodeobj_p,"currentNum",0);
		}
		cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));

		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// 打印JSON数据包  
		
   int len=jsonPackMqttTcp(&out,&root,&sprinBuf,true);
		
		return 1;
}

//char sprintPdBuf[5400*3]    CCMRAM;
//json格式打包   
//输入 phaseEnum X, A B C三相中一个
//输入int num, 输入局放传感器的数目
//int totalLen,总的记录号个数占2个字节  pdBuf个数需要除掉2
//int offset, 记录号偏移地址占2个字节 pdBuf个数需要除掉2
//int readLen 读取的长度 1个记录号占2个字节  记录号的长度  pdBuf个数需要除掉2
static  int atlasJsonReadPack(phaseEnum X,int num,int totalLen,int offset,int readLen)
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


		cJSON_AddNumberToObject(root, "mid",respMid);
		cJSON_AddStringToObject(root, "packetType","PROPERTIES_485DATA_GET_RESP");
		cJSON_AddNumberToObject(root, "code",0);

		cJSON_AddStringToObject(root, "identifier","partial_discharge_monitor");
		cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		char *sprinBuf=RT_NULL;
		sprinBuf=rt_malloc(20);//20个字符串长度 够用了
		
		{
		Array = cJSON_CreateArray();
		if (Array == NULL) return 0;
		cJSON_AddItemToObject(root, "params", Array);

		nodeobj = cJSON_CreateObject();
		cJSON_AddItemToArray(Array, nodeobj);
		cJSON_AddItemToObject(nodeobj,"deviceId",cJSON_CreateString(sheet.partDischag[num].ID));
		cJSON_AddNumberToObject(nodeobj,"responseStatus",partDiscStru_p[num].respStat);
		nodeobj_p= cJSON_CreateObject();
		cJSON_AddItemToObject(nodeobj, "data", nodeobj_p);
		if(X==A){
				sprintf(sprinBuf,"%d",partDiscStru_p[num].amplitudeA);
				cJSON_AddItemToObject(nodeobj_p,"pdA",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].freqA);
				cJSON_AddItemToObject(nodeobj_p,"freqA",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].dischargeA);
				cJSON_AddItemToObject(nodeobj_p,"dischargeDataA",cJSON_CreateString(sprinBuf));
			  for(int j=0;j<readLen/2;j++){
						sprintf((char *)NetTxBuffer+j*5,"%02X%02X%c",pdBuf[offset+4*j],pdBuf[offset+4*j+1],',');
				}
				NetTxBuffer[readLen/2*5]=0;//追加0
				if(strlen((char *)NetTxBuffer)!=readLen){
					rt_kprintf("%sERR:readlen[%d] strlen[%d]\n",sign,strlen((char *)NetTxBuffer),readLen);
				}
				cJSON_AddItemToObject(nodeobj_p,"atlasPdA",cJSON_CreateString((char *)NetTxBuffer));
			  for(int j=0;j<readLen/2;j++){
						sprintf((char *)NetTxBuffer+j*5,"%02X%02X%c",pdBuf[offset+4*j+2],pdBuf[offset+4*j+3],',');
				}
				NetTxBuffer[readLen/2*5]=0;//追加0
				cJSON_AddItemToObject(nodeobj_p,"atlasFreqA",cJSON_CreateString((char *)NetTxBuffer));
				cJSON_AddNumberToObject(nodeobj_p,"totalNum",totalLen);
				cJSON_AddNumberToObject(nodeobj_p,"currentNum",readLen);
				cJSON_AddNumberToObject(nodeobj_p,"startNum",offset);
		}
		else if(X==B){
				sprintf(sprinBuf,"%d",partDiscStru_p[num].amplitudeB);
				cJSON_AddItemToObject(nodeobj_p,"pdB",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].freqB);
				cJSON_AddItemToObject(nodeobj_p,"freqB",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].dischargeB);
				cJSON_AddItemToObject(nodeobj_p,"dischargeDataB",cJSON_CreateString(sprinBuf));
			  for(int j=0;j<readLen/2;j++){
						sprintf((char *)NetTxBuffer+j*5,"%02X%02X%c",pdBuf[offset+4*j],pdBuf[offset+4*j+1],',');
				}
				NetTxBuffer[readLen/2*5]=0;//追加0
				if(strlen((char *)NetTxBuffer)!=readLen){
					rt_kprintf("%sERR:readlen[%d] strlen[%d]\n",sign,strlen((char *)NetTxBuffer),readLen);
				}
				cJSON_AddItemToObject(nodeobj_p,"atlasPdB",cJSON_CreateString((char *)NetTxBuffer));
			  for(int j=0;j<readLen/2;j++){
						sprintf((char *)NetTxBuffer+j*5,"%02X%02X%c",pdBuf[offset+4*j+2],pdBuf[offset+4*j+3],',');
				}
				NetTxBuffer[readLen/2*5]=0;//追加0
				cJSON_AddItemToObject(nodeobj_p,"atlasFreqB",cJSON_CreateString((char *)NetTxBuffer));
				cJSON_AddNumberToObject(nodeobj_p,"totalNum",totalLen);
				cJSON_AddNumberToObject(nodeobj_p,"currentNum",readLen);
				cJSON_AddNumberToObject(nodeobj_p,"startNum",offset);
		}
		else if(X==C){
				sprintf(sprinBuf,"%d",partDiscStru_p[num].amplitudeC);
				cJSON_AddItemToObject(nodeobj_p,"pdC",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].freqC);
				cJSON_AddItemToObject(nodeobj_p,"freqC",cJSON_CreateString(sprinBuf));
				sprintf(sprinBuf,"%d",partDiscStru_p[num].dischargeC);
				cJSON_AddItemToObject(nodeobj_p,"dischargeDataC",cJSON_CreateString(sprinBuf));
			  for(int j=0;j<readLen/2;j++){
						sprintf((char *)NetTxBuffer+j*5,"%02X%02X%c",pdBuf[offset+4*j],pdBuf[offset+4*j+1],',');
				}
				NetTxBuffer[readLen/2*5]=0;//追加0
				if(strlen((char *)NetTxBuffer)!=readLen){
					rt_kprintf("%sERR:readlen[%d] strlen[%d]\n",sign,strlen((char *)NetTxBuffer),readLen);
				}
				cJSON_AddItemToObject(nodeobj_p,"atlasPdC",cJSON_CreateString((char *)NetTxBuffer));
			  for(int j=0;j<readLen/2;j++){
						sprintf((char *)NetTxBuffer+j*5,"%02X%02X%c",pdBuf[offset+4*j+2],pdBuf[offset+4*j+3],',');
				}
				NetTxBuffer[readLen/2*5]=0;//追加0
				cJSON_AddItemToObject(nodeobj_p,"atlasFreqC",cJSON_CreateString((char *)NetTxBuffer));
				cJSON_AddNumberToObject(nodeobj_p,"totalNum",totalLen);
				cJSON_AddNumberToObject(nodeobj_p,"currentNum",readLen);
				cJSON_AddNumberToObject(nodeobj_p,"startNum",offset);
		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));

		}
		sprintf(sprinBuf,"%llu",utcTime_ms());
		cJSON_AddStringToObject(root,"timestamp",sprinBuf);
		// 打印JSON数据包  
		
	
		int len=jsonPackMqttTcp(&out,&root,&sprinBuf,true);
//		static int times=0;
//    rt_kprintf("atlasJson len =%d\n",len);
//		
//		times++;
		
//		rt_kprintf("total send len =%d\n",len*times);
		
		return len;
}
//子站下发读取图谱命令的回应  
//输入 局放的ID号  输出 无
void  partDischgAtlasResp(char *ID)
{ 
	  int readNum=0,len=0;
		for(int i=0;i<PARTDISCHAG_485_NUM;i++){
			 if(strcmp(sheet.partDischag[i].ID,ID)==0){
						readPdFreqDischarge(i);

				    readNum=i;
				    break;
				}
		}
//		partDiscStru_p[readNum].freqA=1000;//test suqi
//		partDiscStru_p[readNum].freqB=2000;//test suqi
//		partDiscStru_p[readNum].freqC=3000;//test suqi
		if(partDiscStru_p[readNum].freqA==0){
				atlasJsonNodataPack(A,readNum);
			  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
			  
		}
		else{
				len=partDischagChanlRead(A,readNum)/2;//除以2 --读取的实际记录号个数
			 // len=2699;//test suqi
			  if(len<=sizeof(pdBuf)/4){//一包就能发完
					 atlasJsonReadPack(A,readNum,len,0,len);
			     rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_kprintf("%s send atlas--A\n",sign);
				}
				else{//分两包发
					 atlasJsonReadPack(A,readNum,len,0,len/2);
					 rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_thread_mdelay(10);
					 atlasJsonReadPack(A,readNum,len,len/2,(len/2));
					 rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_thread_mdelay(10);
				}
		}
		if(partDiscStru_p[readNum].freqB==0){
				atlasJsonNodataPack(B,readNum);
			  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
		}
		else{
				len=partDischagChanlRead(B,readNum)/2;//除以2 --读取的实际记录号个数
				//len=2702;//test suqi
			  if(len<=sizeof(pdBuf)/4){//一包就能发完
					 atlasJsonReadPack(B,readNum,len,0,len);
			     rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_kprintf("%s send atlas--B\n",sign);
				}
				else{//分两包发
					 atlasJsonReadPack(B,readNum,len,0,len/2);
					 rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_thread_mdelay(10);
					 atlasJsonReadPack(B,readNum,len,len/2,(len/2));
					 rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_thread_mdelay(10);
				}
		}
		if(partDiscStru_p[readNum].freqC==0){
				atlasJsonNodataPack(C,readNum);
			  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER);
		}
		else{
				len=partDischagChanlRead(C,readNum)/2;//除以2 --读取的实际记录号个数
//				len=5400;//test suqi
			  if(len<=sizeof(pdBuf)/4){//一包就能发完
					 atlasJsonReadPack(C,readNum,len,0,len);
			     rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_kprintf("%s send atlas--C\n",sign);
				}
				else{//分两包发
					 atlasJsonReadPack(C,readNum,len,0,len/2);
					 rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_thread_mdelay(10);
					 atlasJsonReadPack(C,readNum,len,len/2,(len/2));
					 rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					 rt_thread_mdelay(10);
				}
		}
		
}

