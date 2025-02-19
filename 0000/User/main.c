#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Delay.h"
#include "Usart.h"
#include "stdio.h"

uint8_t RxData;

int main(void)
{
	LED1_Init();
	Serial_Init();
	Serial_SendByte(0x41);
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(MyArray, 4);
	Serial_SendString("HelloWorld!\n\r");
	Serial_SendNumber(12345, 5);
	Serial_Printf("Num=%d\r\n", 666);
	
	while(1)
	{
		
		Serial_SendByte(0x41);
		uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
		Serial_SendArray(MyArray, 4);
		Serial_SendString("HelloWorld!\n\r");
		Serial_SendNumber(12345, 5);
		Serial_Printf("Num=%d\r\n", 666);
		
		
//		if(Serial_GetRxFlag()==1)
//		{
//			RxData = Serial_GetRxData();
//		
//			Serial_SendByte(RxData);
//			if(RxData == '1'){
//				LED1_Turn();
//			}
//		}	
	}	
}

