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
	
	OLED_ShowChar(1,1,0x41);
	
	Serial_SendByte(0x41);
	
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(MyArray, 4);

	
	Serial_SendString("HelloWorld!\n\r");

	Serial_SendNumber(12345, 5);
	
	printf("Num=%d\r\n", 666);
	
	char String[100];
	sprintf(String, "Num=%d\r\n", 666);
	Serial_SendString(String);
	
	Serial_Printf("Num=%d\r\n", 666);
	
	Serial_Printf("你好 \r\n");
	while(1)
	{
		//串口发送可以为字符串，接收字符串有乱码
		if(Serial_GetRxFlag()==1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1, 1, RxData, 2);
		}
	}	
}

