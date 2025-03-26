/***************************************************************************
 * @file    Lin.c
 * @brief   LIN source file
 * @details This file contains the definitions related to the AUTOSAR 
 *					standard LIN Driver on STM32F103.
 * @version 1.0
 * @date    2024-12-06
 * @author  Tran Quang Khai
 ***************************************************************************/
#include "Lin.h"

/**************************************************************************
 * @brief 	  Lin_Init
 * @details   Hàm này khởi tạo LIN.
 * @param[in] Config 	Con trỏ cấu hình LIN
 * @return	  None
 **************************************************************************/
void Lin_Init(const Lin_ConfigType* Config)
{
	// Kiểm tra con trỏ NULL
	if (Config == NULL_PTR) {
		return;
	}
	
	// Cấp clock cho GPIOA và UART1 sử dụng cho LIN
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	// Cấu hình GPIO cho LIN
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;	// TX (PA9)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	// RX (PA10)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Cấu hình UART cho LIN
	USART_InitTypeDef UART_InitStruct;
	
	UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UART_InitStruct.USART_BaudRate = Config->Lin_BaudRate;
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	UART_InitStruct.USART_Parity = USART_Parity_No;
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &UART_InitStruct);
	
	// Bật UART
	USART_Cmd(USART1, ENABLE);
	
	// Bật LIN
	USART_LINCmd(USART1, ENABLE);
	
	// Bật ngắt LIN 
	if (Config->Lin_IRQn != 0) {
		NVIC_EnableIRQ(Config->Lin_IRQn);
	}
}

/**************************************************************************
 * @brief 	  Lin_CheckWakeup
 * @details   Kiểm tra xem có sự kiện wake-up nào xảy ra trên kênh LIN hay không.
 * @param[in] Channel 	Kênh LIN cần kiểm tra
 * @return    Std_ReturnType  Trả về E_OK nếu lời gọi API được chấp nhận  
 *             						 E_NOT_OK nếu lời gọi API không được chấp nhận
 **************************************************************************/
Std_ReturnType Lin_CheckWakeup(uint8 Channel)
{
	// Kiểm tra kênh hợp lệ
	if (Channel >= MAX_LIN_CHANNELS) {
		return E_NOT_OK;
	}

	// Kiểm tra tín hiệu wake-up từ UART
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
        LinChannelState[Channel] = LIN_OPERATIONAL;
        return E_OK;
    }
	
	return E_OK;
}

/**************************************************************************
 * @brief 	  Lin_GetVersionInfo
 * @details   Hàm này dùng để lấy thông tin phiên bản của mô-đun.
 * @param[in] versionInfo	Con trỏ đến nơi lưu trữ phiên bản của mô-đun
 * @return	  None
 **************************************************************************/
void Lin_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	if (versioninfo != NULL_PTR) {
		versioninfo->vendorID = 0x1234;
		versioninfo->moduleID = 0x56;
		versioninfo->sw_major_version = 1;
		versioninfo->sw_minor_version = 0;
		versioninfo->sw_patch_version = 0;
	}
}

/**************************************************************************
 * @brief 	  LIN_CalculateParity
 * @details   Hàm này tính toán bit parity theo ID.
 * @param[in] id 		ID để tính toán bit parity  
 * @return	  uint8 	Trả về 2 bit parity 
 **************************************************************************/
uint8 LIN_CalculateParity(uint8 id)
{
	uint8 p0 = ((id >> 0) & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01);
	uint8 p1 = ~((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01);
	
	return (p0 | (p1 << 1)) << 6;
}

/**************************************************************************
 * @brief 	  LIN_CalculateChecksum
 * @details   Hàm này tính toàn Checksum.
 * @param[in] data 		Con trỏ đến dữ liệu
 * @param[in] length 	Độ dài của dữ liệu
 * @return	  uint8 	Trả về kết quả Checksum
 **************************************************************************/
uint8 LIN_CalculateChecksum(uint8 *data, uint8 length)
{
	uint16 checksum = 0;
	for (uint8 i = 0; i < length; i++)
	{
		checksum += data[i];
		
		if (checksum > 0xFF) 
		{
			checksum -= 0xFF;
		}
	}
	
	return ~checksum;
}

/**************************************************************************
 * @brief 	  Lin_SendFrame
 * @details   Gửi một LIN header và một LIN response (nếu cần thiết).  
 *            Hướng truyền của phản hồi (master response, slave response,  
 *            slave-to-slave communication) được xác định bởi PduInfoPtr.  
 *            Chức năng này chỉ dùng cho node LIN master.
 * @param[in] Channel      Kênh LIN cần gửi dữ liệu
 * @param[in] PduInfoPtr   Con trỏ đến PDU chứa PID, mô hình checksum,  
 *                         loại phản hồi, độ dài dữ liệu (Dl) và con trỏ SDU
 * @return    Std_ReturnType Trả về E_OK nếu lệnh gửi được chấp nhận  
 *            						E_NOT_OK nếu lệnh gửi không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr)
{
	// Kiểm tra con trỏ NULL
	if (PduInfoPtr == NULL_PTR || PduInfoPtr->SduPtr == NULL_PTR) {
		return E_NOT_OK;
	}
	
	// Bắt đầu khung LIN bằng trường Break
	USART_SendBreak(USART1);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Gửi trường Sync
	USART_SendData(USART1, 0x55);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Gửi ID kèm 2 bit parity 
	uint8 id_with_parity = PduInfoPtr->Pid | LIN_CalculateParity(PduInfoPtr->Pid);
	USART_SendData(USART1, id_with_parity);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Gửi trường Data
	for (uint8 i = 0; i < PduInfoPtr->Dl; i++) {
		USART_SendData(USART1, PduInfoPtr->SduPtr[i]);
		while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));	
	}
	
	// Tính toán và gửi Checksum
	uint8 checksum = LIN_CalculateChecksum(PduInfoPtr->SduPtr, PduInfoPtr->Dl);
	USART_SendData(USART1, (uint8)checksum);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	return E_OK;
}

/**************************************************************************
 * @brief 	  Lin_GoToSleep
 * @details   Hàm này chỉ đạo driver gửi lệnh "go-to-sleep" trên kênh LIN được chỉ định.  
 *            Chức năng này chỉ dùng cho node LIN master.
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu lệnh ngủ đã được chấp nhận  
 *            						E_NOT_OK nếu lệnh ngủ không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_GoToSleep(uint8 Channel)
{
	// Kiểm tra kệnh hợp lệ
	if (Channel >= MAX_LIN_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Bắt đầu gửi Break để gửi lệnh "go-to-sleep"
	USART_SendBreak(USART1);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Gửi trường Sync
	USART_SendData(USART1, LIN_GO_TO_SLEEP);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Thiết lập kênh LIN vào trạng thái ngủ
	LinChannelState[Channel] = LIN_CH_SLEEP;
	
	return E_OK;
}

/**************************************************************************
 * @brief 	  Lin_GoToSleepInternal
 * @details   Hàm này thiết lập trạng thái kênh thành LIN_CH_SLEEP, bật chế độ 
 * 			  phát hiện wake-up và tùy chọn thiết lập phần cứng LIN vào chế độ 
 * 			  giảm công suất (nếu phần cứng hỗ trợ).
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu lệnh đã được chấp nhận  
 *            						E_NOT_OK nếu lệnh không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel)
{
	// Kiểm tra kênh hợp lệ
	if (Channel >= MAX_LIN_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Bắt đầu gửi Break 
	USART_SendBreak(USART1);
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Cập nhật trạng thái kênh thành trạng thái ngủ
	LinChannelState[Channel] = LIN_CH_SLEEP;
	
	return E_OK;
}

/**************************************************************************
 * @brief 	  Lin_Wakeup
 * @details   Hàm này tạo xung wake-up và thiết lập trạng thái kênh thành LIN_CH_OPERATIONAL.
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu yêu cầu Wake-up đã được chấp nhận  
 *            						E_NOT_OK nếu yêu cầu Wake-up không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_Wakeup(uint8 Channel)
{
	// Kiểm tra kênh hợp lệ
	if (Channel >= MAX_LIN_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Kiểm tra trạng thái kênh, nếu đang ở trạng thái ngủ thì tiếp tục
	if (LinChannelState[Channel] != LIN_CH_SLEEP) {
		return E_NOT_OK;
	}
	
	// Gửi Wake-up với 1 bit dominant
	USART_SendData(USART1, 0x80);	// 0x10000000
	while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
	
	// Cập nhật trạng thái kênh thành trạng thái bình thường
	LinChannelState[Channel] = LIN_OPERATIONAL;
	
	return E_OK;
}

/**************************************************************************
* @brief 	  Lin_WakeupInternal
 * @details   Hàm này thiết lập trạng thái kênh thành LIN_CH_OPERATIONAL mà không tạo xung wake-up.
 * @param[in] Channel   Kênh LIN cần gửi lệnh
 * @return    Std_ReturnType Trả về E_OK nếu yêu cầu Wake-up đã được chấp nhận  
 *            						E_NOT_OK nếu yêu cầu Wake-up không được chấp nhận 
 * 									hoặc có lỗi trong quá trình phát triển/vận hành
 **************************************************************************/
Std_ReturnType Lin_WakeupInternal(uint8 Channel)
{
	// Kiểm tra hợp lệ
	if (Channel >= MAX_LIN_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Kiểm tra trạng thái kênh, nếu đang ở trạng thái ngủ thì tiếp tục
	if (LinChannelState[Channel] != LIN_CH_SLEEP) {
		return E_NOT_OK;
	}
	
	// Cập nhật trạng thái kênh thành trạng thái bình thường
	LinChannelState[Channel] = LIN_OPERATIONAL;
	
	return E_OK;
}

/**************************************************************************
 * @brief 	   Lin_GetStatus
 * @details    Hàm này lấy trạng thái của driver LIN.  
 *             Chức năng này chỉ dùng cho các node LIN master.
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
Lin_StatusType Lin_GetStatus(uint8 Channel, const uint8** Lin_SduPtr)
{
	// Kiểm tra con trỏ NULL
	if (Lin_SduPtr == NULL_PTR) {
		return LIN_NOT_OK;
	}
	
	// Kiểm tra kênh hợp lệ
	if (Channel >= MAX_LIN_CHANNELS) {
		return E_NOT_OK;
	}
		
	// Lấy trạng thái của kênh LIN 
	Lin_StatusType currentStatus = LinChannelState[Channel];
	
	// Nếu đang ở trạng thái LIN_TX_OK hoặc LIN_RX_OK, cập nhật Lin_SduPtr
	if (currentStatus == LIN_TX_OK || currentStatus == LIN_RX_OK) {
		*Lin_SduPtr = (uint8*)LinChannelState[Channel];
	} else {
		*Lin_SduPtr = NULL_PTR;
	}
	
	return currentStatus;
}