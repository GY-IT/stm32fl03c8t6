#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
	Key_Init();
	OLED_Init();
	
	
	OLED_ShowString(1, 1, "IWDG TEST");
	
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		OLED_ShowString(2, 1, "IWDGRST");
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
													//打开时钟，自动启动
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_16);			//预分
	IWDG_SetReload(2499);							//设置重装值  1000毫秒
	IWDG_ReloadCounter();            				//喂一次狗
	IWDG_Enable();									//启动看门狗
	
	while(1)
	{
		Key_GetNum();
		IWDG_ReloadCounter();            				//喂一次狗
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600);		
		
	}
}
