#include "Usart.h"

void NVICINIT_PRINTF()
{
	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief  USART GPIO 配置,工作参数配置
 * @param  无
 * @retval 无
 */
void USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 打开串口GPIO的时钟
    DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);  // PA时钟

    // 打开串口外设的时钟
    DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);  // RCC_APB2Periph_USART1  打开串口1的时钟

    // 将USART Tx的GPIO配置为推挽复用模式
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;   // Pin_9引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure); // GPIOA 使能

    // 将USART Rx的GPIO配置为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;      // Pin_10引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // 浮空输入
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);   // GPIOA 使能

    // 配置串口的工作参数
    // 配置波特率
    USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;   //115200 波特率
    // 指定帧中发送或接收的数据比特数
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8位或者9位
    // 配置停止位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //停止位 ：1
    // 配置校验位
    USART_InitStructure.USART_Parity = USART_Parity_No ;        //无校验
    // 配置硬件流控制
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;                             //这里设置 无
    /*
        硬件流控就是来解决这个速度匹配的问题。
        它的基本含义非常简单，当接收端接收到的数据处理不过来时，就向发送端发送不再接收的信号，
        发送端接收到这个信号之后就会停止发送，直到收到可以继续发送的信号再继续发送。
        因此流控本身是可以控制数据传输的进度，进而防止数据丢失。
    */
    // 配置工作模式，收发一起
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // 完成串口的初始化配置
    USART_Init(DEBUG_USARTx, &USART_InitStructure);    //USART1 完成串口1的初始化
//	NVICINIT_PRINTF();
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断 接收中断
    // 使能串口
    USART_Cmd(DEBUG_USARTx, ENABLE);                   // 启用USART1 的外设
}


/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef *pUSARTx, uint8_t ch)//'1'为'1'  31为'1'
{
    /* 发送一个字节数据到USART */
    USART_SendData(pUSARTx, ch);

    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef *pUSARTx, char *str)
{
    unsigned int k = 0;
    do
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    }
    while(*(str + k) != '\0');

    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
    {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef *pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* 取出高八位 */
    temp_h = (ch & 0XFF00) >> 8;
    /* 取出低八位 */
    temp_l = ch & 0XFF;

    /* 发送高八位 */
    USART_SendData(pUSARTx, temp_h);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

    /* 发送低八位 */
    USART_SendData(pUSARTx, temp_l);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到串口 */
    USART_SendData(DEBUG_USARTx, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);

    return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//    /* 等待串口输入数据 */
//    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

//    return (int)USART_ReceiveData(DEBUG_USARTx);
//}
int fgetc(FILE *stream)
{
	while(!(DEBUG_USARTx->SR & (1 << 5))){};//等待数据接收完成
	return DEBUG_USARTx->DR;
}
/*发送串口中断*/

//void DEBUG_USART_IRQHandler(void)
//{
//	USART_ClearFlag(DEBUG_USARTx,USART_FLAG_TC);
//	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	{
//		USART_SendData(DEBUG_USARTx,USART_ReceiveData(DEBUG_USARTx));
//		while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE)==RESET);
//	}	
//}
