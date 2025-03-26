/***************************************************************************
 * @file    Spi_Types.h
 * @brief   Định nghĩa các kiểu dữ liệu cho SPI 
 * @details File này chứa các kiểu dữ liệu cần thiết để sử dụng cho giao thức 
 * 			SPI trên vi điều khiển.
 * @version 1.0
 * @date    2024-11-28
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef SPI_TYPES_H
#define SPI_TYPES_H

#include "Std_Types.h"

/**************************************************************************
 * @typedef Spi_BaudrateType
 * @brief 	Định nghĩa kiểu dữ liệu cho tốc độ truyền SPI
 **************************************************************************/
typedef uint16 Spi_BaudrateType;

/**************************************************************************
 * @typedef Spi_ModeType
 * @brief 	Kiểu dữ liệu này được sử dụng để chọn chế độ hoạt động của 
 * 			thiết bị giao tiếp SPI (Master hoặc Slave)
 **************************************************************************/
typedef enum {
	SPI_MODE_MASTER = 0,		/* Chế độ Master */
	SPI_MODE_SLAVE = 1			/* Chế độ Slave */
} Spi_ModeType;

/**************************************************************************
 * @typedef Spi_ClockPolarityType
 * @brief 	Kiểu dữ liệu này được sử dụng để chọn cực tính của chân SCK (CPOL)
 **************************************************************************/
typedef enum {
	SPI_CPOL_LOW = 0,		/* Cực tính mức 0 khi ở trạng thái rảnh */
	SPI_CPOL_HIGH = 1		/* Cực tính mức 1 khi ở trạng thái rảnh */
} Spi_ClockPolarityType;

/**************************************************************************
 * @typedef Spi_ClockPhaseType
 * @brief 	Kiểu dữ liệu này được sử dụng để chọn cạnh xung bắt đầu truyền tín hiệu (CPHA)
 **************************************************************************/
typedef enum {
	SPI_CPHA_1EDGE = 0,		/* Tín hiệu truyền đi ở cạnh xung đầu tiên */
	SPI_CPHA_2EDGE = 1		/* Tín hiệu truyền đi ở cạnh xung thứ hai */
} Spi_ClockPhaseType;

/**************************************************************************
 * @typedef Spi_NSSManagementType
 * @brief 	Kiểu dữ liệu này được sử dụng để chọn việc cấu hình chân SS 
 * 			bằng thiết bị hay phần mềm (Hardware hoặc Software)
 **************************************************************************/
typedef enum {
	SPI_NSS_SOFT = 0,   	/* Tín hiệu NSS được quản lý bằng phần mềm thông qua bit SSI */
	SPI_NSS_HARD = 1    	/* Tín hiệu NSS được quản lý bằng phần cứng (chân NSS) */
} Spi_NSSManagementType;

/**************************************************************************
 * @typedef Spi_DataSizeType
 * @brief 	Kiểu dữ liệu này được sử dụng để chọn kích thước dữ liệu
 **************************************************************************/
typedef enum {
	SPI_DATASIZE_8BIT = 0,		/* Dữ liệu 8 bit */
	SPI_DATASIZE_16BIT = 1		/* Dữ liệu 16 bit */
} Spi_DataSizeType;

/**************************************************************************
 * @typedef Spi_StatusType
 * @brief 	Định nghĩa kiểu dữ liệu cho trạng thái SPI
 * @details Kiểu dữ liệu này được sử dụng để biểu diễn một trạng thái cụ thể 
 * 			của SPI.
 **************************************************************************/
typedef enum {
	SPI_UNINIT = 0x00,  /* SPI chưa được khởi tạo hoặc không thể sử dụng */
	SPI_IDLE   = 0x01,  /* SPI hiện không truyền bất kỳ Job nào */
	SPI_BUSY   = 0x02   /* SPI đang thực hiện một Job SPI (truyền dữ liệu) */
} Spi_StatusType;
	
/**************************************************************************
 * @typedef Spi_JobResultType
 * @brief   Định nghĩa kiểu dữ liệu cho các loại kết quả của Job 
 * @details Kiểu dữ liệu này xác định các trạng thái cụ thể của Job cho SPI.
 **************************************************************************/
typedef enum {
    SPI_JOB_OK,       /* Lần truyền cuối cùng của Job đã hoàn thành thành công */
    
    SPI_JOB_PENDING,  /* SPI đang thực hiện một Job SPI. 
                         Trạng thái này có ý nghĩa tương đương với SPI_BUSY */
    
    SPI_JOB_FAILED,   /* Lần truyền cuối cùng của Job đã thất bại */
    
    SPI_JOB_QUEUED    /* Một Job truyền không đồng bộ đã được chấp nhận, 
                         nhưng quá trình truyền thực tế của Job này chưa bắt đầu */
} Spi_JobResultType;

/**************************************************************************
 * @typedef Spi_SeqResultType
 * @brief 	Định nghĩa kiểu dữ liệu cho các loại kết quả của Sequence
 * @details Kiểu dữ liệu này xác định các trạng thái cụ thể của Sequence cho SPI.
 **************************************************************************/
typedef enum {    
    SPI_SEQ_OK,        /* Lần truyền cuối cùng của Sequence đã hoàn thành thành công */
    
    SPI_SEQ_PENDING,   /* SPI đang thực hiện một Sequence (Trạng thái này có ý nghĩa tương đương với SPI_BUSY) */
    
    SPI_SEQ_FAILED,    /* Lần truyền cuối cùng của Sequence đã thất bại */
    
    SPI_SEQ_CANCELED   /* Lần truyền cuối cùng của Sequence đã bị hủy bởi người dùng */
} Spi_SeqResultType;

/**************************************************************************
 * @typedef Spi_DataBufferType
 * @brief 	Định nghĩa kiểu dữ liệu cho bộ đệm dữ liệu trong SPI
 **************************************************************************/
typedef uint8 Spi_DataBufferType;

/**************************************************************************
 * @typedef Spi_NumberOfDataType
 * @brief   Định nghĩa kiểu dữ liệu cho số lượng dữ liệu trong SPI
 * @details Kiểu dữ liệu này dùng để xác định số lượng phần tử dữ liệu thuộc
 *          kiểu Spi_DataBufferType sẽ được gửi và/hoặc nhận bởi kênh SPI.
 **************************************************************************/
typedef uint16 Spi_NumberOfDataType;

/**************************************************************************
 * @typedef Spi_ChannelType
 * @brief 	Định nghĩa kiểu dữ liệu cho kênh SPI
 **************************************************************************/
typedef uint8 Spi_ChannelType;

/**************************************************************************
 * @typedef Spi_JobType
 * @brief 	Định nghĩa kiểu dữ liệu cho một Job trong SPI
 **************************************************************************/
typedef uint16 Spi_JobType;

/**************************************************************************
 * @typedef Spi_SequenceType
 * @brief 	Định nghĩa kiểu dữ liệu cho một Sequence trong SPI
 **************************************************************************/
typedef uint8 Spi_SequenceType;

/**************************************************************************
 * @typedef Spi_HWUnitType
 * @brief   Định nghĩa kiểu dữ liệu cho một đơn vị phần cứng SPI
 * @details Kiểu dữ liệu này xác định ID cho một đơn vị phần cứng SPI
 *          (bộ ngoại vi của vi điều khiển).
 **************************************************************************/
typedef uint8 Spi_HWUnitType;
#define SPI_HW_UNIT_0	(Spi_HWUnitType)0	/* Bộ SPI1 */
#define SPI_HW_UNIT_1	(Spi_HWUnitType)1	/* Bộ SPI2 */

/**************************************************************************
 * @typedef Spi_AsyncModeType
 * @brief   Định nghĩa kiểu dữ liệu cho chế độ bất đồng bộ của SPI
 * @details Kiểu dữ liệu này xác định cơ chế hoạt động bất đồng bộ cho các 
 * 			bus SPI được xử lý ở LEVEL 2.
 **************************************************************************/
typedef enum {
    SPI_POLLING_MODE,   /* Cơ chế bất đồng bộ được đảm bảo bằng polling, 
                            do đó các ngắt liên quan đến bus SPI xử lý 
                            bất đồng bộ sẽ bị vô hiệu hóa */
    
    SPI_INTERRUPT_MODE  /* Cơ chế bất đồng bộ được đảm bảo bằng ngắt, 
                            do đó các ngắt liên quan đến bus SPI xử lý 
                            bất đồng bộ sẽ được kích hoạt */
} Spi_AsyncModeType;

/**************************************************************************
 * @struct	Spi_ConfigType
 * @brief 	Định nghĩa kiểu dữ liệu cho việc cấu hình SPI
 * @details Cấu trúc này chứa thông tin khởi tạo cho SPI. Ba tham số đầu 
 * 			tiên (Channel, Job, Sequence) là bắt buộc. Các tham số còn lại
 * 			là tùy chọn và có thể sử dụng giá trị mặc định.
 **************************************************************************/
typedef struct {
	// Các thông số bắt buộc
	Spi_ChannelType Channel;		/* Kênh SPI */	
	Spi_JobType Job;				/* Job SPI */
	Spi_SequenceType Sequence;		/* Sequence SPI */
	
	// Các thông số tùy chọn	
	Spi_BaudrateType Baudrate;		/* Tốc độ truyền SPI (Mặc định 1 MHz)*/
	Spi_ModeType Mode;				/* Chế độ hoạt động (Mặc định Master) */
	Spi_ClockPolarityType CPOL;		/* CPOL (Mặc định CPOL = 0) */
	Spi_ClockPhaseType CPHA;		/* CPHA (Mặc định CPHA = 0) */
	Spi_NSSManagementType NSS;		/* Phương thức điều khiển chân SS (Mặc định Software) */
	Spi_DataSizeType DataSize;		/* Kích thước dữ liệu (Mặc định 8 bit) */
} Spi_ConfigType;

#endif	/* SPI_TYPES_H */