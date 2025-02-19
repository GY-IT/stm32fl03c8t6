#include "Key.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(Key_GPIO_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = Key1_GPIO_Pin | Key2_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Key_GPIO, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(Key_GPIO, Key1_GPIO_Pin) == 0)
	{
		while (GPIO_ReadInputDataBit(Key_GPIO, Key1_GPIO_Pin) == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(Key_GPIO, Key2_GPIO_Pin) == 0)
	{
		while (GPIO_ReadInputDataBit(Key_GPIO, Key2_GPIO_Pin) == 0);
		Delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}
