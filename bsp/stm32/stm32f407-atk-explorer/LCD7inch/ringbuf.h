#ifndef __RINGBUFF_H__
#define __RINGBUFF_H__
#include "stdbool.h"
#include "stdint.h"
#include "board.h"
#include "7inchRegist.h"
typedef struct
{
		uint16_t Head;
		uint16_t Tail;
		uint16_t Lenght;
		uint8_t Ring_Buff[LCD_BUF_LEN];
}RingBuff_t;
#define   PRINTF_BUF_LEN    4096  //此次比较奇怪 用1024*6会导致ringbuf异常  suqi
typedef struct
{
		uint16_t Head;
		uint16_t Tail;
		uint16_t Lenght;
		uint8_t Ring_Buff[PRINTF_BUF_LEN];
}RingBuff2_t;

//extern RingBuff_t ringBuff;


void RingBuff_Init(void);
uint8_t Write_RingBuff(uint8_t data);
uint8_t Read_RingBuff(uint8_t *rData);


void RingBuff2_Init(void);
uint8_t Write_RingBuff2(uint8_t data);
uint8_t Read_RingBuff2(uint8_t *rData);










#endif




