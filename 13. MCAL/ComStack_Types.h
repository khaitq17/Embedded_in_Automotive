/***************************************************************************
 * @file    ComStack_Types.h
 * @brief   Tệp tiêu đề kiểu dữ liệu của ngăn xếp truyền thông AUTOSAR
 * @details Tệp này chứa tất cả các kiểu dữ liệu được sử dụng trong nhiều mô-đun 
 *          của ngăn xếp truyền thông trong phần mềm cơ bản, cũng như tất cả 
 * 			các kiểu dữ liệu của các mô-đun phần mềm cơ bản mà không phụ thuộc 
 * 			vào nền tảng và trình biên dịch.
 * @version 1.0
 * @date    2024-12-04
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

#include "Std_Types.h"

/**************************************************************************
 * @typedef PduIdType
 * @brief   Định nghĩa kiểu dữ liệu của đơn vị dữ liệu giao thức (PDU)
 * @details Kiểu dữ liệu này được sử dụng trong toàn bộ ngăn xếp truyền thông 
 *          AUTOSAR, ngoại trừ trình điều khiển bus.
 **************************************************************************/
typedef uint16 PduIdType;

/**************************************************************************
 * @typedef PduLengthType
 * @brief   Định nghĩa kiểu dữ liệu độ dài của đơn vị dữ liệu giao thức (PDU)
 * @details Kiểu dữ liệu này được sử dụng trong toàn bộ ngăn xếp truyền thông 
 *          AUTOSAR của một ECU, ngoại trừ trình điều khiển bus.
 **************************************************************************/
typedef uint32 PduLengthType;

/**************************************************************************
 * @typedef PduInfoType
 * @brief   Định nghĩa kiểu dữ liệu thông tin của đơn vị dữ liệu giao thức (PDU)
 * @details Các biến thuộc kiểu này được sử dụng để lưu trữ thông tin cơ bản 
 *          về một PDU bất kỳ, bao gồm một con trỏ trỏ đến SDU (payload), một 
 *          con trỏ đến Meta Data của PDU và độ dài tương ứng của SDU (tính theo byte).
 **************************************************************************/
typedef struct {
	uint8* SduDataPtr;			/* Con trỏ trỏ đến SDU của PDU. Kiểu của con trỏ này phụ thuộc 
									vào mô hình bộ nhớ được sử dụng tại thời điểm biên dịch. */

	uint8* MetaDataPtr;			/* Con trỏ trỏ đến meta data (Ví dụ: CAN ID, socket ID, địa chỉ 
									chẩn đoán) của PDU, bao gồm một chuỗi các mục meta data. 
									Độ dài và kiểu của các mục meta data được cấu hình tĩnh cho
									từng PDU. Các mục meta data có độ dài lớn hơn 8 bit sẽ sử 
									dụng thứ tự byte của nền tảng. */

	PduLengthType SduLength;	/* Độ dài của SDU tính theo byte */
} PduInfoType;

/**************************************************************************
 * @typedef NetworkHandleType
 * @brief   Định nghĩa kiểu dữ liệu nhận diện mạng
 * @details Các biến thuộc kiểu này sẽ được sử dụng để lưu trữ mã nhận diện 
 *          của một kênh truyền thông.
 **************************************************************************/
typedef uint8 NetworkHandleType;

#endif	/* COMSTACK_TYPES_H */