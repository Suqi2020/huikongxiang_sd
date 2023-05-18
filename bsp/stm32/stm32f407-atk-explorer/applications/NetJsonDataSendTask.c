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
				if(lenth<=2048){
				}
				else{
					rt_kprintf("%slenth  err：%d str0:%x str1:%x str[2]:%d  str[3]:%d\r\n",lenth,str[0],str[1],str[2],str[3],task);
						//rt_kprintf("lenth  err %d str[2]:%d  str[3]:%d\r\n",lenth,str[2],str[3]);
				}
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
		while(0){//suqi
			//rt_thread_mdelay(1000000);
			  if (rt_mb_recv(&mbNetSendData, (rt_ubase_t *)&str, 1000) == RT_EOK)
        { 
						int lenth = strlen((char*)str);
					//	if((lenth!=0)&&(gbNetState ==RT_TRUE)){
								netSend(str,lenth);
//						}
//						else
//							rt_kprintf("%sERR:net offline drop data\r\n",task);
				}
#ifdef  USE_WDT
			rt_event_send(&WDTEvent,EVENT_WDT_SENDTASK);
#endif
		}
		while(1){
			  if (rt_mb_recv(&mbNetSendData, (rt_ubase_t *)&str, 1000) == RT_EOK)
        { 
						int lenth = netDataSendCheck(str);
						if((lenth!=0)&&(gbNetState ==RT_TRUE)){
								netSend(str,lenth);
						}
						else
							rt_kprintf("%sERR:net offline drop data\r\n",task);
				}
		
#ifdef  USE_WDT
			rt_event_send(&WDTEvent,EVENT_WDT_SENDTASK);
#endif
			}
}

#else

#endif

