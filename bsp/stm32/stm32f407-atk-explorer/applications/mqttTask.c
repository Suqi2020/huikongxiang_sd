
#include "board.h"
#include "mqttTask.h"
#include "transport.h"

/*
{"clientId":"a1S9kgxkc8w.humi_protect1|securemode=2,signmethod=hmacsha256,timestamp=1656814731352|",
 "username":"humi_protect1&a1S9kgxkc8w",
 "mqttHostUrl":"a1S9kgxkc8w.iot-as-mqtt.cn-shanghai.aliyuncs.com",
 "passwd":"652b0dc2b786a08a7ab11612246731f87f50e32b78cc6bf9ed3a3a5041c7635e",
 "port":1883}
*/
static char task[]="[mqttTask]";
extern rt_bool_t gbNetState;
void  mqttTask(void *parameter)
{

			while(1){
				  if((gbNetState==RT_TRUE)&&(USE_MQTT))
						  mqttLoopData();
					else{
						 rt_thread_delay(1000);
					}
#ifdef  USE_WDT
					rt_event_send(&WDTEvent,EVENT_WDT_MQTTTASK);
#endif
					
			}
}






