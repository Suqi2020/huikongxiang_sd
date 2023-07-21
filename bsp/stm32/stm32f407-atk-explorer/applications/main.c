/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2018-11-19     flybreak     add stm32f407-atk-explorer bsp
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>

      
#define APP_VER       ((3<<8)+28)//0x0105 ��ʾ1.5�汾
//ע����������json��ʽ������UTF8_��ʽ���루GB2312��ʽ���ģ� �ᵼ�½���ʧ��
//    ��ӡlog���� ��[dataPhrs]err:json cannot phrase��  20230403
const char date[]="20230721";



//��ʾ����汾
void LCDDispSoftVer()
{
		char buf[10]="V_";
		sprintf((char *)buf+2,"%02d.%02d",(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	
	  LCDWtite(SOFT_VER_ADDR ,(uint8_t *)buf,strlen(buf));
}
//static    rt_thread_t tid 	= RT_NULL;
static    rt_thread_t tidW5500 	  = RT_NULL;
static    rt_thread_t tidNetRec 	= RT_NULL;
static    rt_thread_t tidNetSend 	= RT_NULL;
static    rt_thread_t tidUpkeep 	= RT_NULL;
static    rt_thread_t tidLCD      = RT_NULL;
static    rt_thread_t tidAutoCtrl = RT_NULL;

static    rt_thread_t tidSdRTC =RT_NULL;


//�����ź�������
rt_mutex_t   read485_mutex=RT_NULL;//��ֹ����߳�ͬ�¶�ȡmodbus����
rt_mutex_t   lcdSend_mutex=RT_NULL;//��ֹ����߳�ͬ����lcd������
rt_mutex_t   w5500Spi_mutex=RT_NULL;//��ֹ����߳�ͬʱ����spi
rt_mutex_t   sdWrite_mutex=RT_NULL;
rt_mutex_t   printf_mutex=RT_NULL;
//����Ķ���
extern struct  rt_mailbox mbNetSendData;;
static char 	 mbSendPool[20];//���ͻ���20��
static char 	 mbRecPool[20];//���ͻ���20��
extern struct rt_mailbox mbNetRecData;

static char 	 sdWritePool[100];//���ͻ���100��
extern struct rt_mailbox mbsdWriteData;


/* �¼����ƿ� */
struct rt_event mqttAckEvent;
rt_bool_t gbSDExit=true;

#ifdef USE_WDT
struct rt_event WDTEvent;
#endif





//���еĶ���

#if   USE_RINGBUF

#else
	struct  rt_messagequeue LCDmque;//= {RT_NULL} ;//����LCD����
	uint8_t LCDQuePool[LCD_BUF_LEN];  //����lcd���г�
#endif




//struct  rt_messagequeue SDLogmque;//= {RT_NULL} ;//����LCD����
//uint8_t SDLogQuePool[100];  //����lcd���г�
//����Ķ���
extern  void   netDataRecTask(void *para);//�������ݽ���
extern  void   netDataSendTask(void *para);//�������ݷ���
extern  void   upKeepStateTask(void *para);//��ʱ������� ���ڿ��ܼ��붨ʱ��ȡmodbus
extern  void   w5500Task(void *parameter);//w5500����״̬��ά��
extern  void   hardWareDriverTest(void);
extern  void   LCDTask(void *parameter);
extern  void   autoCtrlTask(void *para);
extern  void   logSaveSDTask(void *para);
extern  void   WDTTask(void *parameter);
extern  void   sdRTCTask(void *parameter);
const  static char sign[]="[main]";
extern rt_bool_t gbNetState;

/* ��ʱ���Ŀ��ƿ� */
static rt_timer_t timer1;
rt_bool_t gbNetState_p=RT_TRUE;
static  int netTick=0;
static void netStateSet()
{
		if(gbNetState==RT_TRUE){
			if(gbNetState_p==RT_FALSE){
						netTick++;
						if(netTick>=30){
								gbNetState_p=RT_TRUE;	
						}
				}
  	}
		else{
					gbNetState_p=RT_FALSE;
				  netTick=0;
		}
}
void  netLedLight()
{
	  static int count=0;
    count++;
		if(gbNetState_p==RT_TRUE){
				if(count%10==0){
						HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
				}
		}
		else{
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		}
}
//static int cnt = 0;
/* ��ʱ��1��ʱ���� ÿ100ms��ȥһ��*/
//10������һ�� uart offline״̬
static void timeout1(void *parameter)
{
		static int count=0;
	  extern void timeInc();
	  count++;  
	  if(gbSDExit==false){
				if(count%(100)==0)//10������һ��
						printf("%s�����TF���������豸\n",sign);
		}
	  if(count%10==0){
				timeInc();
		}
		netStateSet();
		netLedLight();
}

extern void creatFolder(void);
void  outIOInit(void);
char  printVer[50];


int main(void)
{


    printf("\n%s%s  ver=%02d.%02d\n",sign,date,(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  sprintf(printVer,"%s%s  ver=%02d.%02d\n",sign,date,(uint8_t)(APP_VER>>8),(uint8_t)APP_VER);
	  rt_err_t result;
		stm32_flash_read(FLASH_IP_SAVE_ADDR,    (uint8_t*)&packFlash,sizeof(packFlash));
		stm32_flash_read(FLASH_MODBUS_SAVE_ADDR,(uint8_t*)&sheet,    sizeof(sheet));
	  printf(" save1=%d save2=%d\n",sizeof(packFlash),sizeof(sheet));
	  if(packFlash_LEN<sizeof(packFlash)){
				printf("err:packFlash_LEN too small\n");
		}
	  if((packFlash_LEN+sizeof(packFlash)+sizeof(sheet))>65536){
				printf("err:64k flash to save user data too small\n");
		}
	  outIOInit();
	  RingBuff2_Init();
		if(packFlash.acuId[0]>=0x7F){
				rt_strcpy(packFlash.acuId,"000000000000001");//������� ִ��cJSON_AddStringToObject(root, "acuId",(char *)packFlash.acuId);
		}    
		  /* ������ʱ�� ���ڶ�ʱ�� */
    timer1 = rt_timer_create("timer1", timeout1,
                             RT_NULL, 100,
                             RT_TIMER_FLAG_PERIODIC);
		if (timer1 != RT_NULL)
        rt_timer_start(timer1);
		//����485�豸�õ��Ļ��� ����
		read485_mutex= rt_mutex_create("read485_mutex", RT_IPC_FLAG_FIFO);
		if (read485_mutex == RT_NULL)
    {
        printf("%screate read485_mutex failed\n",sign);
    }
		lcdSend_mutex= rt_mutex_create("lcdSend_mutex", RT_IPC_FLAG_FIFO);
		if (lcdSend_mutex == RT_NULL)
    {
        printf("%screate lcdSend_mutex failed\n",sign);
    }
		w5500Spi_mutex= rt_mutex_create("w5500Spi_mutex", RT_IPC_FLAG_FIFO);
		if (w5500Spi_mutex == RT_NULL)
    {
        printf("%screate w5500Spi_mutex failed\n",sign);
    }	
		sdWrite_mutex= rt_mutex_create("sdWrite_mutex", RT_IPC_FLAG_FIFO);
		if (sdWrite_mutex == RT_NULL)
    {
        printf("%screate sdWrite_mutex failed\n",sign);
    }		
		printf_mutex= rt_mutex_create("printf_mutex", RT_IPC_FLAG_FIFO);
		if (printf_mutex == RT_NULL)
    {
        printf("%screate sdWrite_mutex failed\n",sign);
    }		
#if   USE_RINGBUF

#else
		int ret = rt_mq_init(&LCDmque,"LCDrecBuf",&LCDQuePool[0],1,LCD_BUF_LEN,RT_IPC_FLAG_FIFO);       
		if (ret != RT_EOK)
		{
				printf("%sinit LCD msgque failed.\n",sign);
				return -1;
		}
#endif
		extern void 	uartMutexQueueCreate();
		uartMutexQueueCreate();
		
//	 ret = rt_mq_init(&SDLogmque,"SDLogmque",&SDLogQuePool[0],1,sizeof(SDLogQuePool),RT_IPC_FLAG_FIFO);       
//	if (ret != RT_EOK)
//	{
//			rt_kprintf("%sinit SDLogmque  failed.\n",sign);
//			return -1;
//	}
		
		


////////////////////////////////////����//////////////////////////////////
		
    result = rt_mb_init(&mbsdWriteData,"sdsend",&sdWritePool[0],sizeof(sdWritePool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox mbsdWriteData failed.\n",sign);
        return -1;
    }
    result = rt_mb_init(&mbNetRecData,"mbRec",&mbRecPool[0],sizeof(mbRecPool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox NetRecData failed.\n",sign);
        return -1;
    }
    result = rt_mb_init(&mbNetSendData,"mbSend",&mbSendPool[0],sizeof(mbSendPool)/4,RT_IPC_FLAG_FIFO);         
    if (result != RT_EOK)
    {
        printf("%sinit mailbox NetSend failed.\n",sign);
        return -1;
    }
#ifdef  USE_WDT   
		if (rt_event_init(&WDTEvent, "WDTEvent", RT_IPC_FLAG_FIFO) != RT_EOK)
    {
        rt_kprintf("%sinit WDTEvent failed.\n",sign);

    }
#endif


////////////////////////////////����////////////////////////////////////

		tidLCD    =  rt_thread_create("LCD",LCDTask,RT_NULL,512*2,2, 10 );
		if(tidLCD!=NULL){
				rt_thread_startup(tidLCD);													 
				printf("%sRTcreat LCDStateTask \r\n",sign);
		}
		tidNetRec =  rt_thread_create("netRec",netDataRecTask,RT_NULL,512*2,3, 10 );
		if(tidNetRec!=NULL){
				rt_thread_startup(tidNetRec);													 
				printf("%sRTcreat netDataRecTask \r\n",sign);
		}
		tidNetSend =  rt_thread_create("netSend",netDataSendTask,RT_NULL,512*2,3, 10 );
		if(tidNetSend!=NULL){
				rt_thread_startup(tidNetSend);													 
				printf("%sRTcreat netDataSendTask \r\n",sign);
		}
		tidUpkeep 	=  rt_thread_create("upKeep",upKeepStateTask,RT_NULL,512*4,4, 10 );
		if(tidUpkeep!=NULL){
				rt_thread_startup(tidUpkeep);													 
				printf("%sRTcreat upKeepStateTask \r\n",sign);
		}
    tidW5500 =  rt_thread_create("w5500",w5500Task,RT_NULL,512*3,2, 10 );
		if(tidW5500!=NULL){
				rt_thread_startup(tidW5500);													 
				printf("%sRTcreat w5500Task task\r\n",sign);
		}

		tidSdRTC =  rt_thread_create("sdRTC",sdRTCTask,RT_NULL,512*4,8, 10 );
		if(tidSdRTC!=NULL){
				rt_thread_startup(tidSdRTC);													 
				printf("%sRTcreat sdRTCTask\r\n",sign);
		}
		tidAutoCtrl =  rt_thread_create("autoCtrl",autoCtrlTask,RT_NULL,512*2,5, 10 );
		if(tidAutoCtrl!=NULL){
				rt_thread_startup(tidAutoCtrl);													 
				printf("%sRTcreat autoCtrlTask\r\n",sign);
		}
#ifdef  USE_WDT
		extern IWDG_HandleTypeDef hiwdg;
		static    rt_thread_t tidWDT      = RT_NULL;
		tidWDT =  rt_thread_create("WDT",WDTTask,RT_NULL,256,10, 10 );
		if(tidWDT!=NULL){
				rt_thread_startup(tidWDT);													 
				printf("%sRTcreat WDTTask\r\n",sign);
		}
		HAL_IWDG_Refresh(&hiwdg);
#endif	
		//���г�ʼ��֮���ٿ��������жϽ���
//		char test[50]={0};
//			uint32_t a=4294967290;
//			sprintf(test,"long:%u", a);//"monitoringTime":"1655172531937"
//			rt_kprintf("%s %s \r\n",sign,test);
//////////////////////////////����//////////////////////////////////////
    while (0)//task���ڲ��� �Լ����Ʋ���
    {
				hardWareDriverTest();
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
				rt_thread_mdelay(250);
			  extern rt_bool_t gbNetState;
			  if(gbNetState==RT_TRUE){
					  rt_thread_mdelay(250);
						
				}
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				rt_thread_mdelay(250);
				if(gbNetState==RT_TRUE){
					  rt_thread_mdelay(250);	
				}
    }
}
//tasklog delete  �̹߳���ʱ�����ɾ��   
void  tasklog(int argc, char *argv[])
{
		if(argc!=2){
				goto ERR;
		}
		if(rt_strcmp("delete",argv[1])==0){
			  if((tidW5500->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidW5500);
				if((tidNetRec->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidNetRec);
				if((tidNetSend->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidNetSend);
				if((tidUpkeep->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidUpkeep);
				if((tidAutoCtrl->stat & RT_THREAD_STAT_MASK)==RT_THREAD_SUSPEND)
						rt_thread_delete(tidAutoCtrl);
			  rt_kprintf("%s[tasklog delete OK]\n",sign);
			  return;
		}
		ERR:
		rt_kprintf("%serr for example:\n",sign);
		rt_kprintf("%s[tasklog delete]\n",sign);
}
MSH_CMD_EXPORT(tasklog,tasklog del);//FINSH_FUNCTION_EXPORT_CMD

