#include "stm32f10x.h"  

#define I2C_SCL		GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7
#define I2C_GPIO	GPIOB

static uint8_t slaveAddr = 0x35;

void GPIO_Config(void);
void I2C_Config(void);
void I2C_Start(uint8_t address);
void I2C_Stop(void);
void Send_I2C_Data(uint8_t data);
uint8_t Read_I2C_Data(uint8_t ack);
void TIM_Config(void);
void delay_ms(uint16_t time_delay);
void EEPROM_Write(uint8_t mem_addr, uint8_t data);
uint8_t EEPROM_Read(uint8_t mem_addr);

int main(void)
{
	GPIO_Config();
	I2C_Config();
	
	EEPROM_Write(0x10, 0x55);  // Ghi 0x55 vào địa chỉ 0x10
    delay_ms(5);  
	
    uint8_t read_data = EEPROM_Read(0x10);

	while(1);
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = I2C_SCL | I2C_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}

void I2C_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C; // Chế độ I2C Fast Mode
	I2C_InitStruct.I2C_ClockSpeed = 100000; // Tốc độ 100 kHz
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // Thời gian xung thấp / xung cao = 2
	I2C_InitStruct.I2C_OwnAddress1 = 0x00; // Địa chỉ khi thiết bị là Slave
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; // Sử dụng ACK
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7 bit địa chỉ
	
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
}

void I2C_Start(uint8_t address)
{
	// Tạo tín hiệu Start
	I2C_GenerateSTART(I2C1, ENABLE);

	// Đợi đến khi bus I2C rảnh
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(I2C1, (uint8_t) (address << 1), I2C_Direction_Transmitter);

	// Đợi xác nhận của Slave với yêu cầu ghi của Master
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void I2C_Stop(void)
{
	// Tạo tín hiệu Stop
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void Send_I2C_Data(uint8_t data)
{
	I2C_SendData(I2C1, data);

	// Đợi đến khi truyền xong 1 byte data 
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t Read_I2C_Data(uint8_t ack)
{
	// Kiểm tra ACK
	if (ack)
        I2C_AcknowledgeConfig(I2C1, ENABLE);
    else
        I2C_AcknowledgeConfig(I2C1, DISABLE);

	// Đợi đến khi Master nhận đủ 1 byte data
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	return I2C_ReceiveData(I2C1);
}

void TIM_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;
	TIM_InitStruct.TIM_Period = 0xFFFF - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint16_t time_delay)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < time_delay * 10) {}
}

void EEPROM_Write(uint8_t mem_addr, uint8_t data)
{
	// Gửi tín hiệu START + Địa chỉ Slave
	I2C_Start(slaveAddr);

	// Gửi địa chỉ bộ nhớ EEPROM cần ghi	
	Send_I2C_Data(mem_addr);

	// Gửi dữ liệu cần ghi vào địa chỉ trong bộ nhớ EEPROM
    Send_I2C_Data(data);

	// Gửi tín hiệu STOP để kết thúc giao tiếp
	I2C_Stop();
}

uint8_t EEPROM_Read(uint8_t mem_addr)
{
	// Gửi tín hiệu Start + Địa chỉ Slave
	I2C_Start(slaveAddr);

	// Gửi địa chỉ bộ nhớ EEPROM cần ghi
	Send_I2C_Data(mem_addr);

	// Gửi dữ liệu cần ghi vào địa chỉ trong bộ nhớ EEPROM
	I2C_Start(slaveAddr);

	// Đọc dữ liệu từ EEPROM (gửi NACK)
	uint8_t data = Read_I2C_Data(0);	

	// Gửi tín hiệu Stop để kết thúc giao tiếp
	I2C_Stop();

	return data;
}