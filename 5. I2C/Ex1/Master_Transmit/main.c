#include "stm32f10x.h"              

#define EEPROM_ADDRESS 0xA0

#define I2C_SCL 			GPIO_Pin_6
#define I2C_SDA 			GPIO_Pin_7
#define I2C_GPIO 			GPIOB

#define WRITE_SDA_0		GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1		GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0		GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1		GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define	READ_SDA_VAL	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum {
	NOT_OK = 0,
	OK = !NOT_OK
} status;

typedef enum {
	ACK = 0,
	NOT_ACK = !ACK
} ACK_Bit;

void RCC_Config(void);
void GPIO_Config(void);
void TIMER_Config(void);
void delay_us(uint32_t timedelay);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
status I2C_Write(uint8_t u8Data);
uint8_t I2C_Read(ACK_Bit _ACK);
void EEPROM_WriteByte(uint16_t address, uint8_t data);
uint8_t EEPROM_ReadByte(uint16_t address);

uint8_t readData;
uint16_t eepromAddress = 0x0010; 
uint8_t writeData = 0x5A;        

int main(void)
{
	

    // Kh?i t?o các c?u hình
  RCC_Config();
  GPIO_Config();
  TIMER_Config();
  I2C_Init();

    // Ghi d? li?u vào EEPROM
  EEPROM_WriteByte(eepromAddress, writeData);

    // Ð?c l?i d? li?u t? EEPROM
  readData = EEPROM_ReadByte(eepromAddress);

    // Ki?m tra k?t qu?
  if (readData == writeData) 
	{
        // N?u d?c thành công, b?n có th? d?t m?t di?m ng?t ho?c dèn LED báo hi?u
  } else 
	{
        // N?u không thành công, x? lý l?i
  }

  while(1) 
	{
        // Vòng l?p vô h?n
  }
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}

void TIMER_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_InitStruct.TIM_Period = 0xFFFF - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint32_t timedelay)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < timedelay){}
}

void I2C_Init(void)
{
	WRITE_SCL_1;
	delay_us(1);
	WRITE_SDA_1;
	delay_us(1);
}

void I2C_Start(void)
{
	WRITE_SCL_1;
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
	
	WRITE_SDA_0;
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}

void I2C_Stop(void)
{
	WRITE_SDA_0;
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}

status I2C_Write(uint8_t u8Data)
{
	uint8_t i;
	status stRet;
	
	// Truyen 8 bit data
	for (i = 0; i < 8; i++)
	{
		if (u8Data & 0x80) {	// 0b1000 0000
			WRITE_SDA_1;
		} else {
			WRITE_SDA_0;
		}
		
		delay_us(3);
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(2);
		u8Data <<= 1;
	}
	
	// Bit ACK set len 1 de chua co xac nhan
	WRITE_SDA_1;					
	delay_us(3);
	// Tao 1 xung
	WRITE_SCL_1;					
	delay_us(3);
	
	// Doc gia tri SDA, '1' la khong xac nhan, '0' la xac nhan
	if (READ_SDA_VAL) {	
		stRet = NOT_OK;				
	} else {
		stRet = OK;					
	}
	delay_us(2);
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}

uint8_t I2C_Read(ACK_Bit _ACK)
{
	uint8_t i;
	uint8_t u8Ret = 0x00;
	
	// Tao xung va doc data tu SDA
	WRITE_SDA_1;
	delay_us(3);
	
	for (i = 0; i < 8 ; i++)
	{
		u8Ret <<= 1;
		WRITE_SCL_1;
		delay_us(3);
		
		if (READ_SDA_VAL) 
		{
			u8Ret |= 0x01;
		}
		
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}
	
	if (_ACK) {	
		WRITE_SDA_0;
	} else {
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}

void EEPROM_WriteByte(uint16_t address, uint8_t data) {
    I2C_Start();
    
    // G?i d?a ch? thi?t b? v?i yêu c?u ghi
    if (I2C_Write(EEPROM_ADDRESS | ((address & 0x0700) >> 7)) != OK) {
        I2C_Stop();
        return;
    }
    
    // G?i byte d?a ch? cao
    if (I2C_Write((uint8_t)(address & 0xFF)) != OK) {
        I2C_Stop();
        return;
    }
    
    // G?i d? li?u
    if (I2C_Write(data) != OK) {
        I2C_Stop();
        return;
    }
    
    I2C_Stop();
    delay_us(5000); // Th?i gian ch? EEPROM hoàn thành ghi
}

uint8_t EEPROM_ReadByte(uint16_t address) {
    uint8_t data;

    I2C_Start();
    
    // G?i d?a ch? thi?t b? v?i yêu c?u ghi
    if (I2C_Write(EEPROM_ADDRESS | ((address & 0x0700) >> 7)) != OK) {
        I2C_Stop();
        return 0xFF; // Giá tr? tr? v? n?u có l?i
    }
    
    // G?i byte d?a ch? cao
    if (I2C_Write((uint8_t)(address & 0xFF)) != OK) {
        I2C_Stop();
        return 0xFF;
    }
    
    // Restart và chuy?n sang d?c
    I2C_Start();
    if (I2C_Write(EEPROM_ADDRESS | ((address & 0x0700) >> 7) | 0x01) != OK) {
        I2C_Stop();
        return 0xFF;
    }
    
    // Ð?c d? li?u t? EEPROM
    data = I2C_Read(NOT_ACK);
    I2C_Stop();
    
    return data;
}
