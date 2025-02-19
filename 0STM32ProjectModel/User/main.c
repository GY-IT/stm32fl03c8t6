#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Delay.h"
#include "Usart.h"
#include "stdio.h"

uint8_t KeyNum;

int main(void)
{
	LED1_Init();
	LED2_Init();
	Key_Init();
	OLED_Init();
	USART_Config();
	printf("123");
	
	while(1)
	{

		
	}
}
