#include "stm32f10x.h"

uint16_t MyDMA_Size;


void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMAInitStructure;
	DMAInitStructure.DMA_PeripheralBaseAddr = AddrA;							//获取数据地址
	DMAInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//数据宽度 字节
	DMAInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;				//是否自增 自增
	DMAInitStructure.DMA_MemoryBaseAddr = AddrB;								//存放数据地址
	DMAInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//数据大小 字节
	DMAInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//是否自增 自增
	DMAInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//外设到存储器 
	DMAInitStructure.DMA_BufferSize = Size;										//传输几个数据
	DMAInitStructure.DMA_Mode = DMA_Mode_Normal;								//模式 正常模式
	DMAInitStructure.DMA_M2M = DMA_M2M_Enable;									//使用软件触发
	DMAInitStructure.DMA_Priority = DMA_Priority_Medium;						//优先级
	DMA_Init(DMA1_Channel1, &DMAInitStructure);
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	//传输计数器大于0 触发源有触发信号 DMA使能
}


//调用一次DMA转换一次
void MyDMA_Transfer(void)    
{
	DMA_Cmd(DMA1_Channel1, DISABLE);									//DMA失能
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);					//传输寄存器赋值
	DMA_Cmd(DMA1_Channel1, ENABLE);										//DMA使能
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);					//等待转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1);										//清除转运完成标志位
}


