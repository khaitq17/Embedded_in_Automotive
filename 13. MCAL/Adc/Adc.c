/***************************************************************************
 * @file    Adc.c
 * @brief   Định nghĩa các hàm điều khiển cho ngoại vi ADC
 * @details File này triển khai các hàm để sử dụng ngoại vi ADC của vi điều khiển. 
 * @version 1.0
 * @date    2024-12-02
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#include "Adc.h"

/**************************************************************************
 * @brief 	  Adc_Init
 * @details   Hàm này khởi tạo ADC. 
 * @param[in] ConfigPtr 	Con trỏ cấu hình ADC
 * @return	  None
 **************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
	// Kiểm tra con trỏ NULL
	if (ConfigPtr == NULL_PTR) {
		return;
	}
	
	// Cấu hình GPIO cho ADC
	Adc_Hw_SetupGPIO(ConfigPtr);
	
	// Khởi tạo các thông số cho ADC1 hoặc ADC2
	if (ConfigPtr->AdcInstance == ADC_INSTANCE_1) {
		Adc_Hw_Init_ADC1(ConfigPtr);
	} else if (ConfigPtr->AdcInstance == ADC_INSTANCE_2) {
		Adc_Hw_Init_ADC2(ConfigPtr);
	}
	
	// Thiết lập độ phân giải nếu cần (Độ phân giải ADC trên VĐK STM32F103 mặc định là 12 bit)
	
	// Gọi hàm callback 
	if (ConfigPtr->InitCallback != NULL_PTR) {
		ConfigPtr->InitCallback();
	}
}

/**************************************************************************
 * @brief     Adc_SetupResultBuffer
 * @details   Hàm này khởi tạo ADC với bộ đệm kết quả cho nhóm kênh ADC, 
 * 			  nơi các kết quả chuyển đổi sẽ được lưu trữ. Việc khởi tạo bằng 
 * 			  Adc_SetupResultBuffer là bắt buộc sau khi reset, trước khi có 
 * 			  thể bắt đầu chuyển đổi nhóm kênh ADC.
 *
 * @param[in] Group          ID của nhóm kênh ADC   
 * @param[in] DataBufferPtr  Con trỏ trỏ đến bộ đệm dữ liệu kết quả
 * @return    Std_ReturnType Trả về E_OK nếu con trỏ bộ đệm kết quả được khởi tạo thành công
 *                					E_NOT_OK nếu thao tác thất bại hoặc có lỗi phát triển xảy ra
 **************************************************************************/
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
	// Kiểm tra con trỏ NULL và Group hợp lệ
	if (DataBufferPtr == NULL_PTR || Group >= ADC_MAX_GROUPS) {
		return E_NOT_OK;
	}
	
	if (adcInstance[Group] == ADC_INSTANCE_1) {
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		*DataBufferPtr = ADC_GetConversionValue(ADC1);
		
	} else if (adcInstance[Group] == ADC_INSTANCE_2) {
		ADC_RegularChannelConfig(ADC2, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		*DataBufferPtr = ADC_GetConversionValue(ADC2);
	
	} else {
		return E_NOT_OK;
	}
	
	return E_OK;
}

/**************************************************************************
 * @brief 	Adc_DeInit
 * @details Hàm này khởi tạo lại ADC.
 * @param	None	
 * @return	None
 **************************************************************************/
void Adc_DeInit(void)
{
	ADC_DeInit(ADC1);
	ADC_DeInit(ADC2);
	
	// Tắt phần cứng ADC
	Adc_Hw_DisableADC();
}
	
/**************************************************************************
 * @brief 	  Adc_StartGroupConversion
 * @details   Hàm này bắt đầu quá trình chuyển đổi của 1 nhóm kênh ADC.
 * @param[in] Group 	ID của nhóm kênh 	
 * @return	  None
 **************************************************************************/
void Adc_StartGroupConversion(Adc_GroupType Group)
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return;
	}
	
	Adc_Hw_StartConversion(adcInstance[Group]);
}

/**************************************************************************
 * @brief 	  Adc_StopGroupConversion
 * @details   Hàm này dừng quá trình chuyển đổi của 1 nhóm kênh ADC.
 * @param[in] Group 	ID của nhóm kênh 	
 * @return	  None
 **************************************************************************/
void Adc_StopGroupConversion(Adc_GroupType Group)
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return;
	}
	
	Adc_Hw_StopConversion(adcInstance[Group]);
}

/**************************************************************************
 * @brief      Adc_ReadGroup
 * @details    Hàm này đọc kết quả chuyển đổi của nhóm ADC từ lần chuyển đổi 
 *             hoàn thành gần nhất và lưu giá trị của các kênh bắt đầu từ địa 
 *             chỉ mà DataBufferPtr trỏ tới. Các giá trị của kênh trong nhóm 
 *             được lưu theo thứ tự số kênh tăng dần (khác với cách lưu trữ của 
 *             bộ đệm kết quả nếu chế độ truy cập streaming được cấu hình).
 *
 * @param[in]  Group          ID của nhóm kênh ADC  
 * @param[out] DataBufferPtr  Kết quả của tất cả các kênh thuộc nhóm kênh ADC 
 *                            được lưu vào bộ đệm dữ liệu tại địa chỉ con trỏ này
 *
 * @return     Std_ReturnType Trả về E_OK nếu kết quả có sẵn và được ghi vào bộ đệm dữ liệu
 *                					 E_NOT_OK nếu không có kết quả hoặc xảy ra lỗi phát triển
 **************************************************************************/
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
	// Kiểm tra con trỏ NULL và Group hợp lệ
	if (DataBufferPtr == NULL_PTR || Group >= ADC_MAX_GROUPS) {
		return E_NOT_OK;
	}
	
	return Adc_Hw_ReadGroup(Group, DataBufferPtr);
}

/**************************************************************************
 * @brief     Adc_EnableHardwareTrigger
 * @details   Hàm này kích hoạt bộ kích hoạt phần cứng (hardware trigger)  
 *            cho nhóm kênh ADC.
 * @param[in] Group   ID của nhóm kênh ADC  
 * @return    None
 **************************************************************************/
void Adc_EnableHardwareTrigger(Adc_GroupType Group) 
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return;
	}
	
	Adc_Hw_EnableHardwareTrigger(Group);
}

/**************************************************************************
 * @brief     Adc_DisableHardwareTrigger
 * @details   Hàm này vô hiệu hóa bộ kích hoạt phần cứng (hardware trigger)  
 *            cho nhóm kênh ADC.
 * @param[in] Group   ID của nhóm kênh ADC  
 * @return    None
 **************************************************************************/
void Adc_DisableHardwareTrigger(Adc_GroupType Group) 
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return;
	}
	
	Adc_Hw_DisableHardwareTrigger(Group);
}

/**************************************************************************
 * @brief     Adc_EnableGroupNotification
 * @details   Hàm này kích hoạt cơ chế thông báo (notification mechanism)  
 *            cho nhóm kênh ADC được yêu cầu.
 * @param[in] Group   ID số của nhóm kênh ADC yêu cầu  
 * @return    None
 **************************************************************************/
void Adc_EnableGroupNotification(Adc_GroupType Group)
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return;
	}
	
	Adc_Hw_EnableNotification(Group);
}

/**************************************************************************
 * @brief 	  Adc_DisableGroupNotification
 * @details   Hàm này tắt cơ chế thông báo (notification mechanism) cho  
 *            nhóm kênh ADC.
 * @param[in] Group   ID số của nhóm kênh ADC yêu cầu  
 * @return    None
 **************************************************************************/
void Adc_DisableGroupNotification(Adc_GroupType Group)
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return;
	}
	
	Adc_Hw_DisableNotification(Group);
}

/**************************************************************************
 * @brief     Adc_GetGroupStatus
 * @details   Hàm này trả về trạng thái chuyển đổi của nhóm kênh ADC.
 * @param[in] Group           ID của nhóm kênh ADC
 * @return    Adc_StatusType  Trạng thái chuyển đổi của nhóm được yêu cầu
 **************************************************************************/
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
	// Kiểm tra Group hợp lệ
	if (Group >= ADC_MAX_GROUPS) {
		return ADC_IDLE;
	}
	
	return Adc_Hw_GetGroupStatus(Group);
}

/**************************************************************************
 * @brief      Adc_GetStreamLastPointer
 * @details    Hàm này trả về số lượng mẫu hợp lệ trên mỗi kênh, được lưu 
 * 			   trong bộ đệm kết quả. Đọc một con trỏ trỏ đến một vị trí trong 
 * 			   bộ đệm kết quả của nhóm kênh. Với vị trí con trỏ này, có thể 
 * 			   truy cập kết quả của nhóm kênh từ quá trình chuyển đổi hoàn 
 * 			   thành gần nhất.
 *
 * @param[in]  Group             ID số của nhóm kênh ADC yêu cầu  
 * @param[out] PtrToSamplePtr    Con trỏ trỏ đến con trỏ bộ đệm kết quả  
 *
 * @return     Adc_StreamNumSampleType  Số lượng mẫu hợp lệ trên mỗi kênh  
 **************************************************************************/
Adc_StreamNumSampleType Adc_GetStreamLastPointer(Adc_GroupType Group, Adc_ValueGroupType** PtrToSamplePtr)
{
	Adc_StreamNumSampleType numSample;

	return numSample;
}

/**************************************************************************
 * @brief      Adc_GetStreamLastPointer
 * @details    Hàm này trả về thông tin phiên bản của mô-đun.
 * @param[out] versioninfo    Con trỏ đến nơi lưu trữ thông tin phiên bản của mô-đun
 * @return     None
 **************************************************************************/
void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo)
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
 * @brief      Adc_SetPowerState
 * @details    Hàm này cấu hình mô-đun ADC để chuyển sang trạng thái nguồn đã 
 *             được chuẩn bị trước, được chọn từ một tập hợp các trạng thái được 
 *             cấu hình sẵn.
 * @param[out] Result  Nếu API trả về E_OK: 
 *                     - ADC_SERVICE_ACCEPTED: Thay đổi trạng thái nguồn đã được thực thi
 *                     
 *                     Nếu API trả về E_NOT_OK: 
 *                     - ADC_NOT_INIT: Mô-đun ADC chưa được khởi tạo  
 *                     - ADC_SEQUENCE_ERROR: Trình tự gọi API sai  
 *                     - ADC_HW_FAILURE: Lỗi phần cứng ngăn ADC vào trạng thái nguồn yêu cầu
 * 
 * @return     Std_ReturnType 	Trả về E_OK nếu thay đổi chế độ nguồn thành công 
 *             						   E_NOT_OK nếu yêu cầu bị từ chối  
 **************************************************************************/
Std_ReturnType Adc_SetPowerState(Adc_PowerStateRequestResultType* Result)
{

	return E_OK;
}

/**************************************************************************
 * @brief      Adc_GetCurrentPowerState
 * @details    Hàm này trả về trạng thái nguồn hiện tại của phần cứng ADC.
 * @param[out] CurrentPowerState  Trạng thái nguồn hiện tại của mô-đun ADC
 * @param[out] Result  
 *             Nếu API trả về E_OK: Trả về trạng thái nguồn hiện tại của ADC  
 *             Nếu API trả về E_NOT_OK: Yêu cầu bị từ chối
 * 
 * @return     Adc_StreamNumSampleType  Trạng thái nguồn hiện tại của ADC
 **************************************************************************/ 
Std_ReturnType Adc_GetCurrentPowerState(Adc_PowerStateType* CurrentPowerState, Adc_PowerStateRequestResultType* Result)
{

	return E_OK;
}

/**************************************************************************
 * @brief      Adc_GetTargetPowerState
 * @details    Hàm này trả về trạng thái nguồn đích của bộ ADC phần cứng.
 * @param[out] TargetPowerState    Trạng thái nguồn đích của bộ ADC phần cứng
 * @param[out] Result Nếu API trả về E_OK: ADC_SERVICE_ACCEPTED: Thay đổi trạng thái nguồn đã được thực hiện.
 *                                      Nếu API trả về E_NOT_OK:
 *                                          - ADC_NOT_INIT: Mô-đun ADC chưa được khởi tạo.
 *                                          - ADC_SEQUENCE_ERROR: Lỗi trình tự gọi API.
 *                                          - ADC_HW_FAILURE: Lỗi phần cứng ADC không cho phép thay đổi trạng thái nguồn.
 * @return     None
 **************************************************************************/
Std_ReturnType Adc_GetTargetPowerState(Adc_PowerStateType* TargetPowerState, Adc_PowerStateRequestResultType* Result)
{

	return E_OK;
}

/**************************************************************************
 * @brief 	   Adc_SetTargetPowerState
 * @details    Hàm này bắt đầu quá trình cần thiết để cho phép module phần 
 *             cứng ADC chuyển sang trạng thái nguồn yêu cầu.
 * @param[in]  PowerState 	Trạng thái nguồn đích cần đạt được 
 * @param[out] Result 		Nếu API trả về E_OK: ADC_SERVICE_ACCEPTED: Thay đổi trạng thái nguồn đã được bắt đầu.
 *							
 *							Nếu API trả về E_NOT_OK: 
 *                          	- ADC_NOT_INIT: Mô-đun ADC chưa được khởi tạo. 
 *                          	- ADC_SEQUENCE_ERROR: Thứ tự gọi API không đúng.
 *                         		- ADC_HW_FAILURE: Lỗi phần cứng ADC, không thể chuyển trạng thái nguồn.
 * @return	   Std_ReturnType 	Trả về E_OK nếu chuyển trạng thái thành công
 *									   E_NOT_OK nếu yêu cầu bị từ chối
 **************************************************************************/
Std_ReturnType Adc_PreparePowerState(Adc_PowerStateType PowerState, Adc_PowerStateRequestResultType* Result)
{

	return E_OK;
}