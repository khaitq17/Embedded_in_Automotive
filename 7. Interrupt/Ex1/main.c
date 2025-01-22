#include "stm32f10x.h"               
#include "stm32f10x_exti.h"         

void RCC_Config(void);
void GPIO_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);

uint32_t count = 0;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	EXTI_Config();
	NVIC_Config();
	
	while(1)
	{
		
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void EXTI_Config(void)
{
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_InitStruct);
}
	
void NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 2 bits for pre-emption priority, 2 bits for subpriority
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;	// 2 bits for pre-emption priority: 0x00 --> 0x03
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;	// 2 bits for subpriority: 0x00 --> 0x03
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)	// Kiem tra co ngat line 0
	{
		count++;
  }
		
	EXTI_ClearITPendingBit(EXTI_Line0);	// Xoa co ngat line 0
}
