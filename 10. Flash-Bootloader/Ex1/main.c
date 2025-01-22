#include "stm32f10x.h"                  // Device header
#include "stm32f10x_flash.h"            // Device:StdPeriph Drivers:Flash

void Flash_Erase(uint32_t addresspage);
void Flash_WriteInt(uint32_t address, uint16_t value);
void Flash_WriteNumByte(uint32_t address, uint8_t *data, int num);
void Flash_ReadNumByte(uint32_t address, int num);
	
uint8_t array[14] = {5,2,7,4,2,6,3,15,17,20,11,23,25,19};
uint8_t buffer[14];

int main(void)
{
	Flash_Erase(0x08010000);
	Flash_WriteNumByte(0x08010000, array, 14);
	Flash_ReadNumByte(0x08010000, 12);
	Flash_WriteInt(0x08010020, 43690);	// 0xAAAA
	
	while(1)
	{
		
	}
}

void Flash_Erase(uint32_t addresspage)
{
	FLASH_Unlock();
	
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	FLASH_ErasePage(addresspage);
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	
	FLASH_Lock();
}

void Flash_WriteInt(uint32_t address, uint16_t value)
{
	FLASH_Unlock();
	
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	FLASH_ProgramHalfWord(address, value);
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	
	FLASH_Lock();
}

void Flash_WriteNumByte(uint32_t address, uint8_t *data, int num)
{
	FLASH_Unlock();
	
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	uint16_t *ptr = (uint16_t*)data;
	for (uint8_t i = 0; i < ((num + 1) / 2); i++)
	{
		FLASH_ProgramHalfWord(address + 2 * i, *ptr);
		while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
		ptr++;
	}
	
	FLASH_Lock();
}

void Flash_ReadNumByte(uint32_t address,int num)
{
	for (int i = 0; i < num; i++) 
	{
    buffer[i] = *(uint8_t*) address;
    address++;
	}
}
