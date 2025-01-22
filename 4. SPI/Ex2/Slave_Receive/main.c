#include "stm32f10x.h"       
#include "stm32f10x_gpio.h"          
#include "stm32f10x_rcc.h"          
#include "stm32f10x_spi.h"          
#include "stm32f10x_tim.h"          

#define SPI1_NSS 	GPIO_Pin_4
#define SPI1_SCK 	GPIO_Pin_5
#define SPI1_MISO	GPIO_Pin_6
#define SPI1_MOSI	GPIO_Pin_7
#define SPI1_GPIO	GPIOA

void RCC_Slave_Config(void);
void GPIO_Slave_Config(void);
void SPI_Slave_Config(void);
void TIMER_Config(void);
void delay_ms(uint16_t time);
uint8_t SPI_Receive1Byte(void);

uint8_t array[8] = {0,0,0,0,0,0,0,0};

int main()
{
	RCC_Slave_Config();
	GPIO_Slave_Config();
	SPI_Slave_Config();
	TIMER_Config();
	while(1)
	{
		while (GPIO_ReadInputDataBit(SPI1_GPIO, SPI1_NSS));
		if (GPIO_ReadInputDataBit(SPI1_GPIO, SPI1_NSS) == 0)
		{
			for (int i = 0; i < 8; i++) {
				array[i] = SPI_Receive1Byte();
				delay_ms(1000);
			}
		}
	}
}

void RCC_Slave_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Slave_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = SPI1_MISO ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI1_MOSI | SPI1_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI1_NSS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
}

void SPI_Slave_Config(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

void TIMER_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint16_t time)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time * 10){}
}

uint8_t SPI_Receive1Byte(void)
{
	uint8_t temp;
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	temp = (uint8_t) SPI_I2S_ReceiveData(SPI1);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return temp;
}


















































































































