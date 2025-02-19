#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Delay.h"
#include "stdio.h"

int main(void)
{
	uint8_t KeyNum=0;
	LED1_Init();
	LED2_Init();
	Key_Init();
	OLED_Init();
	OLED_Clear();
	OLED_ShowBMP(1,1);
	OLED_ShowSignedNum(1, 10, 20, 2);
	while(1)
	{

		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			LED1_Turn();
		}
		if(KeyNum == 2)
		{
			LED2_Turn();
		}
		
//	LED1_ON();
//	LED2_ON();
//	Delay_ms(1000);
//	LED1_OFF();
//	LED2_OFF();
//	LED1_Turn();
//	LED2_Turn();	
	}	
}

