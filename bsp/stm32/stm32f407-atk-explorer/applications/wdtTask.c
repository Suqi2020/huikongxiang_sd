#include <rtthread.h>
#include <board.h>
#ifdef   USE_WDT

extern IWDG_HandleTypeDef hiwdg;


//���Ź����32�븴λ ����10����ι��һ��
void   WDTTask(void *parameter)
{
	  uint32_t e;
		while(1){
				if(rt_event_recv(&WDTEvent,\
					(EVENT_WDT_AUTOCTRL|EVENT_WDT_W5500|EVENT_WDT_UPTASK|EVENT_WDT_LCDTASK|EVENT_WDT_RECTASK|EVENT_WDT_SENDTASK),\
					RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,10000, &e) == RT_EOK){
						HAL_IWDG_Refresh(&hiwdg);
						//rt_kprintf("feed dog 0x%x\n ",e);
					}
					//else
				//	rt_kprintf("feed dog err 0x%x\n ",e);
		}
}

#endif
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


extern void FatReadDirDelEarlyTxt(void);
//static int count=0;
rt_bool_t getUTCFlag=RT_FALSE;
void getUTCFromLCD()
{
		uint32_t lcdUtcTime_beijing();
		void  subTimeStampSet(uint64_t time);
	  uint64_t timeRead1,timeRead2;
    timeRead1=lcdUtcTime_beijing();//���ܼ�
		timeRead2=(timeRead1*1000)+500;//���ܼ�
	  subTimeStampSet(timeRead2);//���ܼ�ֱ�Ӵ����β� ����ᵼ�¼������	
}
void  logPrint();
//void   logSaveSDTask(void *parameter)
//{
////		uint32_t e;
//		//	  char *str=RT_NULL;
//		

//	  rt_thread_mdelay(2500);//��ʱ2�� ����ȥ�� ��Ҫ�ȴ�lcd�����ȶ�����ͬ��ʱ�������д��sd��
//    getUTCFromLCD();
//  	creatFolder();
//	  rt_thread_idle_sethook(logPrint);

//		logSaveToSD((char *)acuRst, strlen(acuRst));
//		logSaveToSD((char *)printVer, strlen(printVer));
//		while(1){
////				bufLen=0;
////			  while(true==Read_RingBuff2((uint8_t *)printBuf+bufLen)){
////					  //printf("/%c",printBuf[bufLen]);
////					  if(printBuf[bufLen]=='\n'){
////							  printBuf[bufLen+1]=0;
////								printf("%s",printBuf);
////								logSaveToSD(printBuf,strlen(printBuf));
////							 // bufLen=0;
////							  break;
////						}
////						if((bufLen+1)==sizeof(printBuf)){
////							printBuf[bufLen-1]='\n';
////							printBuf[bufLen]=0;
////							printf("%s",printBuf);
////							bufLen=0;
////							break;
////						}
////						bufLen++;
////				}
//				rt_thread_mdelay(50);
//				if(count++%(TXT_LOG_TIME*10)==0){
//						FatReadDirDelEarlyTxt();//ÿ��TXT_LOG_TIME/10��ʱ����һ��ɾ���Ƚ����txt�ļ�
//				}
//				if(getUTCFlag==RT_FALSE){//û��ͬ��RTCʱ�� ÿ��1����ͬ��һ��
//						if(count%1200==0){
//								getUTCFromLCD();
//						}
//				}
////			  if (rt_mb_recv(&mbsdWriteData, (rt_ubase_t *)&str, 100) == RT_EOK)
////        { 
////					  printf("<%d%s> \n\n",strlen(str),str);
////					  logSaveToSD(str,strlen(str));
////						
////				}
//		
//#ifdef  USE_WDT
//			rt_event_send(&WDTEvent,EVENT_WDT_SENDTASK);
//#endif
//		}
//}



//log��ӡ����  ע���idle���Ӻ�����
void  logPrint()
{
				bufLen=0;
			  while(true==Read_RingBuff2((uint8_t *)printBuf+bufLen)){
					  //printf("/%c",printBuf[bufLen]);
					  if(printBuf[bufLen]=='\n'){
							  printBuf[bufLen+1]=0;
								printf("%s",printBuf);
								logSaveToSD(printBuf,strlen(printBuf));
							 // bufLen=0;
							  break;
						}
						if((bufLen+1)==sizeof(printBuf)){
							printBuf[bufLen-1]='\n';
							printBuf[bufLen]=0;
							printf("%s",printBuf);
							bufLen=0;
							break;
						}
						bufLen++;
				}
}
//1���ʱ�ӻ�׼����� 
void  sdAndRtcCheck()
{
		static int count =0;
	  
		count++;
		if(count%(TXT_LOG_TIME/10)==0){
				FatReadDirDelEarlyTxt();//ÿ��TXT_LOG_TIME/10��ʱ����һ��ɾ���Ƚ����txt�ļ�
		}
		if(getUTCFlag==RT_FALSE){//û��ͬ��RTCʱ�� ÿ��1����ͬ��һ��
				if(count%10==0){
						getUTCFromLCD();
				}
		}
//		if(gbSDExit==false){
//				if(count%10==0){
//						creatFolder();
//				}
//		}
}

void  sdAndRtcInit()
{
		rt_thread_mdelay(350);//��ʱ2�� ����ȥ�� ��Ҫ�ȴ�lcd�����ȶ�����ͬ��ʱ�������д��sd��
    getUTCFromLCD();
  	creatFolder();
	  rt_thread_idle_sethook(logPrint);

		logSaveToSD((char *)acuRst, strlen(acuRst));
		logSaveToSD((char *)printVer, strlen(printVer));
}