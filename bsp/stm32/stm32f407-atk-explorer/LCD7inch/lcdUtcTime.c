#include "board.h"
#include "time.h"
#include "stdlib.h"
time_t t_unix;//时间撮

//RT_LIBC_DEFAULT_TIMEZONE  已经偏移8个小时时区



/**************************************

* @brief Unix时间戳转北京时间

* @param 带入时间戳值

****************************************/

RTC_TimeTypeDef utc_to_beijingTime(uint32_t time)
{
		time_t t_unix;//时间撮
		struct tm *lt;
		RTC_TimeTypeDef t_location;//本地时间
		t_unix=time;//Unix时间＋8小时为北京时间
		lt=localtime(&t_unix);//在RT_LIBC_DEFAULT_TIMEZONE  已经偏移8个小时时区
		t_location.year=lt->tm_year-100;
		t_location.month=lt->tm_mon+1;
		t_location.day=lt->tm_mday;
		t_location.hour =lt->tm_hour;
		t_location.minute=lt->tm_min;
		t_location.second=lt->tm_sec;
		return t_location;
}

/**************************************

* @brief 北京时间转Unix时间戳

* @param 返回时间戳

****************************************/

uint32_t beijingTime_to_utc(RTC_TimeTypeDef t_location)
{
		time_t t_unix;//时间撮
		struct tm tmCurrentTime;
		tmCurrentTime.tm_year=t_location.year+2000-1900;
		tmCurrentTime.tm_mon=t_location.month-1;
		tmCurrentTime.tm_mday=t_location.day;
		tmCurrentTime.tm_hour=t_location.hour;
		tmCurrentTime.tm_min=t_location.minute;
		tmCurrentTime.tm_sec=t_location.second;
		t_unix = mktime(&tmCurrentTime);
		return (uint32_t)(t_unix);//1686550004  0x6486B5F4
}

///**************************************

//* @brief UTC时间转化成北京时间，北京时间=UTC+8

//* @param

//****************************************/

//RTC_TimeTypeDef utcSwitchLocal(RTC_TimeTypeDef temp_utc)
//{

//		RTC_TimeTypeDef temp_rtc;

//		time_t t_unix;//时间撮

//		t_unix=beijingTime_to_utc(temp_utc);

//		t_unix+=8*60*60;//时间撮加上八个小时

//		temp_rtc=utc_to_beijingTime(t_unix);

//		return temp_rtc;

//}


//装载时间到RTC中
void  writeRTCToLcd(RTC_TimeTypeDef time)
{
	  uint8_t buf[8];

	  buf[0]=0x5a;
	  buf[1]=0xa5;
	  buf[2]=time.year;
	  buf[3]=time.month;
	  buf[4]=time.day;
	  buf[5]=time.hour;
	  buf[6]=time.minute;
	  buf[7]=time.second;
	  extern int LCDWtite(uint16_t addr,uint8_t *data,uint8_t dataLen);
	rt_kprintf("writetoLCD:%x %x %x %x %x %x \n ",time.year,time.month,time.day,time.hour,time.minute,time.second);
	  LCDWtite(0x009c,buf,sizeof(buf));
}


void  readLcdRTC()
{
	  uint8_t data=0x04;
	  extern void LCDRead(uint16_t addr,uint8_t *data,uint8_t dataLen);
	  LCDRead(0x0010,&data,1);
}



RTC_TimeTypeDef readRtc;
RTC_TimeTypeDef writeRtc;
RTC_TimeTypeDef copyBufToRtc(char *buf)
{
	  RTC_TimeTypeDef read;
		read.year=buf[7];
	  read.month=buf[8];
	  read.day=buf[9];
	  read.hour =buf[11];
	  read.minute=buf[12];
	  read.second=buf[13];
	
	  return read;
}
//校准显示屏的RTC时间
//1。读取lcd的rtc时间
//2. 比较一下 偏差大于 3秒
//3。往RTC写入时间
void  correctLcdTime(uint32_t time)
{
	  uint32_t ret=0;
	  memset(&readRtc,0,sizeof(readRtc));
		readLcdRTC();
	  rt_thread_mdelay(100);  //延时等待回应
		if(readRtc.year!=0){
				if((ret=abs(time-beijingTime_to_utc(readRtc)))>3){
					  rt_kprintf("time corret[%d]\n",ret);
					  writeRtc=utc_to_beijingTime(time+1);
					  writeRTCToLcd(writeRtc);
				}
		}
}

//每次上电后读取来矫正  subTimeStampSet()调用
uint32_t lcdUtcTime_beijing()
{

	  memset(&readRtc,0,sizeof(readRtc));
		readLcdRTC();
	  rt_thread_mdelay(100);  //延时等待回应
	
	 	rt_kprintf("[read]%d年%d月%d日%d时%d分%d秒\r\n",readRtc.year,readRtc.month,readRtc.day,\
  	readRtc.hour,readRtc.minute,readRtc.second );
	  return beijingTime_to_utc(readRtc);
}

