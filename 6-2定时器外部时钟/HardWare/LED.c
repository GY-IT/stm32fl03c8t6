#include "LED.h"                  // Device header

void LED1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(LED1_GPIO, LED1_GPIO_Pin);
}
void LED2_Init(void)
{
	RCC_APB2PeriphClockCmd(LED2_GPIO_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED2_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(LED2_GPIO,LED2_GPIO_Pin);
}
void LED1_ON(void)
{
	GPIO_ResetBits(LED1_GPIO, LED1_GPIO_Pin);
}

void LED1_OFF(void)
{
	GPIO_SetBits(LED1_GPIO, LED1_GPIO_Pin);
}

void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED1_GPIO, LED1_GPIO_Pin) == 0)
	{
		GPIO_SetBits(LED1_GPIO, LED1_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(LED1_GPIO, LED1_GPIO_Pin);
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(LED2_GPIO, LED2_GPIO_Pin);
}

void LED2_OFF(void)
{
	GPIO_SetBits(LED2_GPIO, LED2_GPIO_Pin);
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(LED2_GPIO, LED2_GPIO_Pin) == 0)
	{
		GPIO_SetBits(LED2_GPIO, LED2_GPIO_Pin);
	}
	else
	{
		GPIO_ResetBits(LED2_GPIO, LED2_GPIO_Pin);
	}
}
