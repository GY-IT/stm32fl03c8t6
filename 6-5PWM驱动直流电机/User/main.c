#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum;
int8_t Speed=0;

int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	Motor_SetSpeed(20);
	OLED_ShowString(1, 1, "Speed:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Speed += 20;
			if(Speed >100)
			{
				Speed = -100;
			}	
		}
		Motor_SetSpeed(Speed);
		OLED_ShowSignedNum(1, 7, Speed, 2);
	}
}
