#include "stm32f10x.h"


void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);		//没有重映射 +  1，2 部分重映射 + 完全重映射
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//将PA15改为GPIO模式，更改调试引脚模式
	
	GPIO_InitTypeDef GPIO_InitStructuer;
	GPIO_InitStructuer.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructuer.GPIO_Pin = GPIO_Pin_0; //GPIO_Pin_15
	GPIO_InitStructuer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructuer);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;						//ARR 计100个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1;					//PSC 分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);								//赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;					//模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;									//CCR 多少个数反转电平
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);	
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

