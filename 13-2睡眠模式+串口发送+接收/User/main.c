#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "Delay.h"
#include "Usart.h"
#include "stdio.h"

uint8_t RxData;

int main(void)
{

	OLED_Init();
	OLED_Clear();
	Serial_Init();
	
	while(1)
	{
		if(Serial_GetRxFlag()==1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1, 8, RxData, 2);
		}
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		__WFI();				//Æô¶¯Ë¯ÃßÄ£Ê½
	}	
}

