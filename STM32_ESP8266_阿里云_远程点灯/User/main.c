//作者 B站UID UID590045904
//B站有配套教程
//Q群：752565966
//时间2022.10.13 22：55
//功能：STM32+ESP8266，发送数据给阿里云，接收阿里云数据，实现远程点灯
//状态：完成


//STM32+ESP8266资料
//链接：https://pan.baidu.com/s/13n3IFPSInl4gZyhclxd7jA?pwd=esp8 
//提取码：esp8 
//复制这段内容后打开百度网盘手机App，操作更方便哦


//所有我视频里的资料
//链接：https://pan.baidu.com/s/1lwFKTPdxtT_pxnCjvMTP5Q?pwd=bili 
//提取码：bili 
//复制这段内容后打开百度网盘手机App，操作更方便哦

#include "string.h"
#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"


/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	u8 i=0; 
	u16 data=1234;
	float fdata=12.34;
	char str[]="Hello World!";	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	LED_Init();
	USART1_Init(115200);//串口波特率115200
	GPIO_ResetBits(LED_PORT,LED_PIN);	//LED=0 亮
	delay_ms(1000);					//延迟
//以上是初始化
//---------------------------------------------------------
//发送指令，让ESP8266连接到阿里云
	
//	printf("AT+RST\r\n");		//第一步		复位		只用一次
//	delay_ms(2000);					//延迟			
//	
//	printf("AT+CWMODE=1\r\n");		//第二步		模式  只用一次
//	delay_ms(2000);					//延迟
	printf("AT\r\n");		//第一步		AT
	delay_ms(2000);			//延迟

	printf("AT+GMR\r\n");		//第一步		版本号
	delay_ms(2000);			//延迟
	
	printf("AT+CWJAP=						改成自己的东西															\r\n");		//第四步 wifi热点
	delay_ms(5000);					//延迟
	delay_ms(5000);					//延迟
	delay_ms(5000);					//延迟	
	delay_ms(5000);					//延迟

	GPIO_SetBits(LED_PORT,LED_PIN);		//LED=1	灭

	printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");		//第三步
	delay_ms(3000);					//延迟

	printf("AT+MQTTUSERCFG=					改成自己的东西															\r\n");		//第五步
	delay_ms(3000);					//延迟			

	printf("AT+MQTTCLIENTID=0				改成自己的东西															\r\n");		//第六步
	delay_ms(3000);					//延迟	
	
	printf("AT+MQTTCONN=0						改成自己的东西															\r\n");		//第七步
	delay_ms(3000);					//延迟
	GPIO_ResetBits(LED_PORT,LED_PIN);	//LED=0 亮
//连接阿里云，让设备在线
//---------------------------------------------------------
//订阅发布指令
	printf("AT+MQTTSUB=0						改成自己的东西															\r\n");		//订阅指令
	delay_ms(5000);			//延迟
	GPIO_SetBits(LED_PORT,LED_PIN);		//LED=1	灭

	printf("AT+MQTTPUB=0						改成自己的东西															\r\n");		//发数据{"LED":1}
	delay_ms(5000);			//延迟
	
	printf("AT+MQTTPUB=0						改成自己的东西															\r\n");		//发数据{"LED":0}
	delay_ms(5000);			//延迟

	GPIO_ResetBits(LED_PORT,LED_PIN);	//LED=0 亮
//---------------------------------------------------------	
	while(1)
	{
		if(USART_RX_STA&0x8000)//判断接收到完整的字符串  完成
		{					   			
				
				if(strstr((const char*)USART_RX_BUF,"on"))//判断阿里云发下来的数据里是否有on
				{
						GPIO_ResetBits(LED_PORT,LED_PIN);	//LED=0 亮
				}
				if(strstr((const char*)USART_RX_BUF,"off"))//判断阿里云发下来的数据里是否有off
				{
						GPIO_SetBits(LED_PORT,LED_PIN);		//LED=1	灭
				}	

				memset(USART_RX_BUF,0,600);         //将接收区清0
				USART_RX_STA=0;
		}
	}
}


//第一步		AT+RST
// 
//第二步		AT+CWMODE=1

//第三步		AT+CIPSNTPCFG=1,8,"ntp1.aliyun.com"

//第四步		AT+CWJAP="WiFi名字","WiFi密码"

//第五步		AT+MQTTUSERCFG=0,1,"NULL","用户名","密码",0,0,""

//第六步		AT+MQTTCLIENTID=0,"ClienId"    					//clientId第二个参数注意每个逗号前加分隔符\

//第七步		AT+MQTTCONN=0,"连接域名",1883,1

//AT+MQTTSUB=0,"topic",1							//订阅指令

//AT+MQTTPUB=0,"topic","AJson格式内容",1,0		//发布指令


//注意：   "前面要加转义字符    \前面要加转义字符
