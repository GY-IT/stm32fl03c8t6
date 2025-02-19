#ifndef __W25Q64_H
#define __W25Q64_H

#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"


#define 	Page_u16		128
#define 	Page_u8			256
#define 	Sector_u16		16*128
#define 	Sector_u8		16*256

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram_Byte(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData_Byte(uint32_t Address, uint8_t *DataArray, uint32_t Count); 


void W25Q64_PageProgram_TwoByte(uint32_t Address, uint16_t *DataArray, uint16_t Count);
void W25Q64_BlockErase(uint32_t Address);
void W25Q64_ReadData_TwoByte(uint32_t Address, uint16_t *DataArray, uint32_t Count); 

void W25Q64_SectorProgram_TwoByte(uint32_t Address, uint16_t *DataArray, uint16_t Count);
void W25Q64_BlockProgram_TwoByte(uint32_t Address, uint16_t *DataArray, uint32_t Count);

#endif
