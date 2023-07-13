#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H
#include "board.h"
#include "ff_gen_drv.h"
void SD_Test(void);




extern char SDPath[4]; /* SD逻辑驱动器路径 */
extern FATFS fs;													/* FatFs文件系统对象 */
extern  FIL fnew;													/* 文件对象 */
extern FRESULT res_sd;                /* 文件操作结果 */


extern rt_bool_t gbSDExit;
#define  TXT_DATA_TIME    (3600*24)    //每隔3600*24秒创建一个新的目录名  正式使用改为 86400
#define  TXT_LOG_TIME  		(3600*24)    
#define  TXT_LOG_NUM   		30   //最多存储10Tina
#define  TXT_DATA_NUM  		30   //最多存储30天的数据
#endif

extern void ReadAndDelEarlyDataTxt(char *dataDirName,char *ID);
/*****************************END OF FILE**************************/
