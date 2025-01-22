#include "stm32f10x.h"

#define TX_Pin 			GPIO_Pin_9
#define RX_Pin			GPIO_Pin_10
#define UART1_GPIO	GPIOA

void RCC_Config(void);
void GPIO_Config(void);
void UART_Config(void);
void NVIC_Config(void);
void UART_SendChar(USART_TypeDef *USARTx, char data);
void UART_SendString(USART_TypeDef *USARTx, char *str);
char UART_ReceiveChar(USART_TypeDef *USARTx);

int main(void)
{
	RCC_Config();
	GPIO_Config();
	UART_Config();
	NVIC_Config();
	
	while(1)
	{		
		
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = TX_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = RX_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART1_GPIO, &GPIO_InitStruct);
}

void UART_Config(void)
{
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

void UART_SendChar(USART_TypeDef *USARTx, char _data)
{
	USART_SendData(USARTx, _data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void UART_SendString(USART_TypeDef *USARTx, char *str)
{
	while (*str)
	{
		USART_SendData(USARTx, *str);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		str++;
	}
}

char UART_ReceiveChar(USART_TypeDef *USARTx)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	return (uint8_t) USART_ReceiveData(USARTx);
}

void USART1_IRQHandler()
{
	uint8_t data = 0x00;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART1);
		
		USART_SendData(USART1, data);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

