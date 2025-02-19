#include "ESP8266.h"

void ESP8266_SendCmd(char *Command)
{
	char String[100];
	sprintf(String, "%s\r\n", Command);
	Serial_SendString(String);
	Delay_ms(1000);
}

void ESP8266_SendData(char *Command, uint8_t Length)  //数据 + 长度
{
	char String[100];
	sprintf(String, "AT+CIPSEND=0,%c", '0'+Length);
	ESP8266_SendCmd(String);
	sprintf(String, "%s", Command);
	Serial_SendString(String);
	Delay_ms(10);
}
//ESP8266-WIFI模块工作模式初始化
void ESP8266_ModeInit(void)
{
//	ESP8266_RST_Pin=1;
//	ESP8266_CH_PD_Pin=1;
	Serial_Init();
	
	ESP8266_SendCmd("AT+CWMODE=3");//设置路由器模式 1 staTIon模式 2 AP点 路由器模式 3 station+AP混合模式
	ESP8266_SendCmd("AT+CWSAP=\"PRECHIN\",\"prechin168\",11,0"); //设置WIFI热点名及密码
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.2\"");//重新启动wifi模块
	ESP8266_SendCmd("AT+RST");//重新启动wifi模块
	ESP8266_SendCmd("AT+CIPMUX=1");	//开启多连接模式，允许多个各客户端接入
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//启动TCP/IP 端口为8080 实现基于网络控制	
}

