#include "test_hardware.h"
#include <stdint.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>

#include "drv_usart.h"

#include "drv_config.h"

#if 0
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
char SDPath[4]; /* SD逻辑驱动器路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            			  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 F407开发板 今天是个好日子，新建文件系统测试文件\r\n";  

extern FATFS flash_fs;
extern Diskio_drvTypeDef  SD_Driver;

void testSD()
{
	 /* 初始化独立按键 */
   printf("****** 这是一个SD卡文件系统实验 ******\r\n");
   //链接驱动器，创建盘符
   FATFS_LinkDriver(&SD_Driver, SDPath);
   //在外部SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
   res_sd = f_mount(&fs,"0:",1);  


   /*----------------------- 格式化测试 ---------------------------*/  
   /* 如果没有文件系统就格式化创建创建文件系统 */
   if(res_sd == FR_NO_FILESYSTEM)
   {
      printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
      /* 格式化 */
      res_sd=f_mkfs("0:",0,0);							

      if(res_sd == FR_OK)
      {
        printf("》SD卡已成功格式化文件系统。\r\n");
        /* 格式化后，先取消挂载 */
        res_sd = f_mount(NULL,"0:",1);			
        /* 重新挂载	*/			
        res_sd = f_mount(&fs,"0:",1);
      }
      else
      {

        printf("《《格式化失败。》》\r\n");
        while(1);
      }
   }
   else if(res_sd!=FR_OK)
   {
      printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
      printf("！！可能原因：SD卡初始化不成功。\r\n");
      while(1);
   }
   else
   {
      printf("》文件系统挂载成功，可以进行读写测试\r\n");
   }	
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
    res_sd = f_open(&fnew, "0:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
      printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
      /* 将指定存储区内容写入到文件内 */
      res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
      if(res_sd==FR_OK)
      {
        printf("》文件写入成功，写入字节数据：%d\n",fnum);
        printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
      }
      else
      {
        printf("！！文件写入失败：(%d)\n",res_sd);
      }    
      /* 不再读写，关闭文件 */
      f_close(&fnew);
    }
    else
    {	

      printf("！！打开/创建文件失败。\r\n");
    }
    
  /*------------------- 文件系统测试：读测试 ------------------------------------*/
    printf("****** 即将进行文件读取测试... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(res_sd == FR_OK)
    {

      printf("》打开文件成功。\r\n");
      res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
      if(res_sd==FR_OK)
      {
        printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
        printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
      }
      else
      {
        printf("！！文件读取失败：(%d)\n",res_sd);
      }		
    }
    else
    {

      printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);	
    
    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);
    
}
#endif
////////////////////////////////////
#ifdef TEST_UART


uint8_t testRecBuf[200];
int testCount=0;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
void uart2SendRecTest()
{
		static uint8_t Res;//rt_kprintf("rt_UART\n");
		if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				HAL_UART_Receive(&huart2,&Res,1,1000); 
				testRecBuf[testCount++]=Res;
			  UART2_485_SEND;
			  HAL_UART_Transmit(&huart2,&Res,1,1000);
			  UART2_485_REC;
			  if((Res=='\n')||(testCount>=10)){
			    testRecBuf[testCount++]=0;
					UART2_485_SEND;
					HAL_UART_Transmit(&huart2,(uint8_t *)testRecBuf,(uint16_t)strlen((char *)testRecBuf),1000);
					UART2_485_REC;
					testCount = 0;
				}
		}
		HAL_UART_IRQHandler(&huart2);	
}
void uart3SendRecTest()
{
		static uint8_t Res;//rt_kprintf("rt_UART\n");
		if((__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				HAL_UART_Receive(&huart3,&Res,1,1000); 
				testRecBuf[testCount++]=Res;
			  if(Res=='\n'){
			    testRecBuf[testCount++]=0;
					UART3_485_SEND;
					HAL_UART_Transmit(&huart3,(uint8_t *)testRecBuf,(uint16_t)strlen((char *)testRecBuf),1000);
					UART3_485_REC;
					testCount = 0;
				}
			 
		}
		HAL_UART_IRQHandler(&huart3);	
}
void uart4SendRecTest()
{
		static uint8_t Res;//rt_kprintf("rt_UART\n");
		if((__HAL_UART_GET_FLAG(&huart4,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				HAL_UART_Receive(&huart4,&Res,1,1000); 
				testRecBuf[testCount++]=Res;
			  if(Res=='\n'){
			    testRecBuf[testCount++]=0;
					UART4_485_SEND;
					HAL_UART_Transmit(&huart4,(uint8_t *)testRecBuf,(uint16_t)strlen((char *)testRecBuf),1000);
					UART4_485_REC;
					testCount = 0;
				}
			 
		}
		HAL_UART_IRQHandler(&huart4);	
}

void uart5SendRecTest()
{
		static uint8_t Res;//rt_kprintf("rt_UART\n");
		if((__HAL_UART_GET_FLAG(&huart5,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				HAL_UART_Receive(&huart5,&Res,1,1000); 
				testRecBuf[testCount++]=Res;
			  if(Res=='\n'){
			    testRecBuf[testCount++]=0;
		
					HAL_UART_Transmit(&huart5,(uint8_t *)testRecBuf,(uint16_t)strlen((char *)testRecBuf),1000);

					testCount = 0;
				}
			 
		}
		HAL_UART_IRQHandler(&huart5);	
}
void uart6SendRecTest()
{
	 static uint8_t Res;//rt_kprintf("rt_UART\n");
		if((__HAL_UART_GET_FLAG(&huart6,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
				HAL_UART_Receive(&huart6,&Res,1,1000); 
				testRecBuf[testCount++]=Res;
			  if(Res=='\n'){
			    testRecBuf[testCount++]=0;
					UART6_485_SEND;
					HAL_UART_Transmit(&huart6,(uint8_t *)testRecBuf,(uint16_t)strlen((char *)testRecBuf),1000);
					UART6_485_REC;
					testCount = 0;
				}
			 
		}
		HAL_UART_IRQHandler(&huart6);	
}

#endif
#ifdef  TEST_ADC
extern ADC_HandleTypeDef hadc1;


//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
//uint16_t Get_Adc(uint32_t ch)   
//{
//    ADC_ChannelConfTypeDef ADC1_ChanConf;
//    
//    ADC1_ChanConf.Channel=ch;                                   //通道
//    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
//    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
//    ADC1_ChanConf.Offset=0;                 
//    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
//	
//    HAL_ADC_Start(&hadc1);                               //开启ADC
//	
//    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
// 
//	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        //返回最近一次ADC1规则组的转换结果
//}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
//uint16_t Get_Adc_Average(uint16_t ch,uint16_t times)
//{
//		uint16_t temp_val=0;
//		uint16_t t;
//		for(t=0;t<times;t++)
//		{
//				temp_val+=Get_Adc(ch);
//				rt_thread_mdelay(5);
//		}
//		return temp_val/times;
//} 
void adcGetTest()
{

				int rt=Get_Adc_Average(ADC_CHANNEL_8,10);
				rt_kprintf("ADC_CHANNEL_8 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_9,10);
				rt_kprintf("ADC_CHANNEL_9 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_10,10);
				rt_kprintf("ADC_CHANNEL_10 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_11,10);
				rt_kprintf("ADC_CHANNEL_11 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_12,10);
				rt_kprintf("ADC_CHANNEL_12 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_13,10);
				rt_kprintf("ADC_CHANNEL_13 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_14,10);
				rt_kprintf("ADC_CHANNEL_14 ,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_15,10);
				rt_kprintf("ADC_CHANNEL_15 ,adc tick:%d,voltgge 扩大100倍:%dV\n\r\n\r\n",rt,rt*330/4096);
}

#endif

#ifdef TEST_IOIN
static void ioInTest()
{
	   if(HAL_GPIO_ReadPin(IO_IN1_GPIO_Port, IO_IN1_Pin)==GPIO_PIN_SET){
				rt_kprintf("1路高\r\n");
		 }
		 else{
				rt_kprintf("1路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN2_GPIO_Port, IO_IN2_Pin)==GPIO_PIN_SET){
				rt_kprintf("2路高\r\n");
		 }
		 else{
				rt_kprintf("2路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN3_GPIO_Port, IO_IN3_Pin)==GPIO_PIN_SET){
				rt_kprintf("3路高\r\n");
		 }
		 else{
				rt_kprintf("3路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN4_GPIO_Port, IO_IN4_Pin)==GPIO_PIN_SET){
				rt_kprintf("4路高\r\n");
		 }
		 else{
				rt_kprintf("4路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN5_GPIO_Port, IO_IN5_Pin)==GPIO_PIN_SET){
				rt_kprintf("5路高\r\n");
		 }
		 else{
				rt_kprintf("5路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN6_GPIO_Port, IO_IN6_Pin)==GPIO_PIN_SET){
				rt_kprintf("6路高\r\n");
		 }
		 else{
				rt_kprintf("6路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN7_GPIO_Port, IO_IN7_Pin)==GPIO_PIN_SET){
				rt_kprintf("7路高\r\n");
		 }
		 else{
				rt_kprintf("7路低\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN8_GPIO_Port, IO_IN8_Pin)==GPIO_PIN_SET){
				rt_kprintf("8路高\r\n");
		 }
		 else{
				rt_kprintf("8路低\r\n");
		 }
		 
			 
}
#endif

#ifdef TEST_IOOUT
static void ioOutTest()//注意调用函数中的延时加上此处的延时
{
			IOOUT1_ON;
	    IOOUT2_ON;
			IOOUT3_ON;
			IOOUT4_ON;
			IOOUT5_ON;
			IOOUT6_ON;
			IOOUT7_ON;
			IOOUT8_ON;
			rt_thread_mdelay(1000);
			IOOUT1_OFF;
	    IOOUT2_OFF;
			IOOUT3_OFF;
			IOOUT4_OFF;
			IOOUT5_OFF;
			IOOUT6_OFF;
			IOOUT7_OFF;
			IOOUT8_OFF;
			rt_thread_mdelay(1000);

}
#endif






//#ifdef  TEST_SPAKER
//static void  spakerTest()
//{
//			SPAKER1_ON;
//	    SPAKER2_ON;
//	    SPAKER3_ON;
//	    SPAKER4_ON;

//			rt_thread_mdelay(5000);
//			SPAKER1_OFF;
//	    SPAKER2_OFF;
//	    SPAKER3_OFF;
//	    SPAKER4_OFF;

//			rt_thread_mdelay(5000);
//}

//#endif
#ifdef  TEST_RELAY
static void  relayTest()
{
			RELAY1_ON;
	    RELAY2_ON;
	    RELAY3_ON;
	    RELAY4_ON;

			rt_thread_mdelay(5000);
			RELAY1_OFF;
	    RELAY2_OFF;
	    RELAY3_OFF;
	    RELAY4_OFF;

			rt_thread_mdelay(5000);
}

#endif

#ifdef   TEST_W5500


#endif
//放在main函数中 while中掉用测试 注意while中的延时会跟本函数叠加
//IOoutput  
//adc
//ioinput
//uart在drv_gpio中测试
void  hardWareDriverTest(void)
{
		
#ifdef TEST_IOOUT
		ioOutTest();//注意调用函数中的延时加上此处的延时
#endif
	
#ifdef TEST_ADC
		adcGetTest();
#endif
	
#ifdef TEST_IOIN
    ioInTest();
#endif
	
#ifdef  TEST_RELAY
		relayTest();
#endif
	
//#ifdef  TEST_SPAKER
//   spakerTest();
//#endif
#ifdef  TEST_W5500
		w5500Test();
#endif
}
//TX_RX_MAX_BUF_SIZE=16*1024  以及MAX_SOCK_NUM=1

//获取tick值
//不能打印 uint64_t 数据  拆分成32位打印
int tick()
{
    RTC_TimeTypeDef read;
	  uint64_t time =utcTime_ms();

	  uint32_t rtc_s=time/1000;
	  uint32_t rtc_ms=time%1000;
		read=utc_to_beijingTime(rtc_s);
		rt_kprintf("[tick][%lu]-s\r\n",rtc_s);//不能同时打印需要分开打印才正确
		rt_kprintf("[tick][%lu]-ms\r\n",rtc_ms);
	  rt_kprintf("[tick]%d年%d月%d日%d时%d分%d秒\r\n",read.year,read.month,read.day,\
  	read.hour,read.minute,read.second );
	
	  return 0;
}
//FINSH_FUNCTION_EXPORT(tick, tick finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(tick,tick stamp);//FINSH_FUNCTION_EXPORT_CMD




//extern uint64_t subTimeStamp;
// void  subTimeStampSet(uint64_t time)
//{
//	  if(time>=rt_tick_get())
//				subTimeStamp=time-rt_tick_get();//服务器rtc值-当前tick值
//		else
//				subTimeStamp = 0;

//}
int tickSet(int argc, char *argv[])
{


		static uint64_t u64getTick_p;

		u64getTick_p =atoll(argv[1]);
//		rt_kprintf("%stime:[%lu]s \r\n",sign, (uint32_t)((u64getTick_p)/1000));

//		rt_kprintf("%stime:[%lu]ms\r\n",sign, (uint32_t)(u64getTick_p)%1000);
	  extern void  subTimeStampSet(uint64_t time);
	  if(utcTime_ms()-u64getTick_p>=3000){
        subTimeStampSet(u64getTick_p);
        correctLcdTime(u64getTick_p/1000);
			  rt_kprintf("%stime:RTC 误差大于3秒 校时\r\n","[TEST]");
		}
	  

	  return 0;
}
//FINSH_FUNCTION_EXPORT(tick, tick finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(tickSet,tick set stamp);//FINSH_FUNCTION_EXPORT_CMD



uint32_t 	offLTimes;
uint32_t 	onLTimes;

//实现网络掉线的监测  掉线次数 每次掉线时长，
//如果拔掉网线情况下时长不准确 需要加上心跳周期 或者发送数据的间隔时长 
//拔掉网线时候需要发送数据才能验证网络断开
int offline()
{

		extern rt_bool_t gbNetState;
		extern rt_bool_t gbNetResp;



    if(gbNetState==RT_FALSE)
			gbNetResp=RT_FALSE;


		
		rt_kprintf("[offLine]net online state %d ,net resp state %d \r\n",gbNetState,gbNetResp);
		return 0;
}
//FINSH_FUNCTION_EXPORT(offline, offline finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(offline,offline stamp);//FINSH_FUNCTION_EXPORT_CMD



void sdSize_p(int *tot,int *fre)
{
	DWORD fre_clust;
	FATFS *pfs;
	extern rt_mutex_t sdWrite_mutex;
	rt_mutex_take(sdWrite_mutex,RT_WAITING_FOREVER);
//	int tot_sect,fre_sect;
  int res_flash = f_getfree("0:", &fre_clust, &pfs); 
	
  if(FR_OK==0){
  /* 计算得到总的扇区个数和空扇区个数 */
		 *tot = (pfs->n_fatent - 2) * pfs->csize;
		 *fre = fre_clust * pfs->csize;
	}
	else{
		rt_kprintf("err:sdSize read %d\n",FR_OK);
	}
	rt_mutex_release(sdWrite_mutex);
}
int SDSize()
{
	if(gbSDExit==false){
				goto ERROR;
	}
//	DWORD fre_clust;
//	FATFS *pfs;
	int tot_sect,fre_sect;
//  int res_flash = f_getfree("0:", &fre_clust, &pfs); 
//	
//  if(FR_OK==0){
//  /* 计算得到总的扇区个数和空扇区个数 */
//		 tot_sect = (pfs->n_fatent - 2) * pfs->csize;
//		 fre_sect = fre_clust * pfs->csize;
//	}
//	else{
//		rt_kprintf("err:sdSize read %d\n",FR_OK);
//	}
  sdSize_p(&tot_sect,&fre_sect);
  /* 打印信息(4096 字节/扇区) */
	ERROR:
	if(gbSDExit==true){
		  rt_kprintf("设备总空间:%ukB 可用空间:%ukB。\n", (tot_sect>>1), (fre_sect>>1));
			rt_kprintf("设备总空间:%.2fGB 可用空间:%.2fGB。\n", (float)tot_sect/2/1024/1024, (float)fre_sect/2/1024/1024);
	}
	else{
			rt_kprintf("err:sd not exit\n");
	}
	return 0;
}
//FINSH_FUNCTION_EXPORT(offline, offline finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(SDSize,SDSize stamp);//FINSH_FUNCTION_EXPORT_CMD




