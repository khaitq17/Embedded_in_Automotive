#include "stm32f10x.h"                
#include "stm32f10x_gpio.h"             
#include "stm32f10x_rcc.h"              
#include "stm32f10x_tim.h"             

void RCC_Config(void);
void GPIO_Config(void);
void TIM_Config(void);
void delay_ms(uint16_t timedelay);
void chaseLed(uint8_t loop);

int main()
{	
	RCC_Config();
	GPIO_Config();
	TIM_Config();

	while(1)
	{
		chaseLed(3);
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 72MHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1; // Bộ đếm tăng lên sau mỗi 72 chu kỳ xung
	TIM_TimeBaseInitStruct.TIM_Period = 20000 - 1; // Reset về 0 khi bộ đếm đếm đến 20000
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // Chế độ đếm lên
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint16_t timedelay)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < timedelay * 1000) {}
}

void chaseLed(uint8_t loop)
{
	uint16_t LedVal;

	GPIO_Write(GPIOC, 0x00);

	for (uint8_t i = 0; i < loop; i++) {
		LedVal = 0x08;
		for (uint8_t j = 0; j < 5; j++) {
			LedVal <<= 1;
			GPIO_Write(GPIOC, LedVal);
			delay_ms(5000);	// 5000 ms = 5 s
			}
		}
}