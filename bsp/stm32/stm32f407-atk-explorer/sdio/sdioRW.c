/**
  ******************************************************************************
  * @file    BSP/Src/sd.c 
  * @author  MCD Application Team
  * @brief   This example code shows how to use the SD Driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
//������ �ٶȴﵽ 3538992�ֽ�/279��=12.38KB/��
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_sdio_sd.h"

#include "sdioRW.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define BLOCK_START_ADDR         0     /* Block start address      */
//#define NUM_OF_BLOCKS            5   /* Total number of blocks   */
//#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
//    
///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
//uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
//uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
//static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
//static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/
static const char sign[]="[SD]";


char SDPath[4]; /* SD�߼�������·�� */
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
extern Diskio_drvTypeDef  SD_Driver;

extern rt_bool_t gbSDExit;
//extern uint64_t utcTime_ms();

extern rt_mutex_t sdWrite_mutex;
//������Ҫ��ʱ���ת��������ʵ��
DWORD  get_fattime (void)
{
	DWORD Value;


	RTC_TimeTypeDef read;
	uint64_t time =utcTime_ms();

	uint32_t rtc_s=time/1000;
	read=utc_to_beijingTime(rtc_s);


	Value = ((read.year + 20) << 25) | (read.month << 21) | (read.day << 16) |
	(read.hour << 11) | (read.minute << 5) | (read.second >> 1);

  return Value;
 // return 0;//(DWORD )+254275200;//utcTime_s()
}



static char logName[]="log";
//����2����Ŀ¼IDĿ¼
void creatIDFolder(int i)
{
	  
		char *txtName;
//	  uint32_t fnum;
//	  int ret;
		txtName =rt_malloc(50);
		strcpy(txtName,logName);
		f_mkdir(txtName);//����Ŀ¼
		memset(txtName,0,sizeof((char *)txtName));
		switch(i)
		{
			case CIRCULA:
				for(int j=0;j<CIRCULA_485_NUM;j++){//�˶���û�����ù�
						if(sheet.cirCula[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.cirCula[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case PARTDISCHAG:
				for(int j=0;j<PARTDISCHAG_485_NUM;j++){//�˶���û�����ù�
						if(sheet.partDischag[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.partDischag[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case PRESSSETTL:
				for(int j=0;j<PRESSSETTL_485_NUM;j++){//�˶���û�����ù�
						if(sheet.pressSetl[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.pressSetl[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case THREEAXIS:
				for(int j=0;j<THREEAXIS_485_NUM;j++){//�˶���û�����ù�
						if(sheet.threeAxiss[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.threeAxiss[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
#ifdef 	 USE_4GAS 	
			case CH4:
					for(int j=0;j<CH4_485_NUM;j++){//�˶���û�����ù�
							if(sheet.ch4[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.ch4[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
			case O2:
					for(int j=0;j<O2_485_NUM;j++){//�˶���û�����ù�
							if(sheet.o2[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.o2[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
			case H2S:
					for(int j=0;j<H2S_485_NUM;j++){//�˶���û�����ù�
							if(sheet.h2s[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.h2s[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
			case CO:
					for(int j=0;j<CO_485_NUM;j++){//�˶���û�����ù�
							if(sheet.co[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.co[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
#endif
			case TEMPHUM:
				for(int j=0;j<TEMPHUM_485_NUM;j++){//�˶���û�����ù�
						if(sheet.tempHum[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.tempHum[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case WATERDEPTH:
				for(int j=0;j<WATERDEPTH_485_NUM;j++){//�˶���û�����ù�
						if(sheet.waterDepth[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.waterDepth[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case CRACKMETER:
				for(int j=0;j<CRACKMETER_485_NUM;j++){//�˶���û�����ù�
						if(sheet.crackMeter[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.crackMeter[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case COVER:
				for(int j=0;j<COVER_485_NUM;j++){//�˶���û�����ù�
						if(sheet.cover[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.cover[j].ID);
							f_mkdir(txtName);//����Ŀ¼
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			default:
				rt_kprintf("%serr num %d\n",sign,i);
				break;
		}
	 rt_free(txtName);
	 txtName=NULL;
}

//�ϵ�󴴽�jinggai yangqi huanliu�ȴ�����������ļ��� һ��Ŀ¼
//bool fountFlag=false;
void creatFolder()
{
	 volatile int ret;
	    //�����������������̷�Nm
   FATFS_LinkDriver(&SD_Driver, SDPath);
   //���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
   res_sd = f_mount(&fs,"0:",1);  
   if(res_sd==FR_OK){
//		  fountFlag=true;
		  for(int j=0;j<MODBUS_NUM;j++){
					ret=f_mkdir(modbusName[j]);//����Ŀ¼
				  if((ret==FR_OK)||(ret==FR_EXIST)){
						  creatIDFolder(j);
					}
					else
							rt_kprintf("%sERR:mkdir ret=%d\n",sign,ret);
			}
			f_close(&fnew);
	 }
    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
		if(gbSDExit==false){
				rt_kprintf("%s�����TF���������豸\n",sign);
				return;
		}
}




//1685592000  2023 06 01 4:00:00

//log ����sd��


//����ʱ��tick�洢log��txt�ļ���
void  logSaveToSD(char *buf,char lenth)
{
		if(gbSDExit==false){
				return;
		}
		rt_mutex_take(sdWrite_mutex,RT_WAITING_FOREVER);
		char *timeBuf;
	  uint32_t fnum;
	  int ret;
		timeBuf =rt_malloc(100);
	  char *txtName;
		char timeSign[12];
		txtName =rt_malloc(50);
		strcpy(txtName,"log");
		strcat(txtName,"/");
		sprintf(timeSign,"%llu",(utcTime_s()/TXT_LOG_TIME)*TXT_LOG_TIME);//1Сʱ����һ����Ŀ¼ Ϊ�����1Сʱ�ڵ�����Ϊ0 ��ȥ28800 ��С8��Сʱ Ϊ��log������ͷ������12���
		strcat(txtName,timeSign);
		strcat(txtName,".txt");
		ret=f_open(&fnew,txtName, FA_WRITE);
	
		if(lenth>1){
				RTC_TimeTypeDef read;
				uint64_t time =utcTime_ms();
				uint32_t rtc_s=time/1000; 
				read=utc_to_beijingTime(rtc_s);
				int len2=sprintf(timeBuf,"[%d-%d-%d-%d:%d:%d]",read.year,read.month,read.day,\
				read.hour,read.minute,read.second );
		}
		if(ret!=FR_OK){
		    ret=f_open(&fnew,txtName, FA_CREATE_NEW |FA_WRITE);//suqi
				rt_kprintf("create:%s\r\n",txtName );
		}
		if((ret==FR_OK)||(ret==FR_EXIST)){
				f_lseek(&fnew,f_size(&fnew));
				if(lenth>1){//���ȱȽϳ���д��ʱ��
					ret=f_write(&fnew,timeBuf,strlen(timeBuf),&fnum);
				}
		    ret=f_write(&fnew,buf,lenth,&fnum);
		}
		else{
				rt_kprintf("%sERR:f_open  write %s,ret=%d\n",sign,txtName,ret);
		}
		f_close(&fnew);
		rt_free(txtName);
		txtName=NULL;
		rt_free(timeBuf);
	  timeBuf=NULL;
		rt_mutex_release(sdWrite_mutex);
}



//log�ļ�����
//ɾ�����ڵ�log��txt�ļ�
static uint32_t logSaveTime[TXT_LOG_NUM+1]={0};
static char logTime[12];
//�ҵ�logSaveTime�е���Сʱ�� 
static uint32_t findLogMiniTime()
{
	  uint32 mini=logSaveTime[0];//��װ��һ��ֵ �����ҵ���Сֵ
		for(int i=1;i<sizeof(logSaveTime)/sizeof(logSaveTime[0]);i++){
				mini=(mini<logSaveTime[i]?mini:logSaveTime[i]);
		}
		return mini;
}
//��ȡ��ɾ�������txt�ļ�
void FatReadDirDelEarlyLogTxt()
{
		if(gbSDExit==false){
			return;
		}
		rt_mutex_take(sdWrite_mutex,RT_WAITING_FOREVER);
		FILINFO fileinfo;
		DIR Dir;
	  char dirName[10]="0:/";
	  strcat(dirName,logName);
	  int txtCount=0;
	  char delPath[30]="";
	  strcat(delPath,dirName);
	  strcat(delPath,"/");
		for(int i=0;i<TXT_LOG_NUM;i++){
				logSaveTime[i]=0;
		}
    if(f_opendir(&Dir,(const TCHAR*)dirName) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir�ṹ���б��� */
    {
        while(f_readdir(&Dir, &fileinfo) == FR_OK)  /* ���ļ���Ϣ���ļ�״̬�ṹ���� */
        {
            if(!fileinfo.fname[0]) break; /* ����ļ���Ϊ��\0'��˵����ȡ��ɽ��� */
					  
						for(int j=0;j<strlen(fileinfo.fname);j++){
							 if(fileinfo.fname[j]=='.'){
									logTime[j]=0;
								  break;
							 }
							 logTime[j]=fileinfo.fname[j];//��ȡ���� ��Ҫ.txt
						}
						//printf("��ȡʱ�� %s\n",logTime);
						if(txtCount<=TXT_LOG_NUM+1){
								logSaveTime[txtCount]=atoi32(logTime,10);//ʱ���ַ�����ʽ��Ϊ32λ����		
						}
						txtCount++;
        }
    }
		if(txtCount>TXT_LOG_NUM){//�ϴζ�ȡ��		
				for(int i=0;i<=TXT_LOG_NUM;i++){
						rt_kprintf("log[%d] time��%d\n",i+1,logSaveTime[i]);
				}			
				rt_kprintf("find mini:[%d]\n",findLogMiniTime());
				sprintf(delPath+strlen(delPath),"%d.txt",findLogMiniTime());
				rt_kprintf("count[%d]log del:%s\r\n",txtCount,delPath );
//				if(f_opendir(&Dir,(const TCHAR*)dirName) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir[0]=�ṹ���б��� */
//				{
						int ret=f_unlink(delPath);
					  if(ret!=FR_OK){
								rt_kprintf("del err:[%d]\n",ret);
						}
						
//				}
		}
		f_closedir(&Dir);
		rt_mutex_release(sdWrite_mutex);
}








//////////////////////////////////////////////////////////////////
/*�����ļ�����*/

//ɾ�����ڵ�log��txt�ļ�
static uint32_t dataSaveTime[TXT_DATA_NUM+1]={0};
static char   dataTime[20];
//�ҵ�logSaveTime�е���Сʱ�� 
static uint32_t findDataMiniTime()
{
	  uint32 mini=dataSaveTime[0];//��װ��һ��ֵ �����ҵ���Сֵ
		for(int i=1;i<sizeof(dataSaveTime)/sizeof(dataSaveTime[0]);i++){
				mini=(mini<dataSaveTime[i]?mini:dataSaveTime[i]);
		}
		return mini;
}
//��ȡ��ɾ�����ڵ�data.txt�ļ�
//�����ļ�������  �� huanLiu juFang
void ReadAndDelEarlyDataTxt(char *dataDirName,char *ID)
{
		if(gbSDExit==false){
			return;
		}
		rt_mutex_take(sdWrite_mutex,RT_WAITING_FOREVER);
		FILINFO fileinfo;
		DIR Dir;
	  char dirName[30]="0:/";
	  strcat(dirName,dataDirName);
	  int txtCount=0;
	  char delPath[30]="";
	  strcat(delPath,dirName);
	  strcat(delPath,"/");
	  strcat(delPath,ID);
	  strcat(delPath,"/");
		
		strcat(dirName,"/");
	  strcat(dirName,ID);

		for(int i=0;i<TXT_DATA_NUM;i++){
				dataSaveTime[i]=0;
		}
    if(f_opendir(&Dir,(const TCHAR*)dirName) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir�ṹ���б��� */
    {
        while(f_readdir(&Dir, &fileinfo) == FR_OK)  /* ���ļ���Ϣ���ļ�״̬�ṹ���� */
        {
            if(!fileinfo.fname[0]) break; /* ����ļ���Ϊ��\0'��˵����ȡ��ɽ��� */
						for(int j=0;j<strlen(fileinfo.fname);j++){
							 if(fileinfo.fname[j]=='.'){
									dataTime[j]=0;
								  break;
							 }
							 dataTime[j]=fileinfo.fname[j];//��ȡ���� ��Ҫ.txt
						}
						//printf("��ȡʱ�� %s\n",logTime);
						if(txtCount<=TXT_DATA_NUM+1){
								dataSaveTime[txtCount]=atoi32(dataTime,10);//ʱ���ַ�����ʽ��Ϊ32λ����		
						}
						txtCount++;
        }
    }
		if(txtCount>TXT_DATA_NUM){//�ϴζ�ȡ��		
				for(int i=0;i<=TXT_DATA_NUM;i++){
					rt_kprintf("data[%d] time:%d\n",i+1,dataSaveTime[i]);
				}			
				rt_kprintf("find mini:[%d]\n",findDataMiniTime());
				sprintf(delPath+strlen(delPath),"%d.txt",findDataMiniTime());
				rt_kprintf("count[%d]data del:%s\r\n",txtCount,delPath );
//				if(f_opendir(&Dir,(const TCHAR*)dirName) == FR_OK)/* ���ļ���Ŀ¼�ɹ���Ŀ¼��Ϣ�Ѿ���dir[0]=�ṹ���б��� */
//				{
						int ret=f_unlink(delPath);
					  if(ret!=FR_OK){
								rt_kprintf("del err:[%d]\n",ret);
						}
//				}
		}
		f_closedir(&Dir);
		rt_mutex_release(sdWrite_mutex);
}

//����ʱ��ɾ��logtxt�ļ��ӿ��������ʱ���е���

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
