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
#define  TXT_DATA_TIME    (3600*24)    //ÿ��3600*24�봴��һ���µ�Ŀ¼��  ��ʽʹ�ø�Ϊ 86400
#define  TXT_LOG_TIME  		(3600*24)    
#define  TXT_LOG_NUM   		30   //���洢10Tina
#define  TXT_DATA_NUM  		30   //���洢30�������
#endif

extern void ReadAndDelEarlyDataTxt(char *dataDirName,char *ID);
/*****************************END OF FILE**************************/
