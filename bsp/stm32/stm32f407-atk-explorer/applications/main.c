/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */
 
 
 
 
//dmodbus    HuanLiu  GYNJLXSD000000563 GY289 port5 1 60 
//modbus    HuanLiu  GYNJLXSD000000562 GY289 port5 2 60 
//modbus     JuFang  GYNJLXSD000000665 GY298 port5 3 60 
//modbus FangChenJiang GYNJLXSD000000781 GY299 port8 1 100 
//modbus  FangWaiPo GYNJLXSD000000782 GY299 port8 2 100 
//modbus     JiaWan  GYNJLXSD00000068 GY299 port7 1 100 
//modbus LiuHuaQing GYNJLXSD000000683 GY299 port7 3 100 
//modbus  LieFengYi GYNJLXSD000000783 GY299 port8 9 100 

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>

//#define APP_VER       ((4<<8)+22)//0x0105 表示1.5版本
#define APP_VER       ((4<<8)+25)//0x0105 表示1.5版本
//注：本代码中json格式解析非UTF8_格式代码（GB2312格式中文） 会导致解析失败
//    打印log如下 “[dataPhrs]err:json cannot phrase”  20230403
const char date[]="20240108";

bool USE_MQTT=true;

//显示软件版本
void LCDDispSoftVer()
{
		char buf[10]="V_";
		sprintf((char *)buf+2,"%02d.%02d",(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  LCDWtite(SOFT_VER_ADDR ,(uint8_t *)buf,strlen(buf));
}
//static    rt_thread_t tid 	= RT_NULL;
static    rt_thread_t tidW5500 	  = RT_NULL;
static    rt_thread_t tidNetRec 	= RT_NULL;
static    rt_thread_t tidNetSend 	= RT_NULL;
static    rt_thread_t tidUpkeep 	= RT_NULL;
static    rt_thread_t tidLCD      = RT_NULL;
static    rt_thread_t tidAutoCtrl = RT_NULL;
static    rt_thread_t tidMqtt     = RT_NULL;
static    rt_thread_t tidSdRTC 		=	RT_NULL;


//互斥信号量定义
rt_mutex_t   read485_mutex	=	RT_NULL;//防止多个线程同事读取modbus数据
rt_mutex_t   lcdSend_mutex	=	RT_NULL;//防止多个线程同事往lcd发数据
rt_mutex_t   w5500Spi_mutex	=	RT_NULL;//防止多个线程同时操作spi
rt_mutex_t   sdWrite_mutex	=	RT_NULL;
rt_mutex_t   printf_mutex		=	RT_NULL;
rt_sem_t   		uart1234_sem	=	RT_NULL;
rt_sem_t   		uart5678_sem	=	RT_NULL;

//邮箱的定义
extern struct  rt_mailbox mbNetSendData;;
static char 	 mbSendPool[20];//发送缓存20/4条
static char 	 mbRecPool[20];//发送缓存20/4条
extern struct rt_mailbox mbNetRecData;

static char 	 sdWritePool[100];//发送缓存100条
extern struct rt_mailbox mbsdWriteData;


/* 事件控制块 */
struct rt_event mqttAckEvent;
rt_bool_t gbSDExit=false;

#ifdef USE_WDT
struct rt_event WDTEvent;
#endif





//队列的定义 

#if   USE_RINGBUF

#else
	struct  rt_messagequeue LCDmque;//= {RT_NULL} ;//创建LCD队列
	uint8_t LCDQuePool[LCD_BUF_LEN];  //创建lcd队列池
#endif




//struct  rt_messagequeue SDLogmque;//= {RT_NULL} ;//创建LCD队列
//uint8_t SDLogQuePool[100];  //创建lcd队列池
//任务的定义
extern  void   netDataRecTask(void *para);//网络数据接收
extern  void   netDataSendTask(void *para);//网络数据发送
extern  void   upKeepStateTask(void *para);//定时打包数据 后期可能加入定时读取modbus
extern  void   w5500Task(void *parameter);//w5500网络状态的维护
extern  void   hardWareDriverTest(void);
extern  void   LCDTask(void *parameter);
extern  void   autoCtrlTask(void *para);
extern  void   logSaveSDTask(void *para);
extern  void   WDTTask(void *parameter);
extern  void   sdRTCTask(void *parameter);
extern  void   mqttTask(void *parameter);
const  static char sign[]="[main]";
extern rt_bool_t gbNetState;

/* 定时器的控制块 */
static rt_timer_t timer1;
rt_bool_t gbNetState_p=RT_TRUE;
static  int netTick=0;
static void netStateSet()
{
		if(netOKState()==RT_TRUE){
			if(gbNetState_p==RT_FALSE){
						netTick++;
						if(netTick>=30){
								gbNetState_p=RT_TRUE;	
						}
				}
  	}
		else{
					gbNetState_p=RT_FALSE;
				  netTick=0;
		}
}
void  netLedLight()
{
	  static int count=0;
    count++;
		if(gbNetState_p==RT_TRUE){
				if(count%10==0){
						HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				}
		}
		else{
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		}
}
//extern uint32_t gbLcdRecTime;//接收到屏幕数据清0  无操作2分钟内返回主界面
//static int cnt = 0;
/* 定时器1超时函数 每100ms进去一次*/
//10秒提醒一次 uart offline状态
static void timeout1(void *parameter)
{
		static int count=0;
	  extern void timeInc();
	  count++;  
		if(TESTCODE_READ==GPIO_PIN_SET){//工装测试不启动定时器
				if(gbSDExit==false){
						if(count%(100)==0)//10秒提醒一下
								printf("%s请插入TF卡并重启设备\n",sign);
				}
				netLedLight();
		}
	  if(count%10==0){//1秒进去一次
				timeInc();

		}
		
		netStateSet();
}

extern void creatFolder(void);
void  outIOInit(void);
char  printVer[50];
extern void hartWareTest();



int main(void)
{


    printf("\n%s%s  ver=%02d.%02d\n",sign,date,(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  sprintf(printVer,"%s%s  ver=%02d.%02d\n",sign,date,(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  rt_err_t result;
		stm32_flash_read(FLASH_IP_SAVE_ADDR,    (uint8_t*)&packFlash,sizeof(packFlash));
		stm32_flash_read(FLASH_MODBUS_SAVE_ADDR,(uint8_t*)&sheet,    sizeof(sheet));
	  if(0xff==(int)packFlash.protol)
			USE_MQTT=0;
		else
			USE_MQTT=(bool)packFlash.protol;
	  printf(" save1=%d save2=%d MQTT=%d\n",sizeof(packFlash),sizeof(sheet),USE_MQTT);
	  if(packFlash_LEN<sizeof(packFlash)){
				printf("err:packFlash_LEN too small\n");
		}
	  if((packFlash_LEN+sizeof(packFlash)+sizeof(sheet))>65536){		
				printf("err:64k flash to save user data too small\n");
		}
	  outIOInit();
	  RingBuff2_Init();
		if(packFlash.acuId[0]>=0x7F){
				rt_strcpy(packFlash.acuId,"000000000000001");//必须加上 执行cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		}    
		  /* 创建定时器 周期定时器 */

		timer1 = rt_timer_create("timer1", timeout1,
														 RT_NULL, 100,
														 RT_TIMER_FLAG_PERIODIC);
		if (timer1 != RT_NULL)
				rt_timer_start(timer1);

		//创建485设备用到的互斥 队列
		read485_mutex= rt_mutex_create("read485_mutex", RT_IPC_FLAG_FIFO);
		if (read485_mutex == RT_NULL)
    {
        printf("%screate read485_mutex failed\n",sign);
    }
		lcdSend_mutex= rt_mutex_create("lcdSend_mutex", RT_IPC_FLAG_FIFO);
		if (lcdSend_mutex == RT_NULL)
    {
        printf("%screate lcdSend_mutex failed\n",sign);
    }
		w5500Spi_mutex= rt_mutex_create("w5500Spi_mutex", RT_IPC_FLAG_FIFO);
		if (w5500Spi_mutex == RT_NULL)
    {
        printf("%screate w5500Spi_mutex failed\n",sign);
    }	
		sdWrite_mutex= rt_mutex_create("sdWrite_mutex", RT_IPC_FLAG_FIFO);
		if (sdWrite_mutex == RT_NULL)
    {
        printf("%screate sdWrite_mutex failed\n",sign);
    }		
		printf_mutex= rt_mutex_create("printf_mutex", RT_IPC_FLAG_FIFO);
		if (printf_mutex == RT_NULL)
    {
        printf("%screate printf_mutex failed\n",sign);
    }		
		uart1234_sem= rt_sem_create("uart1234_sem",0, RT_IPC_FLAG_FIFO);
		if (uart1234_sem == RT_NULL)
    {
        printf("%screate uart1234_sem failed\n",sign);
    }	
		uart5678_sem= rt_sem_create("uart5678_sem",0, RT_IPC_FLAG_FIFO);
		if (uart5678_sem == RT_NULL)
    {
        printf("%screate uart5678_sem failed\n",sign);
    }		
#if   USE_RINGBUF

#else
		int ret = rt_mq_init(&LCDmque,"LCDrecBuf",&LCDQuePool[0],1,LCD_BUF_LEN,RT_IPC_FLAG_FIFO);       
		if (ret != RT_EOK)
		{
				printf("%sinit LCD msgque failed.\n",sign);
				return -1;
		}
#endif
//		extern void 	uartMutexQueueCreate();
//		uartMutexQueueCreate();
		
//	 ret = rt_mq_init(&SDLogmque,"SDLogmque",&SDLogQuePool[0],1,sizeof(SDLogQuePool),RT_IPC_FLAG_FIFO);       
//	if (ret != RT_EOK)
//	{
//			rt_kprintf("%sinit SDLogmque  failed.\n",sign);
//			return -1;
//	}
		
		


////////////////////////////////////邮箱//////////////////////////////////
		
    result = rt_mb_init(&mbsdWriteData,"sdsend",&sdWritePool[0],sizeof(sdWritePool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox mbsdWriteData failed.\n",sign);
        return -1;
    }
    result = rt_mb_init(&mbNetRecData,"mbRec",&mbRecPool[0],sizeof(mbRecPool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox NetRecData failed.\n",sign);
        return -1;
    }
    result = rt_mb_init(&mbNetSendData,"mbSend",&mbSendPool[0],sizeof(mbSendPool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox NetSend failed.\n",sign);
        return -1;
    }
#ifdef  USE_WDT   
		if (rt_event_init(&WDTEvent, "WDTEvent", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("%sinit WDTEvent failed.\n",sign);

    }
#endif


////////////////////////////////任务////////////////////////////////////

		tidLCD    =  rt_thread_create("LCD",LCDTask,RT_NULL,512*2,2, 10 );
		if(tidLCD!=NULL){
				rt_thread_startup(tidLCD);													 
				printf("%sRTcreat LCDStateTask \r\n",sign);
		}

		hartWareTest();//工装测试
		
		tidNetRec =  rt_thread_create("netRec",netDataRecTask,RT_NULL,512*2,3, 10 );
		if(tidNetRec!=NULL){
				rt_thread_startup(tidNetRec);													 
				printf("%sRTcreat netDataRecTask \r\n",sign);
		}
		tidNetSend =  rt_thread_create("netSend",netDataSendTask,RT_NULL,512*2,3, 10 );
		if(tidNetSend!=NULL){
				rt_thread_startup(tidNetSend);													 
				printf("%sRTcreat netDataSendTask \r\n",sign);
		}
    tidW5500 =  rt_thread_create("w5500",w5500Task,RT_NULL,512*3,2, 10 );
		if(tidW5500!=NULL){
				rt_thread_startup(tidW5500);													 
				printf("%sRTcreat w5500Task task\r\n",sign);
		}

		tidUpkeep 	=  rt_thread_create("upKeep",upKeepStateTask,RT_NULL,512*4,4, 10 );
		if(tidUpkeep!=NULL){
				rt_thread_startup(tidUpkeep);													 
				printf("%sRTcreat upKeepStateTask \r\n",sign);
		}


		tidMqtt = rt_thread_create("mqtt",mqttTask,RT_NULL,512*2,4, 10 );
		if(tidMqtt!=NULL){
				rt_thread_startup(tidMqtt);													 
				rt_kprintf("RTcreat mqtt task\r\n");
		}
		else{
				rt_kprintf("RTcreat mqtt ERR\r\n");
		}		
///////////////////////////////////事件标志组////////////////////////////
    if (rt_event_init(&mqttAckEvent, "mqttAckEvent", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("%sinit mqttAckEvent failed.\n",sign);

    }
		tidSdRTC =  rt_thread_create("sdRTC",sdRTCTask,RT_NULL,512*4,8, 10 );
		if(tidSdRTC!=NULL){
				rt_thread_startup(tidSdRTC);													 
				printf("%sRTcreat sdRTCTask\r\n",sign);
		}
		tidAutoCtrl =  rt_thread_create("autoCtrl",autoCtrlTask,RT_NULL,512*2,5, 10 );
		if(tidAutoCtrl!=NULL){
				rt_thread_startup(tidAutoCtrl);													 
				printf("%sRTcreat autoCtrlTask\r\n",sign);
		}

#ifdef  USE_WDT
		extern IWDG_HandleTypeDef hiwdg;
		static    rt_thread_t tidWDT      = RT_NULL;
		tidWDT =  rt_thread_create("WDT",WDTTask,RT_NULL,256,10, 10 );
		if(tidWDT!=NULL){
				rt_thread_startup(tidWDT);													 
				printf("%sRTcreat WDTTask\r\n",sign);
		}
		HAL_IWDG_Refresh(&hiwdg);
#endif	
		//队列初始化之后再开启串口中断接收
//		char test[50]={0};
//			uint32_t a=4294967290;
//			sprintf(test,"long:%u", a);//"monitoringTime":"1655172531937"
//			rt_kprintf("%s %s \r\n",sign,test);
//////////////////////////////结束//////////////////////////////////////

}

extern rt_bool_t shieldLog;//屏蔽log 
extern int mod_printf(const char *fmt, ...);
//tasklog delete  线程挂起时候才能删除   
void  tasklog(int argc, char *argv[])
{
		if(argc!=2){
				goto ERR;
		}
		if(rt_strcmp("delete",argv[1])==0){
//			  if((tidW5500->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
//						rt_thread_delete(tidW5500);
//				if((tidNetRec->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
//						rt_thread_delete(tidNetRec);
//				if((tidNetSend->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
//						rt_thread_delete(tidNetSend);
//				if((tidUpkeep->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
//						rt_thread_delete(tidUpkeep);
//				if((tidAutoCtrl->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
//						rt_thread_delete(tidAutoCtrl);
				shieldLog=RT_TRUE;
			  mod_printf("[tasklog delete OK]\n");
			  return;
		}
		ERR:
		mod_printf("err for example:\n");
		mod_printf("[tasklog delete]\n");
}
MSH_CMD_EXPORT(tasklog,tasklog del);//FINSH_FUNCTION_EXPORT_CMD


//网络连接成功标志 
bool netOKState()
{
		if(USE_MQTT){
			return mqttState();
		}
		else{
			return gbNetState;
		}
}
#ifdef   USE_WDT

extern IWDG_HandleTypeDef hiwdg;
#endif
void hartWareTest()
{
	//	int read=TESTCODE_READ;//读取工装测试电平
	  void sdAndRtcInit();
		enum swichStep step= IO_inout_step;
		static bool change=true;
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);//开启中断 清除串口冗余数据
		sdAndRtcInit();
    if(TESTCODE_READ!=GPIO_PIN_RESET){
				return;;//
		}
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		rt_thread_mdelay(500);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    while (1)//task用于测试 以及闪灯操作
    {
			
			#ifdef  USE_WDT
				HAL_IWDG_Refresh(&hiwdg);
			#endif
			  if(TESTSWITCH_READ==GPIO_PIN_RESET){
					rt_thread_mdelay(20);
					if(TESTSWITCH_READ==GPIO_PIN_RESET){
						HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
						while(TESTSWITCH_READ==GPIO_PIN_RESET);
						HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
						//等待按键放开
							rt_kprintf("GPIO_PIN_SET\n");
							 step++;
							 if(step>INTERNET_step){
									step=IO_inout_step;
								  change=true;
									if((tidW5500->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
											rt_thread_delete(tidW5500);
									if((tidNetRec->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
											rt_thread_delete(tidNetRec);
									if((tidNetSend->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
											rt_thread_delete(tidNetSend);
							 }
							// rt_kprintf("切换step\n");
							 
					}
				}
				
			  switch(step){

					case IO_inout_step:
						ioInOutTest();
					  rt_thread_mdelay(1000);
						break;
					case ADC_in_step:
						adcGetTest();
					  rt_thread_mdelay(1000);
						break;
					case Uart_step:
						uartTest();
				  	//rt_thread_mdelay(1000);
//					 rt_kprintf("12345step\n");
						break;
					case SWITCH_step:
						relayTest();
						break;

					case INTERNET_step:
						if(change){
							
							change=false;
							tidNetRec =  rt_thread_create("netRec",netDataRecTask,RT_NULL,512*2,3, 10 );
							if(tidNetRec!=NULL){
									rt_thread_startup(tidNetRec);													 
									printf("%sRTcreat netDataRecTask \r\n",sign);
							}
							tidNetSend =  rt_thread_create("netSend",netDataSendTask,RT_NULL,512*2,3, 10 );
							if(tidNetSend!=NULL){
									rt_thread_startup(tidNetSend);													 
									printf("%sRTcreat netDataSendTask \r\n",sign);
							}
							tidW5500 =  rt_thread_create("w5500",w5500Task,RT_NULL,512*3,2, 10 );
							if(tidW5500!=NULL){
									rt_thread_startup(tidW5500);													 
									printf("%sRTcreat w5500Task task\r\n",sign);
							}
						}
					  //rt_thread_mdelay(1000);
						break;
				}
				//hardWareDriverTest();
    }
		
}
