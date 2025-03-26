/***************************************************************************
 * @file    Dio.c
 * @brief   Định nghĩa các hàm điều khiển DIO
 * @details File này triển khai các hàm để điều khiển và sử dụng DIO của 
 *          vi điều khiển. 
 * @version 1.0
 * @date    2024-11-21
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#include "Dio.h"

/**************************************************************************
 * @brief 	  Dio_ReadChannel
 * @details	  Hàm này đọc mức tín hiệu từ chân DIO.
 * @param[in] ChannelId		ID của kênh DIO
 * @return	  Dio_LevelType Trả về STD_HIGH nếu tín hiệu trên chân DIO ở mức cao (VCC)
 *					 			   STD_LOW nếu tín hiệu trên chân DIO ở mức thấp (GND)
 **************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
	GPIO_TypeDef* GPIO_Port;
	uint16 GPIO_Pin;
	
	GPIO_Port = DIO_GET_PORT(ChannelId);
	
	if (GPIO_Port == NULL_PTR) {
		return STD_LOW;
	}
	
	GPIO_Pin = DIO_GET_PIN(ChannelId);
	
	if (GPIO_ReadInputDataBit(GPIO_Port, GPIO_Pin) == Bit_SET) {
		return STD_HIGH;
	} else {
		return STD_LOW;
	}
}

/**************************************************************************
 * @brief 	  Dio_WriteChannel
 * @details   Hàm này ghi giá trị mức tín hiệu ra chân DIO.
 * @param[in] ChannelId		ID của kênh DIO
 * @param[in] Level			Giá trị muốn ghi ra chân DIO
 * @return	  None
 **************************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
	GPIO_TypeDef* GPIO_Port;
	uint16 GPIO_Pin;
	
	GPIO_Port = DIO_GET_PORT(ChannelId);
	
	if (GPIO_Port == NULL_PTR) {
		return;
	}
	
	GPIO_Pin = DIO_GET_PIN(ChannelId);
	
	if (Level == STD_HIGH) {
		GPIO_WriteBit(GPIO_Port, GPIO_Pin, Bit_SET);
	} else {
		GPIO_WriteBit(GPIO_Port, GPIO_Pin, Bit_RESET);
	}
} 
 
/**************************************************************************
 * @brief 	  Dio_ReadPort
 * @details   Hàm này đọc giá trị của toàn bộ một cổng DIO.
 * @param[in] PortId			ID của cổng DIO
 * @return	  Dio_PortLevelType	Trả về giá trị của tất cả các kênh trong cổng DIO
 **************************************************************************/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
	GPIO_TypeDef* GPIO_Port = DIO_GET_PORT((PortId) * 16);	
	
	if (GPIO_Port == NULL_PTR) {
		return 0;
	}
	
	return (Dio_PortLevelType) GPIO_ReadInputData(GPIO_Port);
}

/**************************************************************************
 * @brief 	  Dio_WritePort
 * @details   Hàm này ghi giá trị cho một cổng DIO, dùng để thiết lập mức 
 * 			  logic của cổng.
 * @param[in] PortId	ID của cổng DIO
 * @param[in] Level		Giá trị muốn ghi ra cổng DIO
 * @return	  None
 **************************************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level) 
{
	GPIO_TypeDef* GPIO_Port = DIO_GET_PORT((PortId) * 16);
	
	if (GPIO_Port == NULL_PTR) {
		return;
	}
	
	GPIO_Write(GPIO_Port, Level);
}

/**************************************************************************
 * @brief 	  Dio_ReadChannelGroup
 * @details   Hàm này đọc mức tín hiệu của một nhóm kênh trong một cổng. 
 * @param[in] ChannelGroupIdPtr	Con trỏ trỏ đến nhóm kênh
 * @return	  Dio_PortLevelType	Trả về một tập giá trị của các bit liền kề trong cổng.
 **************************************************************************/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr)
{
	GPIO_TypeDef* GPIO_Port = DIO_GET_PORT((ChannelGroupIdPtr->port) * 16);
	
	if (GPIO_Port == NULL_PTR) {
		return 0;
	}
	
	uint16 portData = GPIO_ReadInputData(GPIO_Port);
	
	return (Dio_PortLevelType) ((portData & ChannelGroupIdPtr->mask) >> ChannelGroupIdPtr->offset);
}

/**************************************************************************
 * @brief 	  Dio_WriteChannelGroup
 * @details   Hàm này ghi giá trị mức tín hiệu cho một nhóm kênh trong một cổng.
 * @param[in] ChannelGroupIdPtr 	Con trỏ trỏ đến nhóm kênh
 * @param[in] Level 				Giá trị muốn ghi cho nhóm kênh
 * @return	  None
 **************************************************************************/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level)
{
	GPIO_TypeDef* GPIO_Port = DIO_GET_PORT((ChannelGroupIdPtr->port) * 16);
	
	if (GPIO_Port == NULL_PTR) {
		return;
	}
	
	uint16 portData = GPIO_ReadInputData(GPIO_Port);
	
	portData &= ~(ChannelGroupIdPtr->mask);
	portData |= ((Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask);
	
	GPIO_Write(GPIO_Port, portData);
}

/**************************************************************************
 * @brief		Dio_GetVersionInfo
 * @details    	Hàm này dùng để lấy thông tin phiên bản của mô-đun.
 * @param[out] 	VersionInfo Con trỏ đến nơi lưu trữ phiên bản của mô-đun
 * @return		None
 **************************************************************************/
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
	if (VersionInfo == NULL_PTR) {
		return;
	}

	// Ví dụ phiên bản của module
	VersionInfo->vendorID = 0x1234;			
	VersionInfo->moduleID = 0x5678;			
	VersionInfo->sw_major_version = 1;
	VersionInfo->sw_minor_version = 0;
	VersionInfo->sw_patch_version = 0;
}

/**************************************************************************
 * @brief 	  Dio_FlipChannel
 * @details   Hàm này đảo trạng thái logic của kênh DIO (từ mức 0 thành 
 * 			  mức 1 và ngược lại).
 * @param[in] ChannelId 	ID của kênh DIO
 * @return	  Dio_LevelType	Trả về mức tín hiệu của kênh sau khi đảo trạng thái
 *		 					STD_HIGH nếu tín hiệu trên chân DIO ở mức cao (VCC)
 *					 		STD_LOW nếu tín hiệu trên chân DIO ở mức thấp (GND)
 **************************************************************************/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId) 
{
	GPIO_TypeDef* GPIO_Port;
	uint16 GPIO_Pin;
	
	GPIO_Port = DIO_GET_PORT(ChannelId);
	
	if (GPIO_Port == NULL_PTR) {
		return STD_LOW;
	}
	
	GPIO_Pin = DIO_GET_PIN(ChannelId);

	if (GPIO_ReadInputDataBit(GPIO_Port, GPIO_Pin) == Bit_SET) {
		GPIO_WriteBit(GPIO_Port, GPIO_Pin, Bit_RESET);
		return STD_LOW;
	} else {
		GPIO_WriteBit(GPIO_Port, GPIO_Pin, Bit_SET);
		return STD_HIGH;
	}
}

/**************************************************************************
 * @brief     Dio_MaskedWritePort
 * @details   Hàm này đặt giá trị cho một cổng DIO với mặt nạ được chỉ định.
 * @param[in] PortId   ID của cổng DIO
 * @param[in] Level    Giá trị sẽ được ghi vào cổng
 * @param[in] Mask     Các kênh làm mặt nạ trong cổng
 * @return    None
 **************************************************************************/
void Dio_MaskedWritePort(Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask)	
{
	GPIO_TypeDef* GPIO_Port = DIO_GET_PORT((PortId) * 16);
	
	if (GPIO_Port == NULL_PTR) {
		return;
	}
	
	uint16 portData = GPIO_ReadInputData(GPIO_Port);
	
	portData &= (~Mask);
	portData |= (Level & Mask);
	
	GPIO_Write(GPIO_Port, portData);
}

/**************************************************************************
 * @brief 	  Dio_Config
 * @details   Hàm này cấu hình các kênh DIO.
 * @param[in] ChannelId		ID của kênh DIO
 * @param[in] Mode			Chế độ hoạt động của kênh DIO
 * @return	  None
 **************************************************************************/
void Dio_Config(Dio_ChannelType ChannelId, Dio_ModeType Mode)
{
	/* Xác định cổng dựa vào ID kênh DIO */													
	GPIO_TypeDef* GPIO_Port = DIO_GET_PORT(ChannelId);		

	if (GPIO_Port == NULL_PTR) {															
		return;																									
	}		
	
	/* Kiểm tra cổng GPIO và cấp clock tương ứng */										
	if (GPIO_Port == GPIOA) {																	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	} else if (GPIO_Port == GPIOB) {													
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	} else if (GPIO_Port == GPIOC) {													
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		
	} else if (GPIO_Port == GPIOD) {													
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);		
	} 		
	
	/* Xác định chân GPIO và chế độ hoạt động */																
	uint16 GPIO_Pin = (uint16)(DIO_GET_PIN(ChannelId));					
	GPIOMode_TypeDef GPIO_Mode = (GPIOMode_TypeDef)(Mode);	
	
	/* Cấu hình GPIO */																			
	GPIO_InitTypeDef GPIO_InitStruct;		
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;											
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode;										
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO_Port, &GPIO_InitStruct);		
}