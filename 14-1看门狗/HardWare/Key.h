#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define Key_GPIO_RCC		RCC_APB2Periph_GPIOB
#define Key_GPIO 			GPIOB
#define Key1_GPIO_Pin		GPIO_Pin_1
#define Key2_GPIO_Pin		GPIO_Pin_11

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
