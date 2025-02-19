#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
	Key_Init();
	OLED_Init();
	
	OLED_ShowString(1, 1, "WWDG TEST");
	if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		RCC_ClearFlag();
	}else
	{
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);					//分频
	WWDG_SetWindowValue(0x40 | 21);							//最低30ms
	WWDG_Enable(0x40 | 54);									//最高50ms
	
	while(1)
	{
		Key_GetNum();
		
//		OLED_ShowString(4, 1, "FEED");
//		Delay_ms(200);
//		OLED_ShowString(4, 1, "    ");
		Delay_ms(51);		
		
		WWDG_SetCounter(0x40 | 54);							//避免连续喂狗，喂狗
		
	}
}
