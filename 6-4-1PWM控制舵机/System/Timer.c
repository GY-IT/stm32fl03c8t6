#include "stm32f10x.h"


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);				//时钟使能
	
	TIM_InternalClockConfig(TIM3);   									//内部时钟给TIM2
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  		//滤波器1分频检测
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//上升
	TIM_TimeBaseInitStructure.TIM_Period = 1000-1;						//计数器 有一个数的偏差
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;					//预分频 有一个数的偏差
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//重复计数器 高级定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);								//清除标志位
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);							//开启中断线路
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//中断分组
	
	NVIC_InitTypeDef NVIC_InitSturcture;
	NVIC_InitSturcture.NVIC_IRQChannel = TIM3_IRQn;						//选择中断通道
	NVIC_InitSturcture.NVIC_IRQChannelCmd = ENABLE;						//通道使能
	NVIC_InitSturcture.NVIC_IRQChannelPreemptionPriority = 2;			//抢占优先级	
	NVIC_InitSturcture.NVIC_IRQChannelSubPriority = 1;					//相应优先级
	NVIC_Init(&NVIC_InitSturcture);	
	
	TIM_Cmd(TIM3, ENABLE);												//使能TIM2
	
}
/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
	{
		
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/


