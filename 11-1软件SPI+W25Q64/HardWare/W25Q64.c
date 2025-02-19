#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

/*
地址宽度24，3字节，最大16MByte 0xffffff 

64Mbit 
64Mbit/8 				 = 8MByte 
8MB/64kB = 8*1024kB/64kB = 128block 
64kB/4kB 				 = 16Sector 
4kB/256bit = 4*1024/256  = 16page

64Mbit 8MByte 	128block 	16Sector 	16page 		256Byte 	256bit

B: 0x7fffff 0xffff 		0xfff 		0xff	b:0xff
*/

void W25Q64_Init(void)							//W25Q64初始化
{
	MySPI_Init();
}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)	//读ID
{
	MySPI_Start();								//开始
	MySPI_SwapByte(W25Q64_JEDEC_ID);			//读ID指令
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换ID
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换ID
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换ID
	MySPI_Stop();								//停止
}

void W25Q64_WriteEnable(void)					//写使能，写，擦除前要使能
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}


void W25Q64_WaitBusy(void)						//读标志位进行等待
{
	uint32_t Timeout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);	//读标志位指令
	Timeout = 1000000;
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	//等待空闲
	{
		Timeout --;
		if(Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)	//按页写，地址，数组，字节数
{
	W25Q64_WaitBusy(); //事前等待
	
	uint16_t i;
	
	W25Q64_WriteEnable();							//使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);			//写指令
	MySPI_SwapByte(Address >> 16);					//所写地址起始
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Count; i ++)						//循环写
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
//	W25Q64_WaitBusy();//事后等待
}

void W25Q64_SectorErase(uint32_t Address)  //擦除所在扇区
{
	W25Q64_WaitBusy();//事前等待
	
	W25Q64_WriteEnable();							//使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);		//擦除扇区指令
	MySPI_SwapByte(Address >> 16);					//字节所在扇区
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
//	W25Q64_WaitBusy();//事后等待
}

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count) 		//读数据，起始地址，数组，字节数
{
	W25Q64_WaitBusy();//事前等待

	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);				//读指令
	MySPI_SwapByte(Address >> 16);					//读起始地址
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Count; i ++)						//循环读
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}

