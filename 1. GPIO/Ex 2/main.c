#include "stm32f10x.h"                
#include "stm32f10x_gpio.h"            
#include "stm32f10x_rcc.h"              

void RCC_Config(void);
void GPIO_Config(void);
void delay(uint32_t timedelay);
void chaseLed(void);

int main()
{	
	RCC_Config();
	GPIO_Config();
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET)
		{
			delay(50);
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

void delay(uint32_t timedelay)
{
	for(uint32_t i=0;i<timedelay;i++){}
}

void chaseLed(void)
{
	uint16_t LedVal = 0x08;
	for(int i=0;i<5;i++){
		LedVal = LedVal << 1;
		GPIO_Write(GPIOC, LedVal);
		delay(5000000);
	}
}
