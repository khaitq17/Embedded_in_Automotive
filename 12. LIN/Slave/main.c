#include "stm32f10x.h"                  

void LIN_UART_Init(void);
void LIN_MasterSend(uint8_t id, uint8_t *data, uint8_t length);
void LIN_SendBreak(void);
void LIN_SendSync(void);
uint8_t LIN_CalculateParity(uint8_t id);
void LIN_SendID(uint8_t id);
void LIN_SendData(uint8_t *data, uint8_t length);
uint8_t LIN_CalculateChecksum(uint8_t *data, uint8_t length);
void LIN_SendChecksum(uint8_t *data, uint8_t length);
uint8_t LIN_ReceiveData(uint8_t *buffer, uint8_t length);
void LIN_SlaveReceive(uint8_t id, uint8_t *buffer, uint8_t length);

uint8_t receivedData[4];

int main(void)
{
	LIN_UART_Init();
	
	while(1)
	{
		LIN_SlaveReceive(0x10, receivedData, 4);
	}
}
	
void LIN_UART_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	// Cấu hình GPIO 
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;	// TX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	// RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Cấu hình UART
	USART_InitTypeDef UART_InitStruct;
	
	UART_InitStruct.USART_BaudRate = 19200; // Tốc độ truyền tiêu chuẩn của LIN
	UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStruct.USART_Parity = USART_Parity_No;
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_Init(USART1, &UART_InitStruct);
	
	USART_Cmd(USART1, ENABLE);
	USART_LINCmd(USART1, ENABLE);
}

void LIN_MasterSend(uint8_t id, uint8_t *data, uint8_t length)
{
	LIN_SendBreak(); // Gửi trường Break
	LIN_SendSync(); // Gửi trường Sync
	LIN_SendID(id); // Gửi trường ID kèm 2 bit parity
	LIN_SendData(data, length); // Gửi trường Data
	LIN_SendChecksum(data, length); // Gửi trường Checksum
}

void LIN_SendBreak(void)
{
	// Gửi trường Break
	USART_SendBreak(USART1);
}

void LIN_SendSync(void)
{
	// Gửi trường Sync, giá trị cố định 0x55
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	USART_SendData(USART1, 0x55);	// 0x01010101
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

uint8_t LIN_CalculateParity(uint8_t id)
{
	// Tính toán 2 bit parity dựa trên ID
	uint8_t p0 = ((id >> 0) & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01) ^ ((id >> 4) & 0x01);
	uint8_t p1 = ~((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01);
	
	return (p0 | (p1 << 1)) << 6;
}

void LIN_SendID(uint8_t id)
{
	// Gửi trường ID kèm theo 2 bit parity
	uint8_t id_with_parity = id | LIN_CalculateParity(id);
	USART_SendData(USART1, id_with_parity);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

void LIN_SendData(uint8_t *data, uint8_t length)
{
	// Gửi trường Data
	for (uint8_t i = 0; i < length; i++)
	{
		USART_SendData(USART1, data[i]);
		while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	}
}

uint8_t LIN_CalculateChecksum(uint8_t *data, uint8_t length)
{
	// Tính toán checksum dựa trên dữ liệu
	uint16_t checksum = 0;
	for (uint8_t i = 0; i < length; i++)
	{
		checksum += data[i];
		
		if (checksum > 0xFF) 
		{
			checksum -= 0xFF;
		}
	}
	
	return ~checksum;
}

void LIN_SendChecksum(uint8_t *data, uint8_t length)
{
	// Gửi trường Checksum
	uint8_t checksum = LIN_CalculateChecksum(data, length);
	USART_SendData(USART1, checksum);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

uint8_t LIN_ReceiveData(uint8_t *buffer, uint8_t length)
{
	// Nhận dữ liệu từ USART1 và lưu vào bộ đệm
	for (uint8_t i = 0; i < length; i++)
	{
		while (!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
		buffer[i] = USART_ReceiveData(USART1);
	}
	
	// Tính toán lại checksum và trả về
	return LIN_CalculateChecksum(buffer, length);
}

void LIN_SlaveReceive(uint8_t id, uint8_t *buffer, uint8_t length)
{
	uint8_t receiveId;
	
	// Chờ nhận ID từ Master
	while (!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));

	receiveId = USART_ReceiveData(USART1);
	
	// Kiểm tra ID, nếu khớp với ID yêu cầu thì tiếp tục nhận dữ liệu
	if (receiveId == (id | LIN_CalculateParity(id)))
	{
		// Nhận dữ liệu từ Master
		LIN_ReveiceData(buffer, length);
		
		// Kiểm tra checksum
		uint8_t receivedChecksum = USART_ReceiveData(USART1);
		if (receivedChecksum == LIN_CalculateChecksum(buffer, length))
		{
			// Dữ liệu hợp lệ, xử lý dữ liệu nhận được trong buffer
		}
		else 
		{
			// Xử lý lỗi checksum
		}
	}
	else 
	{
		// Xử lý lỗi Parity hoặc ID không đúng
	}
}