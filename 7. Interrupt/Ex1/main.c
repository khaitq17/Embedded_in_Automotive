#include "stm32f10x.h"               
#include "stm32f10x_exti.h"         

void RCC_Config(void);
void GPIO_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);

volatile uint32_t count = 0;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	EXTI_Config();
	NVIC_Config();
	
	while(1);
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
	
	EXTIInit_Struct.EXTI_Line = EXTI_Line0; // Cấu hình ngắt Line 0
	EXTIInit_Struct.EXTI_Mode = EXTI_Mode_Interrupt; // Chế độ ngắt
	EXTIInit_Struct.EXTI_Trigger = EXTI_Trigger_Falling; // Ngắt khi có sự thay đổi tín hiệu điện áp từ mức 1 xuống mức 0
	EXTIInit_Struct.EXTI_LineCmd = ENABLE; // Bật ngắt
	
	EXTI_Init(&EXTI_InitStruct);
}
	
void NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 2 bit pre-emption priority, 2 bit sub priority
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;	
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_IRQHandler(void)
{
	// Kiểm tra cờ ngắt Line 0
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)	
	{
		count++;
  	}
		
	// Xoá cờ ngắt Line 0
	EXTI_ClearITPendingBit(EXTI_Line0);	
}