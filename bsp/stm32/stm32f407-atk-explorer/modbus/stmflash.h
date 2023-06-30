#ifndef __STMFLASH_H
#define __STMFLASH_H
#include "uartReconf.h"
#include "board.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//STM32�ڲ�FLASH��д ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//FLASH��ʼ��ַ
/*
���� 0 0x0800 0000 �C 0x0800 03FF 16KB
���� 1 0x0800 4000 �C 0x0800 7FFF 16KB
���� 2 0x0800 8000 �C 0x0800 BFFF 16KB
���� 3 0x0800 C000 �C 0x0800 FFFF 16KB
���� 4 0x0801 0000 �C 0x0801 FFFF 64KB        
���� 5 0x0802 0000 �C 0x0803 FFFF 128KB
���� 6 0x0804 0000 �C 0x0805 FFFF 128KB
���� ���� ����
���� 11 0x080E 0000 �C 0x080F FFFF 128KB
bootloader ռ������0-����3   64k
����4   ����û�����   64k
*/
//#define  packFlash_LEN             2048
//#define  ADDR_FLASH_SECTOR_11       ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */
//#define  FLASH_IP_SAVE_ADDR    			 ADDR_FLASH_SECTOR_11  //���һ������ 128K�ֽ� (STM32_FLASH_BASE+STM_FLASH_SIZE-USE_FLASH_SIZE)  
//#define  FLASH_MODBUS_SAVE_ADDR     (FLASH_IP_SAVE_ADDR+packFlash_LEN)  //ƫ��10��1K

#define  packFlash_LEN             2048
#define  ADDR_FLASH_SECTOR_4       ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define  FLASH_IP_SAVE_ADDR    			 ADDR_FLASH_SECTOR_4  //  
#define  FLASH_MODBUS_SAVE_ADDR     (FLASH_IP_SAVE_ADDR+packFlash_LEN)  //ƫ��10��1K








//uint32_t STMFLASH_ReadWord(uint32_t faddr);		  	//������  
//void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
//void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
////����д��
//void Test_Write(uint32_t WriteAddr,uint32_t WriteData);	
#endif
