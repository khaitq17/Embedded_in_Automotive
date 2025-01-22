#include "stm32f10x.h"          
#include "stm32f10x_rcc.h"              
#include "stm32f10x_gpio.h"             
#include "stm32f10x_tim.h"              

#define SPI_CS_Pin      GPIO_Pin_4
#define SPI_SCK_Pin     GPIO_Pin_5
#define SPI_MISO_Pin    GPIO_Pin_6
#define SPI_MOSI_Pin    GPIO_Pin_7
#define SPI_GPIO        GPIOA
#define SPI_RCC         RCC_APB2Periph_GPIOA

void RCC_Config(void);
void GPIO_Config(void);
void TIMER_Config(void);
void delay_ms(uint16_t timedelay);
void SPI_Init(void);
uint8_t SPI_Slave_Receive(void);

uint8_t array[4];

int main()
{
    RCC_Config();
    GPIO_Config();
    TIMER_Config();
    SPI_Init();
    
    while (1)
    {
			for (int i = 0; i < 4; i++)
			{
				if (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin))  
				{
					array[i] = SPI_Slave_Receive(); 
				}
			}
    }
}

void RCC_Config(void)
{
    RCC_APB2PeriphClockCmd(SPI_RCC | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = SPI_MISO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);
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

void delay_ms(uint16_t timedelay)
{
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < timedelay * 10) {}
}

void SPI_Init(void)
{
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
    GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
    GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
    GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
}

uint8_t SPI_Slave_Receive(void)
{	
	uint8_t dataReceive = 0x00;//0b11000000
	uint8_t temp = 0x00;
	while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	while (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for (int i = 0; i < 8; i++)
  { 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin))
		{
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin)) 
			{
				temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}	
		dataReceive <<= 1;
		dataReceive |= temp;
    		}
	while (!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
	return dataReceive;
}

