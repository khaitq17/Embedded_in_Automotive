/***************************************************************************
 * @file    Spi.h
 * @brief   Khai báo giao diện cho giao thức SPI 
 * @details File này cung cấp giao diện để sử dụng và giao tiếp cho giao thức 
 * 			SPI của vi điều khiển. Nó chứa các kiểu dữ liệu định nghĩa, cấu trúc
 *          cấu hình và các hàm liên quan đến SPI.
 * @version 1.0
 * @date    2024-11-28
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#ifndef SPI_H
#define SPI_H
   
#include "stm32f10x_spi.h"            
#include "Std_Types.h"
#include "Spi_Types.h"
#include "Spi_Cfg.h"
#include "Spi_Hw.h"

/**************************************************************************
 * @brief 	  Spi_Init
 * @param[in] ConfigPtr     Con trỏ đến biến cấu hình SPI
 * @return	  None
 **************************************************************************/
void Spi_Init(const Spi_ConfigType* ConfigPtr);

/**************************************************************************
 * @brief 	Spi_DeInit
 * @param	None
 * @return	Std_ReturnType	Trả về E_OK nếu lệnh hủy khởi tạo được chấp nhận
 *								   E_NOT_OK nếu lệnh hủy khởi tạo không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_DeInit(void);

/**************************************************************************
 * @brief     Spi_WriteIB
 * @param[in] Channel	    ID của kênh
 * @param[in] DataBufferPtr Con trỏ đến bộ đệm dữ liệu nguồn. Nếu con trỏ này 
 *         					là null, dữ liệu truyền đi sẽ không quan trọng và 
 *                  		giá trị mặc định của kênh này sẽ được sử dụng thay thế.
 *
 * @return    Std_ReturnType	Trả về E_OK nếu lệnh ghi được chấp nhận
 *                      			   E_NOT_OK nếu lệnh ghi không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr);

/**************************************************************************
 * @brief 	  Spi_AsyncTransmit
 * @param[in] Sequence    ID của Sequence
 * @return    Std_ReturnType	Trả về E_OK nếu lệnh truyền được chấp nhận
 *                      			   E_NOT_OK nếu lệnh truyền không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

/**************************************************************************
 * @brief      Spi_ReadIB
 * @param[in]  Channel             ID của kênh
 * @param[out] DataBufferPointer   Con trỏ đến bộ đệm dữ liệu đích trong RAM
 * @return     Std_ReturnType	Trả về E_OK nếu lệnh đọc được chấp nhận
 *                      			   E_NOT_OK nếu lệnh đọc không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer);

/**************************************************************************
 * @brief        Spi_SetupEB
 * @param[in]    Channel            ID của kênh
 * @param[in]    SrcDataBufferPtr   Con trỏ đến bộ đệm dữ liệu nguồn
 * @param[in]    Length             Độ dài (số phần tử dữ liệu) của dữ liệu 
 *                                  được truyền từ SrcDataBufferPtr 
 *                                  và/hoặc nhận vào DesDataBufferPtr
 * @param[inout] DesDataBufferPtr   Con trỏ đến bộ đệm dữ liệu đích trong RAM
 *
 * @return       Std_ReturnType     Trả về E_OK nếu lệnh thiết lập được chấp nhận
 *                                         E_NOT_OK nếu lệnh thiết lập không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length);

/**************************************************************************
 * @brief 	Spi_GetStatus
 * @param	None 
 * @return	Spi_StatusType	Trả về SPI_UNINIT nếu chưa khởi tạo hoặc không thể sử dụng
 *								   SPI_IDLE nếu đang ở trạng thái rảnh
 *								   SPI_BUSY nếu đang thực hiện một Job
 **************************************************************************/
Spi_StatusType Spi_GetStatus(void);

/**************************************************************************
 * @brief     Spi_GetJobResult
 * @param[in] Job   ID của Job (nếu ID không hợp lệ, kết quả trả về không xác định)
 *
 * @return    Spi_JobResultType Trả về SPI_JOB_OK nếu truyền thành công
 *                    				   SPI_JOB_PENDING nếu đang truyền Job
 *                    				   SPI_JOB_FAILED nếu truyền thất bại
 *                    				   SPI_JOB_QUEUED nếu lệnh truyền đã được chấp nhận nhưng chưa bắt đầu
 **************************************************************************/
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

/**************************************************************************
 * @brief      Spi_GetSequenceResult
 * @param[in]  Sequence    ID của Sequence
 *
 * @return     Spi_SeqResultType Trả về SPI_SEQ_OK nếu truyền thành công
 *                      				SPI_SEQ_PENDING nếu đang truyền
 *                      				SPI_SEQ_FAILED nếu truyền thất bại
 *                      				SPI_SEQ_CANCELED nếu truyền bị hủy bởi người dùng
 **************************************************************************/
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

/**************************************************************************
 * @brief 	   Spi_GetVersionInfo
 * @param[out] versioninfo Con trỏ đến biến lưu trữ thông tin phiên bản của mô-đun
 * @return	   None
 **************************************************************************/
void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**************************************************************************
 * @brief 	  Spi_SyncTransmit
 * @param[in] Sequence      ID của Sequence
 *
 * @return	  Std_ReturnType Trả về E_OK nếu lệnh truyền được chấp nhận
 *								 	E_NOT_OK nếu lệnh truyền không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

/**************************************************************************
 * @brief 	  Spi_GetHWUnitStatus
 * @param[in] HWUnit 	Bộ SPI muốn kiểm tra trạng thái  
 *
 * @return	  Spi_StatusType Trả về SPI_UNINIT nếu chưa khởi tạo hoặc không thể sử dụng
 *								    SPI_IDLE nếu đang ở trạng thái rảnh
 *								    SPI_BUSY nếu đang thực hiện một Job
 **************************************************************************/
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit);

/**************************************************************************
 * @brief     Spi_Cancel
 * @param[in] Sequence    ID của Sequence
 * @return    None
 **************************************************************************/
void Spi_Cancel(Spi_SequenceType Sequence);

/**************************************************************************
 * @brief     Spi_SetAsyncMode
 * @param[in] Mode    Chế độ mới cần thiết lập
 *
 * @return    Std_ReturnType Trả về E_OK nếu lệnh thiết lập thành công
 *                      			E_NOT_OK nếu lệnh thiết lập thất bại
 **************************************************************************/
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);

#endif /* SPI_H */