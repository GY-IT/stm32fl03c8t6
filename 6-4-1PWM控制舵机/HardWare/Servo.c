#include "stm32f10x.h"                  // Device header
#include "PWM.h"                  // Device header

void Servo_Init(void)
{
	PWM_Init();
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle /180*2000+500);
}
