#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H
#include "board.h"
#include "ff_gen_drv.h"
void SD_Test(void);




extern char SDPath[4]; /* SD�߼�������·�� */
extern FATFS fs;													/* FatFs�ļ�ϵͳ���� */
extern  FIL fnew;													/* �ļ����� */
extern FRESULT res_sd;                /* �ļ�������� */


extern rt_bool_t gbSDExit;


#endif


/*****************************END OF FILE**************************/
