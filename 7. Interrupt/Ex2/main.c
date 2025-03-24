#include "stm32f10x.h"         
#include "stm32f10x_rcc.h"          
#include "stm32f10x_gpio.h"            
#include "stm32f10x_tim.h"            

void RCC_Config(void);
void GPIO_Config(void);
void TIMER_Config(void);
void NVIC_Config(void);
void delay(int time);

int main(void)
{
	RCC_Config();
	GPIO_Config();
	TIMER_Config();
	NVIC_Config();
	
	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(1000);
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(1000);
	}
}

void RCC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void TIMER_Config(void)
{
	// Cấu hình Timer ngắt mỗi 1 ms
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	// Bật ngắt cho TIM2
   	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	// Bật TIM2
   	TIM_Cmd(TIM2, ENABLE);
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

volatile uint16_t count;

// Hàm delay phụ thuộc vào biến count được xử lý trong trình phục vụ ngắt của TIM2
void delay(int time)
{
	count = 0;
	while (count < time) {}
}

void TIM2_IRQHandler(void)
{
	// Kiểm tra cờ ngắt TIM2
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		count++;

		// Xóa cờ ngắt TIM2
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}