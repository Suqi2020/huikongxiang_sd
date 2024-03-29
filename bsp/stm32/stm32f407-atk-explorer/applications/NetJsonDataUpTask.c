#include   "NetJsonDataTask.h"
#include   "board.h"


rt_bool_t gbRegFlag = RT_FALSE;
extern struct rt_mailbox mbNetSendData;

//extern rt_bool_t gbNetState;
const static char task[]="[dataUp]";




extern void 	readPSTempHeight(int num);
extern void  	PSTempHeightPack(void);
extern void  	readThreeTempAcc(int num);
extern void		t3AxisTempAccPack(void);

extern void 	cirCurrConf(void);
#define TIM_NUM  (MODBUS_NUM+2) //目前支持定时器个数+heart+reg
typedef struct 
{
		uint16_t count;     //计数
	  uint16_t threshoVal;//阈值
}timerStru;
static timerStru tim[TIM_NUM];
//给个定时值


//启动从0开始计时
static void timeRestart(upDataTimEnum num)
{
	  
				tim[num].count=0;
}
//初始化后会自动运行
//num 第几个定时器
//value 定时器值
//firstCnt 第一次计数值  为了防止定时器值同时到达 
 void timeInit(upDataTimEnum num,int value,int firstCnt)
{
	  if(value==0){   // if((num>=TIM_NUM)||(value==0)){ 
				rt_kprintf("%stim[%d] inint failed \n",task,num);
				return;
		}
		rt_kprintf("tim init %d %d\n",num,value);
		tim[num].threshoVal=value;
		tim[num].count=firstCnt;
}
//每1秒递增一次
void timeInc()
{
	  for(int i=0;i<TIM_NUM;i++){
			  if(tim[i].count!=0xFFFF)
						tim[i].count++;
		}
}

//快速设置 用于快速来读取modbus数据
static void timeQuckIncSet()
{
	  for(int i=0;i<TIM_NUM;i++){
			  if(i==PARTDISCHAG_TIME)
					tim[i].count=sheet.partDischagColTime-10;
				else
						tim[i].count=0xF000+i*2;

		}
}
//停止
 void timeStop(upDataTimEnum num)
{
		tim[num].count=0xFFFF;
}

//定时时间到
static int timeOut()
{
	  for(int i=0;i<TIM_NUM;i++){
				if(tim[i].threshoVal!=0){
						if(tim[i].count!=0xFFFF){
								if(tim[i].count>=tim[i].threshoVal){
									timeRestart((upDataTimEnum)i);
									rt_kprintf("tim[%d] out\n",i);
									rt_kprintf("[%d %d]\n",tim[i].threshoVal,tim[i].count);
									return i;
								}
						}
				}
		}
		return 0xff;
}



extern void pressSettRead2Send(rt_bool_t netStat,bool respFlag);
extern void threeAxisRead2Send(rt_bool_t netStat,bool respFlag);
extern void partDischagRead2Send(rt_bool_t netStat,bool respFlag);
extern void circulaRead2Send(rt_bool_t netStat,bool respFlag);
extern void waterDepthRead2Send(rt_bool_t netStat,bool respFlag);
extern void tempHumRead2Send(rt_bool_t netStat,bool respFlag);
extern void o2Read2Send(rt_bool_t netStat,bool respFlag);
extern void h2sRead2Send(rt_bool_t netStat,bool respFlag);
extern void ch4Read2Send(rt_bool_t netStat,bool respFlag);
extern void coRead2Send(rt_bool_t netStat,bool respFlag);
//void analogTempHumJsonPack(uint8_t chanl);
#ifndef     ANA_MASK
extern void anaTempHumReadPack2Send(bool gbNetState,bool respFlag);
#endif
extern  uint16_t devRegJsonPack(void);
extern  uint16_t heartUpJsonPack(void);
//extern uint8_t analogTemChanl;
extern void gasJsonPack(rt_bool_t netStat,bool respFlag);
extern  uint16_t digitalInputReport(void);
extern  uint16_t digitalOutputReport(char *identify);
extern  void crackMeterRead2Send(rt_bool_t netStat,bool respFlag);
extern  void coverRead2Send(rt_bool_t netStat,bool respFlag);


//定时时间到  执行相应事件
static void  timeOutRunFun()
{

		rt_mutex_take(read485_mutex,RT_WAITING_FOREVER);
		switch(timeOut()){
	
			case HEART_TIME://心跳
				if(!USE_MQTT){
					heartUpJsonPack();
					//jsonBufPackTest();
					if(netOKState()==RT_TRUE)
							rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					rt_kprintf("%sheart timer out\r\n",task);
				}
			break;
			
			case REG_TIME://注册 注册成功后定时器就关闭 输入输出状态跟谁注册信息上发
			  if(gbRegFlag==RT_FALSE){
//					partDischgAtlasResp("12345");//test 
////					 jsonBufPackTest();
//					rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
				//	break;//suqi
					  devRegJsonPack();//devRegJsonPack();
					  if(netOKState()==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
					//  timeStop(REG_TIME);//正式使用时候需要去掉
					
							  int ret=digitalInputReport();//数字输入上报
								if(ret!=0) {
									 if(netOKState()==RT_TRUE)
										rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
										rt_thread_delay(500);
							  }
								ret=digitalOutputReport("12v_output");
								
								if(ret!=0){ 
									if(netOKState()==RT_TRUE)
									  rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
										rt_thread_delay(500);
								}
								ret=digitalOutputReport("digital_output");
								if(ret!=0){
									if(netOKState()==RT_TRUE)
								rt_mb_send_wait(&mbNetSendData, (rt_ubase_t)&NetTxBuffer,RT_WAITING_FOREVER); 
								rt_thread_delay(500);

						}
				}
				else
						timeStop(REG_TIME);
				rt_kprintf("%sreg timer out\r\n",task);
				break;
			case CIRCULA_TIME://读取环流
				circulaRead2Send(netOKState(),false);
				rt_kprintf("%sCIRCULA_TIME out\r\n",task);
				break;
			case PARTDISCHAG_TIME://读取局放
				partDischagRead2Send(netOKState(),false);
				rt_kprintf("%sPARTDISCHAG_TIME out\r\n",task);
				break;
			case PRESSSETTL_TIME:
        pressSettRead2Send(netOKState(),false);
				rt_kprintf("%sPRESSSETTL_TIME out\r\n",task);
				break;
			case THREEAXIS_TIME:
				threeAxisRead2Send(netOKState(),false);
				rt_kprintf("%sTHREEAXIS_TIMEout\r\n",task);
				break;

			case  CH4_TIME:
				ch4Read2Send(netOKState(),false);
				break;
			case  O2_TIME:
				o2Read2Send(netOKState(),false);
				break;
			case  H2S_TIME:
				h2sRead2Send(netOKState(),false);
				break;
			case  CO_TIME://4种气体在一起读取 所以前三个不使用 只在此处读取并打包发送  关闭时候只需要关闭CO就可以把所有气体全部关闭

			  coRead2Send(netOKState(),false);
				break;
			case  TEMPHUM_TIME:
				tempHumRead2Send(netOKState(),false);
				break;
			case  WATERDEPTH_TIME:
				waterDepthRead2Send(netOKState(),false);
				break;
			case CRACKMETER_TIME:
				crackMeterRead2Send(netOKState(),false);
				rt_kprintf("%sTHREEAXIS_TIMEout\r\n",task);
				break;
			case COVER_TIME:
				coverRead2Send(netOKState(),false);
				rt_kprintf("%sCOVER_TIME out\r\n",task);
				break;
#ifndef     ANA_MASK
			case  ANA_TEMPHUM_TIME:
//				analogTempHumJsonPack(analogTemChanl);
			  anaTempHumReadPack2Send(gbNetState,false);
		
				break;
#endif
			
			default:
				break;
		}
		rt_mutex_release(read485_mutex);
}

modbusFunStru modbusFun[MODBUS_NUM];
//启动定时器列表
void startTimeList()
{
    for(int k=0;k<TIM_NUM;k++){
			timeStop((upDataTimEnum)k);
		}
		
		#if 0
		timeInit(HEART_TIME,      120,2);//心跳定时  定时30秒 第一次28秒就来
		timeInit(REG_TIME,        5,0);//注册 注册成功后定时器就关闭
		timeInit(CIRCULA_TIME, 		5,5);
		timeInit(PARTDISCHAG_TIME,5,10);
		timeInit(PRESSSETTL_TIME, 8,15);
		timeInit(THREEAXIS_TIME,  8,20);
	  timeInit(H2S_TIME, 				sheet.h2sColTime,24);
		timeInit(CH4_TIME, 				sheet.ch4ColTime,28);
		timeInit(O2_TIME, 				sheet.o2ColTime,30);

		timeInit(GAS_TIME, 				10,35);

		timeInit(TEMPHUM_TIME, 		12,40);
		timeInit(WATERDEPTH_TIME, 15,45);
		timeInit(CRACKMETER_TIME, 16,50);
		#else 
		if(!USE_MQTT){
			timeInit(HEART_TIME,      120,2);//心跳定时  定时30秒 第一次28秒就来
		}
		timeInit(REG_TIME,        60,0);//注册 注册成功后定时器就关闭



		timeInit(CIRCULA_TIME, 		sheet.cirCulaColTime,5);//suqi
		timeInit(PARTDISCHAG_TIME,sheet.partDischagColTime,10);
		timeInit(PRESSSETTL_TIME, sheet.pressSetlColTime,15);
		timeInit(THREEAXIS_TIME,  sheet.threeAxissColTime,20);
	  timeInit(H2S_TIME, 				sheet.h2sColTime,24);
		timeInit(CH4_TIME, 				sheet.ch4ColTime,28);
		timeInit(O2_TIME, 				sheet.o2ColTime,30);

		timeInit(CO_TIME, 				sheet.coColTime,35);

		timeInit(TEMPHUM_TIME, 		sheet.tempHumColTime,40);
		timeInit(WATERDEPTH_TIME, sheet.waterDepthColTime,45);
		timeInit(CRACKMETER_TIME, sheet.crackMeterColTime,50);
		timeInit(COVER_TIME, 			sheet.coverColTime,55);
#endif
	  //启动温湿度
#ifndef     ANA_MASK
	  for(int i=0;i<ANALOG_NUM;i++){
				if(rt_strcmp(sheet.analog[i].name,analogName[0])==0){//用 analogName[0]  指明是温湿度
					  if(sheet.analog[i].workFlag==RT_TRUE){
								timeInit(ANA_TEMPHUM_TIME,sheet.analog[i].colTime,30);
								//timeInit(ANA_TEMPHUM_TIME,10,30);
//								analogTemChanl=i;
						}
						break;
				}
		}
#endif
		timeQuckIncSet();
}



#if 1
//上行数据的维护以及重发


void   upKeepStateTask(void *para)
{
		extern void uartReconfig();
		extern void uartIrqEnaAfterQueue();
	  extern void clearUartData();
	  extern void readMultiCirCulaPoint();
	  extern void RingBuff3_Init();
	  
	  uartReconfig();//串口重新配置
		uartIrqEnaAfterQueue();//串口中断中用到了队列  开启中断需要放到后边
    startTimeList();//开启计时器列表
	  //clearUartData();
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);//开启中断 清除串口冗余数据
	  RingBuff3_Init();//清除buf 清除串口冗余数据
	  rt_thread_mdelay(2000);//如果主控板与modbus设备同时上电 modbus设备会延时启动来保证modbus设备稳定启动
	  readMultiCirCulaPoint();//对于环流来讲 运行前需要提取扩大方式
	
  
	  
		

		while(1){
				timeOutRunFun();
				rt_thread_mdelay(500);
#ifdef  USE_WDT
			  rt_event_send(&WDTEvent,EVENT_WDT_UPTASK);
#endif		  
		}
}


#endif

