#ifndef __ESP8266_H
#define __ESP8266_H

#include "stm32f10x.h"                  // Device header
#include "Usart.h"
#include "Delay.h"

void ESP8266_SendCmd(char *Command);
void ESP8266_SendData(char *Command, uint8_t Length);  //数据 + 长度
void ESP8266_ModeInit(void);


#endif
