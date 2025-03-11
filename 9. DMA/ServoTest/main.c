#include "stm32f10x.h"       

#define MIN_PULSE_WIDTH 500
#define MAX_PULSE_WIDTH 2500

void TIM_Config(void);
void delay_ms(uint32_t milliseconds);
void setPWM(uint16_t angle);

int main(void)
{
	TIM_Config();
	
	while(1)
	{
		setPWM(0);
		delay_ms(2000);
		
		setPWM(90);
		delay_ms(2000);
		
		setPWM(180);
		delay_ms(2000);
		
	}
}

void TIM_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_InitStruct.TIM_Period = 20000 - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 1000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}

void setPWM(uint16_t angle)
{
	uint16_t pulseWidth = MIN_PULSE_WIDTH + (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * angle / 180;
	TIM_SetCompare1(TIM2, pulseWidth);
}

void delay_ms(uint32_t milliseconds)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;
	TIM_InitStruct.TIM_Period = 0xFFFF - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
	
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_SetCounter(TIM3, 0);
	while (TIM_GetCounter(TIM3) < milliseconds * 10) {}
}