#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Servo.h"
#include "Key.h"
#include "Timer.h"
uint8_t KeyNum;
float Angle;

int main(void)
{
	OLED_Init();
	Servo_Init();
//	Key_Init();
	Timer_Init();
//	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(1, 1, "Angle:");
	
	
	Angle=70;
	while(1)
	{
//		KeyNum = Key_GetNum();
//		if (KeyNum == 1)
//		{	
//			
//			Angle += 30;
//			if (Angle >180)
//			{
//				Angle = 0;
//			}
//		}
		Servo_SetAngle(Angle);
		
		OLED_ShowNum(1, 7, Angle, 3);

		OLED_ShowNum(2,5,TIM_GetCounter(TIM3),5);
	}
}
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)==SET)
	{
			Angle += 5;
			if (Angle >=70)
			{
				Angle = 70;
			}
			if (Angle >=110)
			{
				Angle = 70;
			}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
