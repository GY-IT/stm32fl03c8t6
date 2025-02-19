#include "AD.h"                  // Device header




void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);     					//ADC时钟频率 定时器分频,只能用两个 72M->12M 72M->9M
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); //ADC 通道 序列 采样时间=周期*一循环时间
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;							//ADC模式独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//数据对齐右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;			//外部触发源，本次用软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							//连续
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//扫描还是非扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1;										//通道数目
	ADC_Init(ADC1, &ADC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//中断分组设置
	NVIC_InitTypeDef NVIC_InitStructurce;
	NVIC_InitStructurce.NVIC_IRQChannel = ADC1_2_IRQn;			//选择中断通道
	NVIC_InitStructurce.NVIC_IRQChannelCmd = ENABLE;				//通道使能
	NVIC_InitStructurce.NVIC_IRQChannelPreemptionPriority = 1;		//抢占优先级
	NVIC_InitStructurce.NVIC_IRQChannelSubPriority = 1;				//相应优先级
	NVIC_Init(&NVIC_InitStructurce);	
	
	
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	
	
	
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);											//开始复位校准	
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);					//等待复位校准完成
	ADC_StartCalibration(ADC1);											//开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);						//等待校准完成
	
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);								//连续触发
}


//连续和单次的区别，单次获取一次结果转换一次，多次不停转换，获取结果
uint16_t AD_GetValue(void) //单次软件触发
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);     						//软件触发
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);				//等待转换完成
	return ADC_GetConversionValue(ADC1);								//返回转换值
}



