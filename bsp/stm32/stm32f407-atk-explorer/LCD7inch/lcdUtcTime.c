#include "board.h"
#include "time.h"
#include "stdlib.h"
time_t t_unix;//ʱ���

//RT_LIBC_DEFAULT_TIMEZONE  �Ѿ�ƫ��8��Сʱʱ��



/**************************************

* @brief Unixʱ���ת����ʱ��

* @param ����ʱ���ֵ

****************************************/

RTC_TimeTypeDef utc_to_beijingTime(uint32_t time)
{
		time_t t_unix;//ʱ���
		struct tm *lt;
		RTC_TimeTypeDef t_location;//����ʱ��
		t_unix=time;//Unixʱ�䣫8СʱΪ����ʱ��
		lt=localtime(&t_unix);//��RT_LIBC_DEFAULT_TIMEZONE  �Ѿ�ƫ��8��Сʱʱ��
		t_location.year=lt->tm_year-100;
		t_location.month=lt->tm_mon+1;
		t_location.day=lt->tm_mday;
		t_location.hour =lt->tm_hour;
		t_location.minute=lt->tm_min;
		t_location.second=lt->tm_sec;
		return t_location;
}

/**************************************

* @brief ����ʱ��תUnixʱ���

* @param ����ʱ���

****************************************/

uint32_t beijingTime_to_utc(RTC_TimeTypeDef t_location)
{
		time_t t_unix;//ʱ���
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

//* @brief UTCʱ��ת���ɱ���ʱ�䣬����ʱ��=UTC+8

//* @param

//****************************************/

//RTC_TimeTypeDef utcSwitchLocal(RTC_TimeTypeDef temp_utc)
//{

//		RTC_TimeTypeDef temp_rtc;

//		time_t t_unix;//ʱ���

//		t_unix=beijingTime_to_utc(temp_utc);

//		t_unix+=8*60*60;//ʱ�����ϰ˸�Сʱ

//		temp_rtc=utc_to_beijingTime(t_unix);

//		return temp_rtc;

//}


//װ��ʱ�䵽RTC��
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
//У׼��ʾ����RTCʱ��
//1����ȡlcd��rtcʱ��
//2. �Ƚ�һ�� ƫ����� 3��
//3����RTCд��ʱ��
void  correctLcdTime(uint32_t time)
{
	  uint32_t ret=0;
	  memset(&readRtc,0,sizeof(readRtc));
		readLcdRTC();
	  rt_thread_mdelay(100);  //��ʱ�ȴ���Ӧ
		if(readRtc.year!=0){
				if((ret=abs(time-beijingTime_to_utc(readRtc)))>3){
					  rt_kprintf("time corret[%d]\n",ret);
					  writeRtc=utc_to_beijingTime(time+1);
					  writeRTCToLcd(writeRtc);
				}
		}
}

//ÿ���ϵ���ȡ������  subTimeStampSet()����
uint32_t lcdUtcTime_beijing()
{

	  memset(&readRtc,0,sizeof(readRtc));
		readLcdRTC();
	  rt_thread_mdelay(100);  //��ʱ�ȴ���Ӧ
	
	 	rt_kprintf("[read]%d��%d��%d��%dʱ%d��%d��\r\n",readRtc.year,readRtc.month,readRtc.day,\
  	readRtc.hour,readRtc.minute,readRtc.second );
	  return beijingTime_to_utc(readRtc);
}

