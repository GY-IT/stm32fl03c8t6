#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"                  // Device header

extern uint16_t ADC_Buffer[100];

void AD_Init(void);
uint16_t AD_GetValue(void);

#endif
