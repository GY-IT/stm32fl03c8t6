#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "AD.h"

uint16_t ADC_ConvertedValue[15];

uint16_t ADC_ConvertedValue_average;



void ADC1_Mode_Config(void)
{
        DMA_InitTypeDef DMA_InitStructure;
        ADC_InitTypeDef ADC_InitStructure;
        
        /* DMA channel1 configuration */
        DMA_DeInit(DMA1_Channel1);
        
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;                    //ADC地址
        DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;        //内存地址  ADC_ConvertedValue[15]一个数组
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize = 15;
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                   //外设地址固定
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                                  //内存地址自加
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;        //半字
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                                   //循环传输
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(DMA1_Channel1, &DMA_InitStructure);
        
        /* Enable DMA channel1 */
        DMA_Cmd(DMA1_Channel1, ENABLE);

        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //传输结束中断
        
        /* ADC1 configuration */        
        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                        //独立ADC模式
        ADC_InitStructure.ADC_ScanConvMode = DISABLE ;                                //禁止扫描模式，扫描模式用于多通道采集
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                        //开启连续转换模式，即不停地进行ADC转换
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;        //不使用外部触发转换
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;         //采集数据右对齐
        ADC_InitStructure.ADC_NbrOfChannel = 1;                                //要转换的通道数目1
        ADC_Init(ADC1, &ADC_InitStructure);
        
        /*配置ADC时钟，为PCLK2的8分频，即9MHz*/
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);
        /*配置ADC1的通道11为55.        5个采样周期，序列为1 */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
        
        /* Enable ADC1 DMA */
        ADC_DMACmd(ADC1, ENABLE);
        
        /* Enable ADC1 */
        ADC_Cmd(ADC1, ENABLE);
        
        /*复位校准寄存器 */   
        ADC_ResetCalibration(ADC1);
        /*等待校准寄存器复位完成 */
        while(ADC_GetResetCalibrationStatus(ADC1));
        
        /* ADC校准 */
        ADC_StartCalibration(ADC1);
        /* 等待校准完成*/
        while(ADC_GetCalibrationStatus(ADC1));
        
        /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void DMA1_Channel1_IRQHandler(void)
{        
        uint8_t i;
        uint32_t ADC_ConvertedValue_TEMP=0;
        
        for(i=0;i<15;i++)
        {
                ADC_ConvertedValue_TEMP += ADC_ConvertedValue[i];
        }
        ADC_ConvertedValue_average = ADC_ConvertedValue_TEMP/15;     //全局变量ADC_ConvertedValue_average
        
        DMA_ClearITPendingBit(DMA1_IT_TC1);
          DMA_ClearFlag(DMA1_FLAG_TC1);
}


void GPIO_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;                          
        //=======================ADC通道配置========================
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                // PA0,输入时不用设置速率
}


void NVIC_Configuration(void)
{
       NVIC_InitTypeDef NVIC_InitStructure;
   
        /* Configure the NVIC Preemption Priority Bits */  
       NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
        NVIC_Init(&NVIC_InitStructure);        
}



int main(void)
{

	OLED_Init();
	ADC1_Mode_Config();
	GPIO_Configuration();
	NVIC_Configuration();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");

	while(1)
	{	
//		AD_GetValue();			//单次软件触发
		
		OLED_ShowNum(2, 5, ADC_ConvertedValue[0], 4);
		OLED_ShowNum(2, 5, ADC_ConvertedValue_average, 4);
		

		Delay_ms(100);
		
	}
}



