#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Key.h"
#include "OLED.h"
#include "Delay.h"

uint16_t Num;

uint8_t num;
int main(void)
{
	OLED_Init();
	Timer_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"Num:");
	while(1)
	{	
		num = Key_GetNum();
		if( num== 1){TIM_Cmd(TIM2, ENABLE);}
		if(num == 2){TIM_Cmd(TIM2, DISABLE);}
		OLED_ShowNum(1,5,Num,5);
		OLED_ShowNum(2,5,TIM_GetCounter(TIM2),5);
	}	
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
	{
		
		
		Num++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
