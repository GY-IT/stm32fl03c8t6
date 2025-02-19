#include "AD.h"                  // Device header


uint16_t AD_Value[4];
//ADC为厨师 DMA为服务员  ADC连续模式 DMA循环扫描


void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//ADC时钟分频6分频 72MHz/6=12MHz
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);			//ADC 通道 序列 采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);			//ADC 通道 序列 采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);			//ADC 通道 序列 采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);			//ADC 通道 序列 采样时间
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;							//ADC模式独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//数据对其右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			//外部触发源，无为软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;							//ADC连续模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;								//ADC扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 4;										//四个通道，菜单有四个
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;       		 	//端菜地址为ADC DR寄存器地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//地址不自增，始终转移同一地址，一个ADC
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;						//端菜目的地为数组
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							//地址自增，四个通道数据
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//传输方向外设站点为源
	DMA_InitStructure.DMA_BufferSize = 4;											//传输数量
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//循环模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//不使用软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;							//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);										//ADC1 DMA通道1
	
	DMA_Cmd(DMA1_Channel1, ENABLE);				//DMA
	
	ADC_DMACmd(ADC1, ENABLE);					//开启ADC到DMA
	
	ADC_Cmd(ADC1, ENABLE);						//ADC
	
	ADC_ResetCalibration(ADC1);								//开始复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);		//等待复位校准完成
	ADC_StartCalibration(ADC1);								//开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);			//等待校准完成
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //软件触发一次，连续触发
}

void AD_GetValue(void) 
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);   		//单次软件触发
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}



