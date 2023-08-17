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



extern ADC_HandleTypeDef hadc1;


void adcGetTest()
{
	   rt_kprintf("adc采集测试\n");

			int rt=Get_Adc_Average(ADC_CHANNEL_9,10);
			rt_kprintf("1路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
			rt=Get_Adc_Average(ADC_CHANNEL_8,10);
			rt_kprintf("2路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_15,10);
			rt_kprintf("3路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_14,10);
			rt_kprintf("4路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_13,10);
			rt_kprintf("5路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_12,10);
			rt_kprintf("6路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_11,10);
			rt_kprintf("7路,adc tick:%d,voltgge 扩大100倍:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_10,10);
			rt_kprintf("8路,adc tick:%d,voltgge 扩大100倍:%dV\n\r\n\r\n",rt,rt*330/4096);
}


 void ioInOutTest()
{
	   rt_kprintf("IO口输入输出测试\n");
	   if(HAL_GPIO_ReadPin(IO_IN1_GPIO_Port, IO_IN1_Pin)==GPIO_PIN_SET){
				rt_kprintf("1路高\r\n");
				IOOUT1_ON;
		 }
		 else{
				rt_kprintf("1路低\r\n");
				IOOUT1_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN2_GPIO_Port, IO_IN2_Pin)==GPIO_PIN_SET){
				rt_kprintf("2路高\r\n");
				IOOUT2_ON;
		 }
		 else{
				rt_kprintf("2路低\r\n");
				IOOUT2_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN3_GPIO_Port, IO_IN3_Pin)==GPIO_PIN_SET){
				rt_kprintf("3路高\r\n");
				IOOUT3_ON;
		 }
		 else{
				rt_kprintf("3路低\r\n");
				IOOUT3_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN4_GPIO_Port, IO_IN4_Pin)==GPIO_PIN_SET){
				rt_kprintf("4路高\r\n");
				IOOUT4_ON;
		 }
		 else{
				rt_kprintf("4路低\r\n");
				IOOUT4_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN5_GPIO_Port, IO_IN5_Pin)==GPIO_PIN_SET){
				rt_kprintf("5路高\r\n");
				IOOUT5_ON;
		 }
		 else{
				rt_kprintf("5路低\r\n");
				IOOUT5_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN6_GPIO_Port, IO_IN6_Pin)==GPIO_PIN_SET){
				rt_kprintf("6路高\r\n");
				IOOUT6_ON;
		 }
		 else{
				rt_kprintf("6路低\r\n");
				IOOUT6_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN7_GPIO_Port, IO_IN7_Pin)==GPIO_PIN_SET){
				rt_kprintf("7路高\r\n");
				IOOUT7_ON;
		 }
		 else{
				rt_kprintf("7路低\r\n");
				IOOUT7_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN8_GPIO_Port, IO_IN8_Pin)==GPIO_PIN_SET){
				rt_kprintf("8路高\r\n");
				IOOUT8_ON;
		 }
		 else{
				rt_kprintf("8路低\r\n");
				IOOUT8_OFF;
		 }
		 
			 
}

 void ioOutTest()//注意调用函数中的延时加上此处的延时
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


static const char testSBuf[11]="1234567890";
//static char testRBuf[UART_NUM-1][11]={0};
static char testRBuf[11];
//static char trecBuf[11]="";


//				rt_thread_mdelay(timeout);
//				while(true==Read_RingBuff3(recBuf+count)){
//					count++;
//					rt_thread_mdelay(2);
//				}
//				
void rs485TestUseUart6()
{
	   int count=0;
    uint8_t Read_RingBuff3(uint8_t *rData);
	   memset(testRBuf,0,sizeof(testRBuf));
				while(true==Read_RingBuff3((uint8_t *)testRBuf+count)){
					
					if(count>=sizeof(testRBuf)){
						testRBuf[10]=0;
						//break;
					}
					else{
						count++;
					}
					rt_thread_mdelay(2);
				}
			  if(strncmp(testSBuf,(char *)testRBuf,strlen((char *)testSBuf))==0){
						rt_kprintf("uart[8] 收发测试成功\n");
				}
				else{
						rt_kprintf("uart[8] 收发测试失败\n");
//					  testRBuf[j][10]=0;
					 // rt_kprintf("rec;[%s]\n",testRBuf[j]);
				}
}
				
#if 0

extern rt_sem_t   uart1234_sem;
extern rt_sem_t   uart5678_sem;
int  rs485UartRecp(int chanl,int timeout)
{
	  int count=0;
	  uint8_t gifr=0;
		switch(chanl)
		{

			case 0:
			case 1:
			case 2:
			case 3:
			if(rt_sem_take(uart1234_sem,timeout)==RT_EOK)
			{
				
			//	rt_kprintf("485 1234\n");
				gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
					do{
							//判断子串口1是否有中断
							if(gifr&GIFR_UT1INT_BIT){ /*数据处理*///pcb映射串口1+1
									/*数据接收*/
									 count=Wk1234UartRxChars(1,(uint8_t *)testRBuf[1]);//一次接收的数据不会超过256Byte
								   rt_kprintf("485 1read\n");
								  return count;
							}	
							//判断子串口2是否有中断
							if(gifr&GIFR_UT2INT_BIT){//pcb映射串口2+1
								/*数据接收*/
									 count=Wk1234UartRxChars(2,(uint8_t *)testRBuf[2]);//一次接收的数据不会超过256Byte
									 rt_kprintf("485 2read\n");
								  return count;
							}
							//判断子串口3是否有中断
							if(gifr&GIFR_UT3INT_BIT){//pcb映射串口0+1
										/*数据接收*/
									count=Wk1234UartRxChars(3,(uint8_t *)testRBuf[0]);//一次接收的数据不会超过256Byte
								  rt_kprintf("485 0read\n");
								  return count;
							}
							//判断子串口4是否有中断
							if(gifr&GIFR_UT4INT_BIT){//pcb映射串口3+1
										/*数据接收*/
									 count=Wk1234UartRxChars(4,(uint8_t *)testRBuf[3]);//一次接收的数据不会超过256Byte
								   rt_kprintf("485 3read\n");
								  return count;
							}
							
							gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
					}while(gifr&0x0f);
				}
			case 4:
			case 5:
			case 6:
			case 7:
			if(rt_sem_take(uart5678_sem,timeout)==RT_EOK)
			{
				rt_kprintf(":485 5678\n");
				gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					do{
							//判断子串口1是否有中断
							if(gifr&GIFR_UT1INT_BIT){ /*数据处理*///pcb映射串口7
									/*数据接收*/
									 count=Wk5678UartRxChars(1,(uint8_t *)testRBuf[5]);//一次接收的数据不会超过256Byte
								rt_kprintf("485 6read\n");
							}	
							//判断子串口2是否有中断
							if(gifr&GIFR_UT2INT_BIT){//pcb映射串口5
								/*数据接收*/
									 count=Wk5678UartRxChars(2,(uint8_t *)testRBuf[6]);//一次接收的数据不会超过256Byte
								rt_kprintf("485 4read\n");
							}
							//判断子串口3是否有中断
							if(gifr&GIFR_UT3INT_BIT){//pcb映射串口6
										/*数据接收*/
									count=Wk5678UartRxChars(3,(uint8_t *)testRBuf[4]);//一次接收的数据不会超过256Byte
								rt_kprintf("485 5read\n");
							}
							//判断子串口4是否有中断
							if(gifr&GIFR_UT4INT_BIT){//pcb映射串口8
										/*数据接收*/
									 count=Wk5678UartRxChars(4,(uint8_t *)testRBuf[7]);//一次接收的数据不会超过256Byte
								rt_kprintf("485 7read\n");
							}
							
							gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					}while(gifr&0x0f);
				}
				break;
			}
		return count;
}
#endif
void uartTest()
{
	  static int confFlag=false;
	  if(confFlag==false){
				for(int i=0;i<UART_NUM;i++){

						packFlash.uartBps[i]=115200;

				}
				extern void uartReconfig();
				uartReconfig();
				confFlag=true;
		}
	  for(int j=0;j<UART_NUM-1;j++){
			  memset(testRBuf,0,sizeof(testRBuf));
				rs485UartSend(j,(uint8_t *)testSBuf,strlen(testSBuf));
			 // rs485TestUseUart6();

				rs485UartRec(j,(uint8_t *)testRBuf,100);
			  if(strncmp(testSBuf,(char *)testRBuf,strlen((char *)testSBuf))==0){
						rt_kprintf("uart[%d] 收发测试成功\n",j);
				}
				else{
						rt_kprintf("uart[%d] 收发测试失败\n",j);
//					  testRBuf[j][10]=0;
					 // rt_kprintf("rec;[%s]\n",testRBuf[j]);
				}
		}
		rt_thread_mdelay(100);
		rs485TestUseUart6();
}




void  relayTest()
{
	   //static bool relay=false;
			rt_kprintf("继电器输出测试\n");
			RELAY1_ON;
	    RELAY2_ON;
	    RELAY3_ON;
	   // RELAY4_ON;

			rt_thread_mdelay(500);
			RELAY1_OFF;
	    RELAY2_OFF;
	    RELAY3_OFF;
//	    RELAY4_OFF;

			rt_thread_mdelay(500);
}



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
MSH_CMD_EXPORT(tickSet,tick set stamp);//FINSH_FUNCTION_EXPORT_CMD



uint32_t 	offLTimes;
uint32_t 	onLTimes;

//实现网络掉线的监测  掉线次数 每次掉线时长，
//如果拔掉网线情况下时长不准确 需要加上心跳周期 或者发送数据的间隔时长 
//拔掉网线时候需要发送数据才能验证网络断开
int offline()
{

	//	extern rt_bool_t gbNetState;
		extern rt_bool_t gbNetResp;



    if(netOKState()==RT_FALSE)
			gbNetResp=RT_FALSE;


		
		rt_kprintf("[offLine]net online state %d ,net resp state %d \r\n",netOKState(),gbNetResp);
		return 0;
}
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
	int tot_sect,fre_sect;
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




