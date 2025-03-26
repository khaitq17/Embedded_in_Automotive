/***************************************************************************
 * @file    Can.c
 * @brief   Định nghĩa các hàm điều khiển cho giao thức CAN
 * @details File này triển khai các hàm để sử dụng và giao tiếp cho giao thức 
 * 			CAN của vi điều khiển.
 * @version 1.0
 * @date    2024-12-04
 * @author  Tran Quang Khai
 ***************************************************************************/
#include "Can.h"
#include "stm32f10x_gpio.h"             

/**************************************************************************
 * @brief Biến static lưu trữ trạng thái của bộ điều khiển CAN
 **************************************************************************/
static Can_ControllerStateType CanControllerState = CAN_CS_UNINIT;

/**************************************************************************
 * @brief 	  Can_Init
 * @details   Hàm này khởi tạo CAN.
 * @param[in] Config 	Con trỏ cấu hình CAN
 * @return	  None
 **************************************************************************/
void Can_Init(const Can_ConfigType* Config)
{
	// Cấp clock cho GPIOA và CAN1 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// Cấu hình GPIO cho CAN RX (PA11) and CAN TX (PA12)
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;		/* CAN RX */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;		/* CAN TX */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Cấu hình CAN 
	CAN_InitTypeDef CAN_InitStruct;
	// Cấu hình cơ bản
	CAN_InitStruct.CAN_Prescaler = Config->BasicConfig.CAN_Prescaler;
	CAN_InitStruct.CAN_Mode = Config->BasicConfig.CAN_Mode;
	CAN_InitStruct.CAN_SJW = Config->BasicConfig.CAN_SJW;
	CAN_InitStruct.CAN_BS1 = Config->BasicConfig.CAN_BS1;
	CAN_InitStruct.CAN_BS2 = Config-> BasicConfig.CAN_BS2;
	// Các tính năng bổ sung
	CAN_InitStruct.CAN_TTCM = Config->Features.CAN_TTCM;
	CAN_InitStruct.CAN_ABOM = Config->Features.CAN_ABOM;
	CAN_InitStruct.CAN_AWUM = Config->Features.CAN_AWUM;
	CAN_InitStruct.CAN_NART = Config->Features.CAN_NART;
	CAN_InitStruct.CAN_RFLM = Config->Features.CAN_RFLM;
	CAN_InitStruct.CAN_TXFP = Config->Features.CAN_TXFP;
	
	if (CAN_Init(CAN1, &CAN_InitStruct) != CAN_InitStatus_Success) {
		// Xử lý lỗi nếu khởi tạo thất bại
	}

	CanControllerState = CAN_CS_STOPPED;
}

/**************************************************************************
 * @brief 	   Can_GetVersionInfo
 * @details    Hàm này trả về thông tin phiên bản của mô-đun.
 * @param[out] versioninfo 	Con trỏ đến biến lưu trữ thông tin phiên bản của mô-đun
 * @return	   None
 **************************************************************************/
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
	if (versioninfo != NULL_PTR) {
		versioninfo->vendorID = 0x1234;
		versioninfo->moduleID = 0x01;
		versioninfo->sw_major_version = 1;
		versioninfo->sw_minor_version = 0;
		versioninfo->sw_patch_version = 0;
	}
}

/**************************************************************************
 * @brief 	Can_DeInit
 * @details Hàm này khởi tạo lại CAN.
 * @param	None
 * @return	None
 **************************************************************************/
void Can_DeInit(void)
{
	// Kiểm tra xem CAN đã khởi tạo chưa, nếu chưa thì không thể khởi tạo lại
	if (CanControllerState == CAN_CS_UNINIT) {
		return;
	}

	// Thiết lập các thanh ghi CAN về mặc định
	CAN_DeInit(CAN1);
	
	// Vô hiệu hóa tất cả các ngắt liên quan đến CAN1
	CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_TME | CAN_IT_ERR, DISABLE);
	
	// Tắt clock CAN1 và GPIOA để tiết kiệm năng lượng
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);
	
	// Xóa cấu hình của CAN TX và CAN RX về mặc định
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;					
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		// Để ở chế độ Analog để tiết kiệm năng lượng
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**************************************************************************
 * @brief     Can_SetBaudrate
 * @details   Thiết lập tốc độ truyền (baud rate) cho bộ điều khiển CAN.
 *            Trong một số trường hợp, bộ điều khiển có thể cần được reset 
 *            để áp dụng cấu hình mới.
 * 
 * @param[in]  Controller 		Bộ điều khiển CAN cần thiết lập baud rate
 * @param[in]  BaudRateConfigID ID của cấu hình baud rate được tham chiếu
 * 
 * @return     Std_ReturnType Trả về E_OK nếu yêu cầu cấu hình thành công và baud rate bắt đầu được thiết lập
 *             		  				 E_NOT_OK nếu yêu cầu cấu hình không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
{
	// Kiểm tra có phải CAN1 không
	if (Controller != 0) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Kiểm tra trạng thái của bộ điều khiển CAN
    if (CanControllerState != CAN_CS_STOPPED) {
        return E_NOT_OK;
    }

	// Đặt về cấu hình mặc định
	CAN_InitTypeDef CAN_InitStruct;
	CAN_StructInit(&CAN_InitStruct);
	
	// Thiết lập các thông số thời gian dựa vào baud rate
	// Baudrate = 72 MHz / (12 * Prescaler)
	switch (BaudRateConfigID) 
	{
		case 125: // 125 kbps
		{
			CAN_InitStruct.CAN_Prescaler = 48;		// Hệ số chia clock cho baud rate 125 kbps
			CAN_InitStruct.CAN_BS1 = CAN_BS1_14tq;
			CAN_InitStruct.CAN_BS2 = CAN_BS2_6tq;
			CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
			break;
		}
		
		case 250: // 250 kbps
		{
			CAN_InitStruct.CAN_Prescaler = 24;		// Hệ số chia clock cho baud rate 250 kbps
			CAN_InitStruct.CAN_BS1 = CAN_BS1_14tq;
			CAN_InitStruct.CAN_BS2 = CAN_BS2_6tq;
			CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
			break;
		}
		
		case 500: // 500 kbps
		{
			CAN_InitStruct.CAN_Prescaler = 12;		// Hệ số chia clock cho baud rate 500 kbps
			CAN_InitStruct.CAN_BS1 = CAN_BS1_14tq;
			CAN_InitStruct.CAN_BS2 = CAN_BS2_6tq;
			CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
			break;
		}
		
		case 1000: // 1 Mbps
		{
			CAN_InitStruct.CAN_Prescaler = 6;		// Hệ số chia clock cho baud rate 1 Mbps
			CAN_InitStruct.CAN_BS1 = CAN_BS1_14tq;
			CAN_InitStruct.CAN_BS2 = CAN_BS2_6tq;
			CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
			break;
		}
		
		default:
			return E_NOT_OK;
	}
	
	if (CAN_Init(CAN1, &CAN_InitStruct) != CAN_InitStatus_Success) {
		return E_NOT_OK;
	}
		
	return E_OK;
}

/**************************************************************************
 * @brief     Can_SetControllerMode
 * @details   Hàm này kích hoạt quá trình chuyển đổi trạng thái của bộ điều khiển CAN
 *            theo giá trị chuyển đổi được chỉ định thông qua phần mềm.
 *
 * @param[in] Controller   Bộ điều khiển CAN cần thay đổi trạng thái
 * @param[in] Transition   Giá trị trạng thái chuyển đổi mong muốn
 * 
 * @return    Std_ReturnType	Trả về E_OK nếu quá trình chuyển đổi trạng thái được chấp nhận
 *             						   E_NOT_OK nếu có lỗi xảy ra
 **************************************************************************/
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition)
{
	// Kiểm tra có phải CAN1 không
	if (Controller != 0) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Chuyển trạng thái mới dựa vào Transition 
	switch (Transition) 
	{
		case CAN_CS_STARTED:
		{
			if (CanControllerState != CAN_CS_STOPPED) {
                return E_NOT_OK;
            }

			CAN1->MCR &= ~CAN_MCR_SLEEP;   // Thoát chế độ Sleep
      		CAN1->MCR &= ~CAN_MCR_INRQ;    // Bật chế độ Normal
			break;
		}
		
		case CAN_CS_STOPPED: 
		{
			if (CanControllerState == CAN_CS_UNINIT) {
                return E_NOT_OK;
            }

			// Cho phép chuyển từ STARTED hoặc SLEEP về STOPPED
      		CAN1->MCR |= CAN_MCR_INRQ;  // Chuyển về Initialization Mode
			break;
		}
		
		case CAN_CS_SLEEP:
		{
			if (CanControllerState != CAN_CS_STOPPED) {
                return E_NOT_OK;
            }

			CAN1->MCR |= CAN_MCR_SLEEP;  // Bật chế độ Sleep
			break;
		}
		
		case CAN_CS_UNINIT:
			break;
			
		default:
			return E_NOT_OK;
	}
	
	// Cập nhật trạng thái
	CanControllerState = Transition;

	return E_OK;
}

/**************************************************************************
 * @brief     Can_DisableControllerInterrupts
 * @details   Hàm này sẽ vô hiệu hóa toàn bộ các ngắt liên quan đến bộ điều 
 * 			  khiển CAN.
 * @param[in] Controller  Bộ điều khiển CAN cần vô hiệu hóa ngắt
 * @return    None
 **************************************************************************/
void Can_DisableControllerInterrupts(uint8 Controller)
{
	// Kiểm tra có phải CAN1 không
	if (Controller != 0) {	// Controller 0 (CAN1)
		return;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN
	if (CanControllerState != CAN_CS_STARTED) {
        return;
    }

	/* Vô hiệu hóa các ngắt của CAN1 */
    CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);	// Tắt ngắt truyền dữ liệu
    CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);	// Tắt ngắt FIFO0 đầy
    CAN_ITConfig(CAN1, CAN_IT_FMP1, DISABLE);	// Tắt ngắt FIFO1 đầy
    CAN_ITConfig(CAN1, CAN_IT_ERR, DISABLE);	// Tắt ngắt lỗi chung
    CAN_ITConfig(CAN1, CAN_IT_WKU, DISABLE);	// Tắt ngắt thức dậy
    CAN_ITConfig(CAN1, CAN_IT_SLK, DISABLE);	// Tắt ngắt chế độ Sleep
    
    /* Xóa cờ ngắt */
    CAN_ClearITPendingBit(CAN1, CAN_IT_TME);	// Xóa cờ ngắt truyền dữ liệu
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);	// Xóa cờ ngắt FIFO0
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);	// Xóa cờ ngắt FIFO1
    CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);	// Xóa cờ ngắt lỗi
    CAN_ClearITPendingBit(CAN1, CAN_IT_WKU);	// Xóa cờ ngắt thức dậy
    CAN_ClearITPendingBit(CAN1, CAN_IT_SLK);	// Xóa cờ ngắt chế độ ngủ
}

/**************************************************************************
 * @brief 	  Can_EnableControllerInterrupts
 * @details   Hàm này cho phép tất cả các ngắt được kích hoạt.
 * @param[in] Controller    Bộ điều khiển CAN cần bật ngắt
 * @return	  None
 **************************************************************************/
void Can_EnableControllerInterrupts(uint8 Controller)
{
	// Kiểm tra có phải CAN1 không
	if (Controller != 0) {	// Controller 0 (CAN1)
		return;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN
	if (CanControllerState != CAN_CS_STARTED) {
        return;
    }

	// Bật các ngắt của CAN1
    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);		// Bật ngắt truyền dữ liệu
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);	// Bật ngắt FIFO0 đầy
    CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);	// Bật ngắt FIFO1 đầy
    CAN_ITConfig(CAN1, CAN_IT_ERR, ENABLE);		// Bật ngắt lỗi chung
    CAN_ITConfig(CAN1, CAN_IT_WKU, ENABLE);		// Bật ngắt thức dậy
    CAN_ITConfig(CAN1, CAN_IT_SLK, ENABLE);		// Bật ngắt chế độ Sleep
}

/**************************************************************************
 * @brief     Can_CheckWakeup
 * @details   Hàm này kiểm tra xem bộ điều khiển được chỉ định có xảy ra 
 * 			  đánh thức hay không.
 * @param[in] Controller 		Bộ điều khiển cần kiểm tra trạng thái đánh thức
 * @return    Std_ReturnType 	Trả về E_OK nếu lệnh API được chấp nhận
 *            						   E_NOT_OK nếu lệnh API không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_CheckWakeup(uint8 Controller)
{
	// Kiểm tra có phải CAN1 không
	if (Controller != 0) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN, chưa khởi tạo không nên kiểm tra Wake-up
	if (CanControllerState == CAN_CS_UNINIT) {
        return E_NOT_OK;
    }

    // Kiểm tra bit Wake-up trong thanh ghi CAN_MSR
    if (CAN1->MSR & CAN_MSR_WKUI) {
        // Xóa cờ ngắt wake-up
        CAN1->MSR &= ~CAN_MSR_WKUI;
        return E_OK;
    }
	
	return E_NOT_OK;
}

/**************************************************************************
 * @brief      Can_GetControllerErrorState
 * @details    Hàm này lấy trạng thái lỗi của bộ điều khiển CAN.
 * @param[in]  ControllerId   ID bộ điều khiển CanIf được ánh xạ đến một bộ 
 *                            điều khiển CAN cần lấy trạng thái lỗi
 * @param[out] ErrorStatePtr  Con trỏ đến vùng nhớ lưu trạng thái lỗi 
 *                            của bộ điều khiển CAN
 * @return     Std_ReturnType Trả về E_OK nếu yêu cầu lấy trạng thái lỗi được chấp nhận
 *              					 E_NOT_OK nếu yêu cầu không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr)
{
	// Kiểm tra có phải CAN1 không và kiểm tra con trỏ NULL
	if (ControllerId != 0 || ErrorStatePtr == NULL_PTR) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN
	if (CanControllerState == CAN_CS_UNINIT) {
        return E_NOT_OK;
    }

	// Lấy trạng thái lỗi từ thanh ghi ESR (Error Status Register)
	uint32 errorStatus = CAN1->ESR;
	
	// Xác định trạng thái lỗi
	if (errorStatus & CAN_ESR_BOFF) {
		*ErrorStatePtr = CAN_ERRORSTATE_BUSOFF;		// Bộ điều khiển CAN đang ở trạng thái Bus-Off
	} else if (errorStatus & CAN_ESR_EPVF) {
		*ErrorStatePtr = CAN_ERRORSTATE_PASSIVE;	// Bộ điều khiển CAN đang ở trạng thái Passive
	} else {
		*ErrorStatePtr = CAN_ERRORSTATE_ACTIVE;		// Bộ điều khiển CAN đang ở trạng thái Active
	}
	
	return E_OK;
}

/**************************************************************************
 * @brief      Can_GetControllerMode
 * @details    Hàm này báo cáo trạng thái hiện tại của bộ điều khiển CAN.
 * @param[in]  Controller    		Bộ điều khiển CAN cần lấy trạng thái
 * @param[out] ControllerModePtr 	Con trỏ đến vùng nhớ lưu trạng thái hiện tại
 *                                 	của bộ điều khiển CAN
 * @return     Std_ReturnType 		Trả về E_OK nếu yêu cầu lấy trạng thái được chấp nhận
 *            							   E_NOT_OK nếu yêu cầu không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType* ControllerModePtr)
{
	// Kiểm tra có phải CAN1 không và kiểm tra con trỏ NULL
	if (Controller != 0 || ControllerModePtr == NULL_PTR) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN
	if (CanControllerState == CAN_CS_UNINIT) {
        return E_NOT_OK;
    }

	// Lấy trạng thái hiện tại ở thanh ghi MSR (Master Status Register)
	uint32 controllerStatus = CAN1->MSR;
	
	// Xác định trạng thái hiện tại
	if (controllerStatus & CAN_MSR_SLAK) {
		*ControllerModePtr = CAN_CS_SLEEP;			// Bộ điều khiển CAN đang ở chế độ ngủ
	} else if (controllerStatus & CAN_MSR_INAK) {
		*ControllerModePtr = CAN_CS_STOPPED;		// Bộ điều khiển CAN đang tạm dừng
	} else  else {
		*ControllerModePtr = CAN_CS_STARTED;		// Bộ điều khiển CAN đang hoạt động
	}
	
	return E_OK;
}

/**************************************************************************
 * @brief      Can_GetControllerRxErrorCounter
 * @details    Hàm này trả về bộ đếm lỗi Rx của bộ điều khiển CAN.
 *             Giá trị này có thể không khả dụng trên tất cả các bộ điều khiển CAN,
 *             trong trường hợp đó, hàm sẽ trả về E_NOT_OK.
 * @param[in]  ControllerId         Bộ điều khiển CAN cần lấy bộ đếm lỗi Rx
 * @param[out] RxErrorCounterPtr    Con trỏ đến vùng nhớ lưu giá trị bộ đếm lỗi Rx 
 *                                  của bộ điều khiển CAN
 * @return     Std_ReturnType		Trả về E_OK nếu bộ đếm lỗi Rx có sẵn
 *             							   E_NOT_OK nếu ControllerId sai hoặc bộ đếm lỗi Rx không khả dụng
 **************************************************************************/
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr)
{
	// Kiểm tra có phải CAN1 không và kiểm tra con trỏ NULL
	if (ControllerId != 0 || RxErrorCounterPtr == NULL_PTR) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN
	if (CanControllerState == CAN_CS_UNINIT) {
        return E_NOT_OK;
    }

	// Lấy bộ đếm lỗi Rx từ thanh ghi ESR (Error Status Register)
	*RxErrorCounterPtr = (uint8)((CAN1->ESR & CAN_ESR_REC) >> 24);	// Rx Error Counter bit [31:24]
	
	return E_OK;
}

/**************************************************************************
 * @brief      Can_GetControllerTxErrorCounter
 * @details    Hàm này trả về bộ đếm lỗi Tx của bộ điều khiển CAN.
 *             Giá trị này có thể không khả dụng trên tất cả các bộ điều khiển CAN,
 *             trong trường hợp đó, hàm sẽ trả về E_NOT_OK.
 * @param[in]  ControllerId         Bộ điều khiển CAN cần lấy bộ đếm lỗi Tx
 * @param[out] TxErrorCounterPtr    Con trỏ đến vùng nhớ lưu giá trị bộ đếm lỗi Tx 
 *                                  của bộ điều khiển CAN
 * @return     Std_ReturnType		Trả về E_OK nếu bộ đếm lỗi Tx có sẵn
 *             							   E_NOT_OK nếu ControllerId sai hoặc bộ đếm lỗi Tx không khả dụng
 **************************************************************************/
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr)
{
	// Kiểm tra có phải CAN1 không và kiểm tra con trỏ NULL
	if (ControllerId != 0 || TxErrorCounterPtr == NULL_PTR) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Kiểm trạng thái bộ điều khiển CAN
	if (CanControllerState == CAN_CS_UNINIT) {
        return E_NOT_OK;
    }

	// Lấy bộ đếm lỗi Tx từ thanh ghi ESR (Error Status Register)
	*TxErrorCounterPtr = (uint8)((CAN1->ESR & CAN_ESR_TEC) >> 16);	// Tx Error Counter bit [23:16]
	
	return E_OK;
}

/**************************************************************************
 * @brief      Can_GetCurrentTime
 * @details    Hàm này trả về giá trị thời gian từ thanh ghi phần cứng (HW) 
 *             theo khả năng của phần cứng.
 * @param[in]  ControllerId   Chỉ mục của bộ điều khiển CAN cần lấy thời gian
 * @param[out] timeStampPtr   Con trỏ đến vùng nhớ lưu dấu thời gian hiện tại
 * @return     Std_ReturnType Trả về E_OK nếu thành công
 *             						 E_NOT_OK nếu thất bại
 **************************************************************************/
Std_ReturnType Can_GetCurrentTime(uint8 ControllerId, Can_TimeStampType* timeStampPtr)
{
	// Kiểm tra có phải CAN1 không và kiểm tra con trỏ NULL
	if (ControllerId != 0 || timeStampPtr == NULL_PTR) {	// Controller 0 (CAN1)
		return E_NOT_OK;
	}
	
	// Giả sử 1 biến lưu trữ thời gian hiện tại
	uint32 currentTime;
	
	timeStampPtr->seconds = (currentTime / 1000000000U);
	timeStampPtr->nanoseconds = (currentTime % 1000000000U);
	
	return E_OK;
}

/**************************************************************************
 * @brief     Can_EnableEgressTimeStamp
 * @details   Hàm này kích hoạt ghi dấu thời gian đầu ra trên HTH chuyên dụng.
 * @param[in] Hth 	Thông tin về phần cứng truyền (HW-transmit handle) 
 *                  sẽ được sử dụng để kích hoạt dấu thời gian
 * @return    None
 **************************************************************************/
void Can_EnableEgressTimeStamp(Can_HwHandleType Hth)
{
	if (Hth > 2) return;  // STM32F103 có tối đa 3 TX mailbox (Hth = 0,1,2)
	
	// Bật timestamp cho từng mailbox dựa trên Hth
	CAN1->sTxMailBox[Hth].TDTR = (1 << 8); // Bật bit TXTS (bit 8)
}

/**************************************************************************
 * @brief      Can_GetEgressTimeStamp
 * @details    Hàm này đọc lại dấu thời gian đầu ra trên một đối tượng tin nhắn chuyên dụng.
 * @param[in]  TxPduId       ID L-PDU của CAN L-PDU mà dấu thời gian sẽ được trả về
 * @param[in]  Hth           ID duy nhất gán cho Hardware Transmit Object để lấy dấu thời gian đầu ra
 * @param[out] timeStampPtr  Con trỏ lưu dấu thời gian hiện tại
 * @return     Std_ReturnType Trả về E_OK nếu thành công
 *                 					 E_NOT_OK nếu thất bại khi đọc dấu thời gian
 **************************************************************************/
Std_ReturnType Can_GetEgressTimeStamp(PduIdType TxPduId, Can_HwHandleType Hth, Can_TimeStampType* timeStampPtr)
{
	// Kiểm tra mailbox và con trỏ NULL
	if (Hth > 2 || timeStampPtr == NULL_PTR) {
        return E_NOT_OK;
    }

	// Lấy timestamp từ mailbox Hth
    uint32 timestamp = CAN1->sTxMailBox[Hth].TDTR & 0xFFFF;  // Timestamp lưu ở 16 bit cuối

    // Giả sử hệ thống đang dùng timestamp dạng microseconds
    timeStampPtr->seconds = timestamp / 1000000U;
    timeStampPtr->nanoseconds = (timestamp % 1000000U) * 1000U;

	return E_OK;
}

/**************************************************************************
 * @brief      Can_GetIngressTimeStamp
 * @details    Hàm này đọc lại dấu thời gian đầu vào trên một đối tượng tin nhắn chuyên dụng.
 * @param[in]  Hrh 		ID duy nhất gán cho Hardware Receive Object để lấy dấu thời gian đầu vào
 * @param[out] timeStampPtr  Con trỏ lưu dấu thời gian hiện tại
 * @return     Std_ReturnType Trả về E_OK nếu thành công
 *                 					 E_NOT_OK nếu thất bại khi đọc dấu thời gian
 **************************************************************************/
Std_ReturnType Can_GetIngressTimeStamp(Can_HwHandleType Hrh, Can_TimeStampType* timeStampPtr)
{
	// Kiểm tra input hợp lệ
    if (Hrh > 1 || timeStampPtr == NULL_PTR) {  
        return E_NOT_OK;
    }

    // Lấy timestamp từ mailbox FIFO
    uint32 timestamp;
    if (Hrh == 0) {
        timestamp = (CAN1->sFIFOMailBox[0].RDTR & 0xFFFF);  // Timestamp từ FIFO0
    } else {
        timestamp = (CAN1->sFIFOMailBox[1].RDTR & 0xFFFF);  // Timestamp từ FIFO1
    }

    // Giả sử timestamp lưu theo đơn vị microseconds
    timeStampPtr->seconds = timestamp / 1000000U;
    timeStampPtr->nanoseconds = (timestamp % 1000000U) * 1000U;

	return E_OK;
}

/**************************************************************************
 * @brief     Can_Write
 * @details   Hàm này được CanIf gọi để truyền một thông điệp CAN đến CanDrv để gửi đi.
 * @param[in] Hth        ID duy nhất gán cho Hardware Transmit Object được sử dụng để truyền
 * @param[in] PduInfo    Con trỏ đến bộ nhớ SDU của người dùng, độ dài dữ liệu và mã định danh
 * @return    Std_ReturnType Trả về E_OK nếu lệnh ghi được chấp nhận
 *                 			 Trả về E_NOT_OK nếu xảy ra lỗi trong quá trình phát triển
 *                 			 Trả về CAN_BUSY nếu không có bộ đệm TX phần cứng khả dụng hoặc cuộc gọi Can_Write
 *                 			 được thực hiện trước khi lệnh trước đó hoàn thành (không thể tái nhập) 
 **************************************************************************/
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
	// Kiểm tra con trỏ NULL
	if (PduInfo == NULL_PTR || PduInfo->SduDataPtr == NULL_PTR) {
		return E_NOT_OK;
	}
	
	// Kiểm tra Hth hợp lệ (STM32F1 có 3 mailbox: 0, 1, 2)
    if (Hth > 2) {
        return E_NOT_OK;
    }
	
	// Kiểm tra trạng thái của bộ điều khiển CAN
	if (CanControllerState != CAN_CS_STARTED) {
        return E_NOT_OK;
    }

	CanTxMsg TxMessage;
	
	TxMessage.StdId = PduInfo->CanId; // ID thông điệp
	TxMessage.IDE = CAN_Id_Standard; // Standard ID
	TxMessage.RTR = CAN_RTR_Data; // Sử dụng Data Frame
	TxMessage.DLC = PduInfo->SduLength; // Độ dài dữ liệu

	// Lưu dữ liệu truyền vào TxMessage
	for (uint8 i = 0; i < TxMessage.DLC; i++) {
		TxMessage.Data[i] = PduInfo->SduDataPtr[i];
	}
	
	// Sử dụng mailbox để truyền dữ liệu đi
	uint8 mailbox = CAN_Transmit(CAN1, &TxMessage);
	
	// Trả về CAN_BUSY nếu không còn mailbox trống
	if (mailbox == CAN_TxStatus_NoMailBox) {
		return CAN_BUSY;
	}
	
	// Chờ đến khi truyền xong
	uint32_t timeout = 1000000;
	while ((CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok) && --timeout);
	
	// Thoát nếu hết thời gian chờ
	if (timeout == 0) {
		return E_NOT_OK;
	}
		
	return E_OK;
}