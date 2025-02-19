#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"


#define OLED_W_GPIO_RCC		RCC_APB2Periph_GPIOB
#define OLED_W_GPIO 		GPIOB
#define OLED_W_SCL_Pin		GPIO_Pin_8
#define OLED_W_SDA_Pin		GPIO_Pin_9

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowCHinese(uint8_t Line,uint8_t Column,uint8_t Chinese);
void OLED_ShowBMP(uint8_t Line,uint8_t Column);

#endif
