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
  *                              �������
  ******************************************************************************
  */
char SDPath[4]; /* SD�߼�������·�� */
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;            			  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F407������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  

extern FATFS flash_fs;
extern Diskio_drvTypeDef  SD_Driver;

void testSD()
{
	 /* ��ʼ���������� */
   printf("****** ����һ��SD���ļ�ϵͳʵ�� ******\r\n");
   //�����������������̷�
   FATFS_LinkDriver(&SD_Driver, SDPath);
   //���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
   res_sd = f_mount(&fs,"0:",1);  


   /*----------------------- ��ʽ������ ---------------------------*/  
   /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
   if(res_sd == FR_NO_FILESYSTEM)
   {
      printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
      /* ��ʽ�� */
      res_sd=f_mkfs("0:",0,0);							

      if(res_sd == FR_OK)
      {
        printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
        /* ��ʽ������ȡ������ */
        res_sd = f_mount(NULL,"0:",1);			
        /* ���¹���	*/			
        res_sd = f_mount(&fs,"0:",1);
      }
      else
      {

        printf("������ʽ��ʧ�ܡ�����\r\n");
        while(1);
      }
   }
   else if(res_sd!=FR_OK)
   {
      printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
      printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
      while(1);
   }
   else
   {
      printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
   }	
    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    /* ���ļ�������ļ��������򴴽��� */
    printf("\r\n****** ���������ļ�д�����... ******\r\n");	
    res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
      printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
      /* ��ָ���洢������д�뵽�ļ��� */
      res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
      if(res_sd==FR_OK)
      {
        printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
        printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
      }
      else
      {
        printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
      }    
      /* ���ٶ�д���ر��ļ� */
      f_close(&fnew);
    }
    else
    {	

      printf("������/�����ļ�ʧ�ܡ�\r\n");
    }
    
  /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
    printf("****** ���������ļ���ȡ����... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(res_sd == FR_OK)
    {

      printf("�����ļ��ɹ���\r\n");
      res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
      if(res_sd==FR_OK)
      {
        printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
        printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
      }
      else
      {
        printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
      }		
    }
    else
    {

      printf("�������ļ�ʧ�ܡ�\r\n");
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);	
    
    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
    f_mount(NULL,"0:",1);
    
}
#endif



extern ADC_HandleTypeDef hadc1;


void adcGetTest()
{
	   rt_kprintf("adc�ɼ�����\n");

			int rt=Get_Adc_Average(ADC_CHANNEL_9,10);
			rt_kprintf("1·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
			rt=Get_Adc_Average(ADC_CHANNEL_8,10);
			rt_kprintf("2·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_15,10);
			rt_kprintf("3·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_14,10);
			rt_kprintf("4·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_13,10);
			rt_kprintf("5·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_12,10);
			rt_kprintf("6·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_11,10);
			rt_kprintf("7·,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_10,10);
			rt_kprintf("8·,adc tick:%d,voltgge ����100��:%dV\n\r\n\r\n",rt,rt*330/4096);
}


 void ioInOutTest()
{
	   rt_kprintf("IO�������������\n");
	   if(HAL_GPIO_ReadPin(IO_IN1_GPIO_Port, IO_IN1_Pin)==GPIO_PIN_SET){
				rt_kprintf("1·��\r\n");
				IOOUT1_ON;
		 }
		 else{
				rt_kprintf("1·��\r\n");
				IOOUT1_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN2_GPIO_Port, IO_IN2_Pin)==GPIO_PIN_SET){
				rt_kprintf("2·��\r\n");
				IOOUT2_ON;
		 }
		 else{
				rt_kprintf("2·��\r\n");
				IOOUT2_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN3_GPIO_Port, IO_IN3_Pin)==GPIO_PIN_SET){
				rt_kprintf("3·��\r\n");
				IOOUT3_ON;
		 }
		 else{
				rt_kprintf("3·��\r\n");
				IOOUT3_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN4_GPIO_Port, IO_IN4_Pin)==GPIO_PIN_SET){
				rt_kprintf("4·��\r\n");
				IOOUT4_ON;
		 }
		 else{
				rt_kprintf("4·��\r\n");
				IOOUT4_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN5_GPIO_Port, IO_IN5_Pin)==GPIO_PIN_SET){
				rt_kprintf("5·��\r\n");
				IOOUT5_ON;
		 }
		 else{
				rt_kprintf("5·��\r\n");
				IOOUT5_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN6_GPIO_Port, IO_IN6_Pin)==GPIO_PIN_SET){
				rt_kprintf("6·��\r\n");
				IOOUT6_ON;
		 }
		 else{
				rt_kprintf("6·��\r\n");
				IOOUT6_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN7_GPIO_Port, IO_IN7_Pin)==GPIO_PIN_SET){
				rt_kprintf("7·��\r\n");
				IOOUT7_ON;
		 }
		 else{
				rt_kprintf("7·��\r\n");
				IOOUT7_OFF;
		 }
		 if(HAL_GPIO_ReadPin(IO_IN8_GPIO_Port, IO_IN8_Pin)==GPIO_PIN_SET){
				rt_kprintf("8·��\r\n");
				IOOUT8_ON;
		 }
		 else{
				rt_kprintf("8·��\r\n");
				IOOUT8_OFF;
		 }
		 
			 
}

 void ioOutTest()//ע����ú����е���ʱ���ϴ˴�����ʱ
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
						rt_kprintf("uart[8] �շ����Գɹ�\n");
				}
				else{
						rt_kprintf("uart[8] �շ�����ʧ��\n");
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
							//�ж��Ӵ���1�Ƿ����ж�
							if(gifr&GIFR_UT1INT_BIT){ /*���ݴ���*///pcbӳ�䴮��1+1
									/*���ݽ���*/
									 count=Wk1234UartRxChars(1,(uint8_t *)testRBuf[1]);//һ�ν��յ����ݲ��ᳬ��256Byte
								   rt_kprintf("485 1read\n");
								  return count;
							}	
							//�ж��Ӵ���2�Ƿ����ж�
							if(gifr&GIFR_UT2INT_BIT){//pcbӳ�䴮��2+1
								/*���ݽ���*/
									 count=Wk1234UartRxChars(2,(uint8_t *)testRBuf[2]);//һ�ν��յ����ݲ��ᳬ��256Byte
									 rt_kprintf("485 2read\n");
								  return count;
							}
							//�ж��Ӵ���3�Ƿ����ж�
							if(gifr&GIFR_UT3INT_BIT){//pcbӳ�䴮��0+1
										/*���ݽ���*/
									count=Wk1234UartRxChars(3,(uint8_t *)testRBuf[0]);//һ�ν��յ����ݲ��ᳬ��256Byte
								  rt_kprintf("485 0read\n");
								  return count;
							}
							//�ж��Ӵ���4�Ƿ����ж�
							if(gifr&GIFR_UT4INT_BIT){//pcbӳ�䴮��3+1
										/*���ݽ���*/
									 count=Wk1234UartRxChars(4,(uint8_t *)testRBuf[3]);//һ�ν��յ����ݲ��ᳬ��256Byte
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
							//�ж��Ӵ���1�Ƿ����ж�
							if(gifr&GIFR_UT1INT_BIT){ /*���ݴ���*///pcbӳ�䴮��7
									/*���ݽ���*/
									 count=Wk5678UartRxChars(1,(uint8_t *)testRBuf[5]);//һ�ν��յ����ݲ��ᳬ��256Byte
								rt_kprintf("485 6read\n");
							}	
							//�ж��Ӵ���2�Ƿ����ж�
							if(gifr&GIFR_UT2INT_BIT){//pcbӳ�䴮��5
								/*���ݽ���*/
									 count=Wk5678UartRxChars(2,(uint8_t *)testRBuf[6]);//һ�ν��յ����ݲ��ᳬ��256Byte
								rt_kprintf("485 4read\n");
							}
							//�ж��Ӵ���3�Ƿ����ж�
							if(gifr&GIFR_UT3INT_BIT){//pcbӳ�䴮��6
										/*���ݽ���*/
									count=Wk5678UartRxChars(3,(uint8_t *)testRBuf[4]);//һ�ν��յ����ݲ��ᳬ��256Byte
								rt_kprintf("485 5read\n");
							}
							//�ж��Ӵ���4�Ƿ����ж�
							if(gifr&GIFR_UT4INT_BIT){//pcbӳ�䴮��8
										/*���ݽ���*/
									 count=Wk5678UartRxChars(4,(uint8_t *)testRBuf[7]);//һ�ν��յ����ݲ��ᳬ��256Byte
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
						rt_kprintf("uart[%d] �շ����Գɹ�\n",j);
				}
				else{
						rt_kprintf("uart[%d] �շ�����ʧ��\n",j);
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
			rt_kprintf("�̵����������\n");
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



//��ȡtickֵ
//���ܴ�ӡ uint64_t ����  ��ֳ�32λ��ӡ
int tick()
{
    RTC_TimeTypeDef read;
	  uint64_t time =utcTime_ms();

	  uint32_t rtc_s=time/1000;
	  uint32_t rtc_ms=time%1000;
		read=utc_to_beijingTime(rtc_s);
		rt_kprintf("[tick][%lu]-s\r\n",rtc_s);//����ͬʱ��ӡ��Ҫ�ֿ���ӡ����ȷ
		rt_kprintf("[tick][%lu]-ms\r\n",rtc_ms);
	  rt_kprintf("[tick]%d��%d��%d��%dʱ%d��%d��\r\n",read.year,read.month,read.day,\
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
			  rt_kprintf("%stime:RTC ������3�� Уʱ\r\n","[TEST]");
		}
	  

	  return 0;
}
MSH_CMD_EXPORT(tickSet,tick set stamp);//FINSH_FUNCTION_EXPORT_CMD



uint32_t 	offLTimes;
uint32_t 	onLTimes;

//ʵ��������ߵļ��  ���ߴ��� ÿ�ε���ʱ����
//����ε����������ʱ����׼ȷ ��Ҫ������������ ���߷������ݵļ��ʱ�� 
//�ε�����ʱ����Ҫ�������ݲ�����֤����Ͽ�
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
  /* ����õ��ܵ����������Ϳ��������� */
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
  /* ��ӡ��Ϣ(4096 �ֽ�/����) */
	ERROR:
	if(gbSDExit==true){
		  rt_kprintf("�豸�ܿռ�:%ukB ���ÿռ�:%ukB��\n", (tot_sect>>1), (fre_sect>>1));
			rt_kprintf("�豸�ܿռ�:%.2fGB ���ÿռ�:%.2fGB��\n", (float)tot_sect/2/1024/1024, (float)fre_sect/2/1024/1024);
	}
	else{
			rt_kprintf("err:sd not exit\n");
	}
	return 0;
}
//FINSH_FUNCTION_EXPORT(offline, offline finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(SDSize,SDSize stamp);//FINSH_FUNCTION_EXPORT_CMD




