/***************************************************************************
 * @file    Can.h
 * @brief   Khai báo giao diện cho giao thức CAN 
 * @details File này cung cấp giao diện để sử dụng và giao tiếp cho giao thức 
 * 			CAN của vi điều khiển. Nó chứa các kiểu dữ liệu định nghĩa, cấu 
 * 			trúc cấu hình và các hàm liên quan đến CAN.
 * @version 1.0
 * @date    2024-12-04
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef CAN_H
#define CAN_H

#include "stm32f10x_can.h"        
#include "Std_Types.h"
#include "Can_GeneralTypes.h"
#include "Can_Cfg.h"

/**************************************************************************
 * @typedef Can_ConfigType
 * @brief   Cấu trúc cấu hình giao thức CAN
 * @details Cấu trúc này chứa các thông tin cấu hình cho bộ điều khiển CAN 
 * 			và các thiết lập ảnh hưởng đến tất cả các bộ điều khiển.
 **************************************************************************/
typedef struct {
    /**************************************************************************
     * @struct 	BasicConfig
     * @brief 	Cấu hình cơ bản của CAN
     * @details	Cấu trúc này chứa các thông số cơ bản để cấu hình bộ điều khiển
     *          CAN, bao gồm: tốc độ truyền (baudrate), chế độ hoạt động và các
     *          thông số thời gian.
     **************************************************************************/
    struct {
        uint16 CAN_Prescaler;   /* @brief  	Hệ số chia tần số (Prescaler) của CAN
                                 * @details Xác định tốc độ truyền dữ liệu (baud rate) của CAN bus. 
                                 *          Giá trị hợp lệ: 1 - 1024. */
        
        uint8 CAN_Mode;        	/* @brief  	Chế độ hoạt động của CAN
                                 * @details 0: Chế độ bình thường (Normal Mode)
                                 *          1: Chế độ vòng lặp (Loopback Mode)
                                 *          2: Chế độ tĩnh (Silent Mode) */
        
        uint8 CAN_SJW;         	/* @brief   Độ rộng bước nhảy đồng bộ CAN (Synchronous Jump Width)
                                 * @details Xác định số lượng tối đa các khoảng thời gian đồng bộ.
                                 *          Giá trị hợp lệ: 1 - 4. */

        uint8 CAN_BS1;         	/* @brief   Bit Segment 1 (BS1)
                                 * @details Số lượng khoảng thời gian trong Bit Segment 1.
                                 *          Giá trị hợp lệ: 1 - 16. */
        
        uint8 CAN_BS2;         	/* @brief  	Bit Segment 2 (BS2)
                                 * @details Xác định số lượng khoảng thời gian trong Bit Segment 2.
                                 *	 		Giá trị hợp lệ: 1 - 8. */
    } BasicConfig;

    /**************************************************************************
     * @struct 	Features
     * @brief 	Tính năng mở rộng của CAN
     * @details	Cấu trúc này chứa các tính năng nâng cao cho bộ điều khiển CAN.
     **************************************************************************/
    struct {
        FunctionalState CAN_TTCM;		/* @brief  	Chế độ giao tiếp kích hoạt theo thời gian (TTCM) 
                                         * @details Cho phép (ENABLE) hoặc tắt (DISABLE) chế độ truyền thông 
                                         *          theo thời gian. Khi được kích hoạt, CAN sẽ truyền dữ liệu 
                                         *          đồng bộ theo thời gian định sẵn. */
        
        FunctionalState CAN_ABOM;		/* @brief 	Chế độ quản lý lỗi tự động (ABOM) 
                                         * @details Khi kích hoạt, CAN sẽ tự động khởi động lại sau khi phát hiện lỗi Bus-Off trên bus. */

        FunctionalState CAN_AWUM;		/* @brief 	Chế độ tự động thức tỉnh (AWUM) 
                                         * @details Khi được kích hoạt, hệ thống sẽ tự động tỉnh dậy từ chế độ 
                                         *          Sleep khi phát hiện có thông tin trên bus CAN. */

        FunctionalState CAN_NART;		/* @brief 	Chế độ không tự động gửi lại (NART) 
                                         * @details Nếu kích hoạt, CAN sẽ không tự động gửi lại các gói tin lỗi. */

        FunctionalState CAN_RFLM;		/* @brief 	Chế độ khóa bộ nhớ FIFO nhận (RFLM) 
                                         * @details Nếu được kích hoạt, FIFO sẽ bị khóa khi đầy và các tin mới 
                                         *          sẽ bị loại bỏ nếu hàng đợi đầy. */

        FunctionalState CAN_TXFP;		/* @brief 	Chế độ ưu tiên FIFO trong truyền (TXFP) 
                                         * @details Khi được kích hoạt, các thông điệp trong hàng đợi truyền 
                                         *          sẽ được gửi theo thứ tự chúng được đưa vào. */
    } Features;
} Can_ConfigType;

/**************************************************************************
 * @brief 	  Can_Init
 * @param[in] Config 	Con trỏ cấu hình CAN
 * @return	  None
 **************************************************************************/
void Can_Init(const Can_ConfigType* Config);

/**************************************************************************
 * @brief 	   Can_GetVersionInfo
 * @param[out] versioninfo 	Con trỏ đến biến lưu trữ thông tin phiên bản của mô-đun
 * @return	   None
 **************************************************************************/
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**************************************************************************
 * @brief  Can_DeInit
 * @param  None
 * @return None
 **************************************************************************/
void Can_DeInit(void);

/**************************************************************************
 * @brief     Can_SetBaudrate 
 * @param[in]  Controller 		Bộ điều khiển CAN cần thiết lập baud rate
 * @param[in]  BaudRateConfigID ID của cấu hình baud rate được tham chiếu
 * @return     Std_ReturnType Trả về E_OK nếu yêu cầu cấu hình thành công và baud rate bắt đầu được thiết lập
 *             		  				 E_NOT_OK nếu yêu cầu cấu hình không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);

/**************************************************************************
 * @brief     Can_SetControllerMode
 * @param[in] Controller   Bộ điều khiển CAN cần thay đổi trạng thái
 * @param[in] Transition   Giá trị trạng thái chuyển đổi mong muốn
 * @return    Std_ReturnType	Trả về E_OK nếu quá trình chuyển đổi trạng thái được chấp nhận
 *             						   E_NOT_OK nếu có lỗi xảy ra
 **************************************************************************/
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);

/**************************************************************************
 * @brief     Can_DisableControllerInterrupts
 * @param[in] Controller  Bộ điều khiển CAN cần vô hiệu hóa ngắt
 * @return    None
 **************************************************************************/
void Can_DisableControllerInterrupts(uint8 Controller);

/**************************************************************************
 * @brief 	  Can_EnableControllerInterrupts
 * @param[in] Controller    Bộ điều khiển CAN cần bật ngắt
 * @return	  None
 **************************************************************************/
void Can_EnableControllerInterrupts(uint8 Controller);

/**************************************************************************
 * @brief     Can_CheckWakeup
 * @param[in] Controller 		Bộ điều khiển cần kiểm tra trạng thái đánh thức
 * @return    Std_ReturnType 	Trả về E_OK nếu lệnh API được chấp nhận
 *            						   E_NOT_OK nếu lệnh API không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_CheckWakeup(uint8 Controller);

/**************************************************************************
 * @brief      Can_GetControllerErrorState
 * @param[in]  ControllerId   ID bộ điều khiển CanIf được ánh xạ đến một bộ 
 *                            điều khiển CAN cần lấy trạng thái lỗi
 * @param[out] ErrorStatePtr  Con trỏ đến vùng nhớ lưu trạng thái lỗi 
 *                            của bộ điều khiển CAN
 * @return     Std_ReturnType Trả về E_OK nếu yêu cầu lấy trạng thái lỗi được chấp nhận
 *              					 E_NOT_OK nếu yêu cầu không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr);

/**************************************************************************
 * @brief      Can_GetControllerMode
 * @details    Hàm này báo cáo trạng thái hiện tại của bộ điều khiển CAN.
 * @param[in]  Controller    		Bộ điều khiển CAN cần lấy trạng thái
 * @param[out] ControllerModePtr	Con trỏ đến vùng nhớ lưu trạng thái hiện tại
 *                                 	của bộ điều khiển CAN
 * @return     Std_ReturnType 		Trả về E_OK nếu yêu cầu lấy trạng thái được chấp nhận
 *            							   E_NOT_OK nếu yêu cầu không được chấp nhận
 **************************************************************************/
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType* ControllerModePtr);

/**************************************************************************
 * @brief      Can_GetControllerRxErrorCounter
 * @param[in]  ControllerId         Bộ điều khiển CAN cần lấy bộ đếm lỗi Rx
 * @param[out] RxErrorCounterPtr    Con trỏ đến vùng nhớ lưu giá trị bộ đếm lỗi Rx 
 *                                  của bộ điều khiển CAN
 * @return     Std_ReturnType		Trả về E_OK nếu bộ đếm lỗi Rx có sẵn
 *             							   E_NOT_OK nếu ControllerId sai hoặc bộ đếm lỗi Rx không khả dụng
 **************************************************************************/
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr);

/**************************************************************************
 * @brief      Can_GetControllerTxErrorCounter
 * @param[in]  ControllerId         Bộ điều khiển CAN cần lấy bộ đếm lỗi Tx
 * @param[out] TxErrorCounterPtr    Con trỏ đến vùng nhớ lưu giá trị bộ đếm lỗi Tx 
 *                                  của bộ điều khiển CAN
 * @return     Std_ReturnType		Trả về E_OK nếu bộ đếm lỗi Tx có sẵn
 *             							   E_NOT_OK nếu ControllerId sai hoặc bộ đếm lỗi Tx không khả dụng
 **************************************************************************/
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr);

/**************************************************************************
 * @brief      Can_GetCurrentTime
 * @param[in]  ControllerId   Chỉ mục của bộ điều khiển CAN cần lấy thời gian
 * @param[out] timeStampPtr   Con trỏ đến vùng nhớ lưu dấu thời gian hiện tại
 * @return     Std_ReturnType Trả về E_OK nếu thành công
 *             						 E_NOT_OK nếu thất bại
 **************************************************************************/
Std_ReturnType Can_GetCurrentTime(uint8 ControllerId, Can_TimeStampType* timeStampPtr);

/**************************************************************************
 * @brief     Can_EnableEgressTimeStamp
 * @param[in] Hth 	Thông tin về phần cứng truyền (HW-transmit handle) 
 *                  sẽ được sử dụng để kích hoạt dấu thời gian
 * @return    None
 **************************************************************************/
void Can_EnableEgressTimeStamp(Can_HwHandleType Hth);

/**************************************************************************
 * @brief      Can_GetEgressTimeStamp
 * @param[in]  TxPduId       ID L-PDU của CAN L-PDU mà dấu thời gian sẽ được trả về
 * @param[in]  Hth           Tay cầm truyền phần cứng (HW-transmit handle) để lấy dấu thời gian đầu ra
 * @param[out] timeStampPtr  Con trỏ lưu dấu thời gian hiện tại
 * @return     Std_ReturnType Trả về E_OK nếu thành công
 *                 					 E_NOT_OK nếu thất bại khi đọc dấu thời gian
 **************************************************************************/
Std_ReturnType Can_GetEgressTimeStamp(PduIdType TxPduId, Can_HwHandleType Hth, Can_TimeStampType* timeStampPtr);

/**************************************************************************
 * @brief      Can_GetIngressTimeStamp
 * @param[in]  Hrh 		Tay cầm nhận phần cứng (HW-receive handle) để lấy dấu thời gian đầu vào
 * @param[out] timeStampPtr  Con trỏ lưu dấu thời gian hiện tại
 * @return     Std_ReturnType Trả về E_OK nếu thành công
 *                 					 E_NOT_OK nếu thất bại khi đọc dấu thời gian
 **************************************************************************/
Std_ReturnType Can_GetIngressTimeStamp(Can_HwHandleType Hrh, Can_TimeStampType* timeStampPtr);

/**************************************************************************
 * @brief     Can_Write
 * @param[in] Hth        Thông tin về tay cầm truyền phần cứng (HW-transmit handle) sẽ được sử dụng để truyền
 * @param[in] PduInfo    Con trỏ đến bộ nhớ SDU của người dùng, độ dài dữ liệu và mã định danh
 * @return    Std_ReturnType Trả về E_OK nếu lệnh ghi được chấp nhận
 *                 			 Trả về E_NOT_OK nếu xảy ra lỗi trong quá trình phát triển
 *                 			 Trả về CAN_BUSY nếu không có bộ đệm TX phần cứng khả dụng hoặc cuộc gọi Can_Write
 *                 			 được thực hiện trước khi lệnh trước đó hoàn thành (không thể tái nhập)
 **************************************************************************/
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);

#endif /* CAN_H */