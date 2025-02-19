#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "OLED.h"
#include <string.h>
#include "ESP8266.h"
#include <stdio.h>
#include <string.h>


int main(void)
{
	
	LED1_Init();
	OLED_Init();
	OLED_Clear();
	
	ESP8266_ModeInit();
	
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	

	while(1)
	{
		
		ESP8266_SendData("123", 3);
		Delay_ms(1000);
		if (Serial_RxFlag == 1)
		{
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, Serial_RxPacket);
			
			if(strcmp(Serial_RxPacket, "LED1_ON") == 0)
			{
				LED1_ON();
				Serial_SendString("LED1_ON OK\r\n");	
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED1_ON_OK");
			}
			else if(strcmp(Serial_RxPacket, "LED1_OFF") == 0)
			{
				LED1_OFF();
				Serial_SendString("LED1_OFF OK\r\n");		
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED1_OFF_OK");
			}
			else
			{
				LED1_ON();
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND");
			}
			Serial_RxFlag = 0;
		}
	}	
}

