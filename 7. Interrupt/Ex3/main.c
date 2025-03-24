#include "stm32f10x.h"

#define TX_Pin 			GPIO_Pin_9
#define RX_Pin			GPIO_Pin_10
#define UART1_GPIO		GPIOA

void RCC_Config(void);
void GPIO_Config(void);
void UART_Config(void);
void NVIC_Config(void);
void UART_SendChar(USART_TypeDef *USARTx, char c);
void UART_SendString(USART_TypeDef *USARTx, char *str);
char UART_ReceiveChar(USART_TypeDef *USARTx);

int main(void)
{
	RCC_Config();
	GPIO_Config();
	UART_Config();
	NVIC_Config();
	
	while(1);
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
	USART_InitTypeDef UART_InitStruct;

	// Cấu hình UART
	UART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	UART_InitStruct.USART_BaudRate = 9600;
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	UART_InitStruct.USART_Parity = USART_Parity_No;
	USART_Init(USART1, &UART_InitStruct);

	// Xóa cờ ngắt nhận ban đầu
	USART_ClearFlag(USART1, USART_IT_RXNE);

	// Kích hoạt ngắt nhận cho USART1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Bật USART1
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

void UART_SendChar(USART_TypeDef *USARTx, char c)
{
	USART_SendData(USARTx, c);

	// Chờ đến khi truyền xong
	while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
}

void UART_SendString(USART_TypeDef *USARTx, char *str)
{
	while (*str)
	{
		UART_SendChar(USARTx, *str);
		while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE)); // Chờ đến khi truyền xong
		str++;
	}
}

char UART_ReceiveChar(USART_TypeDef *USARTx)
{
	// Chờ đến khi nhận xong
	while (!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE));

	return (uint8_t) USART_ReceiveData(USARTx);
}

void USART1_IRQHandler()
{
	uint8_t data = 0x00;

	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		data = USART_ReceiveData(USART1);
		
		USART_SendData(USART1, data);

		// Đợi đến khi dữ liệu truyền xong
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		
		// Xóa cờ ngắt nhận
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}