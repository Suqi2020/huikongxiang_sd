
#include   "board.h"
//5A A5 0B 82 009C 5AA5 17 03 1E 0F 2A 00 
//时间配置      17 03 1E 0F 2A 00  为年月日时分秒配置
#if   USE_RINGBUF

#else
extern struct  rt_messagequeue LCDmque;
#endif
extern void LCDDispIP(void);
extern void LCDDispUart(void);
extern void LCDDispMCUID(void);
extern void LCDDispModbusGet(void);
extern void LDCDispMosbusInfo(void);
extern void LCDDispConfig(uint8_t *recBuf,int len);
extern void firstNameDispInit(void);
extern void LCDDispNetOffline(void);

bool lcdHome=false;
uint32_t lcdRecTime=0;//接收到屏幕数据清0  无操作2分钟内返回主界面
void changeHome()
{
		if(lcdHome){
			lcdRecTime++;
			if(lcdRecTime>2){
					lcdHome=false;
					extern void changeBmp(int num);
					changeBmp(0);
			}
		}
}
uint8_t lcdRecBuf[LCD_BUF_LEN];
uint8_t  lcdRecLen;
void  LCDTask(void *parameter)
{
	  extern void LCDDispErrMosbusState();
	  extern void LCDDispErrModbusGet();
		extern void LDCDispErrMosbusInfo();
#if   USE_RINGBUF
    RingBuff_Init();
#else
		 
#endif
		
	  LCDDispIP();
	  LCDDispUart();
	  LCDDispMCUID();//LCD send:5a a5 15 82 12 10 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 
	  LCDDispModbusGet();
  	LDCDispMosbusInfo();

	  int dispCount=0;
	
    extern void LCDDispRstOK();
		LCDDispRstOK();
		while(1){
			
			
#if   USE_RINGBUF
			  rt_thread_mdelay(50);
			  while(true== Read_RingBuff(lcdRecBuf+lcdRecLen)){
					  lcdRecTime=0;
					  lcdHome=true;
						rt_thread_mdelay(2);
					  lcdRecLen++;
					
							if(((uint16_t)(lcdRecBuf[0]<<8)+lcdRecBuf[1])==LCD_HEAD){
								 if(lcdRecLen>=3+lcdRecBuf[02])//一包数据收满 跳出
									 break;
							
						}
				}
#else
			
				if(rt_mq_recv(&LCDmque, lcdRecBuf+lcdRecLen, 1, 1000) == RT_EOK){
						lcdRecLen++;
						while(rt_mq_recv(&LCDmque, lcdRecBuf+lcdRecLen, 1, 1000) == RT_EOK){
								lcdRecLen++;
								if(((uint16_t)(lcdRecBuf[0]<<8)+lcdRecBuf[1])==LCD_HEAD){
									 if(lcdRecLen>=3+lcdRecBuf[02])//一包数据收满 跳出
										 rt_kprintf("break %d %d\n",lcdRecLen,lcdRecBuf[02]);
										 break;
								
								}
						}
				}		
#endif

				
				
				
				
				if(lcdRecLen){
					rt_kprintf("lcdRecLen:%d\n",lcdRecLen);
						LCDDispConfig(lcdRecBuf,lcdRecLen);

						for(int j=0;j<lcdRecLen;j++)
						rt_kprintf("%02x ",lcdRecBuf[j]);
						rt_kprintf("\n ");
            //bodyLen=0;
						lcdRecLen=0;
					
				}
				if(++dispCount>=1200){
					  changeHome();
						
						dispCount=0;
						LCDDispNetOffline();
						//LCDDispNetErrState();
						LCDDispErrModbusGet();
						LDCDispErrMosbusInfo();
						LCDDispErrMosbusState();
				}
#ifdef  USE_WDT
				rt_event_send(&WDTEvent,EVENT_WDT_LCDTASK);
#endif
		}
}

