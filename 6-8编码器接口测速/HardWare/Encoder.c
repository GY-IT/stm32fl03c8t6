#include "stm32f10x.h"                  // Device header

void Encoder_Init(void) 			
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructuer;
	GPIO_InitStructuer.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructuer.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 			 
	GPIO_InitStructuer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructuer);
	
	//定时器设置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //暂无作用
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	//输入捕单元获设置
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);				//赋初始值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	//通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;				//滤波器
	TIM_ICInit(TIM3, &TIM_ICInitStructure);				//赋初始值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;	//通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;				//滤波器
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	//编码器设置 定时器 模式 极性
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);	
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);				//获取TIM3计数值
	TIM_SetCounter(TIM3, 0);					//清零TIM3计数值
	return Temp;
}
