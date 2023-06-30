#ifndef __STMFLASH_H
#define __STMFLASH_H
#include "uartReconf.h"
#include "board.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//STM32内部FLASH读写 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//FLASH起始地址
/*
扇区 0 0x0800 0000 – 0x0800 03FF 16KB
扇区 1 0x0800 4000 – 0x0800 7FFF 16KB
扇区 2 0x0800 8000 – 0x0800 BFFF 16KB
扇区 3 0x0800 C000 – 0x0800 FFFF 16KB
扇区 4 0x0801 0000 – 0x0801 FFFF 64KB        
扇区 5 0x0802 0000 – 0x0803 FFFF 128KB
扇区 6 0x0804 0000 – 0x0805 FFFF 128KB
…… …… ……
扇区 11 0x080E 0000 – 0x080F FFFF 128KB
bootloader 占用扇区0-扇区3   64k
扇区4   存放用户数据   64k
*/
//#define  packFlash_LEN             2048
//#define  ADDR_FLASH_SECTOR_11       ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */
//#define  FLASH_IP_SAVE_ADDR    			 ADDR_FLASH_SECTOR_11  //最后一个扇区 128K字节 (STM32_FLASH_BASE+STM_FLASH_SIZE-USE_FLASH_SIZE)  
//#define  FLASH_MODBUS_SAVE_ADDR     (FLASH_IP_SAVE_ADDR+packFlash_LEN)  //偏移10个1K

#define  packFlash_LEN             2048
#define  ADDR_FLASH_SECTOR_4       ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define  FLASH_IP_SAVE_ADDR    			 ADDR_FLASH_SECTOR_4  //  
#define  FLASH_MODBUS_SAVE_ADDR     (FLASH_IP_SAVE_ADDR+packFlash_LEN)  //偏移10个1K








//uint32_t STMFLASH_ReadWord(uint32_t faddr);		  	//读出字  
//void STMFLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite);		//从指定地址开始写入指定长度的数据
//void STMFLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead);   		//从指定地址开始读出指定长度的数据
////测试写入
//void Test_Write(uint32_t WriteAddr,uint32_t WriteData);	
#endif
