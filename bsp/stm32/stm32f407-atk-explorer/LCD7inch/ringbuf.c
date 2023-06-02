
#include "ringbuf.h"


#include "board.h"


RingBuff_t ringBuff;




void RingBuff_Init(void)

{

 ringBuff.Head = 0;

 ringBuff.Tail = 0;

 ringBuff.Lenght = 0;

 memset(ringBuff.Ring_Buff, 0, LCD_BUF_LEN);

//printf("ringbuff init\r\n");

}


uint32_t write_ringbufnum=0;

uint32_t read_ringbufnum=0;

uint8_t Write_RingBuff(uint8_t data)

{

 //if (ringBuff.Lenght >= LENTH) //

 if ((ringBuff.Head - ringBuff.Tail == 1)\

||((ringBuff.Head == 0)&&(ringBuff.Tail == (LCD_BUF_LEN-1))))

 {

//printf("too long %d %d\r\n",ringBuff.Tail,ringBuff.Head);

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
