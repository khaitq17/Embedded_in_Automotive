/***************************************************************************
 * @file    Std_Types.h
 * @brief   Định nghĩa các kiểu dữ liệu tiêu chuẩn cho AUTOSAR
 * @details File này chứa tất cả các kiểu dữ liệu và ký hiệu dành cho chuẩn 
 *          AUTOSAR. Các kiểu dữ liệu này được trừu tượng hóa để trở nên 
 *          độc lập với nền tảng và trình biên dịch.
 * @version 1.0
 * @date    2024-11-21
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#ifndef STD_TYPES_H
#define STD_TYPES_H

/**************************************************************************
 * @brief	Định nghĩa kiểu dữ liệu độc lập với nền tảng
 * @details Định nghĩa các kiểu dữ liệu với dấu và kích thước của các kiểu 
 * 			dữ liệu dữ liệu đó theo tiêu chuẩn AUTOSAR.
 **************************************************************************/
typedef unsigned          	char 	uint8;	    /* Số nguyên không dấu 8 bit */
typedef   signed          	char 	sint8;	    /* Số nguyên có dấu 8 bit */
typedef unsigned short     	 int 	uint16;	    /* Số nguyên không dấu 16 bit */
typedef   signed short     	 int 	sint16;	    /* Số nguyên có dấu 16 bit */
typedef unsigned             int 	uint32;	    /* Số nguyên không dấu 32 bit */
typedef   signed           	 int 	sint32;	    /* Số nguyên có dấu 32 bit */
typedef unsigned       long long 	uint64;	    /* Số nguyên không dấu 64 bit */
typedef   signed       long long 	sint64;	    /* Số nguyên có dấu 64 bit */

typedef float	float32;	/* Số thực 32 bit */
typedef	double	float64;	/* Số thực 64 bit */								

/**************************************************************************
 * @brief	Định nghĩa các kiểu dữ liệu volatile
 * @details Định nghĩa các kiểu dữ liệu sử dụng cho các thanh ghi và bộ nhớ 
 * 			phần cứng.
 **************************************************************************/
typedef volatile uint8		vuint8;		/* Số nguyên không dấu 8 bit volatile */
typedef volatile sint8		vsint8;		/* Số nguyên có dấu 8 bit volatile */
typedef volatile uint16		vuint16;	/* Số nguyên không dấu 16 bit volatile */
typedef volatile sint16		vsint16;	/* Số nguyên có dấu 16 bit volatile */
typedef volatile uint32		vuint32;	/* Số nguyên không dấu 32 bit volatile */
typedef volatile sint32		vsint32;	/* Số nguyên có dấu 32 bit volatile */
typedef volatile uint64		vuint64;	/* Số nguyên không dấu 64 bit volatile */
typedef volatile sint64		vsint64;	/* Số nguyên có dấu 64 bit volatile */

/**************************************************************************
 * @typedef boolean
 * @brief	Định nghĩa kiểu dữ liệu boolean
 * @details Kiểu dữ liệu này được sử dụng để định nghĩa giá trị True và False.
 **************************************************************************/
typedef uint8 boolean;
#ifndef TRUE
	#define TRUE	(boolean)1U		/* Giá trị boolean True */
#endif /* TRUE */

#ifndef FALSE
	#define FALSE	(boolean)0U		/* Giá trị boolean False */
#endif /* FALSE */

/**************************************************************************
 * @typedef Std_ReturnType
 * @brief 	Định ngĩa kiểu dữ liệu trả về chuẩn
 * @details Kiểu dữ liệu này được sử dụng để định nghĩa giá trị trả về chuẩn
 *          cho các hàm trong Autosar.
 **************************************************************************/
typedef uint8 Std_ReturnType;
#define E_OK 		(Std_ReturnType)0x00U		/* Thao tác thành công */
#define E_NOT_OK	(Std_ReturnType)0x01U		/* Thao tác không thành công */
#define CAN_BUSY	(Std_ReturnType)0x02U		/* Yêu cầu truyền của Node CAN không thể được xử lý 
													vì không có đối tượng truyền nào khả dụng */
													 
/**************************************************************************
 * @struct 	Std_VersionInfoType
 * @brief 	Cấu trúc lưu trữ thông tin phiên bản phần mềm
 * @details Cấu trúc này được sử dụng để lưu trữ thông tin chi tiết về phiên 
 *          bản của một mô-đun phần mềm, bao gồm ID nhà cung cấp, ID mô-đun 
 *          và các phiên bản phần mềm.
 **************************************************************************/
typedef struct {
	uint16 vendorID;			/* ID nhà cung cấp */
	uint16 moduleID;			/* ID mô-đun */
	uint8 sw_major_version;		/* Phiên bản chính của phần mềm */
	uint8 sw_minor_version;		/* Phiên bản phụ của phần mềm */
	uint8 sw_patch_version;		/* Phiên bản vá lỗi của phần mềm */
} Std_VersionInfoType;

/**************************************************************************
 * @typedef Std_TransformerErrorCode
 * @details Ý nghĩa cụ thể của các giá trị của kiểu dữ liệu này phụ thuộc 
 * 			vào chuỗi transformer cụ thể mà kiểu dữ liệu này đại diện cho 
 * 			lỗi transformer.
 **************************************************************************/
typedef uint8 Std_TransformerErrorCode;

/**************************************************************************
 * @typedef Std_TransformerClass
 * @details Kiểu dữ liệu này đại diện cho lớp transformer mà lỗi đã xảy ra.
 **************************************************************************/
typedef uint8	Std_TransformerClass;
#define STD_TRANSFORMER_UNSPECIFIED	0X00	/* Transformer thuộc một lớp không xác định */
#define STD_TRANSFORMER_SERIALIZER	0X01	/* Transformer thuộc lớp tuần tự hóa (serializer) */
#define STD_TRANSFORMER_SAFETY		0X02	/* Transformer thuộc lớp an toàn */
#define STD_TRANSFORMER_SECURITY	0X03	/* Transformer thuộc lớp bảo mật */
#define STD_TRANSFORMER_CUSTOM		0XFF	/* Transformer thuộc lớp tùy chỉnh, không được AUTOSAR tiêu chuẩn hóa */

/**************************************************************************
 * @struct 	Std_TransformerError
 * @brief 	Cấu trúc chứa mã lỗi và lớp transformer mà lỗi thuộc về.
 * @details Cấu trúc này đại diện cho một lỗi transformer trong ngữ cảnh
 *          của một chuỗi transformer cụ thể.
 **************************************************************************/
typedef struct {
	Std_TransformerErrorCode errorCode;		
	Std_TransformerClass transformerClass;
} Std_TransformerError;

/**************************************************************************
 * @typedef Std_TransformerForwardCode
 * @brief	(Bản nháp)
 **************************************************************************/
typedef uint8 Std_TransformerForwardCode;
#ifndef E_OK
	#define E_OK	0x00
#endif /* E_OK */
#define E_SAFETY_INVALID_REP	0X01	
#define E_SAFETY_INVALID_SEQ	0x02 
#define E_SAFETY_INVALID_CRC	0x03

/**************************************************************************
 * @struct 	Std_TransformerForward
 * @brief 	(Bản nháp)
 **************************************************************************/
typedef struct {
	Std_TransformerForwardCode errorCode;		
	Std_TransformerClass transformerClass;
} Std_TransformerForward;

/**************************************************************************
 * @typedef Std_MessageTypeType
 * @details Kiểu này được sử dụng để mã hóa các loại tin nhắn khác nhau. 
 * 			Hiện tại, kiểu mã hóa này chỉ giới hạn ở sự phân biệt giữa 
 * 			yêu cầu và phản hồi trong giao tiếp C/S.
 **************************************************************************/
typedef uint8	Std_MessageTypeType;
#define STD_MESSAGETYPE_REQUEST		0X00	/* Message type for a request message */
#define STD_MESSAGETYPE_RESPONSE	0X01	/* Message type for a response message */

/**************************************************************************
 * @typedef Std_MessageResultType
 * @details Kiểu dữ liệu này được sử dụng để mã hóa các loại kết quả khác nhau 
 *          cho các thông điệp phản hồi.
 *          Hiện tại, việc mã hóa này chỉ giới hạn ở việc phân biệt giữa 
 *          phản hồi OK và ERROR.
 **************************************************************************/
typedef uint8	Std_MessageResultType;
#define STD_MESSAGERESULT_OK	0X00	/* Kết quả thông điệp cho phản hồi thành công */
#define STD_MESSAGERESULT_ERROR	0X01	/* Kết quả thông điệp cho phản hồi lỗi */

/**************************************************************************
 * @brief      Con trỏ hàm Std_ExtractProtocolHeaderFieldsType
 * @details    Kiểu dữ liệu cho con trỏ hàm dùng để trích xuất các trường 
 *             tiêu đề giao thức liên quan của thông điệp và kiểu kết quả 
 *             của transformer.
 *             Hiện tại, phạm vi sử dụng giới hạn trong giao tiếp 
 *             Client/Server (C/S), bao gồm REQUEST, RESPONSE, OK và ERROR.
 * 
 * @param[in]  buffer          Bộ đệm do RTE cấp phát, nơi dữ liệu đã 
 *                             được transformer chuyển đổi sẽ được lưu trữ.
 * @param[in]  bufferLength    Độ dài của bộ đệm.
 * @param[out] messageType     Biểu diễn chuẩn của loại thông điệp 
 *                             (được trích xuất từ tiêu đề giao thức 
 *                             của transformer).
 * @param[out] messageResult   Biểu diễn chuẩn của loại kết quả thông điệp 
 *                             (được trích xuất từ tiêu đề giao thức 
 *                             của transformer).
 **************************************************************************/
extern Std_ReturnType (*Std_ExtractProtocolHeaderFieldsType) (
	const uint8* buffer,
	uint32 bufferLength,
	Std_MessageTypeType* messageType,
	Std_MessageResultType* messageResult
);

/**************************************************************************
 * @brief	Định nghĩa giá trị logic chuẩn
 * @details Kiểu dữ liệu này được sử dụng cho trạng thái đầu vào/đầu ra.
 **************************************************************************/
#define STD_LOW		0x00U 	/* Trạng thái điện áp 0V */
#define STD_HIGH 	0x01U	/* Trạng thái điện áp 5V hoặc 3.3V */

/**************************************************************************
 * @brief	Định nghĩa trạng thái hoạt động/rảnh
 * @details Kiểu dữ liệu này được sử dụng cho trạng thái hệ thống (Active/Idle).	
 **************************************************************************/
#define STD_IDLE	0x00U 		/* Trạng thái rảnh */
#define STD_ACTIVE 	0x01U		/* Trạng thái hoạt động */

/**************************************************************************
 * @brief	Định nghĩa trạng thái bật/tắt
 * @details Kiểu dữ liệu này được sử dụng cho trạng thái bật/tắt (ON/OFF).
 **************************************************************************/
#define STD_OFF		0x00U 		/* Trạng thái tắt */
#define STD_ON 		0x01U		/* Trạng thái bật */

/**************************************************************************
 * @brief 	Định nghĩa con trỏ NULL
 * @details Định nghĩa con trỏ NULL bằng cách sử dụng con trỏ void trỏ đến 
 * 			địa chỉ 0.
 **************************************************************************/
#ifndef NULL_PTR
	#define NULL_PTR    ((void*)0)
#endif /* NULL_PTR */

#endif /* STD_TYPES_H */