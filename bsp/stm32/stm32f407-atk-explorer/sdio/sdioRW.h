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


#endif


/*****************************END OF FILE**************************/
