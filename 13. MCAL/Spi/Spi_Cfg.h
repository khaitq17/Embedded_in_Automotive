/***************************************************************************
 * @file    Spi_Cfg.h
 * @brief   Cấu hình các thông số cho SPI 
 * @details File này chứa cấu hình các thông số mặc định cho SPI và các macro 
 * 	 		sử dụng cho SPI bao gồm kênh, Job, Sequence.
 * @version 1.0
 * @date    2024-11-28
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef SPI_CFG_H
#define SPI_CFG_H

#include "Spi_Types.h"
#include "Spi_Hw.h"
#include "Spi.h"

/**************************************************************************
 * @brief Định nghĩa số lượng Channel, Job, Sequence tối đa của SPI 
 **************************************************************************/
#define SPI_MAX_CHANNELS 3    
#define SPI_MAX_JOBS     3
#define SPI_MAX_SEQUENCE 3

/**************************************************************************
 * @brief	Định nghĩa các kênh SPI sử dụng
 * @details	Các kênh đại diện cho đường truyền giữa MCU và các thiết bị ngoại vi.
 **************************************************************************/
#define SPI_CHANNEL_1	(Spi_ChannelType)0	/* SPI kênh 1 - Kết nối với cảm biến nhiệt độ */
#define SPI_CHANNEL_2	(Spi_ChannelType)1	/* SPI kênh 2 - Kết nối với EEPROM */
#define SPI_CHANNEL_3	(Spi_ChannelType)2	/* SPI kênh 3 - Có thể mở rộng thêm với các thiết bị khác */

/**************************************************************************
 * @brief Mảng xác định Channel thuộc bộ SPI nào
 **************************************************************************/
const Spi_HWUnitType spiHwUnit[SPI_MAX_CHANNELS] = {
	SPI_HW_UNIT_0,	/* Channel 1 thuộc SPI1 */
	SPI_HW_UNIT_1,	/* Channel 2 thuộc SPI2 */
	SPI_HW_UNIT_0,	/* Channel 3 thuộc SPI1 */
};

/**************************************************************************
 * @brief Định nghĩa các Job sử dụng, bao gồm 1 hoặc nhiều kênh
 **************************************************************************/
#define SPI_JOB_READ_TEMP_SENSOR	(Spi_JobType)0	/* Job đọc dữ liệu từ cảm biến nhiệt độ qua SPI1 */
#define SPI_JOB_WRITE_EEPROM		(Spi_JobType)1	/* Job ghi dữ liệu vào EEPROM qua SPI2 */
#define SPI_JOB_READ_EEPROM			(Spi_JobType)2	/* Job đọc dữ liệu từ EEPROM qua SPI2 */

/**************************************************************************
 * @brief Định nghĩa các Sequence sử dụng, đại diện cho nhiều Job
 **************************************************************************/
#define SPI_SEQUENCE_0	(Spi_SequenceType)0	/* Sequence 0: Đọc dữ liệu từ cảm biến nhiệt độ */
#define SPI_SEQUENCE_1	(Spi_SequenceType)1	/* Sequence 1: Ghi dữ liệu vào EEPROM */
#define SPI_SEQUENCE_2	(Spi_SequenceType)2	/* Sequence 2: Đọc dữ liệu từ EEPROM */

/**************************************************************************
 * @typedef Spi_JobConfigType
 * @brief 	Kiểu dữ liệu cấu hình cho một Job
 **************************************************************************/
typedef struct {
	Spi_ChannelType Channel;	/* Kênh SPI sử dụng cho Job */
	uint32 Baudrate;			/* Tốc độ truyền SPI */
	uint8 CPOL;					/* CPOL (0 = Low, 1 = High) */
	uint8 CPHA;					/* CPHA (0 = 1Edge, 1 = 2Edge) */
	uint8 Mode;					/* Chế độ hoạt động (0 = Master, 1 = Slave) */
} Spi_JobConfigType;

/**************************************************************************
 * @brief Cấu hình chi tiết cho các Job cụ thể 
 **************************************************************************/
// Cấu hình cho Job đọc dữ liệu từ cảm biến nhiệt độ qua SPI1 
const Spi_JobConfigType Spi_JobConfig_ReadTempSensor = {
	.Channel = SPI_CHANNEL_1,	// SPI Channel 1
	.Baudrate = 1000000,		// Baud rate = 1 MHz
	.CPOL = 0,					// Cực tính mức 0 khi ở trạng thái rảnh
	.CPHA = 0,					// Tín hiệu truyền đi ở cạnh xung đầu tiên
	.Mode = 0					// Master 
};

// Cấu hình cho Job ghi dữ liệu vào EEPROM qua SPI2 
const Spi_JobConfigType Spi_JobConfig_WriteEEPROM = {
	.Channel = SPI_CHANNEL_2,	// SPI Channel 2
	.Baudrate = 500000,			// Baud rate = 500 kHz
	.CPOL = 0,					// Cực tính mức 0 khi ở trạng thái rảnh
	.CPHA = 0,					// Tín hiệu truyền đi ở cạnh xung đầu tiên
	.Mode = 0					// Master 
};

// Cấu hình cho Job đọc dữ liệu từ EEPROM qua SPI2 
const Spi_JobConfigType Spi_JobConfig_ReadEEPROM = {
	.Channel = SPI_CHANNEL_2,	// SPI Channel 2
	.Baudrate = 500000,			// Baud rate = 500 kHz
	.CPOL = 0,					// Cực tính mức 0 khi ở trạng thái rảnh
	.CPHA = 0,					// Tín hiệu truyền đi ở cạnh xung đầu tiên
	.Mode = 0					// Master 
};

/**************************************************************************
 * @brief Định nghĩa số Job tối đa trong 1 Sequence
 **************************************************************************/
#define MAX_JOBS_PER_SEQUENCE 2

/**************************************************************************
 * @typedef Spi_SequenceConfigType
 * @brief 	Cấu hình SPI cho chuỗi các công việc sẽ được thực thi
 **************************************************************************/
typedef struct {
	Spi_JobType Jobs[MAX_JOBS_PER_SEQUENCE];	/* Mảng các Job trong Sequence */
	uint8 JobCount;	/* Số lượng Job trong Sequence */
} Spi_SequenceConfigType;


/**************************************************************************
 * @brief Cấu hình chi tiết cho các Sequence cụ thể 
 **************************************************************************/
const Spi_SequenceConfigType Spi_SequenceConfigs[SPI_MAX_SEQUENCE] = {
	// Cấu hình cho Sequence 0: Đọc dữ liệu từ cảm biến nhiệt độ
	{
		.Jobs = {SPI_JOB_READ_TEMP_SENSOR},
		.JobCount = 1
	},
	// Cấu hình cho Sequence 1: Ghi dữ liệu vào EEPROM
	{
		.Jobs = {SPI_JOB_WRITE_EEPROM},
		.JobCount = 1
	},
	// Cấu hình cho Sequence 2: Đọc dữ liệu từ EEPROM
	{
		.Jobs = {SPI_JOB_READ_EEPROM},
		.JobCount = 1
	}
};

/**************************************************************************
 * @brief 	Spi_SetupDefaultConfig
 * @details Hàm này kiểm tra và thiết lập các giá trị mặc định cho Spi_ConfigType.
 * 			Nếu thông số chưa được khởi tạo sẽ sử dụng giá trị mặc định cho
 * 			thông số đó.
 * @param	config	Con trỏ trỏ đến biến muốn cấu hình các giá trị mặc định
 * @return	None
 **************************************************************************/
static inline void Spi_SetupDefaultConfig(Spi_ConfigType* config) {
	if (config->Baudrate == 0) {
		config->Baudrate = SPI_BaudRatePrescaler_16; // 1 MHz
	}
	
	if (config->CPOL == 0) {
		config->CPOL = SPI_CPOL_Low;
	}
	
	if (config->CPHA == 0) {
		config->CPHA = SPI_CPHA_1Edge;
	}
	
	if (config->Mode == 0) {
		config->Mode = (Spi_ModeType)SPI_Mode_Master;
	}
	
	if (config->NSS == 0) {
		config->NSS = (Spi_NSSManagementType)SPI_NSS_Soft;
	}
	
	if (config->DataSize == 0) {
		config->DataSize = SPI_DataSize_8b;
	}
}

#endif	/* SPI_CFG_H */