#include "stm32f10x.h"       
#include "stm32f10x_rcc.h"          
#include "stm32f10x_gpio.h"          
#include "stm32f10x_adc.h"          
#include "stm32f10x_spi.h"          
#include "stm32f10x_tim.h"   
#include "kalman.h"

#define ADC_Pin		GPIO_Pin_1
#define ADC_GPIO	GPIOA

#define SPI1_NSS	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO	GPIO_Pin_6
#define SPI1_MOSI	GPIO_Pin_7
#define SPI1_GPIO	GPIOA


void RCC_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
void SPI_Config(void);
void SPI_Send1Byte(uint8_t data);
void TIMER_Config(void);
void delay_us(uint16_t time);

uint16_t val;
float updateVal;
uint8_t angle;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	ADC_Config();
	SPI_Config();
	TIMER_Config();
	SimpleKalmanFilter(1, 2, 0.001f);
	
	while(1)
	{
		val = ADC_GetConversionValue(ADC1);
		updateVal = updateEstimate((float)val);
		delay_us(100);
		
		angle = (uint8_t)((updateVal * 180.0f) / 4096.0f); // ADC 2^12
		
		SPI_Send1Byte(angle);
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Cấu hình GPIO cho ADC
	GPIO_InitStruct.GPIO_Pin = ADC_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_GPIO, &GPIO_InitStruct);
	
	// Cấu hình GPIO cho SPI
	GPIO_InitStruct.GPIO_Pin = SPI1_SCK | SPI1_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI1_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI1_NSS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                      
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void SPI_Config(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

void SPI_Send1Byte(uint8_t data)
{
	GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);
	
	SPI_I2S_SendData(SPI1, data);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);
}

void TIMER_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_InitStruct.TIM_Period = 20000 - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint16_t time)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < time) {}
}