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


extern RingBuff_t ringBuff;


void RingBuff_Init(void);
uint8_t Write_RingBuff(uint8_t data);
uint8_t Read_RingBuff(uint8_t *rData);












#endif




