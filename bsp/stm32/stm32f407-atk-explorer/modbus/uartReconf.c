#include "board.h"

//配置每个串口对应的波特率 以及modbus传感器
//从网口端数来依次是 UART2(1) UART3(2) UART6(3) UART1(debug) UART4(4)
//目前的配置是       环流  局放  沉降仪             三轴



const static char sign[]="[uartRecfg]";
//uartConfStru  uartDev[UART_NUM];
//uartChanlStru  chanl={USE_DIS_UART,USE_DIS_UART,USE_DIS_UART,USE_DIS_UART};
//static rt_mutex_t uartMutex[UART_NUM] ;//= {RT_NULL}; //创建4个串口的互斥量保护

//队列的定义
//struct  rt_messagequeue uartmque[UART_NUM];//= {RT_NULL} ;//创建4个串口的队列
//static uint8_t uartQuePool[UART_NUM][MSGPOOL_LEN];  //创建4个串口的队列池
extern rt_sem_t   uart1234_sem;
extern rt_sem_t   uart5678_sem;

//串口接收  发送完读取modbus命令后调用
//输入0-8  一共9个485串口
//数据接收
//输出返回数据长度
extern uint8_t Read_RingBuff3(uint8_t *rData);
int  rs485UartRec(int chanl,uint8_t *recBuf,int timeout)
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
				
				rt_kprintf("485 1read\n");
				gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
					do{
							//判断子串口1是否有中断
							if(gifr&GIFR_UT1INT_BIT){ /*数据处理*/
									/*数据接收*/
									 count=Wk1234UartRxChars(1,recBuf);//一次接收的数据不会超过256Byte
							}	
							//判断子串口2是否有中断
							if(gifr&GIFR_UT2INT_BIT){
								/*数据接收*/
									 count=Wk1234UartRxChars(2,recBuf);//一次接收的数据不会超过256Byte
							}
							//判断子串口3是否有中断
							if(gifr&GIFR_UT3INT_BIT){
										/*数据接收*/
									count=Wk1234UartRxChars(3,recBuf);//一次接收的数据不会超过256Byte
							}
							//判断子串口4是否有中断
							if(gifr&GIFR_UT4INT_BIT){
										/*数据接收*/
									 count=Wk1234UartRxChars(4,recBuf);//一次接收的数据不会超过256Byte
							}
							
							gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
					}while(gifr&0x0f);
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			if(rt_sem_take(uart5678_sem,timeout)==RT_EOK)
			{
				rt_kprintf(":485 2read\n");
				gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					do{
							//判断子串口1是否有中断
							if(gifr&GIFR_UT1INT_BIT){ /*数据处理*/
									/*数据接收*/
									 count=Wk5678UartRxChars(1,recBuf);//一次接收的数据不会超过256Byte
							}	
							//判断子串口2是否有中断
							if(gifr&GIFR_UT2INT_BIT){
								/*数据接收*/
									 count=Wk5678UartRxChars(2,recBuf);//一次接收的数据不会超过256Byte
							}
							//判断子串口3是否有中断
							if(gifr&GIFR_UT3INT_BIT){
										/*数据接收*/
									count=Wk5678UartRxChars(3,recBuf);//一次接收的数据不会超过256Byte
							}
							//判断子串口4是否有中断
							if(gifr&GIFR_UT4INT_BIT){
										/*数据接收*/
									 count=Wk5678UartRxChars(4,recBuf);//一次接收的数据不会超过256Byte
							}
							
							gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					}while(gifr&0x0f);
				}
				break;
			case 8://单独一路  备用
				rt_thread_mdelay(timeout);
				while(true==Read_RingBuff3(recBuf+count)){
					count++;
					rt_thread_mdelay(2);
				}
				break;
		}
		return count;
}





//串口重新配置 选择对应的modbus传感器
void uartReconfig()
{
		for(int i=0;i<UART_NUM;i++){
				if(packFlash.uartBps[i]==0){//没用到的串口 给个默认波特率 
					  packFlash.uartBps[i]=9600;
				}
		}
		for(int i=0;i<UART_NUM;i++){
				rt_kprintf("%sport%d bps[%d]\n",sign,i+1,packFlash.uartBps[i]);
		}
	


		MX_USART6_UART_Init(packFlash.uartBps[8]);
		
		Wk1xxxRstInit();
		/*主串口波特率匹配*/
		Wk1234MasterUartBaudAdaptive();
//		//初始化子串口
		Wk1234UartInit(1);
		Wk1234UartInit(2);
		Wk1234UartInit(3);
		Wk1234UartInit(4);
//		//设置子串口波特率
		Wk1234UartSetBaud(3,packFlash.uartBps[0]);
		Wk1234UartSetBaud(1,packFlash.uartBps[1]);
		Wk1234UartSetBaud(2,packFlash.uartBps[2]);
		Wk1234UartSetBaud(4,packFlash.uartBps[3]);
		Wk1234UartRS485(1);
		Wk1234UartRS485(2);
		Wk1234UartRS485(3);
		Wk1234UartRS485(4);
//		
//		
//		
//		
		Wk2xxxRstInit();
//		/*主串口波特率匹配*/
		Wk5678MasterUartBaudAdaptive();
//		//初始化子串口
		Wk5678UartInit(1);
		Wk5678UartInit(2);
		Wk5678UartInit(3);
		Wk5678UartInit(4);
//		//设置子串口波特率
		Wk5678UartSetBaud(3,packFlash.uartBps[4]);
		Wk5678UartSetBaud(1,packFlash.uartBps[5]);
		Wk5678UartSetBaud(2,packFlash.uartBps[6]);
		Wk5678UartSetBaud(4,packFlash.uartBps[7]);
		Wk5678UartRS485(1);
		Wk5678UartRS485(2);
		Wk5678UartRS485(3);
		Wk5678UartRS485(4);
	  rt_kprintf("%sUART re config\n",sign);

}
//串口波特率重新初始化
//输入串口0-8  波特率
void uartSingConf(int chanl,int bps)
{
	packFlash.uartBps[chanl]=bps;
	switch(chanl){
//		case USE_UART2:
////			MX_USART2_UART_Init(bps	);
////		  rt_kprintf("%sUART conf uart2 bps=%d\n",sign,packFlash.uartBps[USE_UART2]);
//			break;
		case 8:
			MX_USART6_UART_Init(bps);
			break;

		case 0:
			Wk1234UartInit(3);
			//设置子串口波特率
			Wk1234UartSetBaud(3,bps);
	   	Wk1234UartRS485(3);
			break;
		case 1:
			Wk1234UartInit(1);
			Wk1234UartSetBaud(1,bps);
		  Wk1234UartRS485(1);
			break;
		case 2:
			Wk1234UartInit(2);
			Wk1234UartSetBaud(2,bps);
			Wk1234UartRS485(2);
			break;
		case 3:
			Wk1234UartInit(4);
			Wk1234UartSetBaud(4,bps);
		  Wk1234UartRS485(4);
			break;
		case 4:
			Wk5678UartInit(3);
			//设置子串口波特率
			Wk5678UartSetBaud(3,bps);
		  Wk5678UartRS485(3);
			break;
		case 5:
			Wk5678UartInit(1);
			Wk5678UartSetBaud(1,bps);
		  Wk5678UartRS485(1);
			break;
		case 6:
			Wk5678UartInit(2);
			Wk5678UartSetBaud(2,bps);
		  Wk5678UartRS485(2);
			break;
		case 7:
			Wk5678UartInit(4);
			Wk5678UartSetBaud(4,bps);
		  Wk5678UartRS485(4);
			break;
	}
}


extern void RingBuff3_Init(void);
//485根据串口发送
void rs485UartSend(uint8_t chanl,uint8_t *buf,int len)
{
//UART2(1) UART3(2) UART6(3) UART1(debug) UART4(4)
	  if(len==0)
			return;
		rt_kprintf("%srs485 send %d\n",sign,chanl);
		switch(chanl){
			case 8:
				RingBuff3_Init();
				HAL_UART_Transmit(&huart6,(uint8_t *)buf,len,1000);
				break;
			case 0://OK
				Wk1234UartTxChars(3,len,buf);
				break;
			case 1:
				Wk1234UartTxChars(1,len,buf);
				break;
			case 2:
				Wk1234UartTxChars(2,len,buf);
				break;
			case 3:
				Wk1234UartTxChars(4,len,buf);//port 0
//			Wk1234UartTxChars(3,len,buf);
//			Wk1234UartTxChars(2,len,buf);
//			Wk1234UartTxChars(1,len,buf);
//			Wk5678UartTxChars(1,len,buf);
//			Wk5678UartTxChars(2,len,buf);
//			Wk5678UartTxChars(3,len,buf);
//			Wk5678UartTxChars(4,len,buf);
				break;
			case 4:
				Wk5678UartTxChars(3,len,buf);
				break;
			case 5:
				Wk5678UartTxChars(1,len,buf);
				break;
			case 6:
				Wk5678UartTxChars(2,len,buf);
				break;
			case 7:
				Wk5678UartTxChars(4,len,buf);
				break;
			default:
				rt_kprintf("%sERR:UART chanl config\n",sign);
				break;
		}
}


//modbus+huanliu（设备名称）+1(通道)+1（设备地址）
uint16 atoi16(char* str,uint16 base); 
