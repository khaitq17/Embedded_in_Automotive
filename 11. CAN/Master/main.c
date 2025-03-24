#include "stm32f10x.h"        

#define CAN_RX_PIN	GPIO_Pin_11
#define CAN_TX_PIN	GPIO_Pin_12
#define CAN_GPIO	GPIOA

void GPIO_Config(void);
void CAN_Config(void);
void CAN_TransmitData(uint8_t* data, uint8_t length);

uint8_t dataTransmit[8] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80}; 

int main(void)
{
	GPIO_Config();
	CAN_Config();

	CAN_TransmitData(dataTransmit, 8);
	
	while(1);
}

void GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_GPIO, &GPIO_InitStruct);
}

void CAN_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	CAN_InitTypeDef CAN_InitStruct;
	
	// Cấu hình CAN
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal; // Chế độ bình thường
	CAN_InitStruct.CAN_TTCM = DISABLE;	// Không sử dụng chế độ giao tiếp kích hoạt theo thời gian
	CAN_InitStruct.CAN_ABOM = ENABLE;	// Tự động ngắt ra khỏi bus nếu ở trạng thái Bus-Off
	CAN_InitStruct.CAN_AWUM = ENABLE;	// Tự động thức dậy khi nhận được một thông điệp trên bus CAN
	CAN_InitStruct.CAN_NART = DISABLE;	// Node tự động truyền lại khung dữ liệu nếu không nhận được ACK
	CAN_InitStruct.CAN_RFLM = DISABLE;	// Dữ liệu mới sẽ ghi đè lên dữ liệu cũ nếu FIFO đầy
	CAN_InitStruct.CAN_TXFP = DISABLE;	// Khung dữ liệu sẽ được truyền theo thứ tự, không theo mức ưu tiên
	
	// Cấu hình thời gian truyền
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;	// Thời gian trễ phần cứng = 1 tq
	CAN_InitStruct.CAN_BS1 = CAN_BS1_6tq;	// Thời gian đồng bộ đầu frame truyền = 6 tq
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;	// Thời gian đồng bộ cuối frame truyền = 8 tq
	CAN_InitStruct.CAN_Prescaler = 12;		// Tốc độ baudrate = 72 MHz / (Prescaler * 12) = 500 Kbps 
	
	CAN_Init(CAN1, &CAN_InitStruct);
}

void CAN_TransmitData(uint8_t* data, uint8_t length)
{
	CanTxMsg TxMessage;
	
	TxMessage.StdId = 0x123; // ID thông điệp
	TxMessage.IDE = CAN_Id_Standard; // CAN Standard 
	TxMessage.RTR = CAN_RTR_Data; // Sử dụng Data Frame
	TxMessage.DLC = length; // Độ dài dữ liệu

	// Lưu dữ liệu truyền vào TxMessage
	for (int i = 0; i < length; i++) {
		TxMessage.Data[i] = data[i];
	}

	// Sử dụng mailbox để truyền dữ liệu đi
	uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);

	// Chờ đến khi truyền xong
	while (CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok);
}