#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "AD.h"

//1000 数据  一秒触发1000次
uint16_t ADC_Buffer[1000];
volatile uint32_t ADC_Index = 0;

uint16_t Num;

void Timer_Init(void);
int main(void)
{

	OLED_Init();
	AD_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Voltage:0.00V");
	
	while(1)
	{
		OLED_ShowNum(1, 9, ADC_Buffer[0], 4);
		OLED_ShowNum(3,5,Num,5);
		OLED_ShowNum(4,5,TIM_GetCounter(TIM3),5);
	}
}


void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);				//时钟使能
	
	TIM_InternalClockConfig(TIM3);   									//内部时钟给TIM2
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  		//滤波器1分频检测
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//上升
	TIM_TimeBaseInitStructure.TIM_Period = 10-1;						//计数器 有一个数的偏差
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;					//预分频 有一个数的偏差 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;				//重复计数器 高级定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
	
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

void ADC1_2_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1,ADC_IT_EOC))
	{
	
		ADC_Buffer[ADC_Index++] = ADC_GetConversionValue(ADC1);

        /* Reset the index if necessary */
        if (ADC_Index >= 1) {
            ADC_Index = 0;
        }
		ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
	
	}

}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update)==SET)
	{
		
		Num++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

