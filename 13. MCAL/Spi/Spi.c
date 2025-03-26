/***************************************************************************
 * @file    Spi.c
 * @brief   Định nghĩa các hàm điều khiển cho giao thức SPI
 * @details File này triển khai các hàm để sử dụng và giao tiếp cho giao thức 
 * 			SPI của vi điều khiển. 
 * @version 1.0
 * @date    2024-11-28
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#include "Spi.h"

/**************************************************************************
 * @brief Các biến static lưu trữ trạng thái của SPI và kết quả của Job, Sequence
 **************************************************************************/
static Spi_StatusType SpiStatus = SPI_UNINIT;
static Spi_JobResultType JobResult = SPI_JOB_OK;
static Spi_SeqResultType SeqResult = SPI_SEQ_OK;
 
/**************************************************************************
 * @brief 	  Spi_Init
 * @details   Hàm này khởi tạo các thông số cho SPI.
 * @param[in] ConfigPtr		Con trỏ đến biến cấu hình SPI
 * @return	  None
 **************************************************************************/
void Spi_Init(const Spi_ConfigType* ConfigPtr)
{
	// Kiểm tra con trỏ NULL
	if (ConfigPtr == NULL_PTR) {
		return;
	}
	
	// Thiết lập các thông số mặc định
	Spi_SetupDefaultConfig((Spi_ConfigType*) ConfigPtr);
	
	// Xác định bộ SPI thông qua Channel
	Spi_HWUnitType spi_hw = spiHwUnit[ConfigPtr->Channel];
	
	// Khởi tạo và bật phần cứng SPI
	if (spi_hw == SPI_HW_UNIT_0) {	// SPI1
		Spi_Hw_Init_SPI1();
		Spi_Hw_Enable_SPI1();
	} else if (spi_hw == SPI_HW_UNIT_1) {	// SPI2
		Spi_Hw_Init_SPI2();
		Spi_Hw_Enable_SPI2();
	} else {
		// Kênh 3,4,...
		return;
	}
	
	// Cập nhật trạng thái SPI
	SpiStatus = SPI_IDLE;
	JobResult = SPI_JOB_OK;
	SeqResult = SPI_SEQ_OK;
}

/**************************************************************************
 * @brief 	Spi_DeInit
 * @details Hàm này khởi tạo lại SPI.
 * @param	None
 * @return	Std_ReturnType	Trả về E_OK nếu lệnh hủy khởi tạo được chấp nhận
 *								   E_NOT_OK nếu lệnh hủy khởi tạo không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_DeInit()
{
	// Kiểm tra xem SPI đã khởi tạo chưa, nếu chưa thì không thể khởi tạo lại
	if (SpiStatus == SPI_UNINIT) {
		return E_NOT_OK;
	}
	
	// Khởi tạo lại phần cứng cho SPI
	Spi_Hw_DeInit_SPI1();
	Spi_Hw_DeInit_SPI2();
	
	// Cập nhật trạng thái SPI
	SpiStatus = SPI_UNINIT;
	JobResult = SPI_JOB_OK;
	SeqResult = SPI_SEQ_OK;
	
	// Khởi tạo lại thành công
	return E_OK;	
}

/**************************************************************************
 * @brief     Spi_WriteIB
 * @details   Hàm này dùng để ghi một hoặc nhiều dữ liệu vào bộ đệm trong IB
 * 			  (Internal Buffer) của kênh SPI.
 * @param[in] Channel		ID của kênh
 * @param[in] DataBufferPtr	Con trỏ đến bộ đệm dữ liệu nguồn. Nếu con trỏ này 
 *         					là null, dữ liệu truyền đi sẽ không quan trọng và 
 *                  		giá trị mặc định của kênh này sẽ được sử dụng thay thế.
 *
 * @return    Std_ReturnType	Trả về E_OK nếu lệnh ghi được chấp nhận
 *                      			   E_NOT_OK nếu lệnh ghi không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr)
{
	// Kiểm tra xem SPI đã khởi tạo chưa, kiểm tra DataBufferPtr có phải con trỏ NULL không và kiểm tra Channel hợp lệ
	if (SpiStatus == SPI_UNINIT || DataBufferPtr == NULL_PTR || Channel >= SPI_MAX_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Xác định bộ SPI thông qua Channel
	Spi_HWUnitType spi_hw = spiHwUnit[Channel];
	
	// Ghi dữ liệu vào phần cứng SPI tương ứng
	if (spi_hw == SPI_HW_UNIT_0) {
		// Chờ đến khi bộ đệm truyền của SPI1 trống
		Spi_Hw_WaitTransmitBufferEmpty_SPI1();
		// Gửi dữ liệu qua SPI1
		Spi_I2S_SendData_SPI1(DataBufferPtr);
		
	} else if (spi_hw == SPI_HW_UNIT_1) {
		// Chờ đến khi bộ đệm truyền của SPI2 trống
		Spi_Hw_WaitTransmitBufferEmpty_SPI2();
		// Gửi dữ liệu qua SPI2
		Spi_I2S_SendData_SPI2(DataBufferPtr);
		
	} else {
		return E_NOT_OK;
	}
	
	// Ghi dữ liệu thành công
	return E_OK;
}

/**************************************************************************
 * @brief 	  Spi_AsyncTransmit
 * @details   Hàm này dùng để truyền dữ liệu trên bus SPI.
 * @param[in] Sequence    ID của Sequence
 *
 * @return    Std_ReturnType	Trả về E_OK nếu lệnh truyền được chấp nhận
 *                      			   E_NOT_OK nếu lệnh truyền không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence)
{
	// Kiểm tra xem SPI đã khởi tạo chưa, kiểm tra Sequence hợp lệ
	if (SpiStatus == SPI_UNINIT || Sequence >= SPI_MAX_SEQUENCE) {
		return E_NOT_OK;
	}
	
	return E_OK;
}


/**************************************************************************
 * @brief      Spi_ReadIB
 * @details    Hàm này dùng để đọc đồng bộ một hoặc nhiều dữ liệu từ bộ đệm 
 * 			   trong (Internal Buffer) của kênh SPI.
 * @param[in]  Channel             ID của kênh
 * @param[out] DataBufferPointer   Con trỏ đến bộ đệm dữ liệu đích trong RAM
 *
 * @return     Std_ReturnType	Trả về E_OK nếu lệnh đọc được chấp nhận
 *                      			   E_NOT_OK nếu lệnh đọc không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPointer)
{
	// Kiểm tra xem SPI đã khởi tạo chưa, kiểm tra DataBufferPointer có phải con trỏ NULL không và kiểm tra Channel hợp lệ
	if (SpiStatus == SPI_UNINIT || DataBufferPointer == NULL_PTR || Channel >= SPI_MAX_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Xác định bộ SPI thông qua Channel
	Spi_HWUnitType spi_hw = spiHwUnit[Channel];
	
	// Đọc dữ liệu từ phần cứng SPI tương ứng
	if (spi_hw == SPI_HW_UNIT_0) {
		// Chờ đến khi bộ đệm nhận của SPI1 đầy
		Spi_Hw_WaitReceiveBufferFull_SPI1();
		// Nhận dữ liệu từ SPI1
		*DataBufferPointer = Spi_I2S_ReceiveData_SPI1();
		
	} else if (spi_hw == SPI_HW_UNIT_1) {
		// Chờ đến khi bộ đệm nhận của SPI2 đầy
		Spi_Hw_WaitReceiveBufferFull_SPI2();
		// Nhận dữ liệu từ SPI2
		*DataBufferPointer = Spi_I2S_ReceiveData_SPI2();
		
	} else {
		return E_NOT_OK;
	}
	
	// Đọc dữ liệu thành công
	return E_OK;
}

/**************************************************************************
 * @brief        Spi_SetupEB
 * @details      Hàm này dùng để thiết lập bộ đệm và độ dài dữ liệu cho bộ
 * 				 đệm ngoài (External Buffer) của kênh SPI.
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
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length)
{
	// Kiểm tra xem SPI đã khởi tạo chưa và kiểm tra Channel hợp lệ
	if (SpiStatus == SPI_UNINIT || Channel >= SPI_MAX_CHANNELS) {
		return E_NOT_OK;
	}
	
	// Kiểm tra các con trỏ đến bộ đệm có phải NULL không và kiểm tra độ dài dữ liệu hợp lệ
	if ((SrcDataBufferPtr == NULL_PTR) || (DesDataBufferPtr == NULL_PTR) || (Length <= 0)) {
		return E_NOT_OK;
	}
	
	// Xác định bộ SPI thông qua Channel
	Spi_HWUnitType spi_hw = spiHwUnit[Channel];
	
	for (Spi_NumberOfDataType i = 0; i < Length; i++) {
		if (spi_hw == SPI_HW_UNIT_0) {
			// Chờ đến khi bộ đệm truyền của SPI1 trống
			Spi_Hw_WaitTransmitBufferEmpty_SPI1();
			
			// Gửi dữ liệu từ SrcDataBufferPtr qua SPI1
			Spi_I2S_SendData_SPI1(&SrcDataBufferPtr[i]);
			
			// Chờ đến khi bộ đệm nhận của SPI1 đầy
			Spi_Hw_WaitReceiveBufferFull_SPI1();
			
			// Lưu dữ liệu đọc được từ SPI1 vào DesDataBufferPtr
			DesDataBufferPtr[i] = Spi_I2S_ReceiveData_SPI1();
			
		} else if (spi_hw == SPI_HW_UNIT_1) {
			// Chờ đến khi bộ đệm truyền của SPI2 trống
			Spi_Hw_WaitTransmitBufferEmpty_SPI2();
			
			// Gửi dữ liệu từ SrcDataBufferPtr qua SPI2
			Spi_I2S_SendData_SPI2(&SrcDataBufferPtr[i]);
			
			// Chờ đến khi bộ đệm nhận của SPI2 đầy
			Spi_Hw_WaitReceiveBufferFull_SPI2();
			
			// Lưu dữ liệu đọc được từ SPI2 vào DesDataBufferPtr
			DesDataBufferPtr[i] = Spi_I2S_ReceiveData_SPI2();
			
		} else {
			return E_NOT_OK;
		}
	}
	
	return E_OK;
}

/**************************************************************************
 * @brief 	Spi_GetStatus
 * @details Hàm này trả về trạng thái phần mềm của SPI.
 * @param	None 
 * @return	Spi_StatusType	Trả về SPI_UNINIT nếu chưa khởi tạo hoặc không thể sử dụng
 *								   SPI_IDLE nếu đang ở trạng thái rảnh
 *								   SPI_BUSY nếu đang thực hiện một Job
 **************************************************************************/
Spi_StatusType Spi_GetStatus()
{
	// Kiểm tra xem SPI đã khởi tạo chưa
	if (SpiStatus == SPI_UNINIT) {
		return SPI_UNINIT;
	}
	
	// Kiểm tra trạng thái SPI1
	if (Spi_Hw_CheckStatus_SPI1() == SPI_BUSY) {
		return SPI_BUSY;
	}
	
	// Kiểm tra trạng thái SPI2
	if (Spi_Hw_CheckStatus_SPI2() == SPI_BUSY) {
		return SPI_BUSY;
	}
	
	return SPI_IDLE;
}

/**************************************************************************
 * @brief     Spi_GetJobResult
 * @details   Hàm này trả về kết quả truyền gần nhất của Job.
 * @param[in] Job   ID của Job (nếu ID không hợp lệ, kết quả trả về không xác định)
 *
 * @return    Spi_JobResultType Trả về SPI_JOB_OK nếu truyền thành công
 *                    				   SPI_JOB_PENDING nếu đang truyền Job
 *                    				   SPI_JOB_FAILED nếu truyền thất bại
 *                    				   SPI_JOB_QUEUED nếu lệnh truyền đã được chấp nhận nhưng chưa bắt đầu
 **************************************************************************/
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job)
{
	// Kiểm tra Job hợp lệ
	if (Job >= SPI_MAX_JOBS) {
		return SPI_JOB_OK;
	}
	
	return JobResult;
}

/**************************************************************************
 * @brief      Spi_GetSequenceResult
 * @details    Hàm này trả về kết quả truyền gần nhất của Sequence.
 * @param[in]  Sequence    ID của Sequence (nếu ID không hợp lệ, kết quả trả về không xác định)
 *
 * @return     Spi_SeqResultType Trả về SPI_SEQ_OK nếu truyền thành công
 *                      				SPI_SEQ_PENDING nếu đang truyền
 *                      				SPI_SEQ_FAILED nếu truyền thất bại
 *                      				SPI_SEQ_CANCELED nếu truyền bị hủy bởi người dùng
 **************************************************************************/
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence)
{
	// Kiểm tra Sequence hợp lệ
	if (Sequence >= SPI_MAX_SEQUENCE) {
		return SPI_SEQ_OK;
	}
	
	return SeqResult;
}

/**************************************************************************
 * @brief 	   Spi_GetVersionInfo
 * @details    Hàm này trả về thông tin phiên bản của mô-đun.
 * @param[out] versioninfo	Con trỏ đến biến lưu trữ thông tin phiên bản của mô-đun
 * @return	   None
 **************************************************************************/
void Spi_GetVersionInfo(Std_VersionInfoType* versioninfo) 
{
	// Kiểm tra con trỏ NULL
	if (versioninfo == NULL_PTR) {
		return;
	}
	
	versioninfo->vendorID = 1;
	versioninfo->moduleID = 123;
	versioninfo->sw_major_version = 1;
	versioninfo->sw_minor_version = 0;
	versioninfo->sw_patch_version = 0;
}

/**************************************************************************
 * @brief 	  Spi_SyncTransmit
 * @details   Hàm này dùng để truyền dữ liệu trên bus SPI.
 * @param[in] Sequence		Sequence ID
 *
 * @return	  Std_ReturnType Trả về E_OK nếu lệnh truyền được chấp nhận
 *								 	E_NOT_OK nếu lệnh truyền không được chấp nhận
 **************************************************************************/
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence)
{
	// Kiểm tra xem SPI được khởi tạo chưa và kiểm tra Sequence hợp lệ
	if (SpiStatus == SPI_UNINIT || Sequence >= SPI_MAX_SEQUENCE) {
		return E_NOT_OK;
	}
	
	// Truyền bất đồng bộ trước khi bắt đầu
	Std_ReturnType result = Spi_AsyncTransmit(Sequence);
	if (result != E_OK) {
		return E_NOT_OK;
	}
	
	// Chờ các Job của Sequence hoàn thành
	Spi_SeqResultType seqResult;
	do {
		SeqResult = Spi_GetSequenceResult(Sequence);
	} while (seqResult == SPI_SEQ_PENDING);
	
	// Kiểm tra kết quả Sequence cuối cùng
	if (SeqResult == SPI_SEQ_OK) {
		return E_OK;			
	} else {
		return E_NOT_OK;
	}
}

/**************************************************************************
 * @brief 	  Spi_GetHWUnitStatus
 * @details   Hàm này trả về trạng thái của phần cứng SPI.
 * @param[in] HWUnit 	Bộ SPI muốn kiểm tra trạng thái  
 *
 * @return	  Spi_StatusType Trả về SPI_UNINIT nếu chưa khởi tạo hoặc không thể sử dụng
 *								    SPI_IDLE nếu đang ở trạng thái rảnh
 *								    SPI_BUSY nếu đang thực hiện một Job
 **************************************************************************/
Spi_StatusType Spi_GetHWUnitStatus(Spi_HWUnitType HWUnit)
{
	// Kiểm tra trạng thái phần cứng SPI1
	if (Spi_Hw_CheckHWStatus_SPI1() == SPI_BUSY) {
		return SPI_BUSY;
	}
	
	// Kiểm tra trạng thái phần cứng SPI2
	if (Spi_Hw_CheckHWStatus_SPI2() == SPI_BUSY) {
		return SPI_BUSY;
	}
	
	// Trả về IDLE nếu cả SPI1 và SPI2 đều không bận
	return SPI_IDLE;
}

/**************************************************************************
 * @brief     Spi_Cancel
 * @details   Hàm này hủy quá trình truyền của Sequence đang diễn ra.
 * @param[in] Sequence    ID của Sequence
 * @return    None
 **************************************************************************/
void Spi_Cancel(Spi_SequenceType Sequence)
{
	// Kiểm tra xem SPI đã được khởi tạo chưa và kiểm tra Sequence hợp lệ
	if (SpiStatus == SPI_UNINIT || Sequence >= SPI_MAX_SEQUENCE) {
		return;
	}
	
	// Hủy quá trình truyền SPI dựa trên Sequence tương ứng
	if (Sequence == SPI_SEQUENCE_0) {
		Spi_Hw_Cancel_SPI1();
		
	} else if (Sequence == SPI_SEQUENCE_1 || Sequence == SPI_SEQUENCE_2) {
		Spi_Hw_Cancel_SPI2();
		
	} else {
		return;
	}
	
	SeqResult = SPI_SEQ_CANCELED;
}

/**************************************************************************
 * @brief     Spi_SetAsyncMode
 * @details   Hàm này thiết lập chế độ cơ chế bất đồng bộ cho các bus SPI 
 *            được xử lý theo cách bất đồng bộ.
 * @param[in] Mode    Chế độ mới cần thiết lập
 *
 * @return    Std_ReturnType Trả về E_OK nếu lệnh thiết lập thành công
 *                      			E_NOT_OK nếu lệnh thiết lập thất bại
 **************************************************************************/
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode)
{
	if (Mode == SPI_POLLING_MODE) {
		// Vô hiệu hóa ngắt SPI
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_ERR, DISABLE);
		
	} else if (Mode == SPI_INTERRUPT_MODE) {
		// Kích hoạt ngắt SPI
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_ERR, ENABLE);
	} else {
		return E_NOT_OK;  
	}
		
	return E_OK;
}