
#include "board.h"
#include "wk2xxx.h"
//使用串口3 1234
/**************************************Wk2xxxRstInit***********************************/
//函数功能:wk芯片需要用MCU的GPIO去控制RST引脚，本函数通过stm32的PB.8引脚连接WK的RST引脚
//初始化STM32的PB8引脚。
//      
//*************************************************************************/
void Wk1xxxRstInit(void)
{
	/*STM32F105 -GPIO(GPIOB.8)*/
	HAL_GPIO_WritePin(RESET1234_GPIO_Port, RESET1234_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(RESET1234_GPIO_Port, RESET1234_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

HAL_StatusTypeDef Uart3Transmit( uint8_t *pData, uint16_t size)
{
		return	HAL_UART_Transmit(&huart3,pData,size,100);
	    //rt_krt_kprintf("uart3 send\n");
}


HAL_StatusTypeDef Uart3Receive(uint8_t *pData, uint16_t size)
{
		return	HAL_UART_Receive(&huart3,pData,size,100); 	
	    //rt_krt_kprintf("uart3 rec\n");
}

void wk1234clearRec()
{
	uint8_t rec;
	while(HAL_OK==HAL_UART_Receive(&huart3,&rec,1,1000)){
		rt_kprintf("clear uart3 buf\n");
	}
}

/**********************WkWriteGlobalRegister*************
*@param greg:Global register 6-bit address
*@param dat:Writes the value of the global register
*@retval 
*/		
void Wk1234WriteGlobalRegister(uint8_t greg,uint8_t dat)
{	 
	 uint8_t	cmd,txbuf[2];
	 cmd=0|greg;
	 txbuf[0]=cmd;
	 txbuf[1]=dat;
	 Uart3Transmit(txbuf,2);
}
/********************** WkReadGlobalRegister()***********
**
*@param  greg:Global register 6-bit address
*@retval :Read the value of the global register
*/
uint8_t Wk1234ReadGlobalRegister(uint8_t greg)
{	 uint8_t cmd,rec;
	 cmd=0x40|greg;
	 Uart3Transmit(&cmd,1);
	 Uart3Receive(&rec,1);		 								
	 return rec;
}

/************************WkWriteSlaveRegister()*******************
**
* @param port:Uart port
* @param sreg:slave rigister 4-bit address 
* @param dat:Writes the value of the slave register
* @retval
**
*/
void Wk1234WriteSlaveRegister(uint8_t port,uint8_t sreg,uint8_t dat)
{	 uint8_t cmd,txbuf[2];
	 cmd=0x0|((port-1)<<4)|sreg;
	 txbuf[0]=cmd;
	 txbuf[1]=dat;
   Uart3Transmit(txbuf,2);
}

/********************** WkReadSlaveRegister()***********
**
*@param  sreg:slave register 4-bit address
*@retval :Read the value of the slave register
**
*/
uint8_t Wk1234ReadSlaveRegister(uint8_t port,uint8_t sreg)
{	 uint8_t cmd,rec;
	 cmd=0x40|((port-1)<<4)|sreg;
	 Uart3Transmit(&cmd,1);
	 Uart3Receive(&rec,1);		 								
	 return rec;
	

}
/************************WkWriteSlaveFifo()*******************
**
* @param port:Uart port
* @param dat:pointer to transmission data buffer
* @param len:amount of data to be sent
* @retval
**
*/
uint8_t Wk1234WriteSlaveFifo(uint8_t port,uint8_t *dat,uint16_t num)
{	 
	 
	 uint8_t cmd,*pdata=dat;
	 uint16_t len=num;
   while(len){
		 
			 if(len>=16){
					cmd=0x80|((port-1)<<4)|15; 
					Uart3Transmit(&cmd,1);
					Uart3Transmit(pdata,16);
					len=len-16;
			 }else if(len>0){
					cmd=0x80|((port-1)<<4)|(len-1);
					Uart3Transmit(&cmd,1);
					Uart3Transmit(pdata,len);
					pdata=pdata+len;
					len=0;		 
			 }
   }
	 return 0;
}

/************************WkReadSlaveFifo()*******************
**
* @param port:Uart port
* @param rec:pointer to reception data buffer
* @param len:amount of data to be received
* @retval
**
*/
uint8_t Wk1234ReadSlaveFifo(uint8_t port,uint8_t *rec,uint16_t num)
{
		uint8_t cmd;
	  uint16_t len=num;
		while(len){
		
			 if(len>=16){
				  cmd=0xc0|((port-1)<<4)|0x0f;
					Uart3Transmit(&cmd,1);
					Uart3Receive(rec,16);
					rec=rec+16;
					len=len-16;
			 }else if(len>0){
					cmd=0xc0|((port-1)<<4)|(len-1);
					Uart3Transmit(&cmd,1);
					Uart3Receive(rec,len);
					rec=rec+len;
					len=0;		 
			 }
			 
		 }
		 return 0;		 
}




/**********************Wk_BaudAdaptive*******************************************/
//波特率匹配
/*********************************************************************************/
uint8_t Wk1234MasterUartBaudAdaptive(void)
{    
	uint8_t cmd=0x55,ret=0;
  wk1234clearRec();
	Wk1xxxRstInit();
	
	Uart3Transmit(&cmd,1);
	HAL_Delay(10);
	wk1234clearRec();
	ret=Wk1234ReadGlobalRegister(WK2XXX_GENA_REG);
	rt_kprintf("WkMasterUartBaudAdaptive---gena:%x\n",ret);
	if((ret&0xf0)==0xf0){
		return 0;
	}else{
		return ret;
	}	
}

/*******WkInit*******************************************/
//函数功能：初始化子串口
/*******************************************************/
void Wk1234UartInit(uint8_t port)
{
    uint8_t gena,grst,gier,sier,scr;
		//使能子串口时钟
    gena=Wk1234ReadGlobalRegister(WK2XXX_GENA_REG);
		gena=gena|(1<<(port-1));
		Wk1234WriteGlobalRegister(WK2XXX_GENA_REG,gena);
		//软件复位子串口
		grst=Wk1234ReadGlobalRegister(WK2XXX_GRST_REG);
		grst=grst|(1<<(port-1));
		Wk1234WriteGlobalRegister(WK2XXX_GRST_REG,grst);
		//使能串口总中断
		gier=Wk1234ReadGlobalRegister(WK2XXX_GIER_REG);
		gier=gier|(1<<(port-1));
		Wk1234WriteGlobalRegister(WK2XXX_GIER_REG,gier);
	  //使能子串口接收触点中断和超时中断
	  sier=Wk1234ReadSlaveRegister(port,WK2XXX_SIER_REG); 
	  sier |= SIER_RFTRIG_IEN_BIT|SIER_RXOUT_IEN_BIT;
	  Wk1234WriteSlaveRegister(port,WK2XXX_SIER_REG,sier);
	  //初始化FIFO和设置固定中断触点
	  Wk1234WriteSlaveRegister(port,WK2XXX_FCR_REG,0XFF);  
	  //设置任意中断触点，如果下面的设置有效，
	  //那么上面FCR寄存器中断的固定中断触点将失效
	  Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//切换到page1
	  Wk1234WriteSlaveRegister(port,WK2XXX_RFTL_REG,0X40);//设置接收触点为64个字节
	  Wk1234WriteSlaveRegister(port,WK2XXX_TFTL_REG,0X10);//设置发送触点为16个字节
	  Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//切换到page0 
	  //使能子串口的发送和接收使能
	  scr=Wk1234ReadSlaveRegister(port,WK2XXX_SCR_REG); 
	  scr|=SCR_TXEN_BIT|SCR_RXEN_BIT;
	  Wk1234WriteSlaveRegister(port,WK2XXX_SCR_REG,scr);
}

/******************************Wk_DeInit*******************************************/
//函数功能：初始化子串口
/*********************************************************************************/
void Wk1234UartDeInit(uint8_t port)
{
	  uint8_t gena,grst,gier;
		//关闭子串口总时钟
    gena=Wk1234ReadGlobalRegister(WK2XXX_GENA_REG);
		gena=gena&(~(1<<(port-1)));
		Wk1234WriteGlobalRegister(WK2XXX_GENA_REG,gena);
		//使能子串口总中断
		gier=Wk1234ReadGlobalRegister(WK2XXX_GIER_REG);
		gier=gier&(~(1<<(port-1)));
		Wk1234WriteGlobalRegister(WK2XXX_GIER_REG,gier);
		//软件复位子串口
		grst=Wk1234ReadGlobalRegister(WK2XXX_GRST_REG);
		grst=grst|(1<<(port-1));
		Wk1234WriteGlobalRegister(WK2XXX_GRST_REG,grst);
	
}

/**************************WkUartSetBaud*******************************************************/
//函数功能：设置子串口波特率函数、此函数中波特率的匹配值是根据11.0592Mhz下的外部晶振计算的
// port:子串口号
// baud:波特率大小.波特率表示方式，
/**************************Wk2114SetBaud*******************************************************/
uint8_t Wk1234UartSetBaud(uint8_t port,uint32_t baudrate)
{  
		uint32_t temp,freq;
		uint8_t scr;
		uint8_t baud1,baud0,pres;
		freq=11059200;/*芯片外部时钟频率*/
		//freq=32000000;/*芯片外部时钟频率*/
	  if(freq>=(baudrate*16))
		{
			temp=(freq)/(baudrate*16);
			temp=temp-1;
			baud1=(uint8_t)((temp>>8)&0xff);
			baud0=(uint8_t)(temp&0xff);
			temp=(((freq%(baudrate*16))*100)/(baudrate));
			pres=(temp+100/2)/100;
			rt_kprintf("Wk_SetBaud---freq:%d,baudrate:%d\n",freq,baudrate);
			rt_kprintf("Wk_SetBaud---baud1:%x,baud0:%x,pres:%x\n",baud1,baud0,pres);
			//关掉子串口收发使能
			scr=Wk1234ReadSlaveRegister(port,WK2XXX_SCR_REG); 
			Wk1234WriteSlaveRegister(port,WK2XXX_SCR_REG,0);
			//设置波特率相关寄存器
			Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//切换到page1
			Wk1234WriteSlaveRegister(port,WK2XXX_BAUD1_REG,baud1);
			Wk1234WriteSlaveRegister(port,WK2XXX_BAUD0_REG,baud0);
			Wk1234WriteSlaveRegister(port,WK2XXX_PRES_REG,pres);
			Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//切换到page0 
			//使能子串口收发使能
			Wk1234WriteSlaveRegister(port,WK2XXX_SCR_REG,scr);
			return 0;
		}else{
			rt_kprintf("Wk_SetBaud error！！！！\n");
			return 1;
		}
}

/**************************设置数据格式*******************************************************/
//函数功能：设置子串口通信数据格式。设置停止位和校验方式
//参数：
/**********************************************************************************************/
// stopformat:支持一位停止位和两位停止位，参数如下：
//WK2XXX_UART_STOPBITS_1 ,WK2XXX_UART_STOPBITS_2
/**********************************************************************************************/
//paritytype:校验方式：支持多种校验方式：NONE(无校验),ODD,EVEN,MARK,SPACE，具体函数参数如下：
//WK2XXX_UART_PARITY_NONE,  无校验
//WK2XXX_UART_PARITY_ODD,   奇校验
//WK2XXX_UART_PARITY_EVEN,  偶校验
//WK2XXX_UART_PARITY_MARK ，MARK校验,1校验
//WK2XXX_UART_PARITY_SPACE，SPACE校验，0校验
/***********************************************************************************************/
/**************************Wk2xxxFormatSet()*******************************************************/
uint8_t Wk1234UartFormatSet(uint8_t port,uint8_t stopformat,uint8_t paritytype)
{  
	  uint8_t scr=0,sier=0,lcr=0;
		scr=Wk1234ReadSlaveRegister(port,WK2XXX_SCR_REG); 
		Wk1234WriteSlaveRegister(port,WK2XXX_SCR_REG,0);
		sier=Wk1234ReadSlaveRegister(port,WK2XXX_SIER_REG); 
		Wk1234WriteSlaveRegister(port,WK2XXX_SIER_REG,0);

		if(stopformat==WK2XXX_UART_STOPBITS_1){
			lcr=lcr|0;
		}
		else if(stopformat==WK2XXX_UART_STOPBITS_2){
			lcr=lcr|LCR_STPL_BIT;
		}
		else {
			return 1;
		}

		/**/
		if(paritytype==WK2XXX_UART_PARITY_NONE){
			lcr=lcr|0;
		}
		else if(paritytype==WK2XXX_UART_PARITY_ODD){
			lcr=lcr|(LCR_PAM0_BIT | LCR_PAEN_BIT);
		}
		else if(paritytype==WK2XXX_UART_PARITY_EVEN){
			lcr=lcr|(LCR_PAM1_BIT | LCR_PAEN_BIT);
		}
		else if(paritytype==WK2XXX_UART_PARITY_MARK){
			lcr=lcr|(LCR_PAM0_BIT | LCR_PAM1_BIT|LCR_PAEN_BIT);
		}
		else if(paritytype==WK2XXX_UART_PARITY_SPACE){
			lcr=lcr|LCR_PAEN_BIT;
		}
		else {
			return 1;
		}

    Wk1234WriteSlaveRegister(port,WK2XXX_LCR_REG,lcr);
		//使能子串口收发使能
		Wk1234WriteSlaveRegister(port,WK2XXX_SCR_REG,scr);
		Wk1234WriteSlaveRegister(port,WK2XXX_SIER_REG,sier);
				
		rt_kprintf("Wk_FormatSet--LCR:%x,SCR:%x,SIER:%x\n",lcr,scr,sier);
		return 0;
		
}
/**************************WK_TxLen*******************************************/
//函数功能:获取子串口发送FIFO剩余空间长度
// port:端口号
// 返回值：发送FIFO剩余空间长度
/**************************WK_Len********************************************/
uint16_t Wk1234UartTxLen(uint8_t port)
{
	uint8_t fsr,tfcnt;
	uint16_t len=0;
	fsr  =Wk1234ReadSlaveRegister(port,WK2XXX_FSR_REG);
	tfcnt=Wk1234ReadSlaveRegister(port,WK2XXX_TFCNT_REG);
	if(fsr& FSR_TFULL_BIT){ 
	  len=0;
	}else{
		len=256-tfcnt;
	}
  return len;
}

/**************************WK_TxChars*******************************************/
//函数功能:通过子串口发送固定长度数据
// port:端口号
// len:单次发送长度不超过256
// 
/**************************WK_TxChars********************************************/
uint16_t Wk1234UartTxChars(uint8_t port,int len,uint8_t *sendbuf)
{
	
#if 1
	Wk1234WriteSlaveFifo(port,sendbuf,len);//通过fifo方式发送数据
	return 0;
#else
	uint16_t num=len;
	for(num=0;num<len;num++)
	{
		Wk1234WriteSlaveRegister(port,WK2XXX_FDAT_REG,*(sendbuf+num));
	}
	return 0;
#endif	
}

/**************************WK_RxChars*******************************************/
//函数功能:读取子串口fifo中的数据
// port:端口号
// recbuf:接收到的数据
// 返回值：接收数据的长度
/**************************WK_RxChars********************************************/
uint16_t Wk1234UartRxChars(uint8_t port,uint8_t *recbuf)
{
	uint8_t  fsr=0,rfcnt=0,rfcnt2=0,sifr=0;
  uint16_t len=0;
	sifr=Wk1234ReadSlaveRegister(port,WK2XXX_SIFR_REG);
	

	if((sifr&SIFR_RFTRIG_INT_BIT)||(sifr&SIFR_RXOVT_INT_BIT))//有接收中断和接收超时中断
	{ 
			fsr  =Wk1234ReadSlaveRegister(port,WK2XXX_FSR_REG);
		  if (fsr& FSR_RDAT_BIT){
				rfcnt=Wk1234ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				if(rfcnt==0){
					rfcnt=Wk1234ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				}
				rfcnt2=Wk1234ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				if(rfcnt2==0){
					rfcnt2=Wk1234ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				}
			/*判断fifo中数据个数*/
				rfcnt=(rfcnt2>=rfcnt)?rfcnt:rfcnt2;
				len=(rfcnt==0)?256:rfcnt;	
				#if 1
					Wk1234ReadSlaveFifo(port,recbuf,len);
				#else
					for(n=0;n<len;n++)
					 *(recbuf+n)=Wk1234ReadSlaveRegister(port,WK2XXX_FDAT_REG);
				#endif	
				return len;
			}else{
				return len;
			}
		}
	return len;
}

/**************************WK_RS485*******************************************************/
//函数功能:设置子串口RS485的收发转换函数，使用RTS引脚控制485电平转换芯片的收发
// port:子串口号
// 
//注意：只有WK2168/WK2204支持该功能
/**************************WK_RS485*******************************************************/
void Wk1234UartRS485(uint8_t port)
{   
	  //Wk1234WriteSlaveRegister(port,WK2XXX_RS485_REG,0x02);//默认高电平
	  Wk1234WriteSlaveRegister(port,WK2XXX_RS485_REG,0x03);//默认低电平
		Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//切换到page1
		Wk1234WriteSlaveRegister(port,WK2XXX_RTSDLY_REG,0X10);
		Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//切换到page0 
}
/**************************WK_RTSCTS*******************************************************/
//函数功能:硬件自动流量控制，需要子设备的支持
// port:子串口号
// 
//注意：只有WK2168/WK2204支持该功能
/**************************WK_RTSCTS*******************************************************/
void Wk1234UartRTSCTS(uint8_t port)
{   
	  Wk1234WriteSlaveRegister(port,WK2XXX_FWCR_REG,0x30);//
		Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//切换到page1
		Wk1234WriteSlaveRegister(port,WK2XXX_FWTH_REG,0XF0);//停止接收触点
		Wk1234WriteSlaveRegister(port,WK2XXX_FWTL_REG,0X20);//继续接收触点
		Wk1234WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//切换到page0 
}
#if 0
/************************************************************************************************/
//函数功能:数据处理。本函数主要实现把接收的数据，然后再发送出来。
/**************************uint8_t WK2xxxPortProcess(void)********************************************/
uint8_t Wk1234UartProcess(void)
{ 
	uint16_t rxlen=0;
	uint8_t gifr,rxbuf[260];
	gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
	do{
			//判断子串口1是否有中断
			if(gifr&GIFR_UT1INT_BIT){ /*数据处理*/
					/*数据接收*/
					 rxlen=Wk1234UartRxChars(1,rxbuf);//一次接收的数据不会超过256Byte
					/*数据发送*/
					//把接收的数据发送出去
					 if(rxlen>0){
							Wk1234UartTxChars(1,rxlen,rxbuf);
					 }
				 
			 }
			
			//判断子串口2是否有中断
			if(gifr&GIFR_UT2INT_BIT){
				/*数据接收*/
						rxlen=Wk1234UartRxChars(2,rxbuf);//一次接收的数据不会超过256Byte
						/*数据发送*/
						//把接收的数据发送出去
						if(rxlen>0){
						  Wk1234UartTxChars(2,rxlen,rxbuf);
						}
			
			 }
			//判断子串口3是否有中断
			if(gifr&GIFR_UT3INT_BIT){
				    /*数据接收*/
						rxlen=Wk1234UartRxChars(3,rxbuf);//一次接收的数据不会超过256Byte
						/*数据发送*/
						//把接收的数据发送出去
				    if(rxlen>0){
						  Wk1234UartTxChars(3,rxlen,rxbuf);
						}
			}
			//判断子串口4是否有中断
			if(gifr&GIFR_UT4INT_BIT){
				    /*数据接收*/
						rxlen=Wk1234UartRxChars(4,rxbuf);//一次接收的数据不会超过256Byte
						/*数据发送*/
						//把接收的数据发送出去
				    if(rxlen>0){
						  Wk1234UartTxChars(4,rxlen,rxbuf);
						}
			}
			
		  gifr=Wk1234ReadGlobalRegister(WK2XXX_GIFR_REG);
	}while(gifr&0x0f);
	
	return gifr;
}
#endif
