/***************************************************************************
 * @file    Can_GeneralTypes.h
 * @brief   Định nghĩa kiểu dữ liệu chung cho CAN
 * @details Tệp này chứa các kiểu dữ liệu chung liên quan đến CAN theo 
 * 			tiêu chuẩn AUTOSAR.
 * @version 1.0
 * @date    2024-12-04
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef CAN_GENERALTYPES_H
#define CAN_GENERALTYPES_H

#include "Std_Types.h"
#include "ComStack_Types.h"

/**************************************************************************
 * @typedef Can_IdType
 * @brief   Định nghĩa kiểu ID của Protocol Data Unit (PDU) trong CAN
 * @details Hai bit có trọng số cao nhất quy định kiểu khung dữ liệu:
 *          00 - Khung CAN với ID tiêu chuẩn (Standard CAN ID)
 *          01 - Khung CAN FD với ID tiêu chuẩn
 *          10 - Khung CAN với ID mở rộng (Extended CAN ID)
 *          11 - Khung CAN FD với ID mở rộng
 **************************************************************************/
typedef uint32 Can_IdType;		/* Phạm vi của Standard32Bit: 0 - 0x400007FF
								   Phạm vi của Extended32Bit: 0 - 0xDFFFFFFF */

/**************************************************************************
 * @file    Can_PduType.h
 * @brief   Định nghĩa kiểu đơn vị dữ liệu giao thức CAN (PDU)
 * @details Kiểu dữ liệu này kết hợp các thông tin: PduId (swPduHandle), 
 *          độ dài dữ liệu (length), dữ liệu PDU (sdu), và ID của CAN (id).
 **************************************************************************/
typedef struct {
    PduIdType swPduHandle;	/* ID của PDU */
    uint8 SduLength;   		/* Độ dài của SDU (tính theo byte) */
    Can_IdType CanId;		/* ID của CAN */
    uint8* SduDataPtr;   	/* Con trỏ đến dữ liệu SDU */
} Can_PduType;

/**************************************************************************
 * @typedef Can_HwHandleType
 * @brief   Định nghĩa kiểu handle của phần cứng CAN
 * @details Kiểu dữ liệu này biểu diễn các handle của đối tượng phần cứng 
 *          trong một đơn vị CAN. Đối với các đơn vị phần cứng CAN có nhiều  
 *          hơn 255 đối tượng phần cứng thì cần sử dụng phạm vi mở rộng.
 **************************************************************************/
typedef uint8 Can_HwHandleType;

/**************************************************************************
 * @typedef Can_HwType
 * @brief   Định nghĩa kiểu dữ liệu thông tin phần cứng của CAN
 * @details Kiểu dữ liệu này đại diện cho một cấu trúc dữ liệu chứa thông tin 
 * 			cơ bản của một đối tượng phần cứng CAN, bao gồm ID của PDU, 
 * 			Hardware Object Handle, và Controller ID.
 **************************************************************************/
typedef struct {
	Can_IdType id;       	/* CAN ID của PDU */
	Can_HwHandleType Hoh;	/* ID của đối tượng phần cứng tương ứng */
	uint8 ControllerId;  	/* ID của bộ điều khiển CAN, được cung cấp bởi CanIf */
} Can_HwType;

/**************************************************************************
 * @typedef Can_ErrorStateType
 * @brief   Định nghĩa kiểu trạng thái lỗi của bộ điều khiển CAN
 * @details Kiểu dữ liệu này biểu diễn trạng thái lỗi của bộ điều khiển CAN.
 **************************************************************************/
typedef enum {
	CAN_ERRORSTATE_ACTIVE,    /* Bộ điều khiển CAN hoạt động bình thường */
	CAN_ERRORSTATE_PASSIVE,   /* Bộ điều khiển CAN hoạt động nhưng không gửi các khung lỗi chủ động */
	CAN_ERRORSTATE_BUSOFF     /* Bộ điều khiển CAN không thể tham gia vào giao tiếp */
} Can_ErrorStateType;

/**************************************************************************
 * @typedef Can_ControllerStateType
 * @brief   Định nghĩa kiểu trạng thái của bộ điều khiển CAN
 * @details Các biến thuộc kiểu này sẽ được sử dụng bởi tầng CanIf để xác 
 * 			định rõ trạng thái của bộ điều khiển.
 **************************************************************************/
typedef enum {
	CAN_CS_UNINIT,   /* Bộ điều khiển CAN chưa được khởi tạo */
	CAN_CS_STARTED,  /* Bộ điều khiển CAN đang hoạt động */
	CAN_CS_STOPPED,  /* Bộ điều khiển CAN đang tạm dừng */
	CAN_CS_SLEEP     /* Bộ điều khiển CAN đang ở chế độ ngủ */
} Can_ControllerStateType;

/**************************************************************************
 * @typedef Can_ErrorType
 * @brief   Định nghĩa kiểu dữ liệu lỗi của CAN
 * @details Kiểu dữ liệu này đại diện cho một tập hợp lỗi có thể xảy ra trên 
 *          giao tiếp CAN.
 **************************************************************************/
typedef enum {
	CAN_ERROR_NONE = 0x00,        /* Không có lỗi */
	CAN_ERROR_BIT_0 = 0x01,       /* Truyền bit 0 nhưng nhận về bit 1 */
	CAN_ERROR_BIT_1 = 0x02,       /* Truyền bit 1 nhưng nhận về bit 0 */
	CAN_ERROR_BIT = 0X03,         /* Lỗi bit nhưng không xác định cụ thể loại lỗi */
	CAN_ERROR_FORM = 0x04,        /* Lỗi cấu trúc khung CAN */
	CAN_ERROR_STUFF = 0x05,       /* Lỗi bit stuffing */
	CAN_ERROR_ACK = 0x06,         /* Không nhận được bit xác nhận (ACK) */
	CAN_ERROR_CRC = 0x07,         /* Lỗi kiểm tra CRC */
	CAN_ERROR_BUSOFF = 0x08       /* Bộ điều khiển CAN vào trạng thái Bus-Off */
} Can_ErrorType;

/**************************************************************************
 * @typedef Can_TimeStampType
 * @brief   Định nghĩa kiểu dấu thời gian của CAN
 * @details Các biến thuộc kiểu này được sử dụng để lưu trữ các dấu thời gian dựa 
 *          trên thời gian tương đối.
 **************************************************************************/
typedef struct {
	uint32 seconds;       /* Giá trị giây của dấu thời gian */
	uint32 nanoseconds;   /* Giá trị nano giây của dấu thời gian */
} Can_TimeStampType;

#endif	/* CAN_GENERALTYPES_H */