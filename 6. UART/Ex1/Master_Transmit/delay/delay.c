#include "delay.h"

/*
 * Function: TIMER_Config
 * Description: Configure Timer
 * Input: None
 * Return: None 
 */
void TIMER_Config(void)
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
	
/*
 * Function: delay_ms
 * Description: Delay milliseconds
 * Input: None
 * Return: time-- Time delay
 */
void delay_ms(uint16_t time)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time * 10){}
}

/*
 * Function: delay_us
 * Description: Delay microseconds
 * Input: None
 * Return: time-- Time delay
 */
void delay_us(uint16_t time)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < time * 0.01){}
}




