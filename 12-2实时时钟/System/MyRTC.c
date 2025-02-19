#include "stm32f10x.h"                  // Device header
#include <time.h>


void MyRTC_SetTime(void);


uint16_t MyRTC_Time[] = {1970, 1, 1, 8, 0, 0};

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x6464)
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  	//时钟选择
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro();   					//等待
		RTC_WaitForLastTask();						//等待
		
		RTC_SetPrescaler(32768 - 1);				//设置分频，分频后计数完为1s,
		RTC_WaitForLastTask();						//等待
			
		MyRTC_SetTime();
	
		BKP_WriteBackupRegister(BKP_DR1, 0x6464);
	}
	else
	{
		RTC_WaitForSynchro();   					//等待
		RTC_WaitForLastTask();						//等待
	
	}

}

void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
												//将时间赋予结构体
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];

	time_cnt = mktime(&time_date) - 8*60*60;				//获得秒数
	
	RTC_SetCounter(time_cnt);					//设置计数初始值
	RTC_WaitForLastTask();						//等待
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8*60*60;
	
	time_date = *localtime(&time_cnt);
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}




