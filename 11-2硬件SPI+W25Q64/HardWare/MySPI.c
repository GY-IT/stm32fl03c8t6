#include "stm32f10x.h"                  // Device header


void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}


void MySPI_Init(void)													//SPI初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;						//设备选择引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//当前设备为主机
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//双线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						//数据帧大小8位
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;						//高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; 	//波特率预分频 SPI1 72MHz/128
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			//模式选择，模式0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		//模式选择，模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			//软件NSS，暂未用到
	SPI_InitStructure.SPI_CRCPolynomial = 7;			//暂未用到
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);							//默认高电平，不选择
	
}

void MySPI_Start(void)    					//SPI设备1，开始信号
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)  						//SPI设备1，结束信号
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)	//SPI交换一个字节 16位
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);  	//等待可以发送，卡死概率不大
	SPI_I2S_SendData(SPI1, ByteSend);								//发送字节
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);  	//等待接收完成，卡死概率不大
	return SPI_I2S_ReceiveData(SPI1);								//读取接收数据
}


