/***************************************************************************
 * @file    Lin_GeneralTypes.h
 * @brief   Định nghĩa kiểu dữ liệu chung cho LIN
 * @details Tệp này chứa các kiểu dữ liệu chung liên quan đến LIN theo 
 * 			tiêu chuẩn AUTOSAR.
 * @version 1.0
 * @date    2024-12-06
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef LIN_GENERALTYPES_H
#define LIN_GENERALTYPES_H

#include "Std_Types.h"
#include "ComStack_Types.h"

/**************************************************************************
 * @typedef Lin_FramePidType
 * @brief 	Định nghĩa kiểu dữ liệu đại diện cho ID khung LIN
 * @details Kiểu dữ liệu này đại diện cho tất cả các mã nhận dạng bảo vệ hợp lệ 
 *          được sử dụng bởi Lin_SendFrame().
 **************************************************************************/
typedef uint8 Lin_FramePidType;

/**************************************************************************
 * @typedef Lin_FrameCsModelType
 * @brief   Định nghĩa kiểu dữ liệu cho mô hình Checksum của khung LIN
 * @details Kiểu dữ liệu này được sử dụng để xác định loại mô hình Checksum 
 * 			được áp dụng cho khung LIN.
 **************************************************************************/
typedef enum {
	LIN_ENHANCED_CS,		/* Mô hình Checksum Enhanced (Nâng cao) */
	LIN_CLASSIC_CS			/* Mô hình Checksum Classic (Cổ điển) */
} Lin_FrameCsModelType;

/**************************************************************************
 * @typedef Lin_FrameCsModelType
 * @brief   Định nghĩa kiểu phản hồi của khung LIN
 * @details Kiểu dữ liệu này được sử dụng để chỉ định liệu bộ xử lý khung có 
 *          cần truyền phần phản hồi của khung LIN hay không.
 **************************************************************************/
typedef enum {
	LIN_FRAMERESPONSE_TX,		/* Phản hồi được tạo từ nút này */
	LIN_FRAMERESPONSE_RX, 		/* Phản hồi được nhận từ một nút khác và xử lý tại đây */
	LIN_FRAMERESPONSE_IGNORE	/* Phản hồi được tạo từ một nút khác nhưng không cần xử lý tại nút này */
} Lin_FrameResponseType;

/**************************************************************************
 * @typedef Lin_FrameDlType
 * @brief   Kiểu độ dài dữ liệu của khung LIN
 * @details Kiểu dữ liệu này được sử dụng để chỉ định số byte dữ liệu SDU cần sao chép.
 **************************************************************************/
typedef uint8 Lin_FrameDlType;

/**************************************************************************
 * @typedef Lin_PduType
 * @brief   Định nghĩa kiểu đơn vị dữ liệu giao thức LIN (Pdu)
 * @details Kiểu dữ liệu này được sử dụng để cung cấp PID, mô hình kiểm tra tổng, 
 *          độ dài dữ liệu và con trỏ SDU từ giao diện LIN đến trình điều khiển LIN.
 **************************************************************************/
typedef struct {
	Lin_FramePidType Pid;		/* PID của khung LIN */
	Lin_FrameCsModelType Cs;	/* Mô hình Checksum của khung LIN */
	uint8 Dl;					/* Độ dài dữ liệu của khung LIN */
	uint8* SduPtr;				/* Con trỏ trỏ đến dữ liệu SDU */
} Lin_PduType;

/**************************************************************************
 * @typedef Lin_StatusType
 * @brief 	Định nghĩa kiểu trạng thái của LIN
 * @details Các trạng thái hoạt động của LIN cho một kênh hoặc một frame,  
 *          được trả về bởi API Lin_GetStatus().
 **************************************************************************/
typedef enum {
	/* Giá trị trả về của quá trình truyền nhận LIN */
	LIN_NOT_OK,             /* Lỗi phát sinh trong quá trình phát triển hoặc vận hành */

	LIN_TX_OK,              /* Truyền thành công */

	LIN_TX_BUSY,            /* Đang truyền (Header hoặc Response) */

	LIN_TX_HEADER_ERROR,    /* Lỗi khi truyền Header, có thể do:
	                         *  - Sai lệch giữa dữ liệu gửi đi và dữ liệu đọc lại
	                         *  - Lỗi parity của Identifier
	                         *  - Lỗi vật lý trên bus */

	LIN_TX_ERROR,           /* Lỗi khi truyền Response, có thể do:
	                         *  - Sai lệch giữa dữ liệu gửi đi và dữ liệu đọc lại
	                         *  - Lỗi vật lý trên bus */

	LIN_RX_OK,              /* Nhận phản hồi đúng */

	LIN_RX_BUSY,            /* Đang nhận dữ liệu: Đã nhận ít nhất một byte phản hồi,  
	                         * nhưng chưa nhận được byte checksum */

	LIN_RX_ERROR,           /* Lỗi khi nhận phản hồi, có thể do:
	                         *  - Lỗi framing
	                         *  - Lỗi tràn bộ đệm (Overrun)
	                         *  - Lỗi checksum
	                         *  - Phản hồi ngắn hơn dự kiến */

	LIN_RX_NO_RESPONSE,     /* Chưa nhận được byte phản hồi nào */

	/* Giá trị trả về trạng thái của kênh LIN */ 
	LIN_OPERATIONAL,        /* Hoạt động bình thường: Kênh LIN sẵn sàng truyền  
	                         * 	header tiếp theo và không có dữ liệu cũ từ frame trước */

	LIN_CH_SLEEP,           /* Trạng thái Sleep: Hệ thống cho phép phát hiện  
	                         * 	tín hiệu wake-up từ các node slave */
} Lin_StatusType;

/**************************************************************************
 * @typedef Lin_SlaveErrorType
 * @brief 	Định nghĩa lỗi của slave trong LIN
 * @details Kiểu dữ liệu này đại diện cho các loại lỗi của slave được phát 
 * 			hiện trong quá trình nhận header và truyền/nhận phản hồi.
 **************************************************************************/
typedef enum {
	LIN_ERR_HEADER,          /* Lỗi trong header */

	LIN_ERR_RESP_STOPBIT,    /* Lỗi framing trong phản hồi */

	LIN_ERR_RESP_CHKSUM,     /* Lỗi checksum */

	LIN_ERR_RESP_DATABIT,    /* Lỗi giám sát bit dữ liệu được truyền trong phản hồi */

	LIN_ERR_NO_RESP,         /* Không có phản hồi */

	LIN_ERR_INC_RESP,        /* Phản hồi không đầy đủ */
} Lin_SlaveErrorType;

#endif	/* LIN_GENERALTYPES_H */ 