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
		if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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
		if((__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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
		if((__HAL_UART_GET_FLAG(&huart4,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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
		if((__HAL_UART_GET_FLAG(&huart5,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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
		if((__HAL_UART_GET_FLAG(&huart6,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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


//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
//uint16_t Get_Adc(uint32_t ch)   
//{
//    ADC_ChannelConfTypeDef ADC1_ChanConf;
//    
//    ADC1_ChanConf.Channel=ch;                                   //ͨ��
//    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
//    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //����ʱ��
//    ADC1_ChanConf.Offset=0;                 
//    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //ͨ������
//	
//    HAL_ADC_Start(&hadc1);                               //����ADC
//	
//    HAL_ADC_PollForConversion(&hadc1,10);                //��ѯת��
// 
//	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        //�������һ��ADC1�������ת�����
//}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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
				rt_kprintf("ADC_CHANNEL_8 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
				 rt=Get_Adc_Average(ADC_CHANNEL_9,10);
				rt_kprintf("ADC_CHANNEL_9 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_10,10);
				rt_kprintf("ADC_CHANNEL_10 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_11,10);
				rt_kprintf("ADC_CHANNEL_11 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_12,10);
				rt_kprintf("ADC_CHANNEL_12 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_13,10);
				rt_kprintf("ADC_CHANNEL_13 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_14,10);
				rt_kprintf("ADC_CHANNEL_14 ,adc tick:%d,voltgge ����100��:%dV\n",rt,rt*330/4096);
					 rt=Get_Adc_Average(ADC_CHANNEL_15,10);
				rt_kprintf("ADC_CHANNEL_15 ,adc tick:%d,voltgge ����100��:%dV\n\r\n\r\n",rt,rt*330/4096);
}

#endif

#ifdef TEST_IOIN
static void ioInTest()
{
	   if(HAL_GPIO_ReadPin(IO_IN1_GPIO_Port, IO_IN1_Pin)==GPIO_PIN_SET){
				rt_kprintf("1·��\r\n");
		 }
		 else{
				rt_kprintf("1·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN2_GPIO_Port, IO_IN2_Pin)==GPIO_PIN_SET){
				rt_kprintf("2·��\r\n");
		 }
		 else{
				rt_kprintf("2·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN3_GPIO_Port, IO_IN3_Pin)==GPIO_PIN_SET){
				rt_kprintf("3·��\r\n");
		 }
		 else{
				rt_kprintf("3·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN4_GPIO_Port, IO_IN4_Pin)==GPIO_PIN_SET){
				rt_kprintf("4·��\r\n");
		 }
		 else{
				rt_kprintf("4·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN5_GPIO_Port, IO_IN5_Pin)==GPIO_PIN_SET){
				rt_kprintf("5·��\r\n");
		 }
		 else{
				rt_kprintf("5·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN6_GPIO_Port, IO_IN6_Pin)==GPIO_PIN_SET){
				rt_kprintf("6·��\r\n");
		 }
		 else{
				rt_kprintf("6·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN7_GPIO_Port, IO_IN7_Pin)==GPIO_PIN_SET){
				rt_kprintf("7·��\r\n");
		 }
		 else{
				rt_kprintf("7·��\r\n");
		 }
		 if(HAL_GPIO_ReadPin(IO_IN8_GPIO_Port, IO_IN8_Pin)==GPIO_PIN_SET){
				rt_kprintf("8·��\r\n");
		 }
		 else{
				rt_kprintf("8·��\r\n");
		 }
		 
			 
}
#endif

#ifdef TEST_IOOUT
static void ioOutTest()//ע����ú����е���ʱ���ϴ˴�����ʱ
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
//����main������ while�е��ò��� ע��while�е���ʱ�������������
//IOoutput  
//adc
//ioinput
//uart��drv_gpio�в���
void  hardWareDriverTest(void)
{
		
#ifdef TEST_IOOUT
		ioOutTest();//ע����ú����е���ʱ���ϴ˴�����ʱ
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
//TX_RX_MAX_BUF_SIZE=16*1024  �Լ�MAX_SOCK_NUM=1

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




//extern uint64_t subTimeStamp;
// void  subTimeStampSet(uint64_t time)
//{
//	  if(time>=rt_tick_get())
//				subTimeStamp=time-rt_tick_get();//������rtcֵ-��ǰtickֵ
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
			  rt_kprintf("%stime:RTC ������3�� Уʱ\r\n","[TEST]");
		}
	  

	  return 0;
}
//FINSH_FUNCTION_EXPORT(tick, tick finsh);//FINSH_FUNCTION_EXPORT_CMD
MSH_CMD_EXPORT(tickSet,tick set stamp);//FINSH_FUNCTION_EXPORT_CMD



uint32_t 	offLTimes;
uint32_t 	onLTimes;

//ʵ��������ߵļ��  ���ߴ��� ÿ�ε���ʱ����
//����ε����������ʱ����׼ȷ ��Ҫ������������ ���߷������ݵļ��ʱ�� 
//�ε�����ʱ����Ҫ�������ݲ�����֤����Ͽ�
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
//	DWORD fre_clust;
//	FATFS *pfs;
	int tot_sect,fre_sect;
//  int res_flash = f_getfree("0:", &fre_clust, &pfs); 
//	
//  if(FR_OK==0){
//  /* ����õ��ܵ����������Ϳ��������� */
//		 tot_sect = (pfs->n_fatent - 2) * pfs->csize;
//		 fre_sect = fre_clust * pfs->csize;
//	}
//	else{
//		rt_kprintf("err:sdSize read %d\n",FR_OK);
//	}
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




