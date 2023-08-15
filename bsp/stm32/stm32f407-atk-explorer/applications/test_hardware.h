#ifndef  __TEST_HARDWARE_H__
#define  __TEST_HARDWARE_H__
#include  "board.h"




typedef struct{
	 uint32_t times;
	 uint32_t relayTimer[100];
}offLineStru;

extern uint32_t offLTimes;
extern uint32_t onLTimes;
extern offLineStru offLine;
void adcGetTest();
void ioInOutTest();
void ioOutTest();
void uartTest();
void  relayTest();


































#endif

