#include "board.h"

//����ÿ�����ڶ�Ӧ�Ĳ����� �Լ�modbus������
//�����ڶ����������� UART2(1) UART3(2) UART6(3) UART1(debug) UART4(4)
//Ŀǰ��������       ����  �ַ�  ������             ����



const static char sign[]="[uartRecfg]";
//uartConfStru  uartDev[UART_NUM];
//uartChanlStru  chanl={USE_DIS_UART,USE_DIS_UART,USE_DIS_UART,USE_DIS_UART};
//static rt_mutex_t uartMutex[UART_NUM] ;//= {RT_NULL}; //����4�����ڵĻ���������

//���еĶ���
//struct  rt_messagequeue uartmque[UART_NUM];//= {RT_NULL} ;//����4�����ڵĶ���
//static uint8_t uartQuePool[UART_NUM][MSGPOOL_LEN];  //����4�����ڵĶ��г�
extern rt_sem_t   uart1234_sem;
extern rt_sem_t   uart5678_sem;

//���ڽ���  �������ȡmodbus��������
//����0-8  һ��9��485����
//���ݽ���
//����������ݳ���
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
							//�ж��Ӵ���1�Ƿ����ж�
							if(gifr&GIFR_UT1INT_BIT){ /*���ݴ���*/
									/*���ݽ���*/
									 count=Wk1234UartRxChars(1,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}	
							//�ж��Ӵ���2�Ƿ����ж�
							if(gifr&GIFR_UT2INT_BIT){
								/*���ݽ���*/
									 count=Wk1234UartRxChars(2,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}
							//�ж��Ӵ���3�Ƿ����ж�
							if(gifr&GIFR_UT3INT_BIT){
										/*���ݽ���*/
									count=Wk1234UartRxChars(3,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}
							//�ж��Ӵ���4�Ƿ����ж�
							if(gifr&GIFR_UT4INT_BIT){
										/*���ݽ���*/
									 count=Wk1234UartRxChars(4,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
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
							//�ж��Ӵ���1�Ƿ����ж�
							if(gifr&GIFR_UT1INT_BIT){ /*���ݴ���*/
									/*���ݽ���*/
									 count=Wk5678UartRxChars(1,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}	
							//�ж��Ӵ���2�Ƿ����ж�
							if(gifr&GIFR_UT2INT_BIT){
								/*���ݽ���*/
									 count=Wk5678UartRxChars(2,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}
							//�ж��Ӵ���3�Ƿ����ж�
							if(gifr&GIFR_UT3INT_BIT){
										/*���ݽ���*/
									count=Wk5678UartRxChars(3,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}
							//�ж��Ӵ���4�Ƿ����ж�
							if(gifr&GIFR_UT4INT_BIT){
										/*���ݽ���*/
									 count=Wk5678UartRxChars(4,recBuf);//һ�ν��յ����ݲ��ᳬ��256Byte
							}
							
							gifr=Wk5678ReadGlobalRegister(WK2XXX_GIFR_REG);
					}while(gifr&0x0f);
				}
				break;
			case 8://����һ·  ����
				rt_thread_mdelay(timeout);
				while(true==Read_RingBuff3(recBuf+count)){
					count++;
					rt_thread_mdelay(2);
				}
				break;
		}
		return count;
}





//������������ ѡ���Ӧ��modbus������
void uartReconfig()
{
		for(int i=0;i<UART_NUM;i++){
				if(packFlash.uartBps[i]==0){//û�õ��Ĵ��� ����Ĭ�ϲ����� 
					  packFlash.uartBps[i]=9600;
				}
		}
		for(int i=0;i<UART_NUM;i++){
				rt_kprintf("%sport%d bps[%d]\n",sign,i+1,packFlash.uartBps[i]);
		}
	


		MX_USART6_UART_Init(packFlash.uartBps[8]);
		
		Wk1xxxRstInit();
		/*�����ڲ�����ƥ��*/
		Wk1234MasterUartBaudAdaptive();
//		//��ʼ���Ӵ���
		Wk1234UartInit(1);
		Wk1234UartInit(2);
		Wk1234UartInit(3);
		Wk1234UartInit(4);
//		//�����Ӵ��ڲ�����
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
//		/*�����ڲ�����ƥ��*/
		Wk5678MasterUartBaudAdaptive();
//		//��ʼ���Ӵ���
		Wk5678UartInit(1);
		Wk5678UartInit(2);
		Wk5678UartInit(3);
		Wk5678UartInit(4);
//		//�����Ӵ��ڲ�����
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
//���ڲ��������³�ʼ��
//���봮��0-8  ������
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
			//�����Ӵ��ڲ�����
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
			//�����Ӵ��ڲ�����
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
//485���ݴ��ڷ���
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


//modbus+huanliu���豸���ƣ�+1(ͨ��)+1���豸��ַ��
uint16 atoi16(char* str,uint16 base); 
