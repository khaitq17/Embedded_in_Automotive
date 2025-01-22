#include "stm32f10x.h"        

#define CAN_RX_PIN	GPIO_Pin_11
#define CAN_TX_PIN	GPIO_Pin_12
#define CAN_GPIO		GPIOA

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

void GPIO_Config()
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

void CAN_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	CAN_InitTypeDef CAN_InitStruct;
	
	// Cau hinh CAN
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStruct.CAN_TTCM = DISABLE;	// Time Triggered Communication Mode
	CAN_InitStruct.CAN_ABOM = ENABLE;		// Automatic Bus-Off Management
	CAN_InitStruct.CAN_AWUM = ENABLE;		// Automatic Wake-Up Mode
	CAN_InitStruct.CAN_NART = DISABLE;	// No Automatic Retransmission
	CAN_InitStruct.CAN_RFLM = DISABLE;	// Receive FIFO Locked Mode
	CAN_InitStruct.CAN_TXFP = DISABLE;	// Transmit FIFO Priority
	
	// Cau hinh thoi gian truyen
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;	// Synchronization Jump Width = 1 time quantum
	CAN_InitStruct.CAN_BS1 = CAN_BS1_6tq;	// Bit Segment 1 = 6 time quanta
	CAN_InitStruct.CAN_BS2 = CAN_BS2_8tq;	// Bit Segment 2 = 8 time quanta
	CAN_InitStruct.CAN_Prescaler = 6;			// Toc do baudrate = 36 MHz / (Prescaler * 12) = 500 Kbps 
	
	CAN_Init(CAN1, &CAN_InitStruct);
}

void CAN_FilterConfig()
{
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x123 << 5;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x00;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0xFFE0;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x00;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	
	CAN_FilterInit(&CAN_FilterInitStruct);
}

void CAN_ReceiveData(uint8_t* data)
{
	while(CAN_MessagePending(CAN1, CAN_FIFO0) < 1);
	CanRxMsg RxMessage;
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	for (int i = 0; i < RxMessage.DLC; i++) {
		data[i] = RxMessage.Data[i];
	}
	
	CAN_FIFORelease(CAN1, CAN_FIFO0);
}

