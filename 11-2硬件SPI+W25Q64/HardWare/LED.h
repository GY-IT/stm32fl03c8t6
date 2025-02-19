#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header


#define LED1_GPIO_RCC		RCC_APB2Periph_GPIOA
#define LED1_GPIO 			GPIOA
#define LED1_GPIO_Pin		GPIO_Pin_0
#define LED2_GPIO_RCC		RCC_APB2Periph_GPIOA
#define LED2_GPIO 			GPIOA
#define LED2_GPIO_Pin		GPIO_Pin_1

void LED1_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED1_Turn(void);
void LED2_Init(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED2_Turn(void);

#endif
