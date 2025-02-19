#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "W25Q64.h"

uint16_t yucaiji_length = 0xFFF;
uint8_t MID;
uint16_t DID;

uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};
uint8_t ArrayRead[4];

uint16_t ArrayWrite_16[0xFFF];
uint16_t ArrayRead_16[256];


void SmallRingBuff_Init(uint16_t* SmallRingBuffer_S)    
{
	uint16_t i;
	for (i = 0; i <= yucaiji_length - 1; i++) 
	{
		SmallRingBuffer_S[i] = i;
	}
}

int main(void)
{

	OLED_Init();
	W25Q64_Init();
	
	
	SmallRingBuff_Init(ArrayWrite_16);
	
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	OLED_ShowString(4, 1, "W16:    R16:");

	
	W25Q64_ReadID(&MID, &DID);
	
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);

	W25Q64_BlockErase(0x000000);
	
//	W25Q64_PageProgram_Byte(0x000000, ArrayWrite, 4); 	//写
//	W25Q64_ReadData_Byte(0x000000, ArrayRead, 4);		//读
//	OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);
//	OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
//	OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
//	OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
//	OLED_ShowHexNum(3, 3, ArrayRead[0], 2);
//	OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
//	OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
//	OLED_ShowHexNum(3, 12, ArrayRead[3], 2);	
	
	
	
	
	W25Q64_SectorProgram_TwoByte(0x000000, &ArrayWrite_16[0], 0xFFF); 	//写
	W25Q64_ReadData_TwoByte(0x000200, &ArrayRead_16[0], 256);		//读0x200 = 256*2 数组数 为0x200/2=256

	
	OLED_ShowHexNum(4, 5, ArrayWrite_16[0x110], 4);	
	OLED_ShowHexNum(4, 13, ArrayRead_16[0x10], 4);	
	

	
	while(1)
	{

		
	}
}
