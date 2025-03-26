/***************************************************************************
 * @file    Lin_Cfg.h
 * @brief   Cấu hình các thông số cho LIN 
 * @details File này chứa cấu hình các thông số mặc định cho giao thức LIN.
 * @version 1.0
 * @date    2024-12-06
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef LIN_CFG_H
#define LIN_CFG_H

#include "Std_Types.h"
#include "stm32f10x_gpio.h"            
#include "Lin_GeneralTypes.h"

/**************************************************************************
 * @brief Định nghĩa số kênh tối đa có thể của LIN
 **************************************************************************/
#define MAX_LIN_CHANNELS	1

/**************************************************************************
 * @brief Định nghĩa ID sleep
 **************************************************************************/
#define LIN_GO_TO_SLEEP		0x17

/**************************************************************************
 * @typedef Lin_ChannelConfigType
 * @brief 	Cấu hình cho 1 kênh LIN
 **************************************************************************/
typedef struct {
	uint32 Lin_BaudRate;						/* Tốc độ truyền LIN */
	FunctionalState Lin_ChannelWakeupSupport;	/* Hỗ trợ Wake-up cho kênh LIN */
	uint8 Lin_ChannelID;						/* ID của kênh LIN */
	GPIO_TypeDef* Lin_Port; 					/* Cổng GPIO sử dụng cho LIN */
	uint16 Lin_TxPin;							/* Chân LIN TX */
	uint16 Lin_RxPin;							/* Chân LIN RX */
} Lin_ChannelConfigType;

/**************************************************************************
 * @brief Mảng lưu trữ trạng thái của các kênh LIN 
 **************************************************************************/
Lin_StatusType LinChannelState[MAX_LIN_CHANNELS] = {
	LIN_CH_SLEEP,		// Trạng thái kênh 1
	// Trạng thái kênh 2, 3, ...
};

/**************************************************************************
 * @brief Mảng lưu trữ cấu hình cho các kênh LIN
 **************************************************************************/
Lin_ChannelConfigType LinChannelConfig[MAX_LIN_CHANNELS] = {
	{
		.Lin_BaudRate = 19200,				// Tốc độ truyền chuẩn của LIN
		.Lin_ChannelWakeupSupport = ENABLE,	// Hỗ trợ Wake-up
		.Lin_ChannelID = 0,					// Kênh 0
		.Lin_Port = GPIOA,					// Cổng GPIOA
		.Lin_TxPin = GPIO_Pin_9,			// Chân TX (PA9)
		.Lin_RxPin = GPIO_Pin_10			// Chân RX (PA10)
	},
	// Cấu hình thêm các kênh khác (nếu có)
};

#endif	/* LIN_CFG_H */