/***************************************************************************
 * @file    Dio.h
 * @brief   Khai báo giao diện điều khiển DIO (Digital Input/Output)
 * @details File này cung cấp giao diện để điều khiển và sử dụng DIO của
 *          của vi điều khiển. Nó chứa các kiểu dữ liệu định nghĩa, cấu trúc
 *          cấu hình và các hàm liên quan đến DIO.
 * @version 1.0
 * @date    2024-11-21
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#ifndef DIO_H
#define DIO_H
              
#include "stm32f10x_gpio.h" 
#include "Std_Types.h"

/**************************************************************************
 * @typedef Dio_ChannelType
 * @brief 	Định nghĩa kiểu dữ liệu cho kênh DIO
 * @details Kiểu dữ liệu này được sử dụng cho tất cả các kênh DIO.
 **************************************************************************/
typedef uint8 Dio_ChannelType;

/**************************************************************************
 * @typedef Dio_PortType
 * @brief 	Định nghĩa kiểu dữ liệu cho cổng DIO
 * @details Kiểu dữ liệu này được sử dụng cho tất cả các cổng DIO.
 **************************************************************************/
typedef uint16 Dio_PortType;

/**************************************************************************
 * @typedef Dio_LevelType
 * @brief 	Định nghĩa kiểu dữ liệu cho mức tín hiệu trên một kênh DIO.
 * @details Kiểu dữ liệu này bao gồm các mức tín hiệu có thể có của một
 * 			kênh DIO.
 **************************************************************************/
typedef uint8 Dio_LevelType;

/**************************************************************************
 * @typedef Dio_PortLevelType
 * @brief 	Định nghĩa kiểu dữ liệu cho mức tín hiệu trên một cổng DIO.
 * @details Kiểu dữ liệu này được sử dụng để biểu diễn giá trị của tất cả 
 * 			các kênh trong một cổng DIO.
 **************************************************************************/
typedef uint16 Dio_PortLevelType;

/**************************************************************************
 * @typedef Dio_ChannelGroupType
 * @brief 	Định nghĩa kiểu dữ liệu cho nhóm kênh
 * @details Kiểu dữ liệu này được sử dụng cho một nhóm kênh liền kề trong
 * 			cùng một cổng DIO.
 **************************************************************************/
typedef struct {
	Dio_PortType port;	/* Cổng DIO chứa nhóm kênh */
	uint16 mask;		/* Mặt nạ xác định vị trí của nhóm kênh */
	uint8 offset;		/* Vị trí của nhóm kênh trên cổng, được đếm từ LSB */
} Dio_ChannelGroupType;

/**************************************************************************
 * @brief Định nghĩa các cổng DIO
 **************************************************************************/
#define DIO_PORT_A	(Dio_PortType)0		/* GPIOA */
#define DIO_PORT_B	(Dio_PortType)1		/* GPIOB */
#define DIO_PORT_C	(Dio_PortType)2		/* GPIOC */
#define DIO_PORT_D	(Dio_PortType)3		/* GPIOD */

/**************************************************************************
 * @brief 	DIO_GET_PORT
 * @details	Macro xác định cổng GPIO dựa vào ID kênh DIO.
 **************************************************************************/
#define DIO_GET_PORT(channelId) 	\
	(((channelId) < 16) ? GPIOA : 	\
	 ((channelId) < 32) ? GPIOB : 	\
	 ((channelId) < 48) ? GPIOC : 	\
	 ((channelId) < 64) ? GPIOD : NULL_PTR)

/**************************************************************************
 * @brief	DIO_GET_PIN
 * @details	Macro xác định kênh GPIO dựa vào ID kênh DIO.
 **************************************************************************/
#define DIO_GET_PIN(channelId) (1 << ((channelId) % 16))

/**************************************************************************
 * @brief Định nghĩa các chân DIO
 **************************************************************************/
#define DIO_CHANNEL_A0 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 0) 	/* GPIOA Pin 0 */
#define DIO_CHANNEL_A1 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 1) 	/* GPIOA Pin 1 */
#define DIO_CHANNEL_A2 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 2) 	/* GPIOA Pin 2 */
#define DIO_CHANNEL_A3 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 3) 	/* GPIOA Pin 3 */
#define DIO_CHANNEL_A4 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 4) 	/* GPIOA Pin 4 */
#define DIO_CHANNEL_A5 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 5) 	/* GPIOA Pin 5 */
#define DIO_CHANNEL_A6 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 6) 	/* GPIOA Pin 6 */
#define DIO_CHANNEL_A7 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 7) 	/* GPIOA Pin 7 */
#define DIO_CHANNEL_A8 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 8) 	/* GPIOA Pin 8 */
#define DIO_CHANNEL_A9 		(Dio_ChannelType)(16 * (DIO_PORT_A) + 9) 	/* GPIOA Pin 9 */
#define DIO_CHANNEL_A10		(Dio_ChannelType)(16 * (DIO_PORT_A) + 10)	/* GPIOA Pin 10 */
#define DIO_CHANNEL_A11		(Dio_ChannelType)(16 * (DIO_PORT_A) + 11)	/* GPIOA Pin 11 */
#define DIO_CHANNEL_A12		(Dio_ChannelType)(16 * (DIO_PORT_A) + 12)	/* GPIOA Pin 12 */
#define DIO_CHANNEL_A13		(Dio_ChannelType)(16 * (DIO_PORT_A) + 13)	/* GPIOA Pin 13 */
#define DIO_CHANNEL_A14		(Dio_ChannelType)(16 * (DIO_PORT_A) + 14)	/* GPIOA Pin 14 */
#define DIO_CHANNEL_A15		(Dio_ChannelType)(16 * (DIO_PORT_A) + 15)	/* GPIOA Pin 15 */

#define DIO_CHANNEL_B0  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 0)    /* GPIOB Pin 0 */
#define DIO_CHANNEL_B1  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 1)    /* GPIOB Pin 1 */
#define DIO_CHANNEL_B2  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 2)    /* GPIOB Pin 2 */
#define DIO_CHANNEL_B3  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 3)    /* GPIOB Pin 3 */
#define DIO_CHANNEL_B4  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 4)    /* GPIOB Pin 4 */
#define DIO_CHANNEL_B5  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 5)    /* GPIOB Pin 5 */
#define DIO_CHANNEL_B6  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 6)    /* GPIOB Pin 6 */
#define DIO_CHANNEL_B7  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 7)    /* GPIOB Pin 7 */
#define DIO_CHANNEL_B8  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 8)    /* GPIOB Pin 8 */
#define DIO_CHANNEL_B9  	(Dio_ChannelType)(16 * (DIO_PORT_B) + 9)    /* GPIOB Pin 9 */
#define DIO_CHANNEL_B10 	(Dio_ChannelType)(16 * (DIO_PORT_B) + 10)   /* GPIOB Pin 10 */
#define DIO_CHANNEL_B11 	(Dio_ChannelType)(16 * (DIO_PORT_B) + 11)   /* GPIOB Pin 11 */
#define DIO_CHANNEL_B12 	(Dio_ChannelType)(16 * (DIO_PORT_B) + 12)   /* GPIOB Pin 12 */
#define DIO_CHANNEL_B13 	(Dio_ChannelType)(16 * (DIO_PORT_B) + 13)   /* GPIOB Pin 13 */
#define DIO_CHANNEL_B14		(Dio_ChannelType)(16 * (DIO_PORT_B) + 14)   /* GPIOB Pin 14 */
#define DIO_CHANNEL_B15 	(Dio_ChannelType)(16 * (DIO_PORT_B) + 15)   /* GPIOB Pin 15 */

#define DIO_CHANNEL_C0  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 0)    /* GPIOC Pin 0 */
#define DIO_CHANNEL_C1  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 1)    /* GPIOC Pin 1 */
#define DIO_CHANNEL_C2  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 2)    /* GPIOC Pin 2 */
#define DIO_CHANNEL_C3  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 3)    /* GPIOC Pin 3 */
#define DIO_CHANNEL_C4  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 4)    /* GPIOC Pin 4 */
#define DIO_CHANNEL_C5  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 5)    /* GPIOC Pin 5 */
#define DIO_CHANNEL_C6  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 6)    /* GPIOC Pin 6 */
#define DIO_CHANNEL_C7  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 7)    /* GPIOC Pin 7 */
#define DIO_CHANNEL_C8  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 8)    /* GPIOC Pin 8 */
#define DIO_CHANNEL_C9  	(Dio_ChannelType)(16 * (DIO_PORT_C) + 9)    /* GPIOC Pin 9 */
#define DIO_CHANNEL_C10 	(Dio_ChannelType)(16 * (DIO_PORT_C) + 10)   /* GPIOC Pin 10 */
#define DIO_CHANNEL_C11 	(Dio_ChannelType)(16 * (DIO_PORT_C) + 11)   /* GPIOC Pin 11 */
#define DIO_CHANNEL_C12 	(Dio_ChannelType)(16 * (DIO_PORT_C) + 12)   /* GPIOC Pin 12 */
#define DIO_CHANNEL_C13 	(Dio_ChannelType)(16 * (DIO_PORT_C) + 13)   /* GPIOC Pin 13 */
#define DIO_CHANNEL_C14 	(Dio_ChannelType)(16 * (DIO_PORT_C) + 14)   /* GPIOC Pin 14 */
#define DIO_CHANNEL_C15 	(Dio_ChannelType)(16 * (DIO_PORT_C) + 15)   /* GPIOC Pin 15 */

#define DIO_CHANNEL_D0  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 0)    /* GPIOD Pin 0 */
#define DIO_CHANNEL_D1  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 1)    /* GPIOD Pin 1 */
#define DIO_CHANNEL_D2  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 2)    /* GPIOD Pin 2 */
#define DIO_CHANNEL_D3  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 3)    /* GPIOD Pin 3 */
#define DIO_CHANNEL_D4  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 4)    /* GPIOD Pin 4 */
#define DIO_CHANNEL_D5  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 5)    /* GPIOD Pin 5 */
#define DIO_CHANNEL_D6  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 6)    /* GPIOD Pin 6 */
#define DIO_CHANNEL_D7  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 7)    /* GPIOD Pin 7 */
#define DIO_CHANNEL_D8  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 8)    /* GPIOD Pin 8 */
#define DIO_CHANNEL_D9  	(Dio_ChannelType)(16 * (DIO_PORT_D) + 9)    /* GPIOD Pin 9 */
#define DIO_CHANNEL_D10 	(Dio_ChannelType)(16 * (DIO_PORT_D) + 10)   /* GPIOD Pin 10 */
#define DIO_CHANNEL_D11 	(Dio_ChannelType)(16 * (DIO_PORT_D) + 11)   /* GPIOD Pin 11 */
#define DIO_CHANNEL_D12 	(Dio_ChannelType)(16 * (DIO_PORT_D) + 12)   /* GPIOD Pin 12 */
#define DIO_CHANNEL_D13 	(Dio_ChannelType)(16 * (DIO_PORT_D) + 13)   /* GPIOD Pin 13 */
#define DIO_CHANNEL_D14 	(Dio_ChannelType)(16 * (DIO_PORT_D) + 14)   /* GPIOD Pin 14 */
#define DIO_CHANNEL_D15 	(Dio_ChannelType)(16 * (DIO_PORT_D) + 15)   /* GPIOD Pin 15 */

/**************************************************************************
 * @typedef Dio_ModeType
 * @brief 	Định nghĩa kiểu dữ liệu cho chế độ DIO
 * @details Kiểu dữ liệu này được sử dụng cho tất cả các chế độ của một kênh DIO.
 **************************************************************************/
typedef uint16 Dio_ModeType;
#define DIO_MODE_AIN			(Dio_ModeType)0X00
#define DIO_MODE_IN_FLOATING	(Dio_ModeType)0X04
#define DIO_MODE_IN_PD			(Dio_ModeType)0X28
#define DIO_MODE_IN_PU			(Dio_ModeType)0X48
#define DIO_MODE_OUT_OD			(Dio_ModeType)0X14
#define DIO_MODE_OUT_PP			(Dio_ModeType)0X10
#define DIO_MODE_AF_OD			(Dio_ModeType)0X1C
#define DIO_MODE_AF_PP			(Dio_ModeType)0X18

/**************************************************************************
 * @brief 	  Dio_ReadChannel
 * @param[in] ChannelId		ID của kênh DIO
 * @return	  Dio_LevelType Trả về STD_HIGH nếu tín hiệu trên chân DIO ở mức cao (VCC)
 *					 			   STD_LOW nếu tín hiệu trên chân DIO ở mức thấp (GND)
 **************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/**************************************************************************
 * @brief 	  Dio_WriteChannel
 * @param[in] ChannelId		ID của kênh DIO
 * @param[in] Level			Giá trị muốn ghi ra chân DIO
 * @return	  None
 **************************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/**************************************************************************
 * @brief 	  Dio_ReadPort
 * @param[in] PortId			ID của cổng DIO
 * @return	  Dio_PortLevelType	Trả về giá trị của tất cả các kênh trong cổng DIO
 **************************************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/**************************************************************************
 * @brief 	  Dio_WritePort
 * @param[in] PortId	ID của cổng DIO
 * @param[in] Level		Giá trị muốn ghi ra cổng DIO
 * @return	  None
 **************************************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/**************************************************************************
 * @brief 	  Dio_ReadChannelGroup
 * @param[in] ChannelGroupIdPtr	Con trỏ trỏ đến nhóm kênh
 * @return	  Dio_PortLevelType	Trả về một tập giá trị của các bit liền kề trong cổng.
 **************************************************************************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr);

/**************************************************************************
 * @brief 	  Dio_WriteChannelGroup
 * @param[in] ChannelGroupIdPtr		Con trỏ trỏ đến nhóm kênh
 * @param[in] Level					Giá trị muốn ghi cho nhóm kênh
 * @return	  None
 **************************************************************************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);

/**************************************************************************
 * @brief		Dio_GetVersionInfo
 * @details    	Hàm này dùng để lấy thông tin phiên bản của mô-đun.
 * @param[out] 	VersionInfo	Con trỏ đến nơi lưu trữ phiên bản của mô-đun
 * @return		None
 **************************************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/**************************************************************************
 * @brief 	  Dio_FlipChannel
 * @param[in] ChannelId		ID của kênh DIO
 * @return	  Dio_LevelType	Trả về mức tín hiệu của kênh sau khi đảo trạng thái
 *		 					STD_HIGH nếu tín hiệu trên chân DIO ở mức cao (VCC)
 *					 		STD_LOW nếu tín hiệu trên chân DIO ở mức thấp (GND)
 **************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

/**************************************************************************
 * @brief     Dio_MaskedWritePort
 * @param[in] PortId   ID của cổng DIO
 * @param[in] Level    Giá trị sẽ được ghi vào cổng
 * @param[in] Mask     Các kênh làm mặt nạ trong cổng
 * @return    None
 **************************************************************************/
void Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask);
	
/**************************************************************************
 * @brief 	  Dio_Config
 * @param[in] ChannelId		ID của kênh DIO
 * @param[in] Mode			Chế độ hoạt động của kênh DIO
 * @return	  None
 **************************************************************************/
void Dio_Config(Dio_ChannelType ChannelId, Dio_ModeType Mode);

#endif  /* DIO_H */