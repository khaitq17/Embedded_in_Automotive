#include "stm32f10x.h"                
#include "stm32f10x_rcc.h"             
#include "stm32f10x_gpio.h"             
#include "stm32f10x_spi.h"             
#include "stm32f10x_dma.h"             
#include "stm32f10x_tim.h"             

#define MIN_PULSE_WIDTH	500	 // Độ rộng xung khi Servo quay góc 0° (nhỏ nhất)
#define MAX_PULSE_WIDTH	2500 // Độ rộng xung khi Servo quay góc 180° (lớn nhất)

#define SPI1_NSS	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO	GPIO_Pin_6
#define SPI1_MOSI	GPIO_Pin_7
#define SPI1_GPIO	GPIOA

void RCC_Config(void);
void GPIO_Config(void);
void SPI_Config(void);
uint8_t Receive1Byte(void);
void DMA_Config(void);
void TIMER_Config(void);
void delay_us(uint16_t time);
void setPWM(uint16_t angle);

uint8_t data;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	SPI_Config();
	DMA_Config();
	TIMER_Config();

	while(1)
	{
		while (GPIO_ReadInputDataBit(SPI1_GPIO, SPI1_NSS));

		if (GPIO_ReadInputDataBit(SPI1_GPIO, SPI1_NSS) == Bit_RESET)
		{
			// Nhận dữ liệu từ SPI
			data = Receive1Byte();

			// Điều chỉnh độ rộng xung thông qua giá trị góc đọc được
			setPWM(data);

			delay_us(10000);
		}
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Cấu hình GPIO cho SPI
	GPIO_InitStruct.GPIO_Pin = SPI1_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI1_MOSI | SPI1_SCK;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI1_NSS;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStruct);
	
	// Cấu hình GPIO cho Servo
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SPI_Config(void)
{
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t Receive1Byte(void)
{
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) {}
  	return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR; // Địa chỉ thanh ghi nơi DMA sẽ đọc dữ liệu từ SPI1
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&data; // Địa chỉ bộ nhớ đệm để lưu dữ liệu từ ngoại vi
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // DMA lấy dữ liệu từ ngoại vi (SPI1) và lưu vào bộ nhớ
	DMA_InitStruct.DMA_BufferSize = 1; // Truyền 1 byte dữ liệu
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // Không tăng địa chỉ ngoại vi, vì SPI1->DR luôn có cùng một địa chỉ
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // Dữ liệu ngoại vi có kích thước 1 byte
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable; // Không tăng địa chỉ bộ nhớ vì chỉ truyền 1 byte dữ liệu
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // Kích thước dữ liệu mỗi lần truyền là 1 byte
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // Chế độ Normal
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium; // Mức ưu tiên trung bình
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; // Không sử dụng chế độ truyền Memory-to-Memory
	
	// Khởi tạo cho kênh 2 của DMA1
	DMA_Init(DMA1_Channel2, &DMA_InitStruct);

	// Bật DMA1 kênh 2, bộ DMA sẽ tự động truyền nhận data cũng như ghi dữ liệu vào vùng nhớ cụ thể
	DMA_Cmd(DMA1_Channel2, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
}

void TIMER_Config(void)
{
	// Sử dụng TIM2 ở chế độ Output Compare để sử dụng PWM điều khiển Servo
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_InitStruct.TIM_Period = 20000 - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = ENABLE;
	TIM_OCInitStruct.TIM_Pulse = 1000;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_Cmd(TIM2, ENABLE);
}	

void delay_us(uint16_t time)
{
	// Sử dụng TIM3 để tạo delay
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 72 - 1;
	TIM_InitStruct.TIM_Period = 20000 - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_SetCounter(TIM3, 0);
	while (TIM_GetCounter(TIM3) < time) {}
}

void setPWM(uint16_t angle)
{
	// Quy đổi độ rộng xung cho Servo tương ứng với góc quay 
	uint16_t pulseWidth = MIN_PULSE_WIDTH + (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * angle / 180;

	// Điều chỉnh độ rộng xung cho TIM2
	TIM_SetCompare1(TIM2, pulseWidth);

	delay_us(100);
}