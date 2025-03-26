/***************************************************************************
 * @file    Lin.h
 * @brief   LIN header file
 * @details This file contains the declarations related to the AUTOSAR 
 *					standard LIN Driver on STM32F103.
 * @version 1.0
 * @date    2024-12-06
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef LIN_H
#define LIN_H

#include "stm32f10x_usart.h" 
#include "Std_Types.h"
#include "Lin_GeneralTypes.h"
#include "Lin_Cfg.h"

/**************************************************************************
 * @typedef Lin_ConfigType
 * @brief 	Định nghĩa kiểu cấu hình LIN
 * @details Đây là kiểu dữ liệu của cấu trúc bên ngoài chứa toàn bộ thông tin  
 *          khởi tạo cho driver LIN và các thiết lập SFR ảnh hưởng đến các kênh LIN.  
 *          Con trỏ đến cấu trúc này sẽ được truyền vào hàm khởi tạo của driver LIN  
 *          để cấu hình driver, phần cứng LIN và các kênh phần cứng LIN.
 **************************************************************************/
typedef struct {
	uint32 Lin_BaudRate;				/* Tốc độ truyền LIN */
	GPIO_TypeDef* Lin_Port;				/* Cổng GPIO sử dụng cho LIN */
	uint16 Lin_TxPin;					/*  Chân LIN TX */	
	uint16 Lin_RxPin;					/*  Chân LIN RX */		
	uint8 Lin_Channel;					/* Kênh LIN */
	FunctionalState Lin_WakeupSupport;	/* Chế độ hỗ trợ Wake-up */
	IRQn_Type Lin_IRQn;					/* Ngắt LIN */
	uint32 Lin_Prescaler;				/* Hệ số chia clock cho LIN */									
	uint32 Lin_Mode;					/* Chế độ LIN */				
	uint8 Lin_TimeoutDuration;			/* Timeout phát hiện lỗi */						
} Lin_ConfigType;

/**************************************************************************
 * @brief 	  Lin_Init
 * @param[in] Config 	Con trỏ cấu hình LIN
 * @return	  None
 **************************************************************************/
void Lin_Init(const Lin_ConfigType* Config);

/**************************************************************************
 * @brief 	  Lin_CheckWakeup
 * @param[in] Channel 	Kênh LIN cần kiểm tra
 * @return    Std_ReturnType  Trả về E_OK nếu lời gọi API được chấp nhận  
 *             						 E_NOT_OK nếu lời gọi API không được chấp nhận
 **************************************************************************/
Std_ReturnType Lin_CheckWakeup(uint8 Channel);

/**************************************************************************
 * @brief 	  Lin_GetVersionInfo
 * @param[in] versionInfo	Con trỏ đến nơi lưu trữ phiên bản của mô-đun
 * @return	  None
 **************************************************************************/
void Lin_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**************************************************************************
 * @brief 	  LIN_CalculateParity
 * @param[in] id 		ID để tính toán bit parity  
 * @return	  uint8 	Trả về 2 bit parity 
 **************************************************************************/
uint8 LIN_CalculateParity(uint8 id);

/**************************************************************************
 * @brief 	  LIN_CalculateChecksum
 * @param[in] data 		Con trỏ đến dữ liệu
 * @param[in] length 	Độ dài của dữ liệu
 * @return	  uint8 	Trả về kết quả Checksum
 **************************************************************************/
uint8 LIN_CalculateChecksum(uint8 *data, uint8 length);

/**************************************************************************
 * @brief 	  Lin_SendFrame
 * @param[in] Channel      Kênh LIN cần gửi dữ liệu
 * @param[in] PduInfoPtr   Con trỏ đến PDU chứa PID, mô hình checksum,  
 *                         loại phản hồi, độ dài dữ liệu (Dl) và con trỏ SDU
 * @return    Std_ReturnType Trả về E_OK nếu lệnh gửi được chấp nhận  
 *            						E_NOT_OK nếu lệnh gửi không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr);

/**************************************************************************
 * @brief 	  Lin_GoToSleep
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu lệnh ngủ đã được chấp nhận  
 *            						E_NOT_OK nếu lệnh ngủ không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_GoToSleep(uint8 Channel);

/**************************************************************************
 * @brief 	  Lin_GoToSleepInternal
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu lệnh đã được chấp nhận  
 *            						E_NOT_OK nếu lệnh không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel);

/**************************************************************************
 * @brief 	  Lin_Wakeup
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu yêu cầu Wake-up đã được chấp nhận  
 *            						E_NOT_OK nếu yêu cầu Wake-up không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_Wakeup(uint8 Channel);

/**************************************************************************
* @brief 	  Lin_WakeupInternal
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu yêu cầu Wake-up đã được chấp nhận  
 *            						E_NOT_OK nếu yêu cầu Wake-up không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_WakeupInernal(uint8 Channel);

/**************************************************************************
 * @brief 	   Lin_GetStatus
 * @param[in]  Channel        Kênh LIN cần kiểm tra trạng thái
 * @param[out] Lin_SduPtr     Con trỏ tới con trỏ của buffer shadow hoặc bộ đệm nhận LIN  
 *                            đã được ánh xạ bộ nhớ nơi SDU hiện tại được lưu trữ
 * 
 * @return	   Lin_StatusType  
 *                - Trả về LIN_NOT_OK nếu có lỗi trong quá trình phát triển hoặc vận hành  
 *                - Trả về LIN_TX_OK nếu truyền thành công  
 *                - Trả về LIN_TX_BUSY nếu đang truyền (Header hoặc Response)  
 *                - Trả về LIN_TX_HEADER_ERROR nếu lỗi khi truyền Header, ví dụ:
 *                      - Sai lệch giữa dữ liệu gửi đi và dữ liệu đọc lại
 *                      - Lỗi parity của Identifier hoặc Lỗi bus vật lý
 *                - Trả về LIN_TX_ERROR nếu lỗi khi truyền Response, ví dụ:
 *                      - Sai lệch giữa dữ liệu gửi đi và dữ liệu đọc lại
 *                      - Lỗi bus vật lý
 *                - Trả về LIN_RX_OK nếu nhận phản hồi đúng  
 *                - Trả về LIN_RX_BUSY nếu đang nhận dữ liệu: ít nhất một byte phản hồi đã được  
 *                  nhận, nhưng byte checksum chưa được nhận
 *                - Trả về LIN_RX_ERROR nếu có lỗi khi nhận phản hồi, ví dụ:
 *                      - Lỗi framing
 *                      - Lỗi tràn bộ đệm (Overrun)
 *                      - Lỗi checksum hoặc phản hồi ngắn
 *                - Trả về LIN_RX_NO_RESPONSE nếu chưa nhận được byte phản hồi nào
 *                - Trả về LIN_OPERATIONAL nếu hoạt động bình thường, kênh LIN liên quan đã được đánh thức  
 *                  từ trạng thái LIN_CH_SLEEP và không có dữ liệu nào đã được gửi
 *                - Trả về LIN_CH_SLEEP nếu trạng thái Sleep, trong trạng thái này sẽ bật chế độ phát hiện wake-up 
 *                  từ các node slave
 **************************************************************************/
Lin_StatusType Lin_GetStatus(uint8 Channel, const uint8** Lin_SduPtr);

#endif	/* LIN_H */