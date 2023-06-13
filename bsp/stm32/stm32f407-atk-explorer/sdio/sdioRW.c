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
//经测试 速度达到 3538992字节/279秒=12.38KB/秒
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


char SDPath[4]; /* SD逻辑驱动器路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
extern Diskio_drvTypeDef  SD_Driver;

extern rt_bool_t gbSDExit;
//extern uint64_t utcTime_ms();


//后期需要用时间戳转换函数来实现
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
//创建2级别目录ID目录
void creatIDFolder(int i)
{
	  
		char *txtName;
//	  uint32_t fnum;
//	  int ret;
		txtName =rt_malloc(50);
		strcpy(txtName,logName);
		f_mkdir(txtName);//创建目录
		memset(txtName,0,sizeof((char *)txtName));
		switch(i)
		{
			case CIRCULA:
				for(int j=0;j<CIRCULA_485_NUM;j++){//核对有没有配置过
						if(sheet.cirCula[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.cirCula[j].ID);
							f_mkdir(txtName);//创建目录
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case PARTDISCHAG:
				for(int j=0;j<PARTDISCHAG_485_NUM;j++){//核对有没有配置过
						if(sheet.partDischag[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.partDischag[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_NEW);
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case PRESSSETTL:
				for(int j=0;j<PRESSSETTL_485_NUM;j++){//核对有没有配置过
						if(sheet.pressSetl[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.pressSetl[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case THREEAXIS:
				for(int j=0;j<THREEAXIS_485_NUM;j++){//核对有没有配置过
						if(sheet.threeAxiss[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.threeAxiss[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
#ifdef 	 USE_4GAS 	
			case CH4:
					for(int j=0;j<CH4_485_NUM;j++){//核对有没有配置过
							if(sheet.ch4[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.ch4[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
			case O2:
					for(int j=0;j<O2_485_NUM;j++){//核对有没有配置过
							if(sheet.o2[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.o2[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
			case H2S:
					for(int j=0;j<H2S_485_NUM;j++){//核对有没有配置过
							if(sheet.h2s[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.h2s[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
			case CO:
					for(int j=0;j<CO_485_NUM;j++){//核对有没有配置过
							if(sheet.co[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.co[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
							}
					}
			break;
#endif
			case TEMPHUM:
				for(int j=0;j<TEMPHUM_485_NUM;j++){//核对有没有配置过
						if(sheet.tempHum[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.tempHum[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case WATERDEPTH:
				for(int j=0;j<WATERDEPTH_485_NUM;j++){//核对有没有配置过
						if(sheet.waterDepth[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.waterDepth[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case CRACKMETER:
				for(int j=0;j<CRACKMETER_485_NUM;j++){//核对有没有配置过
						if(sheet.crackMeter[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.crackMeter[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
							memset(txtName,0,sizeof((char *)txtName));
						}
				}
			break;
			case COVER:
				for(int j=0;j<COVER_485_NUM;j++){//核对有没有配置过
						if(sheet.cover[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.cover[j].ID);
							strcat(txtName,".txt");
							f_open(&fnew,txtName,FA_CREATE_ALWAYS);
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

//上电后创建jinggai yangqi huanliu等传感器命令的文件夹 一级目录
//bool fountFlag=false;
void creatFolder()
{
	 volatile int ret;
	    //链接驱动器，创建盘符Nm
   FATFS_LinkDriver(&SD_Driver, SDPath);
   //在外部SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
   res_sd = f_mount(&fs,"0:",1);  
   if(res_sd==FR_OK){
//		  fountFlag=true;
		  for(int j=0;j<MODBUS_NUM;j++){
					ret=f_mkdir(modbusName[j]);//创建目录
				  if((ret==FR_OK)||(ret==FR_EXIST)){
						  creatIDFolder(j);
					}
					else
							rt_kprintf("%sERR:mkdir ret=%d\n",sign,ret);
			}
			f_close(&fnew);
	 }
    /* 不再使用文件系统，取消挂载文件系统 */
		if(gbSDExit==false){
				rt_kprintf("%s请插入TF卡并重启设备\n",sign);
				return;
		}
}




//1685592000  2023 06 01 4:00:00

//log 存入sd卡
//extern bool log_save_sdFlag;
//static char *logName="log.txt";


//按照时间tick存储log到txt文件中
void  logSaveToSD(char *buf,char lenth)
{
		if(gbSDExit==false){
				return;
		 }
	  extern rt_mutex_t sdWrite_mutex;
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
		sprintf(timeSign,"%llu",(utcTime_s()/TXT_LOG_TIME)*TXT_LOG_TIME);//1小时创建一个新目录 为了清除1小时内的数字为0
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
		}
		if((ret==FR_OK)||(ret==FR_EXIST)){
				f_lseek(&fnew,f_size(&fnew));
				if(lenth>1){//长度比较长才写入时间
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




//删除早期的txt文件
void FatReadDirDelEarlyTxt()
{
		if(gbSDExit==false){
				return;
		 }

		FILINFO fileinfo;
		DIR Dir;
	  char dirName[10]="0:/";
	  strcat(dirName,logName);
	  int txtCount=0;
	  char delPath[30]="";
	  strcat(delPath,dirName);
	  strcat(delPath,"/");
    if(f_opendir(&Dir,(const TCHAR*)dirName) == FR_OK)/* 打开文件夹目录成功，目录信息已经在dir结构体中保存 */
    {
        while(f_readdir(&Dir, &fileinfo) == FR_OK)  /* 读文件信息到文件状态结构体中 */
        {
						if(txtCount==0){
								strcat(delPath,fileinfo.fname );//每次提取第一个txtname
						}
            if(!fileinfo.fname[0]) break; /* 如果文件名为‘\0'，说明读取完成结束 */
		        txtCount++;
        }
    }
		if(txtCount>TXT_LOG_NUM){//上次读取后				
				printf("count[%d]del：%s\r\n",txtCount,delPath );
				if(f_opendir(&Dir,(const TCHAR*)dirName) == FR_OK)/* 打开文件夹目录成功，目录信息已经在dir结构体中保存 */
				{
						f_unlink(delPath);
				}
		}
}


//按照时间删除logtxt文件接口在软件定时器中调用

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
