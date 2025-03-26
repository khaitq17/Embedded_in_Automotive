/***************************************************************************
 * @file    Spi_Hw.h
 * @brief   Quản lý phần cứng SPI 
 * @details File này chứa các macro và hàm để quản lý phần cứng SPI trên 
 * 			vi điều khiển STM32F103.
 * @version 1.0
 * @date    2024-11-28
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef SPI_HW_H
#define SPI_HW_H

#include "stm32f10x_gpio.h"      
#include "stm32f10x_spi.h"            
#include "Spi_Types.h"

/**************************************************************************
 * @typedef	Spi_NssStateType
 * @brief 	Kiểu dữ liệu chứa trạng thái tín hiệu của chân NSS (Slave Select/Chip Select)
 **************************************************************************/
typedef enum {
	SPI_NSS_LOW = 0,		/* Chân NSS ở mức thấp */
	SPI_NSS_HIGH = 1		/* Chân NSS ở mức cao */
} Spi_NssStateType;

/**************************************************************************
 * @brief Định nghĩa RCC cấp clock cho SPI1 và SPI2
 **************************************************************************/
#define SPI1_CLOCK_RCC	RCC_APB2Periph_SPI1		/* Clock cho SPI1 */
#define SPI2_CLOCK_RCC	RCC_APB1Periph_SPI2		/* Clock cho SPI2 */

/**************************************************************************
 * @brief Định nghĩa các chân và cổng cho SPI1
 **************************************************************************/
#define SPI1_GPIO_RCC		RCC_APB2Periph_GPIOA	/* Clock cho GPIOA (SPI1) */
#define SPI1_GPIO_PORT		GPIOA					/* GPIO cho SPI1 */
#define SPI1_NSS_PIN		GPIO_Pin_4				/* Chân NSS SPI1 (PA4) */
#define SPI1_SCK_PIN		GPIO_Pin_5				/* Chân SCK SPI1 (PA5) */		
#define SPI1_MISO_PIN		GPIO_Pin_6				/* Chân MISO SPI1 (PA6) */			
#define SPI1_MOSI_PIN		GPIO_Pin_7				/* Chân MOSI SPI1 (PA7) */			

/**************************************************************************
 * @brief Định nghĩa các chân và cổng cho SPI2
 **************************************************************************/
#define SPI2_GPIO_RCC		RCC_APB2Periph_GPIOB	/* Clock for GPIOB (SPI2) */
#define SPI2_GPIO_PORT		GPIOB					/* GPIO cho SPI2 */
#define SPI2_NSS_PIN		GPIO_Pin_12				/* Chân NSS SPI2 (PB12) */
#define SPI2_SCK_PIN		GPIO_Pin_13				/* Chân SCK SPI2 (PB13) */		
#define SPI2_MISO_PIN		GPIO_Pin_14				/* Chân MISO SPI2 (PB14) */			
#define SPI2_MOSI_PIN		GPIO_Pin_15				/* Chân MOSI SPI2 (PB15) */

/**************************************************************************
 * @brief Bật clock và cấu hình GPIO cho SPI
 * @param SPI_CHANNEL	Kênh SPI (SPI1, SPI2)
 * @param RCC_SPI		Clock cho SPI  
 * @param RCC_GPIO		Clock cho GPIO SPI
 * @param GPIO_PORT		Cổng GPIO 
 * @param GPIO_PIN		Chân GPIO 
 **************************************************************************/
#define SETUP_SPI_GPIO(SPI_CHANNEL, RCC_SPI, RCC_GPIO, GPIO_PORT, GPIO_PIN)	\
	do {																	\
		/* Cấp clock cho SPI */												\
		if ((SPI_CHANNEL) == SPI1) {										\
			RCC_APB2PeriphClockCmd((RCC_SPI), ENABLE);						\
		} else {															\
			RCC_APB1PeriphClockCmd((RCC_SPI), ENABLE);						\
		}																	\
		/* Cấp clock cho GPIO */											\
		RCC_APB2PeriphClockCmd((RCC_GPIO), ENABLE);							\
		/* Cấu hình GPIO cho SPI */											\
		GPIO_InitTypeDef GPIO_InitStruct;									\
		GPIO_InitStruct.GPIO_Pin = (GPIO_PIN);								\
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;						\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;						\
		GPIO_Init((GPIO_PORT), &GPIO_InitStruct);							\
	} while(0)

/**************************************************************************
 * @brief 	Khởi tạo đơn vị phần cứng cho SPI1
 * @param 	None
 * @return	None
 **************************************************************************/	
static inline void Spi_Hw_Init_SPI1(void)
{
	SETUP_SPI_GPIO(SPI1, SPI1_CLOCK_RCC, SPI1_GPIO_RCC, SPI1_GPIO_PORT,
									SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN);
	// Cấu hình chân NSS 
	GPIO_InitTypeDef GPIO_InitStruct;																				
	GPIO_InitStruct.GPIO_Pin = SPI1_NSS_PIN;																	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;														
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;													
	GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStruct);
}

/**************************************************************************
 * @brief 	Khởi tạo đơn vị phần cứng cho SPI2
 * @param 	None
 * @return	None
 **************************************************************************/	
static inline void Spi_Hw_Init_SPI2(void)
{
	SETUP_SPI_GPIO(SPI2, SPI2_CLOCK_RCC, SPI2_GPIO_RCC, SPI2_GPIO_PORT,
									SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN);
	// Cấu hình chân NSS
	GPIO_InitTypeDef GPIO_InitStruct;																				
	GPIO_InitStruct.GPIO_Pin = SPI2_NSS_PIN;																	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;														
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;													
	GPIO_Init(SPI2_GPIO_PORT, &GPIO_InitStruct);
}

/**************************************************************************
 * @brief 	Bật phần cứng cho bộ SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_Enable_SPI1(void)
{
	SPI_Cmd(SPI1, ENABLE);
}

/**************************************************************************
 * @brief 	Tắt phần cứng cho bộ SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_Disable_SPI1(void)
{
	SPI_Cmd(SPI1, DISABLE);
}

/**************************************************************************
 * @brief 	Bật phần cứng cho bộ SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_Enable_SPI2(void)
{
	SPI_Cmd(SPI2, ENABLE);
}

/**************************************************************************
 * @brief 	Tắt phần cứng cho bộ SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_Disable_SPI2(void)
{
	SPI_Cmd(SPI2, DISABLE);
}

/**************************************************************************
 * @brief 	Bật clock cho SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_EnableClock_SPI1(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}

/**************************************************************************
 * @brief 	Tắt clock cho SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_DisableClock_SPI1(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);
}

/**************************************************************************
 * @brief 	Bật clock cho SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_EnableClock_SPI2(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/**************************************************************************
 * @brief 	Tắt clock cho SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_DisableClock_SPI2(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
}

/**************************************************************************
 * @brief 	Khởi tạo lại phần cứng cho bộ SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_DeInit_SPI1(void)
{
	Spi_Hw_Disable_SPI1();
	Spi_Hw_DisableClock_SPI1();
}

/**************************************************************************
 * @brief 	Khởi tạo lại phần cứng cho bộ SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_DeInit_SPI2(void)
{
	Spi_Hw_Disable_SPI2();
	Spi_Hw_DisableClock_SPI2();
}

/**************************************************************************
 * @brief 	Chờ đến khi bộ đệm truyền của SPI1 trống
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI1(void)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
}

/**************************************************************************
 * @brief 	Gửi dữ liệu qua SPI1
 * @param 	DataBufferPtr Con trỏ trỏ đến bộ đệm truyền của SPI1 để truyền dữ liệu
 * @return	None
 **************************************************************************/
static inline void Spi_I2S_SendData_SPI1(const Spi_DataBufferType* DataBufferPtr)
{
	SPI_I2S_SendData(SPI1, *DataBufferPtr);
}

/**************************************************************************
 * @brief 	Chờ đến khi bộ đệm truyền của SPI2 trống
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI2(void)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
}

/**************************************************************************
 * @brief 	Gửi dữ liệu qua SPI2
 * @param 	DataBufferPtr	Con trỏ trỏ đến bộ đệm truyền của SPI2 để truyền dữ liệu
 * @return	None
 **************************************************************************/
static inline void Spi_I2S_SendData_SPI2(const Spi_DataBufferType* DataBufferPtr)
{
	SPI_I2S_SendData(SPI2, *DataBufferPtr);
}

/**************************************************************************
 * @brief 	Chờ đến khi bộ đệm nhận của SPI1 đầy
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_WaitReceiveBufferFull_SPI1(void)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
}

/**************************************************************************
 * @brief 	Nhận dữ liệu từ SPI1
 * @param 	None
 * @return	uint16	Dữ liệu nhận từ SPI1
 **************************************************************************/
static inline uint16 Spi_I2S_ReceiveData_SPI1(void)
{
	return SPI_I2S_ReceiveData(SPI1);
}

/**************************************************************************
 * @brief 	Chờ đến khi bộ đệm nhận của SPI2 đầy
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_WaitReceiveBufferFull_SPI2(void)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
}

/**************************************************************************
 * @brief 	Nhận dữ liệu từ SPI2
 * @param 	None
 * @return	uint16	Dữ liệu nhận từ SPI2
 **************************************************************************/
static inline uint16 Spi_I2S_ReceiveData_SPI2(void)
{
	return SPI_I2S_ReceiveData(SPI2);
}

/**************************************************************************
 * @brief 	Kiểm tra trạng thái phần mềm của SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI1(void)
{
	if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {
		return SPI_BUSY;
	}
	
	return SPI_IDLE;
}

/**************************************************************************
 * @brief 	Kiểm tra trạng thái phần mềm của SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI2(void)
{
	if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
		return SPI_BUSY;
	}
	
	return SPI_IDLE;
}

/**************************************************************************
 * @brief 	Kiểm tra trạng thái phần cứng của SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI1(void)
{
	if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {
		return SPI_BUSY;
	}
	
	return SPI_IDLE;
}

/**************************************************************************
 * @brief 	Kiểm tra trạng thái phần cứng của SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI2(void)
{
	if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
		return SPI_BUSY;
	}
	
	return SPI_IDLE;
}

/**************************************************************************
 * @brief 	Hủy truyền dữ liệu SPI1
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_Cancel_SPI1(void)
{
	Spi_Hw_Disable_SPI1();
	Spi_Hw_Enable_SPI1();
}

/**************************************************************************
 * @brief 	Hủy truyền dữ liệu SPI2
 * @param 	None
 * @return	None
 **************************************************************************/
static inline void Spi_Hw_Cancel_SPI2(void)
{
	Spi_Hw_Disable_SPI2();
	Spi_Hw_Enable_SPI2();
}

#endif	/* SPI_HW_H */