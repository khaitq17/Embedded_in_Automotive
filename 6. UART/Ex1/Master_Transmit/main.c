#include "stm32f10x.h"            
#include "stm32f10x_usart.h"          
#include "delay.h"
#include <stdio.h>
#include <string.h>

#define TX_Pin 			GPIO_Pin_9
#define RX_Pin			GPIO_Pin_10
#define UART1_GPIO	GPIOA

int fputc(int ch, FILE *f);
void RCC_Config(void);
void GPIO_Config(void);
void UART_Config(void);
void UART_SendChar(USART_TypeDef *USARTx, char data);
void UART_SendString(USART_TypeDef *USARTx, char *str);
char UART_ReceiveChar(USART_TypeDef *USARTx);

char data;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	UART_Config();
	
	printf("Hello World!\n");
	UART_SendString(USART1, "Test\n");
	
	while(1)
	{		
		
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		{
			data = UART_ReceiveChar(USART1);
			UART_SendChar(USART1, data);
		}
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
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
	USART_Cmd(USART1, ENABLE);
}

void UART_SendChar(USART_TypeDef *USARTx, char _data)
{
	//USARTx->DR = 0x00; 
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

int fputc(int ch, FILE *f)
{
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	USART_SendData(USART1, (uint8_t)ch);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC)){}
    
  return ch;
}

