#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"                  // Device header

extern uint16_t AD_Value[4];



void AD_Init(void);

void AD_GetValue(void);		 //单次软件触发

#endif
