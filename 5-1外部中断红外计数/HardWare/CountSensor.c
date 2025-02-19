#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;
//红外计数 GPIOB 的 GPIO_Pin_14
void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//使能AFIO时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;						
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);	//PB14接入中断线路(14号线路)
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;						//选择中断线
	EXTI_InitStructure.EXTI_LineCmd =  ENABLE;						//使能中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//中断模式选择
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//触发方式
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//中断分组设置
	NVIC_InitTypeDef NVIC_InitStructurce;
	NVIC_InitStructurce.NVIC_IRQChannel = EXTI15_10_IRQn;			//选择中断通道
	NVIC_InitStructurce.NVIC_IRQChannelCmd = ENABLE;				//通道使能
	NVIC_InitStructurce.NVIC_IRQChannelPreemptionPriority = 1;		//抢占优先级
	NVIC_InitStructurce.NVIC_IRQChannelSubPriority = 1;				//相应优先级
	NVIC_Init(&NVIC_InitStructurce);
}


uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count ++;
	
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}


