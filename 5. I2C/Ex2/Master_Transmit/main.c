#include "stm32f10x.h"  

#define I2C_SCL		GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7
#define I2C_GPIO	GPIOB

static uint8_t slaveAdd = 0x35;

void GPIO_Config(void);
void I2C_Config(void);
void I2C_Start(uint8_t address);
void Send_I2C_Data(uint8_t data);
uint8_t Read_I2C_Data(void);
void TIM_Config(void);
void delay_ms(uint16_t time_delay);


int main(void)
{
	GPIO_Config();
	I2C_Config();
	
	
	
	while(1)
	{
		
	}
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
	
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
}

void I2C_Start(uint8_t address)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	// Waiting for flag
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, (uint8_t) (address << 1), I2C_Direction_Transmitter);
	// And check the transmitting
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void Send_I2C_Data(uint8_t data)
{
	I2C_SendData(I2C1, data);
	// wait for the data trasnmitted flag
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t Read_I2C_Data(void)
{
	uint8_t data = I2C_ReceiveData(I2C1);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return data;
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
	while(TIM_GetCounter(TIM2) < time_delay * 10);
}
