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
#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            5   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/
static const char sign[]="[SD]";


char SDPath[4]; /* SD逻辑驱动器路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
extern Diskio_drvTypeDef  SD_Driver;

extern rt_bool_t gbSDExit;
//在 creatFolder创建的文件夹下边创建带有ID号命令的txt文件
//如 GYNJLXSD000000562.txt
const char huanliuText[]="earthCurA  runCurA  loadRatioA  earthCurB  runCurB  loadRatioB  earthCurC  runCurC  loadRatioC  time\r\n";
#define HUANLIU_DATA_LEN 200
void huanLiuTxtReadSD(char *id)
{
		char *txtName;
	  char *readData;
		uint32_t fnum;
    int ret;
		if(gbSDExit==false){
				return;
		 }
		txtName =rt_malloc(50);
		readData=rt_malloc(HUANLIU_DATA_LEN);
		strcpy(txtName,modbusName[CIRCULA]);
		strcat(txtName,"/");
		strcat(txtName,id);
		strcat(txtName,".txt");
		ret=f_open(&fnew,txtName, FA_OPEN_EXISTING|FA_READ);//suqi
		int realLen=0;
		if(ret==FR_OK){
			  //printf("%ssizeof(fnew):%d\r\n",sign,fnew.fsize);
				memset(readData,0,HUANLIU_DATA_LEN);
				while(realLen<f_size(&fnew))
				{
					f_gets(readData,HUANLIU_DATA_LEN,&fnew);
					printf("%sread:%s\r\n",sign,readData);
				
					realLen=f_tell(&fnew);
					printf("%sreallen:%d\r\n",sign,realLen);
				
					f_lseek(&fnew, realLen);  
					memset(readData,0,HUANLIU_DATA_LEN);
				}

		}
		else{
				rt_kprintf("%sERR:f_open read %s,ret=%d\n",sign,txtName,ret);
		}
		f_close(&fnew);
		rt_free(txtName);
		txtName=NULL;
		rt_free(readData);
		readData=NULL;
		static int testtime=0;
		testtime++;
		while(testtime==3){
			rt_thread_delay(2000);
			rt_kprintf("%sRead end\n",sign);
		}
}
uint32_t writetime=0;
void huanLiuTxtSaveSD(char *id,char *data)
{
		char *txtName;
		uint32_t fnum;
    int ret;
		if(gbSDExit==false){
				return;
		 }
		txtName =rt_malloc(50);
		strcpy(txtName,modbusName[CIRCULA]);
		strcat(txtName,"/");
		strcat(txtName,id);
		strcat(txtName,".txt");
		ret=f_open(&fnew,txtName, FA_WRITE);//suqi
		if(ret==FR_OK){
			f_lseek(&fnew,f_size(&fnew));
			res_sd=f_write(&fnew,data,strlen(data),&fnum);
			if(res_sd!=FR_OK){
				//while(1)
					{

				rt_kprintf("%sERR:f_write %s,res_sd=%d\n",sign,txtName,res_sd);
					rt_thread_delay(3000);
				}
			}
			else {
				writetime++;
				rt_kprintf("%sOK:f_write %s total times[%d] realy times[%d]\n",sign,txtName,writetime,(((writetime-1)/3)+1));
				rt_kprintf("%s %s\n",sign,txtName);
			}
		}
		else{
			//while(1)
			{

			rt_kprintf("%sERR:f_open  write %s,ret=%d\n",sign,txtName,ret);
					rt_thread_delay(3000);
			}
		}
		f_close(&fnew);
		rt_free(txtName);
		txtName=NULL;
}
void creatIDtxt(int i)
{
		char *txtName;
	  uint32_t fnum;
	  int ret;
		txtName =rt_malloc(50);
		switch(i)
		{
			case CIRCULA:
				for(int j=0;j<CIRCULA_485_NUM;j++){//核对有没有配置过
						if(sheet.cirCula[j].workFlag==RT_TRUE){
							strcpy(txtName,modbusName[i]);
							strcat(txtName,"/");
							strcat(txtName,sheet.cirCula[j].ID);
							strcat(txtName,".txt");
							ret=f_open(&fnew,txtName,FA_CREATE_NEW | FA_WRITE);
							if(!((ret==0)||(ret==8))){
									rt_kprintf("%sERR:creat txtname %d\n",sign,ret);
							}
							res_sd=f_write(&fnew,huanliuText,strlen(huanliuText),&fnum);
							f_lseek(&fnew,f_size(&fnew));
							f_close(&fnew);
//												cJSON_AddItemToObject(nodeobj_p,"earthCurA",cJSON_CreateString(sprinBuf));
//					cJSON_AddItemToObject(nodeobj_p,"runCurA",cJSON_CreateString(""));
//					cJSON_AddItemToObject(nodeobj_p,"loadRatioA",cJSON_CreateString(""));
//					
//					sprintf(sprinBuf,"%02f",cirCurStru_p[i].circlCurB);
//					cJSON_AddItemToObject(nodeobj_p,"earthCurB",cJSON_CreateString(sprinBuf));
//					cJSON_AddItemToObject(nodeobj_p,"runCurB",cJSON_CreateString(""));
//					cJSON_AddItemToObject(nodeobj_p,"loadRatioB",cJSON_CreateString(""));
//					
//					sprintf(sprinBuf,"%02f",cirCurStru_p[i].circlCurC);
//					cJSON_AddItemToObject(nodeobj_p,"earthCurC",cJSON_CreateString(sprinBuf));
//					cJSON_AddItemToObject(nodeobj_p,"runCurC",cJSON_CreateString(""));
//					cJSON_AddItemToObject(nodeobj_p,"loadRatioC",cJSON_CreateString(""));
//					sprintf(sprinBuf,"%llu",utcTime());
//					cJSON_AddItemToObject(nodeobj_p,"monitoringTime",cJSON_CreateString(sprinBuf));
					
					
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
			break;
			case O2:
			break;
			case H2S:
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

//上电后创建jinggai yangqi huanliu等传感器命令的文件夹
void creatFolder()
{
	 volatile int ret;
	    //链接驱动器，创建盘符Nm
   FATFS_LinkDriver(&SD_Driver, SDPath);
   //在外部SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
   res_sd = f_mount(&fs,"0:",1);  
   if(res_sd==FR_OK){
			//f_close(&fnew);
		  for(int j=0;j<MODBUS_NUM;j++){
					ret=f_mkdir(modbusName[j]);//创建目录
				  if((ret==FR_OK)||(ret==FR_EXIST))
							creatIDtxt(j);
					else
					//f_open(&fnew, "FatFs读写测试文件1.txt",FA_CREATE_ALWAYS);//创建文档
					rt_kprintf("%sERR:mkdir ret=%d\n",sign,ret);
			}
			f_close(&fnew);
	 }
    /* 不再使用文件系统，取消挂载文件系统 */
    //f_mount(NULL,"0:",1);
		if(gbSDExit==false){
				rt_kprintf("%s请插入TF卡并重启设备\n",sign);
				return;
		 }
}
/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
