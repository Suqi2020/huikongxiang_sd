
#include "board.h"
#include "wk2xxx.h"
//ʹ�ô���3 5678
/**************************************Wk2xxxRstInit***********************************/
//��������:wkоƬ��Ҫ��MCU��GPIOȥ����RST���ţ�������ͨ��stm32��PB.8��������WK��RST����
//��ʼ��STM32��PB8���š�
//      
//*************************************************************************/
void Wk2xxxRstInit(void)
{
	/*STM32F105 -GPIO(GPIOB.8)*/
	HAL_GPIO_WritePin(RESET5678_GPIO_Port, RESET5678_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(RESET5678_GPIO_Port, RESET5678_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

HAL_StatusTypeDef Uart4Transmit( uint8_t *pData, uint16_t size)
{
	return HAL_UART_Transmit(&huart4,pData,size,100);
}


HAL_StatusTypeDef Uart4Receive(uint8_t *pData, uint16_t size)
{
	return HAL_UART_Receive(&huart4,pData,size,100); 	
}



/**********************WkWriteGlobalRegister*************
*@param greg:Global register 6-bit address
*@param dat:Writes the value of the global register
*@retval 
*/		
void Wk5678WriteGlobalRegister(uint8_t greg,uint8_t dat)
{	 
	 uint8_t	cmd,txbuf[2];
	 cmd=0|greg;
	 txbuf[0]=cmd;
	 txbuf[1]=dat;
	 Uart4Transmit(txbuf,2);
}
/********************** WkReadGlobalRegister()***********
**
*@param  greg:Global register 6-bit address
*@retval :Read the value of the global register
*/
uint8_t Wk5678ReadGlobalRegister(uint8_t greg)
{	 uint8_t cmd,rec;
	 cmd=0x40|greg;
	 Uart4Transmit(&cmd,1);
	 Uart4Receive(&rec,1);		 								
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
void Wk5678WriteSlaveRegister(uint8_t port,uint8_t sreg,uint8_t dat)
{	 uint8_t cmd,txbuf[2];
	 cmd=0x0|((port-1)<<4)|sreg;
	 txbuf[0]=cmd;
	 txbuf[1]=dat;
   Uart4Transmit(txbuf,2);
}

/********************** WkReadSlaveRegister()***********
**
*@param  sreg:slave register 4-bit address
*@retval :Read the value of the slave register
**
*/
uint8_t Wk5678ReadSlaveRegister(uint8_t port,uint8_t sreg)
{	 uint8_t cmd,rec;
	 cmd=0x40|((port-1)<<4)|sreg;
	 Uart4Transmit(&cmd,1);
	 Uart4Receive(&rec,1);		 								
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
uint8_t Wk5678WriteSlaveFifo(uint8_t port,uint8_t *dat,uint16_t num)
{	 
	 
	 uint8_t cmd,*pdata=dat;
	 uint16_t len=num;
   while(len){
		 
			 if(len>=16){
					cmd=0x80|((port-1)<<4)|15; 
					Uart4Transmit(&cmd,1);
					Uart4Transmit(pdata,16);
					len=len-16;
			 }else if(len>0){
					cmd=0x80|((port-1)<<4)|(len-1);
					Uart4Transmit(&cmd,1);
					Uart4Transmit(pdata,len);
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
uint8_t Wk5678ReadSlaveFifo(uint8_t port,uint8_t *rec,uint16_t num)
{
		uint8_t cmd;
	  uint16_t len=num;
		while(len){
		
			 if(len>=16){
				  cmd=0xc0|((port-1)<<4)|0x0f;
					Uart4Transmit(&cmd,1);
					Uart4Receive(rec,16);
					rec=rec+16;
					len=len-16;
			 }else if(len>0){
					cmd=0xc0|((port-1)<<4)|(len-1);
					Uart4Transmit(&cmd,1);
					Uart4Receive(rec,len);
					rec=rec+len;
					len=0;		 
			 }
			 
		 }
		 return 0;		 
}

/**********************Wk_BaudAdaptive*******************************************/
//������ƥ��
/*********************************************************************************/
uint8_t Wk5678MasterUartBaudAdaptive(void)
{    
	uint8_t cmd=0x55,ret=0;

	Wk2xxxRstInit();
	Uart4Transmit(&cmd,1);
	HAL_Delay(10);
	ret=Wk5678ReadGlobalRegister(WK2XXX_GENA_REG);
	printf("WkMasterUartBaudAdaptive---gena:%x\n",ret);
	if((ret&0xf0)==0xf0){
		return 0;
	}else{
		return ret;
	}	
}

/*******WkInit*******************************************/
//�������ܣ���ʼ���Ӵ���
/*******************************************************/
void Wk5678UartInit(uint8_t port)
{
    uint8_t gena,grst,gier,sier,scr;
		//ʹ���Ӵ���ʱ��
    gena=Wk5678ReadGlobalRegister(WK2XXX_GENA_REG);
		gena=gena|(1<<(port-1));
		Wk5678WriteGlobalRegister(WK2XXX_GENA_REG,gena);
		//�����λ�Ӵ���
		grst=Wk5678ReadGlobalRegister(WK2XXX_GRST_REG);
		grst=grst|(1<<(port-1));
		Wk5678WriteGlobalRegister(WK2XXX_GRST_REG,grst);
		//ʹ�ܴ������ж�
		gier=Wk5678ReadGlobalRegister(WK2XXX_GIER_REG);
		gier=gier|(1<<(port-1));
		Wk5678WriteGlobalRegister(WK2XXX_GIER_REG,gier);
	  //ʹ���Ӵ��ڽ��մ����жϺͳ�ʱ�ж�
	  sier=Wk5678ReadSlaveRegister(port,WK2XXX_SIER_REG); 
	  sier |= SIER_RFTRIG_IEN_BIT|SIER_RXOUT_IEN_BIT;
	  Wk5678WriteSlaveRegister(port,WK2XXX_SIER_REG,sier);
	  //��ʼ��FIFO�����ù̶��жϴ���
	  Wk5678WriteSlaveRegister(port,WK2XXX_FCR_REG,0XFF);  
	  //���������жϴ��㣬��������������Ч��
	  //��ô����FCR�Ĵ����жϵĹ̶��жϴ��㽫ʧЧ
	  Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//�л���page1
	  Wk5678WriteSlaveRegister(port,WK2XXX_RFTL_REG,0X40);//���ý��մ���Ϊ64���ֽ�
	  Wk5678WriteSlaveRegister(port,WK2XXX_TFTL_REG,0X10);//���÷��ʹ���Ϊ16���ֽ�
	  Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//�л���page0 
	  //ʹ���Ӵ��ڵķ��ͺͽ���ʹ��
	  scr=Wk5678ReadSlaveRegister(port,WK2XXX_SCR_REG); 
	  scr|=SCR_TXEN_BIT|SCR_RXEN_BIT;
	  Wk5678WriteSlaveRegister(port,WK2XXX_SCR_REG,scr);
}

/******************************Wk_DeInit*******************************************/
//�������ܣ���ʼ���Ӵ���
/*********************************************************************************/
void Wk5678UartDeInit(uint8_t port)
{
	  uint8_t gena,grst,gier;
		//�ر��Ӵ�����ʱ��
    gena=Wk5678ReadGlobalRegister(WK2XXX_GENA_REG);
		gena=gena&(~(1<<(port-1)));
		Wk5678WriteGlobalRegister(WK2XXX_GENA_REG,gena);
		//ʹ���Ӵ������ж�
		gier=Wk5678ReadGlobalRegister(WK2XXX_GIER_REG);
		gier=gier&(~(1<<(port-1)));
		Wk5678WriteGlobalRegister(WK2XXX_GIER_REG,gier);
		//�����λ�Ӵ���
		grst=Wk5678ReadGlobalRegister(WK2XXX_GRST_REG);
		grst=grst|(1<<(port-1));
		Wk5678WriteGlobalRegister(WK2XXX_GRST_REG,grst);
	
}

/**************************WkUartSetBaud*******************************************************/
//�������ܣ������Ӵ��ڲ����ʺ������˺����в����ʵ�ƥ��ֵ�Ǹ���11.0592Mhz�µ��ⲿ��������
// port:�Ӵ��ں�
// baud:�����ʴ�С.�����ʱ�ʾ��ʽ��
/**************************Wk2114SetBaud*******************************************************/
uint8_t Wk5678UartSetBaud(uint8_t port,uint32_t baudrate)
{  
		uint32_t temp,freq;
		uint8_t scr;
		uint8_t baud1,baud0,pres;
   	freq=11059200;/*оƬ�ⲿʱ��Ƶ��*/
		//freq=32000000;/*оƬ�ⲿʱ��Ƶ��*/
	  if(freq>=(baudrate*16))
		{
			temp=(freq)/(baudrate*16);
			temp=temp-1;
			baud1=(uint8_t)((temp>>8)&0xff);
			baud0=(uint8_t)(temp&0xff);
			temp=(((freq%(baudrate*16))*100)/(baudrate));
			pres=(temp+100/2)/100;
			printf("Wk_SetBaud---freq:%d,baudrate:%d\n",freq,baudrate);
			printf("Wk_SetBaud---baud1:%x,baud0:%x,pres:%x\n",baud1,baud0,pres);
			//�ص��Ӵ����շ�ʹ��
			scr=Wk5678ReadSlaveRegister(port,WK2XXX_SCR_REG); 
			Wk5678WriteSlaveRegister(port,WK2XXX_SCR_REG,0);
			//���ò�������ؼĴ���
			Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//�л���page1
			Wk5678WriteSlaveRegister(port,WK2XXX_BAUD1_REG,baud1);
			Wk5678WriteSlaveRegister(port,WK2XXX_BAUD0_REG,baud0);
			Wk5678WriteSlaveRegister(port,WK2XXX_PRES_REG,pres);
			Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//�л���page0 
			//ʹ���Ӵ����շ�ʹ��
			Wk5678WriteSlaveRegister(port,WK2XXX_SCR_REG,scr);
			return 0;
		}else{
			printf("Wk_SetBaud error��������\n");
			return 1;
		}
}

/**************************�������ݸ�ʽ*******************************************************/
//�������ܣ������Ӵ���ͨ�����ݸ�ʽ������ֹͣλ��У�鷽ʽ
//������
/**********************************************************************************************/
// stopformat:֧��һλֹͣλ����λֹͣλ���������£�
//WK2XXX_UART_STOPBITS_1 ,WK2XXX_UART_STOPBITS_2
/**********************************************************************************************/
//paritytype:У�鷽ʽ��֧�ֶ���У�鷽ʽ��NONE(��У��),ODD,EVEN,MARK,SPACE�����庯���������£�
//WK2XXX_UART_PARITY_NONE,  ��У��
//WK2XXX_UART_PARITY_ODD,   ��У��
//WK2XXX_UART_PARITY_EVEN,  żУ��
//WK2XXX_UART_PARITY_MARK ��MARKУ��,1У��
//WK2XXX_UART_PARITY_SPACE��SPACEУ�飬0У��
/***********************************************************************************************/
/**************************Wk2xxxFormatSet()*******************************************************/
uint8_t Wk5678UartFormatSet(uint8_t port,uint8_t stopformat,uint8_t paritytype)
{  
	  uint8_t scr=0,sier=0,lcr=0;
		scr=Wk5678ReadSlaveRegister(port,WK2XXX_SCR_REG); 
		Wk5678WriteSlaveRegister(port,WK2XXX_SCR_REG,0);
		sier=Wk5678ReadSlaveRegister(port,WK2XXX_SIER_REG); 
		Wk5678WriteSlaveRegister(port,WK2XXX_SIER_REG,0);

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

    Wk5678WriteSlaveRegister(port,WK2XXX_LCR_REG,lcr);
		//ʹ���Ӵ����շ�ʹ��
		Wk5678WriteSlaveRegister(port,WK2XXX_SCR_REG,scr);
		Wk5678WriteSlaveRegister(port,WK2XXX_SIER_REG,sier);
				
		printf("Wk_FormatSet--LCR:%x,SCR:%x,SIER:%x\n",lcr,scr,sier);
		return 0;
		
}
/**************************WK_TxLen*******************************************/
//��������:��ȡ�Ӵ��ڷ���FIFOʣ��ռ䳤��
// port:�˿ں�
// ����ֵ������FIFOʣ��ռ䳤��
/**************************WK_Len********************************************/
uint16_t Wk5678UartTxLen(uint8_t port)
{
	uint8_t fsr,tfcnt;
	uint16_t len=0;
	fsr  =Wk5678ReadSlaveRegister(port,WK2XXX_FSR_REG);
	tfcnt=Wk5678ReadSlaveRegister(port,WK2XXX_TFCNT_REG);
	if(fsr& FSR_TFULL_BIT){ 
	  len=0;
	}else{
		len=256-tfcnt;
	}
  return len;
}

/**************************WK_TxChars*******************************************/
//��������:ͨ���Ӵ��ڷ��͹̶���������
// port:�˿ں�
// len:���η��ͳ��Ȳ�����256
// 
/**************************WK_TxChars********************************************/
uint16_t Wk5678UartTxChars(uint8_t port,int len,uint8_t *sendbuf)
{
	
#if 1
	Wk5678WriteSlaveFifo(port,sendbuf,len);//ͨ��fifo��ʽ��������
	return 0;
#else
	uint16_t num=len;
	for(num=0;num<len;num++)
	{
		Wk5678WriteSlaveRegister(port,WK2XXX_FDAT_REG,*(sendbuf+num));
	}
	return 0;
#endif	
}

/**************************WK_RxChars*******************************************/
//��������:��ȡ�Ӵ���fifo�е�����
// port:�˿ں�
// recbuf:���յ�������
// ����ֵ���������ݵĳ���
/**************************WK_RxChars********************************************/
uint16_t Wk5678UartRxChars(uint8_t port,uint8_t *recbuf)
{
	uint8_t  fsr=0,rfcnt=0,rfcnt2=0,sifr=0;
  uint16_t len=0;
	sifr=Wk5678ReadSlaveRegister(port,WK2XXX_SIFR_REG);
	

	if((sifr&SIFR_RFTRIG_INT_BIT)||(sifr&SIFR_RXOVT_INT_BIT))//�н����жϺͽ��ճ�ʱ�ж�
	{ 
			fsr  =Wk5678ReadSlaveRegister(port,WK2XXX_FSR_REG);
		  if (fsr& FSR_RDAT_BIT){
				rfcnt=Wk5678ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				if(rfcnt==0){
					rfcnt=Wk5678ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				}
				rfcnt2=Wk5678ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				if(rfcnt2==0){
					rfcnt2=Wk5678ReadSlaveRegister(port,WK2XXX_RFCNT_REG);
				}
			/*�ж�fifo�����ݸ���*/
				rfcnt=(rfcnt2>=rfcnt)?rfcnt:rfcnt2;
				len=(rfcnt==0)?256:rfcnt;	
				#if 1
					Wk5678ReadSlaveFifo(port,recbuf,len);
				#else
					for(n=0;n<len;n++)
					 *(recbuf+n)=Wk5678ReadSlaveRegister(port,WK2XXX_FDAT_REG);
				#endif	
				return len;
			}else{
				return len;
			}
		}
	return len;
}

/**************************WK_RS485*******************************************************/
//��������:�����Ӵ���RS485���շ�ת��������ʹ��RTS���ſ���485��ƽת��оƬ���շ�
// port:�Ӵ��ں�
// 
//ע�⣺ֻ��WK2168/WK2204֧�ָù���
/**************************WK_RS485*******************************************************/
void Wk5678UartRS485(uint8_t port)
{   
	  //Wk5678WriteSlaveRegister(port,WK2XXX_RS485_REG,0x02);//Ĭ�ϸߵ�ƽ
	  Wk5678WriteSlaveRegister(port,WK2XXX_RS485_REG,0x03);//Ĭ�ϵ͵�ƽ
		Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//�л���page1
		Wk5678WriteSlaveRegister(port,WK2XXX_RTSDLY_REG,0X10);
		Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//�л���page0 
}
/**************************WK_RTSCTS*******************************************************/
//��������:Ӳ���Զ��������ƣ���Ҫ���豸��֧��
// port:�Ӵ��ں�
// 
//ע�⣺ֻ��WK2168/WK2204֧�ָù���
/**************************WK_RTSCTS*******************************************************/
void Wk5678UartRTSCTS(uint8_t port)
{   
	  Wk5678WriteSlaveRegister(port,WK2XXX_FWCR_REG,0x30);//
		Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,1);//�л���page1
		Wk5678WriteSlaveRegister(port,WK2XXX_FWTH_REG,0XF0);//ֹͣ���մ���
		Wk5678WriteSlaveRegister(port,WK2XXX_FWTL_REG,0X20);//�������մ���
		Wk5678WriteSlaveRegister(port,WK2XXX_SPAGE_REG,0);//�л���page0 
}

/************************************************************************************************/
//��������:���ݴ�����������Ҫʵ�ְѽ��յ����ݣ�Ȼ���ٷ��ͳ�����
/**************************uint8_t WK2xxxPortProcess(void)********************************************/
uint8_t Wk5678UartProcess(void)
{ 
	uint16_t rxlen=0;
	uint8_t gifr,rxbuf[260];
	gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
	do{
			//�ж��Ӵ���1�Ƿ����ж�
			if(gifr&GIFR_UT1INT_BIT){ /*���ݴ���*/
					/*���ݽ���*/
					 rxlen=Wk5678UartRxChars(1,rxbuf);//һ�ν��յ����ݲ��ᳬ��256Byte
					/*���ݷ���*/
					//�ѽ��յ����ݷ��ͳ�ȥ
					 if(rxlen>0){
							Wk5678UartTxChars(1,rxlen,rxbuf);
					 }
				 
			 }
			
			//�ж��Ӵ���2�Ƿ����ж�
			if(gifr&GIFR_UT2INT_BIT){
				/*���ݽ���*/
						rxlen=Wk5678UartRxChars(2,rxbuf);//һ�ν��յ����ݲ��ᳬ��256Byte
						/*���ݷ���*/
						//�ѽ��յ����ݷ��ͳ�ȥ
						if(rxlen>0){
						  Wk5678UartTxChars(2,rxlen,rxbuf);
						}
			
			 }
			//�ж��Ӵ���3�Ƿ����ж�
			if(gifr&GIFR_UT3INT_BIT){
				    /*���ݽ���*/
						rxlen=Wk5678UartRxChars(3,rxbuf);//һ�ν��յ����ݲ��ᳬ��256Byte
						/*���ݷ���*/
						//�ѽ��յ����ݷ��ͳ�ȥ
				    if(rxlen>0){
						  Wk5678UartTxChars(3,rxlen,rxbuf);
						}
			}
			//�ж��Ӵ���4�Ƿ����ж�
			if(gifr&GIFR_UT4INT_BIT){
				    /*���ݽ���*/
						rxlen=Wk5678UartRxChars(4,rxbuf);//һ�ν��յ����ݲ��ᳬ��256Byte
						/*���ݷ���*/
						//�ѽ��յ����ݷ��ͳ�ȥ
				    if(rxlen>0){
						  Wk5678UartTxChars(4,rxlen,rxbuf);
						}
			}
			
		  gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
	}while(gifr&0x0f);
	
	return gifr;
}

