#include "stm32f10x.h"                  
#include "stm32f10x_flash.h"            

void Flash_Erase(uint32_t addresspage);
void Flash_WriteInt(uint32_t address, uint16_t value);
void Flash_WriteNumByte(uint32_t address, uint8_t *data, int num);
void Flash_ReadNumByte(uint32_t address, int num);
	
uint8_t array[14] = {5, 2, 7, 4, 2, 6, 3, 15, 17, 20, 11, 23, 25, 19};
uint8_t buffer[14];

int main(void)
{
	Flash_Erase(0x08010000);
	Flash_WriteNumByte(0x08010000, array, 14);
	Flash_ReadNumByte(0x08010000, 12);
	Flash_WriteInt(0x08010020, 43690);	// 0xAAAA
	
	while(1);
}

void Flash_Erase(uint32_t addresspage)
{
	// Mở khóa Flash
	FLASH_Unlock();
	
	// Chờ đến khi Flash không bận
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);

	// Xóa page ở địa chỉ tương ứng
	FLASH_ErasePage(addresspage);

	// Chờ đến khi Flash không bận
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	
	// Khóa Flash
	FLASH_Lock();
}

void Flash_WriteInt(uint32_t address, uint16_t value)
{
	// Mở khóa Flash
	FLASH_Unlock();
	
	// Chờ đến khi Flash không bận
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);

	// Ghi dữ liệu vào vùng nhớ với kích thước mỗi 2 byte
	FLASH_ProgramHalfWord(address, value);

	// Chờ đến khi Flash không bận
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);
	
	// Khóa Flash
	FLASH_Lock();
}

void Flash_WriteNumByte(uint32_t address, uint8_t *data, int num)
{
	// Mở khóa Flash
	FLASH_Unlock();
	
	// Chờ đến khi Flash không bận
	while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);

	uint16_t *ptr = (uint16_t*)data;
	for (uint8_t i = 0; i < ((num + 1) / 2); i++)
	{
		// Ghi dữ liệu vào vùng nhớ với kích thước mỗi 2 byte
		FLASH_ProgramHalfWord(address + 2 * i, *ptr);

		// Chờ đến khi Flash không bận
		while (FLASH_GetFlagStatus(FLASH_FLAG_BSY) == SET);

		ptr++;
	}
	
	// Khóa Flash
	FLASH_Lock();
}

void Flash_ReadNumByte(uint32_t address, int num)
{
	// Đọc dữ liệu từ bộ nhớ và lưu vào bộ đệm
	for (int i = 0; i < num; i++) 
	{
    	buffer[i] = *(uint8_t*) address;
    	address++;
	}
}