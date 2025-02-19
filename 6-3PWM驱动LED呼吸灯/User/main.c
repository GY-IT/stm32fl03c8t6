#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "PWM.h"

uint8_t i;

int main(void)
{
	OLED_Init();
	PWM_Init();
	while(1)
	{
		for(i=0; i<100; i++)
		{
			PWM_SetCompare1(i);  		//调整占空比
			Delay_ms(20);	
		}
	}
}
