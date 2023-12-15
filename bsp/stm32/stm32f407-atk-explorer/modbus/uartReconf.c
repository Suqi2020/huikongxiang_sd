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
//note：此程序有个bug 如果是子设备主动上传数据  不知道对应通道 需要重新封装函数
extern uint8_t Read_RingBuff3(uint8_t *rData);



//串口滤波函数 用在发送函数之前 如果有冗余数据  需要丢掉 局放重启会发出冗余数据
static void  filterRs485ErrData()
{
	  uint8_t *buf=rt_malloc(200);
	  int count=0;
	  uint8_t gifr=0;
	  rt_sem_trytake(uart1234_sem);

		gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
		do{
				//判断子串口1是否有中断
				if(gifr&GIFR_UT1INT_BIT){ /*数据处理*///pcb映射串口1+1
						/*数据接收*/
						 count=Wk1234UartRxChars(1,buf);//一次接收的数据不会超过256Byte
						 rt_kprintf("485 1 filter\n");

				}	
				//判断子串口2是否有中断
				if(gifr&GIFR_UT2INT_BIT){//pcb映射串口2+1
					/*数据接收*/
						 count=Wk1234UartRxChars(2,buf);//一次接收的数据不会超过256Byte
						 rt_kprintf("485 2 filter\n");

				}
				//判断子串口3是否有中断
				if(gifr&GIFR_UT3INT_BIT){//pcb映射串口0+1
							/*数据接收*/
						count=Wk1234UartRxChars(3,buf);//一次接收的数据不会超过256Byte
						rt_kprintf("485 3 filter\n");
				}
				//判断子串口4是否有中断
				if(gifr&GIFR_UT4INT_BIT){//pcb映射串口3+1
							/*数据接收*/
						 count=Wk1234UartRxChars(4,buf);//一次接收的数据不会超过256Byte
						 rt_kprintf("485 4 filter\n");
				}
				
				gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
		}while(gifr&0x0f);
				
    rt_sem_trytake(uart5678_sem);
		gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
		do{
				//判断子串口1是否有中断
				if(gifr&GIFR_UT1INT_BIT){ /*数据处理*///pcb映射串口7
						/*数据接收*/
						 count=Wk5678UartRxChars(1,buf);//一次接收的数据不会超过256Byte
							rt_kprintf("485 5 filter\n");
				}	
				//判断子串口2是否有中断
				if(gifr&GIFR_UT2INT_BIT){//pcb映射串口5
					/*数据接收*/
						 count=Wk5678UartRxChars(2,buf);//一次接收的数据不会超过256Byte
							rt_kprintf("485 6 filter\n");
				}
				//判断子串口3是否有中断
				if(gifr&GIFR_UT3INT_BIT){//pcb映射串口6
							/*数据接收*/
						count=Wk5678UartRxChars(3,buf);//一次接收的数据不会超过256Byte
						rt_kprintf("485 7 filter\n");
				}
				//判断子串口4是否有中断
				if(gifr&GIFR_UT4INT_BIT){//pcb映射串口8
							/*数据接收*/
						 count=Wk5678UartRxChars(4,buf);//一次接收的数据不会超过256Byte
						rt_kprintf("485 8 filter\n");
				}
				gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
		}while(gifr&0x0f);

		while(true==Read_RingBuff3(buf+count)){
			count++;
			rt_thread_mdelay(2);
		}
		rt_free(buf);
		buf=NULL;
}


int  rs485UartRec(int chanl,uint8_t *recBuf,int timeout)
{
	  int count=0;
	  uint8_t gifr=0;
    int irq=0;
		if((chanl==0)||(chanl==1)||(chanl==2)||(chanl==3)){
			if(rt_sem_take(uart1234_sem,timeout)==RT_EOK){
				gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
				do{
					rt_kprintf("485-1234 read chanl%d\n",chanl);
					irq=1;
						//判断子串口3是否有中断
					if(chanl==0){
						if(gifr&GIFR_UT3INT_BIT){//pcb映射串口0+1
									/*数据接收*/
								count=Wk1234UartRxChars(3,recBuf);//一次接收的数据不会超过256Byte
								rt_kprintf("485 0read\n");
								break;
						}
					}
						//判断子串口1是否有中断
					if(chanl==1){
						if(gifr&GIFR_UT1INT_BIT){ /*数据处理*///pcb映射串口1+1
								/*数据接收*/
								 count=Wk1234UartRxChars(1,recBuf);//一次接收的数据不会超过256Byte
								 rt_kprintf("485 1read\n");
								break;
						}	
					}
						//判断子串口2是否有中断
					if(chanl==2){
						if(gifr&GIFR_UT2INT_BIT){//pcb映射串口2+1
							/*数据接收*/
								 count=Wk1234UartRxChars(2,recBuf);//一次接收的数据不会超过256Byte
								 rt_kprintf("485 2read\n");
								break;
						}
					}

						//判断子串口4是否有中断
					if(chanl==3){
						if(gifr&GIFR_UT4INT_BIT){//pcb映射串口3+1
									/*数据接收*/
								 count=Wk1234UartRxChars(4,recBuf);//一次接收的数据不会超过256Byte
								 rt_kprintf("485 3read\n");
								break;
						}
					}
					gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
				}while(gifr&0x0f);
			}

		}
		if((chanl==4)||(chanl==5)||(chanl==6)||(chanl==7)){
			if(rt_sem_take(uart5678_sem,timeout)==RT_EOK){
				gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					do{
						rt_kprintf("485-5678 read chanl%d\n",chanl);
						irq=1;
							//判断子串口3是否有中断
						if(chanl==4){
							if(gifr&GIFR_UT3INT_BIT){//pcb映射串口6
										/*数据接收*/
									count=Wk5678UartRxChars(3,recBuf);//一次接收的数据不会超过256Byte
									rt_kprintf("485 4read\n");
									break;
							}
						}
							//判断子串口1是否有中断
						if(chanl==5){
							if(gifr&GIFR_UT1INT_BIT){ /*数据处理*///pcb映射串口7
									/*数据接收*/
									count=Wk5678UartRxChars(1,recBuf);//一次接收的数据不会超过256Byte
									rt_kprintf("485 5read\n");
									break;
							}	
						}
							//判断子串口2是否有中断
						if(chanl==6){
							if(gifr&GIFR_UT2INT_BIT){//pcb映射串口5
								/*数据接收*/
									count=Wk5678UartRxChars(2,recBuf);//一次接收的数据不会超过256Byte
									rt_kprintf("485 6read\n");
									break;
							}
						}

							//判断子串口4是否有中断
						if(chanl==7){
							if(gifr&GIFR_UT4INT_BIT){//pcb映射串口8
										/*数据接收*/
									count=Wk5678UartRxChars(4,recBuf);//一次接收的数据不会超过256Byte
									rt_kprintf("485 7read\n");
									break;
							}
						}
							
						gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					}while(gifr&0x0f);
				}

			}
		  if((count<=2)&&(irq==1)){
				rt_kprintf("ERR:uart irq come nodata rst uart%d\n",count);
				extern void uartReconfig();
				extern void uartIrqEnaAfterQueue();
				uartReconfig();//串口重新配置
				uartIrqEnaAfterQueue();//串口中断中用到了队列  开启中断需要放到后边
				rt_thread_mdelay(1000);
			}
			if(chanl==8){
				rt_thread_mdelay(timeout);
				while(true==Read_RingBuff3(recBuf+count)){
					count++;
					rt_thread_mdelay(2);
				}
	
			}
		  return count;
}




//串口重新配置 选择对应的modbus传感器
void uartReconfig()
{
		for(int i=0;i<UART_NUM;i++){
				if((packFlash.uartBps[i]==0)||(packFlash.uartBps[i]==-1)){//没用到的串口 给个默认波特率 
					  packFlash.uartBps[i]=9600;
				}
		}
		for(int i=0;i<UART_NUM;i++){
				rt_kprintf("%sport%d bps[%d]\n",sign,i+1,packFlash.uartBps[i]);
		}
   extern void RingBuff3_Init(void);

		RingBuff3_Init();
		MX_USART6_UART_Init(packFlash.uartBps[8]);
		__HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
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



//485根据串口发送
void rs485UartSend(uint8_t chanl,uint8_t *buf,int len)
{
    filterRs485ErrData();//读取数据前先进行滤波
	  rt_thread_mdelay(2);
	  if(len==0)
			return;
		rt_kprintf("%srs485 send %d\n",sign,chanl);
		switch(chanl){
			case 8:
				//RingBuff3_Init();
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
