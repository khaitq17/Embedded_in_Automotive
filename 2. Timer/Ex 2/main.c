#include "stm32f10x.h"                
#include "stm32f10x_gpio.h"            
#include "stm32f10x_rcc.h"  
#include "stm32f10x_tim.h"              


void RCC_Config(void);
void GPIO_Config(void);
void TIM_Config(void);
void delay_ms(uint16_t timedelay);
void chaseLed(void);

int main()
{	
	RCC_Config();
	GPIO_Config();
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET)
		{
			delay_ms(50);
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET)
			{
				chaseLed();
			}
		}
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Init_Input;
	
	GPIO_Init_Input.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init_Input.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init_Input.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Init_Input);
	
	GPIO_InitTypeDef GPIO_Init_Output;
	
	GPIO_Init_Output.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init_Output.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init_Output.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_Init_Output);
}

void TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;	// 72MHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint16_t timedelay)
{
	TIM_SetCounter(TIM2, 0);
	while(TIM_GetCounter(TIM2) < timedelay * 10){}
}

void chaseLed(void)
{
	uint16_t LedVal = 0x08;
	for(int i=0;i<5;i++){
		LedVal = LedVal << 1;
		GPIO_Write(GPIOC, LedVal);
		delay_ms(5000);
	}
}
