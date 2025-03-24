#include "stm32f10x.h"        

#define CAN_RX_PIN	GPIO_Pin_11
#define CAN_TX_PIN	GPIO_Pin_12
#define CAN_GPIO	GPIOA

void GPIO_Config(void);
void CAN_Config(void);
void CAN_FilterConfig(void);
void CAN_ReceiveData(uint8_t* data);

uint8_t dataReceive[8] = {0}; 

int main(void)
{
	GPIO_Config();
	CAN_Config();
	CAN_FilterConfig();
	
	while(1)
	{
		CAN_ReceiveData(dataReceive);
	}
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

void CAN_FilterConfig(void)
{
	// Cấu hình bộ lọc CAN
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	CAN_FilterInitStruct.CAN_FilterNumber = 0; // Bộ lọc 0
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask; // Dùng mặt nạ bit để lọc ID
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit; // Kích thước bộ lọc 32 bit

	// Cài đặt giá trị cho bộ lọc (32 bit)
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123 << 5;  
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x00;

	// Cài đặt giá trị cho Mask (32 bit)
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xFFE0;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x00;

	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FIFO0; // Bộ đệm FIFO0
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE; // Kích hoạt bộ lọc ID
	
	CAN_FilterInit(&CAN_FilterInitStruct);
}

void CAN_ReceiveData(uint8_t* data)
{
	// Kiểm tra bộ FIFO có dữ liệu không
	while (CAN_MessagePending(CAN1, CAN_FIFO0) < 1);

	CanRxMsg RxMessage;

	// Nhận dữ liệu từ bộ FIFO
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	// Lưu dữ liệu nhận được vào RxMessage
	for (int i = 0; i < RxMessage.DLC; i++) {
		data[i] = RxMessage.Data[i];
	}

	// Giải phóng FIFO
	CAN_FIFORelease(CAN1, CAN_FIFO0);	
}