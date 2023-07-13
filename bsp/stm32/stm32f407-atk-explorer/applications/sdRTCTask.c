#include <rtthread.h>
#include <board.h>
char  sdData[DATA_LEN]="";//写入sd的数据  用于保存各个传感器的采集数据
const static char task[]="[sdRTCTask]";
extern struct rt_mailbox mbsdWriteData;
extern void  logSaveToSD(char *buf,char lenth);
extern void RingBuff2_Init(void);
extern uint8_t Read_RingBuff2(uint8_t *rData);
char printBuf[1024];
int bufLen=0;
//extern bool log_save_sdFlag;
const char acuRst[]="ACU RESET>>>>>>>>>>>>>>\n";
extern char printVer[50];
extern void creatFolder(void);


extern void FatReadDirDelEarlyLogTxt(void);
//static int count=0;
rt_bool_t getUTCFlag=RT_FALSE;
void getUTCFromLCD()
{
		uint32_t lcdUtcTime_beijing();
		void  subTimeStampSet(uint64_t time);
	  uint64_t timeRead1,timeRead2;
    timeRead1=lcdUtcTime_beijing();//不能简化
		timeRead2=(timeRead1*1000)+500;//不能简化
	  subTimeStampSet(timeRead2);//不能简化直接带入形参 带入会导致计算错误	
}
void  logPrint();




//log打印函数  注册进idle钩子函数里
void  logPrint()
{
				bufLen=0;
			  while(true==Read_RingBuff2((uint8_t *)printBuf+bufLen)){

						if((bufLen+1)==sizeof(printBuf)){
							printBuf[bufLen-1]='\n';
							printBuf[bufLen]=0;
							logSaveToSD(printBuf,strlen(printBuf));
							//printf("%s",printBuf);
							bufLen=0;
							break;
						}
					  if(printBuf[bufLen]=='\n'){
							  printBuf[bufLen+1]=0;
								logSaveToSD(printBuf,strlen(printBuf));
							  //printf("%s",printBuf);
							  bufLen=0;
							  break;
						}
						bufLen++;
				}

}
//1分钟的时钟基准情况下 
//log和数据记录按天存储的情况西下 
void  sdCheck()
{
		static int count =0;
	  
		count++;
		if(count%60==0){
				FatReadDirDelEarlyLogTxt();//每隔TXT_LOG_TIME/10秒时间检查一次删除比较早的txt文件
			  rt_kprintf("%scheck sd delete log\n",task);
			  
		}
}
void  rtcCheck()
{
		static int count =0;

		if(getUTCFlag==RT_FALSE){//没有同步RTC时钟 每隔1分钟同步一次
				getUTCFromLCD();
			  rt_kprintf("%scheck UTC\n",task);
		}
}


extern void huanLiuDelEarlyTxt(void);
extern void juFangDelEarlyTxt(void);
extern void fCJiangDelEarlyTxt(void);
extern void fWaiPoDelEarlyTxt(void);
extern void coDelEarlyTxt(void);
extern void o2DelEarlyTxt(void);
extern void wenShiDuDelEarlyTxt(void);
extern void shuiWeiDelEarlyTxt(void);
extern void h2sDelEarlyTxt(void);
extern void lieFengYiDelEarlyTxt(void);
extern void jingGaiDelEarlyTxt(void);
extern void ch4DelEarlyTxt(void);
void  modbusDataCheck()
{
			static int count =30;
	    count++;
	    if(count%60==0){
					huanLiuDelEarlyTxt();
				  juFangDelEarlyTxt();
					fCJiangDelEarlyTxt();
				  fWaiPoDelEarlyTxt();
					coDelEarlyTxt();
					o2DelEarlyTxt();
					wenShiDuDelEarlyTxt();
					shuiWeiDelEarlyTxt();
					h2sDelEarlyTxt();
					lieFengYiDelEarlyTxt();
					jingGaiDelEarlyTxt();
					ch4DelEarlyTxt();
				  rt_kprintf("%scheck DATA\n",task);
			}
			
}
int log_save_sdFlag=0;
void  sdAndRtcInit()
{
		rt_thread_mdelay(350);//延时2秒 不能去掉 需要等待lcd启动稳定并且同步时钟完毕再写入sd卡
	  extern void changeBmp(int num);
	  changeBmp(0);
    getUTCFromLCD();
  	creatFolder();
//	  rt_thread_idle_sethook(logPrint);
    log_save_sdFlag=1;
		logSaveToSD((char *)acuRst, strlen(acuRst));
		logSaveToSD((char *)printVer, strlen(printVer));
}




//sd定期检查是不是存满 进行删除  
//rtc核对是否同步过时钟
void   sdRTCTask(void *parameter)
{
		sdAndRtcInit();
	  int count=0;
		while(1){
#if 1
			  count++;
			  if(count>3000){//大约1分钟进去一次
					count=0;
					sdCheck();
					rtcCheck();
					modbusDataCheck();
				}
				logPrint();
#endif
			  rt_thread_mdelay(20);
		}
}