/***************************************************************************
 * @file    Adc_Types.h
 * @brief   Định nghĩa các kiểu dữ liệu cho ADC 
 * @details File này chứa các kiểu dữ liệu cần thiết để sử dụng cho ngoại vi 
 * 			ADC trên vi điều khiển.
 * @version 1.0
 * @date    2024-12-02
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef ADC_TYPES_H
#define ADC_TYPES_H

#include "Std_Types.h"

/**************************************************************************
 * @typedef Adc_ConversionModeType
 * @brief 	Định nghĩa kiểu dữ liệu cho chế độ chuyển đổi
 * @details Kiểu dữ liệu dùng để chọn chế độ chuyển đổi của ADC (đơn kênh 
 * 			hoặc đa kênh).
 **************************************************************************/
typedef enum {
	ADC_CONV_MODE_SINGLE = 0,		/* ADC chuyển đổi đơn kênh */
	ADC_CONV_MODE_SCAN = 1			/* ADC chuyển đổi đa kênh */
} Adc_ConversionModeType;

/**************************************************************************
 * @typedef Adc_InstanceType
 * @brief 	Định nghĩa kiểu dữ liệu đại diện cho các bộ ADC (ADC1, ADC2) 
 **************************************************************************/
typedef uint8 Adc_InstanceType;
#define ADC_INSTANCE_1	(Adc_InstanceType)0		/* ADC1 */
#define ADC_INSTANCE_2	(Adc_InstanceType)1		/* ADC2 */

/**************************************************************************
 * @typedef Adc_ChannelType
 * @brief 	Định nghĩa kiểu dữ liệu sử dụng cho các kênh ADC
 **************************************************************************/
typedef uint8 Adc_ChannelType;
#define ADC_CHANNEL_0   (Adc_ChannelType)0      /* ADC Channel 0 */
#define ADC_CHANNEL_1   (Adc_ChannelType)1      /* ADC Channel 1 */
#define ADC_CHANNEL_2   (Adc_ChannelType)2      /* ADC Channel 2 */
#define ADC_CHANNEL_3   (Adc_ChannelType)3      /* ADC Channel 3 */
#define ADC_CHANNEL_4   (Adc_ChannelType)4      /* ADC Channel 4 */
#define ADC_CHANNEL_5   (Adc_ChannelType)5      /* ADC Channel 5 */
#define ADC_CHANNEL_6   (Adc_ChannelType)6      /* ADC Channel 6 */
#define ADC_CHANNEL_7   (Adc_ChannelType)7      /* ADC Channel 7 */
#define ADC_CHANNEL_8   (Adc_ChannelType)8      /* ADC Channel 8 */
#define ADC_CHANNEL_9   (Adc_ChannelType)9      /* ADC Channel 9 */
#define ADC_CHANNEL_10  (Adc_ChannelType)10     /* ADC Channel 10 */
#define ADC_CHANNEL_11  (Adc_ChannelType)11     /* ADC Channel 11 */
#define ADC_CHANNEL_12  (Adc_ChannelType)12     /* ADC Channel 12 */
#define ADC_CHANNEL_13  (Adc_ChannelType)13     /* ADC Channel 13 */
#define ADC_CHANNEL_14  (Adc_ChannelType)14     /* ADC Channel 14 */
#define ADC_CHANNEL_15  (Adc_ChannelType)15     /* ADC Channel 15 */
#define ADC_CHANNEL_16  (Adc_ChannelType)16     /* ADC Channel 16 (Temp sensor) */
#define ADC_CHANNEL_17  (Adc_ChannelType)17     /* ADC Channel 17 (Vrefint) */

/**************************************************************************
 * @typedef Adc_GroupType
 * @brief 	Định nghĩa kiểu dữ liệu cho một nhóm kênh ADC
 **************************************************************************/
typedef uint8 Adc_GroupType	;

/**************************************************************************
 * @typedef Adc_ValueGroupType
 * @brief 	Định nghĩa kiểu dữ liệu cho giá trị của nhóm kênh ADC
 * @details Kiểu dữ liệu được sử dụng để đọc giá trị chuyển đổi ADC của một 
 * 			nhóm kênh.
 **************************************************************************/
typedef int	Adc_ValueGroupType;

/**************************************************************************
 * @typedef Adc_PrescalerType
 * @brief 	Định nghĩa kiểu dữ liệu cho giá trị chia tần số
 * @details Kiểu dữ liệu này được sử dụng như hệ số chia clock cho ADC
 **************************************************************************/
typedef uint16 Adc_PrescalerType;
#define ADC_CLOCK_PRESCALER_2		(Adc_PrescalerType)2		/* Hệ số chia clock là 2 */
#define ADC_CLOCK_PRESCALER_4		(Adc_PrescalerType)4		/* Hệ số chia clock là 4 */
#define ADC_CLOCK_PRESCALER_6		(Adc_PrescalerType)6		/* Hệ số chia clock là 6 */
#define ADC_CLOCK_PRESCALER_8		(Adc_PrescalerType)8		/* Hệ số chia clock là 8 */
#define ADC_CLOCK_PRESCALER_16		(Adc_PrescalerType)16		/* Hệ số chia clock là 16 */
#define ADC_CLOCK_PRESCALER_32		(Adc_PrescalerType)32		/* Hệ số chia clock là 32 */
#define ADC_CLOCK_PRESCALER_64		(Adc_PrescalerType)64		/* Hệ số chia clock là 64 */
#define ADC_CLOCK_PRESCALER_128		(Adc_PrescalerType)128		/* Hệ số chia clock là 128 */

/**************************************************************************
 * @typedef Adc_ConversionTimeType
 * @brief 	Định nghĩa kiểu dữ liệu cho thời gian chuyển đổi ADC 
 **************************************************************************/
typedef uint32 Adc_ConversionTimeType;

/**************************************************************************
 * @typedef Adc_SamplingTimeType
 * @brief 	Định nghĩa kiểu dữ liệ cho thời gian lấy mẫu 
 * @details Kiểu dữ liệu này đại diện cho thời gian lấy mẫu (Sampling Time)
 *          của bộ xử lý ADC. Giá trị này được biểu thị theo chu kỳ xung.
 **************************************************************************/
typedef uint8 Adc_SamplingTimeType;
#define ADC_SAMPLETIME_1CYCLE5    (Adc_SamplingTimeType)0   /* Thời gian lấy mẫu 1,5 chu kỳ */
#define ADC_SAMPLETIME_7CYCLE5    (Adc_SamplingTimeType)1   /* Thời gian lấy mẫu 7,5 chu kỳ */
#define ADC_SAMPLETIME_13CYCLE5   (Adc_SamplingTimeType)2   /* Thời gian lấy mẫu 13,5 chu kỳ */
#define ADC_SAMPLETIME_28CYCLE5   (Adc_SamplingTimeType)3   /* Thời gian lấy mẫu 28,5 chu kỳ */
#define ADC_SAMPLETIME_41CYCLE5   (Adc_SamplingTimeType)4   /* Thời gian lấy mẫu 41,5 chu kỳ */
#define ADC_SAMPLETIME_55CYCLE5   (Adc_SamplingTimeType)5   /* Thời gian lấy mẫu 55,5 chu kỳ */
#define ADC_SAMPLETIME_71CYCLE5   (Adc_SamplingTimeType)6   /* Thời gian lấy mẫu 71,5 chu kỳ */
#define ADC_SAMPLETIME_239CYCLE5  (Adc_SamplingTimeType)7   /* Thời gian lấy mẫu 239,5 chu kỳ */


/**************************************************************************
 * @typedef Adc_ResolutionType
 * @brief 	Định nghĩa kiểu dữ liệu cho độ phân giải
 * @details Kiểu dữ liệu này đại diện cho độ phân giải ADC. 
 * 			Trên vi điều khiển STM32F103, độ phân giải là 12 bit.
 **************************************************************************/
typedef uint8 Adc_ResolutionType;
#define ADC_RESOLUTION_8BIT		(Adc_ResolutionType)8		/* Độ phân giải 8 bit */
#define ADC_RESOLUTION_10BIT	(Adc_ResolutionType)10		/* Độ phân giải 10 bit */
#define ADC_RESOLUTION_12BIT	(Adc_ResolutionType)12		/* Độ phân giải 12 bit */

/**************************************************************************
 * @typedef Adc_StatusType
 * @brief 	Định nghĩa kiểu dữ liệu cho trạng thái ADC
 * @details Kiểu dữ liệu này dùng để biểu diễn trạng thái của quá trình chuyển 
 * 			đổi của một nhóm kênh ADC.
 **************************************************************************/
typedef enum {
    ADC_IDLE = 0x00,   			/* Quá trình chuyển đổi của nhóm kênh chỉ định chưa bắt đầu */

    ADC_BUSY = 0X01,  			/* Đang trong quá trình chuyển đổi, chưa có kết quả */

    ADC_COMPLETED = 0X02, 		/* Quá trình chuyển đổi của nhóm kênh chỉ định đã hoàn thành */

    ADC_STREAM_COMPLETED = 0X03 /* Đã lấy đủ số mẫu yêu cầu cho mỗi kênh trong nhóm,
                                  	không có chuyển đổi bổ sung nào sẽ được thực hiện */
} Adc_StatusType;

/**************************************************************************
 * @typedef Adc_TriggerSourceType
 * @brief   Định nghĩa kiểu dữ liệu cho trạng thái nguồn kích hoạt ADC
 * @details Kiểu dữ liệu này dùng để lựa chọn nguồn kích hoạt cho bộ chuyển 
 * 			đổi ADC, xác định cách thức khởi tạo quá trình chuyển đổi ADC 
 * 			bằng phần cứng hoặc phần mềm.
 **************************************************************************/
typedef enum {
    ADC_TRIGG_SRC_SW = 0x00,  /* Chuyển đổi ADC được kích hoạt bởi phần mềm */
    ADC_TRIGG_SRC_HW = 0X01   /* Chuyển đổi ADC được kích hoạt bởi sự kiện phần cứng */
} Adc_TriggerSourceType;

/**************************************************************************
 * @typedef Adc_GroupConvModeType
 * @brief 	Định nghĩa kiểu dữ liệu cho chế độ chuyển đổi nhóm ADC
 * @details Kiểu dữ liệu này được sử dụng để xác định cách thức một nhóm ADC 
 * 			thực hiện chuyển đổi tín hiệu sau khi được kích hoạt. 
 **************************************************************************/
typedef enum {
    ADC_CONV_MODE_ONESHOT = 0x00,   /* Chế độ chuyển đổi một lần: Nhóm kênh ADC chỉ thực hiện 
                                       một lần chuyển đổi cho mỗi lần kích hoạt. 
                                       	- Nếu kích hoạt bằng phần mềm: Có thể dừng quá trình 
                                        chuyển đổi thông qua lệnh API. 
                                       	- Nếu kích hoạt bằng phần cứng: Có thể dừng bằng cách 
                                        vô hiệu hóa sự kiện kích hoạt (nếu phần cứng hỗ trợ). */

    ADC_CONV_MODE_CONTINUOUS = 0x01 /* Chế độ chuyển đổi liên tục: Nhóm kênh ADC liên tục thực hiện 
                                        các lần chuyển đổi mà không cần kích hoạt lại. */
} Adc_GroupConvModeType;

/**************************************************************************
 * @typedef Adc_GroupPriorityType
 * @brief 	Định nghĩa kiểu dữ liệu cho mức ưu tiên của nhóm kênh
 * @details Kiểu dữ liệu này đại diện cho mức ưu tiên của nhóm kênh ADC. 
 * 			Mức ưu tiên thấp nhất là 0.
 **************************************************************************/
typedef uint8 Adc_GroupPriorityType;	/* Mức độ ưu tiên của nhóm kênh ADC có giá trị từ 0 đến 255 */

/**************************************************************************
 * @typedef Adc_GroupDefType
 * @brief   Định nghĩa kiểu dữ liệu để định nghĩa một nhóm kênh ADC
 * @details Kiểu dữ liệu này dùng để gán các kênh ADC vào một nhóm kênh cụ thể.
 **************************************************************************/
typedef struct {
    Adc_ChannelType Channels[16];     /* Mảng chứa các kênh ADC được cấu hình trong nhóm */
    Adc_GroupPriorityType Priority;   /* Độ ưu tiên của nhóm */
    uint8 NumChannels;                /* Số lượng kênh trong nhóm */
} Adc_GroupDefType;


/**************************************************************************
 * @typedef Adc_StreamNumSampleType
 * @brief   Định nghĩa kiểu dữ liệu xác định số lượng lần chuyển đổi của một 
 * 			nhóm ADC khi hoạt động ở chế độ truy cập streaming.
 **************************************************************************/
typedef uint8 Adc_StreamNumSampleType;

/**************************************************************************
 * @typedef Adc_StreamBufferModeType
 * @brief   Định nghĩa kiểu chế độ bộ đệm trong chế độ streaming của ADC.
 * @details Kiểu dữ liệu này được sử dụng để cấu hình cách thức lưu trữ dữ  
 *          liệu khi ADC hoạt động ở chế độ streaming.
 **************************************************************************/
typedef enum {
    ADC_STREAM_BUFFER_LINEAR = 0x00,   	/* Bộ đệm tuyến tính: Dừng chuyển đổi ngay khi 
                                          bộ đệm đầy (đạt số mẫu tối đa). */

    ADC_STREAM_BUFFER_CIRCULAR = 0x01  	/* Bộ đệm vòng lặp: Tiếp tục chuyển đổi ngay cả 
                                          khi bộ đệm đầy bằng cách ghi đè lên dữ liệu cũ. */
} Adc_StreamBufferModeType;

/**************************************************************************
 * @typedef Adc_GroupAccessModeType
 * @brief   Định nghĩa kiểu chế độ truy cập kết quả nhóm ADC
 * @details Kiểu dữ liệu này được sử dụng để cấu hình cách truy cập kết quả 
 *          của nhóm chuyển đổi ADC.
 **************************************************************************/
typedef enum {
    ADC_ACCESS_MODE_SINGLE = 0x00,      /* Truy cập đơn: Chỉ lấy một giá trị kết quả mỗi lần */
    ADC_ACCESS_MODE_STREAMING = 0x01    /* Truy cập streaming: Lấy nhiều kết quả liên tục */
} Adc_GroupAccessModeType;

/**************************************************************************
 * @typedef Adc_HwTriggerSignalType
 * @brief   Định nghĩa kiểu tín hiệu kích hoạt phần cứng của ADC
 * @details Kiểu dữ liệu này được sử dụng để cấu hình cạnh của tín hiệu kích hoạt
 *          phần cứng mà driver sẽ phản ứng.
 **************************************************************************/
typedef enum {
    ADC_HW_TRIG_RISING_EDGE = 0x00,   /* Kích hoạt khi có cạnh lên của tín hiệu phần cứng */
    ADC_HW_TRIG_FALLING_EDGE = 0x01,  /* Kích hoạt khi có cạnh xuống của tín hiệu phần cứng */
    ADC_HW_TRIG_BOTH_EDGES = 0x02     /* Kích hoạt khi có cả cạnh lên và cạnh xuống */
} Adc_HwTriggerSignalType;

/**************************************************************************
 * @typedef Adc_HwTriggerTimeType
 * @brief 	Định nghĩa kiểu thời gian kích hoạt phần cứng của ADC
 * @details	Kiểu dữ liệu này được sử dụng cho giá trị tải lại của bộ đếm Timer trong ADC.
 **************************************************************************/
typedef uint16 Adc_HwTriggerTimeType;

/**************************************************************************
 * @typedef Adc_PriorityImplementationType
 * @brief 	Định nghĩa kiểu dữ liệu cho cơ chế ưu tiên
 * @details	Kiểu dữ liệu này được sử dụng để cấu hình cơ chế ưu tiên trong ADC.
 **************************************************************************/
typedef enum {
	ADC_PRIORITY_NONE = 0x00,		/* Không có cơ chế ưu tiên */
	ADC_PRIORITY_HW = 0x01,			/* Chỉ có cơ chế ưu tiên phần cứng */
	ADC_PRIORITY_HW_SW = 0x02		/* Có cả cơ chế ưu tiên phần cứng và phần mềm */
} Adc_PriorityImplementationType;

/**************************************************************************
 * @typedef Adc_GroupReplacementType
 * @brief   Định nghĩa kiểu thay thế nhóm ADC
 * @details Kiểu dữ liệu này được sử dụng cho cơ chế thay thế khi một nhóm ADC  
 *          bị gián đoạn bởi một nhóm có độ ưu tiên cao hơn.
 **************************************************************************/
typedef enum {
    ADC_GROUP_REPL_ABORT_RESTART = 0x00,  	/* Cơ chế Hủy/Bắt đầu lại: Nếu một nhóm bị gián đoạn bởi nhóm có độ ưu tiên cao hơn,
                                             	toàn bộ quá trình chuyển đổi của nhóm bị gián đoạn sẽ được khởi động lại sau khi
                                             	nhóm có độ ưu tiên cao hơn hoàn thành. Trong chế độ truy cập liên tục (streaming), 
                                             	chỉ các kết quả của lần chuyển đổi bị gián đoạn mới bị loại bỏ, các kết quả trước đó 
                                             	trong bộ đệm vẫn được giữ nguyên. */

    ADC_GROUP_REPL_SUSPEND_RESUME = 0x01  	/* Cơ chế Tạm dừng/Tiếp tục: Nếu một nhóm bị gián đoạn bởi nhóm có độ ưu tiên cao hơn,
                                             	quá trình chuyển đổi sẽ tiếp tục sau khi nhóm ưu tiên cao hơn hoàn thành. Các kết  
                                             	quả đã được ghi vào bộ đệm không bị ảnh hưởng. */
} Adc_GroupReplacementType;

/**************************************************************************
 * @typedef Adc_ChannelRangeSelectType
 * @brief   Định nghĩa kiểu lựa chọn phạm vi kênh ADC
 * @details Kiểu dữ liệu này xác định các giá trị chuyển đổi nào được tính đến dựa trên
 *          giới hạn thấp (AdcChannelLowLimit) và giới hạn cao (AdcChannelHighLimit).
 **************************************************************************/
typedef enum {
    ADC_RANGE_UNDER_LOW      = 0x00,  /* Phạm vi dưới giới hạn thấp - bao gồm giá trị giới hạn thấp */
    ADC_RANGE_BETWEEN        = 0x01,  /* Phạm vi giữa giới hạn thấp và giới hạn cao - bao gồm giá trị giới hạn cao */
    ADC_RANGE_OVER_HIGH      = 0x02,  /* Phạm vi trên giới hạn cao */
    ADC_RANGE_ALWAYS         = 0x03,  /* Toàn bộ phạm vi - không phụ thuộc vào thiết lập giới hạn kênh */
    ADC_RANGE_NOT_UNDER_LOW  = 0x04,  /* Phạm vi trên giới hạn thấp */
    ADC_RANGE_NOT_BETWEEN    = 0x05,  /* Phạm vi trên giới hạn cao hoặc dưới giới hạn thấp - bao gồm giá trị giới hạn thấp */
    ADC_RANGE_NOT_OVER_HIGH  = 0x06   /* Phạm vi dưới giới hạn cao - bao gồm giá trị giới hạn cao */
} Adc_ChannelRangeSelectType;

/**************************************************************************
 * @typedef Adc_ResultAlignmentType
 * @brief 	Định nghĩa kiểu căn lề của ADC
 * @details Kiểu dữ liệu này được sử dụng để căn lề cho kết quả thô của ADC
 * 			trong bộ đệm (căn trái/căn phải).
 **************************************************************************/
typedef enum {
	ADC_ALIGN_LEFT = 0x00,			/* Căn lề trái */
	ADC_ALIGN_RIGHT = 0x01			/* Căn lề phải */
} Adc_ResultAlignmentType;

/**************************************************************************
 * @typedef Adc_PowerStateType
 * @brief   Định nghĩa kiểu trạng thái nguồn của ADC
 * @details Kiểu dữ liệu này xác định trạng thái nguồn hiện tại hoặc trạng  
 *          thái nguồn được đặt làm mục tiêu.
 **************************************************************************/
typedef enum {
    ADC_FULL_POWER = 0,  /* Chế độ đầy đủ công suất */
    // Các chế độ nguồn với mức tiêu thụ điện giảm dần (giá trị từ 1 đến 255)
} Adc_PowerStateType;

/**************************************************************************
 * @typedef Adc_PowerStateRequestResultType
 * @brief   Định nghĩa kiểu kết quả yêu cầu trạng thái nguồn của ADC
 * @details Kiểu này xác định kết quả của các yêu cầu liên quan đến chuyển đổi 
 *          trạng thái nguồn.
 **************************************************************************/
typedef enum {
    ADC_SERVICE_ACCEPTED = 0,       /* Thay đổi trạng thái nguồn được thực hiện thành công */
    ADC_NOT_INIT = 1,               /* Mô-đun ADC chưa được khởi tạo */
    ADC_SEQUENCE_ERROR = 2,         /* Gọi API sai trình tự */
    ADC_HW_FAILURE = 3,             /* Lỗi phần cứng ngăn ADC chuyển sang trạng thái nguồn mong muốn */
    ADC_POWER_STATE_NOT_SUPP = 4,   /* Mô-đun ADC không hỗ trợ trạng thái nguồn được yêu cầu */
    ADC_TRANS_NOT_POSSIBLE = 5      /* Không thể chuyển trực tiếp từ trạng thái nguồn hiện tại sang trạng thái yêu cầu 
                                       hoặc phần cứng đang bận */
} Adc_PowerStateRequestResultType;

/**************************************************************************
 * @typedef Adc_ConfigType
 * @brief   Định nghĩa kiểu cấu hình ADC
 * @details Cấu trúc dữ liệu này chứa tập hợp các tham số cấu hình cần thiết 
 *          để khởi tạo ADC và các đơn vị phần cứng ADC.
 **************************************************************************/
typedef struct {
    Adc_PrescalerType ClockPrescaler;    	/* Bộ chia tần số xung nhịp ADC */
    Adc_GroupConvModeType ConversionMode;	/* Chế độ chuyển đổi ADC (1 lần hoặc liên tục) */
    Adc_TriggerSourceType TriggerSource;	/* Nguồn kích hoạt ADC (phần mềm hoặc phần cứng) */
    Adc_ResolutionType Resolution;     		/* Độ phân giải ADC (8, 10, 12, ... bit) */
    uint8 NumChannels;                    	/* Số lượng kênh được cấu hình */
    Adc_InstanceType AdcInstance;  			/* Đơn vị phần cứng ADC (ADC1, ADC2) */

    /**************************************************************************
    * @struct Adc_ChannelConfigType
    * @brief  Cấu trúc này chứa các tham số cấu hình cho từng kênh ADC
    **************************************************************************/
    struct {    
        Adc_ChannelType Channel;             /* Kênh ADC */
        Adc_SamplingTimeType SamplingTime;   /* Thời gian lấy mẫu ADC */
        uint8 Rank;                           /* Mức ưu tiên của kênh trong quá trình chuyển đổi */
    } Channels[16];  /* Mảng cấu hình cho từng kênh (tối đa 16 kênh) */

    /**************************************************************************
    * @function InitCallback
    * @brief    Con trỏ hàm sẽ được gọi khi khởi tạo ADC thành công
    **************************************************************************/
    void (*InitCallback)(void);  /* Con trỏ đến hàm callback khi khởi tạo ADC thành công */
} Adc_ConfigType;

#endif	/* ADC_TYPES_H */