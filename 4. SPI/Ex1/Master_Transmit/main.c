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
void Clock(void);
void SPI_Init(void);
void SPI_Master_Transmit(uint8_t u8Data);
uint8_t SPI_Slave_Receive(void);

uint8_t array[] = {7, 8, 4, 2};

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
				SPI_Master_Transmit(array[i]);
        delay_ms(1000);  
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
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(SPI_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = SPI_MISO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

void Clock(void)
{
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
    delay_ms(1); 
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
    delay_ms(1);
}

void SPI_Init(void)
{
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
    GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET); 
    GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
    GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
}

void SPI_Master_Transmit(uint8_t u8Data) {//0b10010000
	uint8_t u8Mask = 0x80;// 0b10000000
	uint8_t tempData;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
	
	for (int i = 0; i < 8; i++) {
		tempData = u8Data & u8Mask;
		if (tempData) {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		} else {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		u8Data <<= 1;
		Clock();
	}
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	delay_ms(1);
}

