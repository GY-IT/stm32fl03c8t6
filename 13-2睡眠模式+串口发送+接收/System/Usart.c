#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);



	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;										//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//	
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;					//工作模式		
	USART_InitStructure.USART_Parity = USART_Parity_No;								//校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//数据长度
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//打开终端线
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);									//中断分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								//通道选择
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;						
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}
	
void Serial_SendByte(uint8_t Byte)									//发送8位
{
	USART_SendData(USART1, Byte);									//发送
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET ) ;	//等待发送完成
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)				//发送数组
{
	uint16_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);									//循环发送8位
	}
}

void Serial_SendString(char  *String)								//发送字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)								//
	{
		Serial_SendByte(String[i]);									//循环发送8位
	}
}


uint32_t Serial_Pow(uint32_t X, uint32_t Y)							//计算x的y次方
{
	uint32_t Result = 1;;
	while (Y--)
	{
		Result *=X ;
	
	}
	return Result;
}
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1) % 10 + '0');
	}
}
int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
	
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;

}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;

}
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
