#include "stm32f10x.h"


void IC_Init(void) 			//TIM3输入捕获
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructuer;
	GPIO_InitStructuer.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructuer.GPIO_Pin = GPIO_Pin_6; 			 
	GPIO_InitStructuer.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructuer);
	
	TIM_InternalClockConfig(TIM3);										//选择时钟源
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//滤波器
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    	//上升沿计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;						//ARR  设为最大可计65536个数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;						//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//重复计数器 高级定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;					//通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;								//滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;			//上升沿计数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;				//分频，多少个沿记一次
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;		//直连方式
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);						//选择输入触发器信号源
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);						//选择从模式
	
	TIM_Cmd(TIM3, ENABLE);	
}
uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);   					// +1 为了好看
}
