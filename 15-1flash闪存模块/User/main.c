#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Key.h"
#include "Store.h"

uint8_t KeyNum;
uint16_t block_num=0x00;

int main(void)
{
	Key_Init();
	OLED_Init();
	Store_Init();
	
	OLED_ShowString(1, 1, "Flag");
	OLED_ShowString(2, 1, "Data");
	while(1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{	
			Store_Data[1] =block_num;
			Store_Data[2] += 2;
			Store_Data[3] += 3;
			Store_Data[4] += 4;
			Store_Save();
		}
		if (KeyNum == 2)
		{	
			Store_Clear();
		}
		
		OLED_ShowHexNum(1, 6, Store_Data[0], 4);
		OLED_ShowHexNum(3, 1, Store_Data[1], 4);
		OLED_ShowHexNum(3, 6, Store_Data[2], 4);
		OLED_ShowHexNum(4, 1, Store_Data[3], 4);
		OLED_ShowHexNum(4, 6, Store_Data[4], 4);
	}
}
