#include   "NetJsonDataTask.h"
#include   "board.h"

const static char task[]="[dataSend]";

struct rt_mailbox mbNetSendData;

extern uint16_t netSend(uint8_t *data,int len);
extern rt_bool_t gbNetState;
uint16_t netDataSendCheck(uint8_t *str)
{
	  int lenth=0;
		if((str[0]==(uint8_t)((uint16_t)HEAD>>8))&&(str[1]==(uint8_t)(HEAD))){
			  lenth= (str[2]<<8)+str[3]+HEAD_LEN+LENTH_LEN+CRC_LEN+TAIL_LEN;  
					rt_kprintf("%slenth ：%d str0:%x str1:%x str[2+3]:%d\r\n",task,lenth,str[0],str[1],(str[2]<<8)+str[3]);
		}
		else{
				 rt_kprintf("%ssend head  err\r\n",task);
		}
		return lenth;
}
//网络数据的发送

#if 1
void   netDataSendTask(void *para)
{
		uint8_t *str=RT_NULL;
		while(1){
			  if (rt_mb_recv(&mbNetSendData, (rt_ubase_t *)&str, 1000) == RT_EOK)
        { 
						if(!USE_MQTT){
										int lenth = netDataSendCheck(str);
										if((lenth!=0)&&(gbNetState ==RT_TRUE)){
											
												netSend(str,lenth);
				//								for(int i=0;i<lenth;i++)
				//										rt_kprintf("%c",str[i]);
				//								rt_kprintf("\n");

										}
										else
											rt_kprintf("%sERR:net offline drop data\r\n",task);
									}
						else{
							uint32_t lenth = (str[0]<<24)+(str[1]<<16)+(str[2]<<8)+str[3];
							if((lenth!=0)&&(gbNetState ==RT_TRUE)){
								netSend(str+PACK_HEAD_LEN,lenth);//头部不发送
							}
							else
								rt_kprintf("%sERR:net offline drop data\r\n",task);		
					}
				}
		
#ifdef  USE_WDT
			rt_event_send(&WDTEvent,EVENT_WDT_SENDTASK);
#endif
			}
}

#else

#endif

