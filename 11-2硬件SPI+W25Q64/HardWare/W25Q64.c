#include "W25Q64.h"



/*
地址宽度24，3字节，最大16MByte 0xffffff 

64Mbit 
64Mbit/8 				 = 8MByte 
8MB/64kB = 8*1024kB/64kB = 128block 
64kB/4kB 				 = 16Sector 
4kB/256bit = 4*1024/256  = 16page

64Mbit 8MByte 	128block 	16Sector 	16page 	256bit=0xff

	   0x7fffff 0xffff 		0xfff 		0xff
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

//写一页时间为0.7ms
void W25Q64_PageProgram_Byte(uint32_t Address, uint8_t *DataArray, uint16_t Count)	//按页写，地址，数组，字节数 最多写一页
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

//可以有8个u16 ,count最大为256/2=128
//输入传输的地址 数组 u16数目最大为 128
void W25Q64_PageProgram_TwoByte(uint32_t Address, uint16_t *DataArray, uint16_t Count)	//按页写，地址，数组，字节数 最多写一页
{
	W25Q64_WaitBusy(); //事前等待
	
	uint16_t i;
	uint8_t tihuan;
	
	W25Q64_WriteEnable();							//使能
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);			//写指令
	MySPI_SwapByte(Address >> 16);					//所写地址起始
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Count; i ++)						//循环写，写先放高位再放低位，读先取低位再取高位
	{
		tihuan = (DataArray[i] >> 8);
		MySPI_SwapByte(tihuan);
		tihuan = DataArray[i];
		MySPI_SwapByte(tihuan);
	}
	MySPI_Stop();
	
//	W25Q64_WaitBusy();//事后等待
}

//一个Sector 256*16 位  256 u8  128 = 16 *8  u16
//输入传输的地址 数组 u16数目最大为 128*16=2048
void W25Q64_SectorProgram_TwoByte(uint32_t Address, uint16_t *DataArray, uint16_t Count)	//按扇区写，地址，数组，字节数 最多写一页
{
	uint16_t i ;
	uint8_t chushu,yushu;
	
	chushu = Count / Page_u16;
	yushu = Count % Page_u16;
	
	for(i = 0; i < chushu; i++)
	{
		W25Q64_PageProgram_TwoByte(Address+i*Page_u8, &DataArray[i*Page_u16], Page_u16);
	}
	if(yushu != 0)
	{
		W25Q64_PageProgram_TwoByte(Address+chushu*Page_u8, &DataArray[chushu*Page_u16], yushu);
	}
}
//输入传输的地址 数组 u16数目最大为 128*16*16=32768
void W25Q64_BlockProgram_TwoByte(uint32_t Address, uint16_t *DataArray, uint32_t Count)	//按Block写，地址，数组，字节数 最多写一页
{
	uint16_t i ;
	uint8_t chushu,yushu;
	
	chushu = Count / (Page_u8*16);
	yushu = Count % (Page_u16*16);
	
	for(i = 0; i < chushu; i++)
	{
		W25Q64_SectorProgram_TwoByte(Address+i*Sector_u8, &DataArray[i*Sector_u16], Sector_u16);
	}
	if(yushu != 0)
	{
		W25Q64_SectorProgram_TwoByte(Address+chushu*Sector_u8, &DataArray[chushu*Sector_u16], yushu);
	}
}

void W25Q64_ReadData_Byte(uint32_t Address, uint8_t *DataArray, uint32_t Count) 		//读数据，起始地址，数组，字节数
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

void W25Q64_ReadData_TwoByte(uint32_t Address, uint16_t *DataArray, uint32_t Count) 		//读数据，起始地址，数组，字节数
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
		DataArray[i] <<= 8;
		DataArray[i] |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	
	}
	MySPI_Stop();
}

//擦除一个扇区为30ms
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

//擦除一个块为150ms
void W25Q64_BlockErase(uint32_t Address)  //擦除所在扇区
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
