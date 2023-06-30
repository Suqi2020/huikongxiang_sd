
#include "ringbuf.h"


#include "board.h"


RingBuff_t  ringBuff   CCMRAM;
RingBuff2_t ringBuff2  CCMRAM;



void RingBuff_Init(void)
{

		ringBuff.Head = 0;
		ringBuff.Tail = 0;
		ringBuff.Lenght = 0;
		memset(ringBuff.Ring_Buff, 0, LCD_BUF_LEN);
}


uint32_t write_ringbufnum=0;
uint32_t read_ringbufnum=0;
uint8_t Write_RingBuff(uint8_t data)
{

		if ((ringBuff.Head - ringBuff.Tail == 1)\
		||((ringBuff.Head == 0)&&(ringBuff.Tail == (LCD_BUF_LEN-1))))
		{
				return false;
		}

		write_ringbufnum++;
		ringBuff.Ring_Buff[ringBuff.Tail] = data;
		ringBuff.Tail = (ringBuff.Tail + 1) % LCD_BUF_LEN;//
		ringBuff.Lenght++;
		return true;
		}



uint8_t Read_RingBuff(uint8_t *rData)
{
		if (ringBuff.Head== ringBuff.Tail)//
		{
				return false;
		}
		read_ringbufnum++;
		*rData = ringBuff.Ring_Buff[ringBuff.Head];//
		ringBuff.Head++;
		if(ringBuff.Head>=LCD_BUF_LEN)
		{
				ringBuff.Head=0;
		}
		ringBuff.Lenght--;
		return true;

}




void RingBuff2_Init(void)
{

		ringBuff2.Head = 0;
		ringBuff2.Tail = 0;
		ringBuff2.Lenght = 0;
		memset(ringBuff2.Ring_Buff, 0, PRINTF_BUF_LEN);
}



uint8_t Write_RingBuff2(uint8_t data)
{

		if ((ringBuff2.Head - ringBuff2.Tail == 1)\
		||((ringBuff2.Head == 0)&&(ringBuff2.Tail == (PRINTF_BUF_LEN-1))))
		{
				return false;
		}

		write_ringbufnum++;
		ringBuff2.Ring_Buff[ringBuff2.Tail] = data;
	//	printf(">%c",ringBuff2.Ring_Buff[ringBuff2.Tail]);
		ringBuff2.Tail = (ringBuff2.Tail + 1) % PRINTF_BUF_LEN;//
		ringBuff2.Lenght++;
		return true;
		}


//
//		bool readFlag=false;
uint8_t Read_RingBuff2(uint8_t *rData)
{
		if (ringBuff2.Head== ringBuff2.Tail)//
		{
				return false;
		}
		read_ringbufnum++;
		*rData = ringBuff2.Ring_Buff[ringBuff2.Head];//
//		if(readFlag==false){
//			readFlag =true;
//			printf("ringbuf %d %d\n",write_ringbufnum,read_ringbufnum);
//		}
			
//		printf("/%c",ringBuff2.Ring_Buff[ringBuff2.Head]);
		ringBuff2.Head++;
		if(ringBuff2.Head>=PRINTF_BUF_LEN)
		{
				ringBuff2.Head=0;
		}
		ringBuff2.Lenght--;
		return true;

}
