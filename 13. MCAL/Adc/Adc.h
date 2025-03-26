/***************************************************************************
 * @file    Adc.h
 * @brief   Khai báo giao diện cho ADC (Analog-to-Digital Converter) 
 * @details File này cung cấp giao diện để sử dụng ngoại vi ADC của vi điều khiển. 
 *          Nó chứa các kiểu dữ liệu định nghĩa, cấu trúc cấu hình và các hàm 
 *          liên quan đến ADC.
 * @version 1.0
 * @date    2024-12-02
 * @author  Tran Quang Khai
 ***************************************************************************/ 
#ifndef ADC_H
#define ADC_H 

#include "stm32f10x_adc.h"
#include "Adc_Types.h"
#include "Adc_Hw.h"
#include "Adc_Cfg.h"

/**************************************************************************
 * @brief 	  Adc_Init
 * @param[in] ConfigPtr 	Con trỏ cấu hình ADC
 * @return	  None
 **************************************************************************/
void Adc_Init(const Adc_ConfigType* ConfigPtr);

/**************************************************************************
 * @brief     Adc_SetupResultBuffer
 * @param[in] Group          ID của nhóm kênh ADC   
 * @param[in] DataBufferPtr  Con trỏ trỏ đến bộ đệm dữ liệu kết quả
 * @return    Std_ReturnType Trả về E_OK nếu con trỏ bộ đệm kết quả được khởi tạo thành công
 *                					E_NOT_OK nếu thao tác thất bại hoặc có lỗi phát triển xảy ra
 **************************************************************************/
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/**************************************************************************
 * @brief 	Adc_DeInit
 * @param	None	
 * @return	None
 **************************************************************************/
void Adc_DeInit(void);

/**************************************************************************
 * @brief 	  Adc_StartGroupConversion
 * @param[in] Group 	ID của nhóm kênh 	
 * @return	  None
 **************************************************************************/
void Adc_StartGroupConversion(Adc_GroupType Group);

/**************************************************************************
 * @brief 	  Adc_StopGroupConversion
 * @param[in] Group 	ID của nhóm kênh 	
 * @return	  None
 **************************************************************************/
void Adc_StopGroupConversion(Adc_GroupType Group);

/**************************************************************************
 * @brief      Adc_ReadGroup
 * @param[in]  Group          ID của nhóm kênh ADC  
 * @param[out] DataBufferPtr  Kết quả của tất cả các kênh thuộc nhóm kênh ADC 
 *                            được lưu vào bộ đệm dữ liệu tại địa chỉ con trỏ này
 *
 * @return     Std_ReturnType Trả về E_OK nếu kết quả có sẵn và được ghi vào bộ đệm dữ liệu
 *                					 E_NOT_OK nếu không có kết quả hoặc xảy ra lỗi phát triển
 **************************************************************************/
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/**************************************************************************
 * @brief     Adc_EnableHardwareTrigger
 * @param[in] Group   ID của nhóm kênh ADC  
 * @return    None
 **************************************************************************/
void Adc_EnableHardwareTrigger(Adc_GroupType Group);

/**************************************************************************
 * @brief     Adc_DisableHardwareTrigger
 * @param[in] Group   ID của nhóm kênh ADC  
 * @return    None
 **************************************************************************/
void Adc_DisableHardwareTrigger(Adc_GroupType Group);

/**************************************************************************
 * @brief     Adc_EnableGroupNotification
 * @param[in] Group   ID của nhóm kênh ADC  
 * @return    None
 **************************************************************************/
void Adc_EnableGroupNotification(Adc_GroupType Group);

/**************************************************************************
 * @brief 	  Adc_DisableGroupNotification
 * @param[in] Group   ID số của nhóm kênh ADC yêu cầu  
 * @return    None
 **************************************************************************/
void Adc_DisableGroupNotification(Adc_GroupType Group);

/**************************************************************************
 * @brief     Adc_GetGroupStatus
 * @param[in] Group           ID của nhóm kênh ADC
 * @return    Adc_StatusType  Trạng thái chuyển đổi của nhóm được yêu cầu
 **************************************************************************/
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);

/**************************************************************************
 * @brief      Adc_GetStreamLastPointer
 * @param[in]  Group             ID số của nhóm kênh ADC yêu cầu  
 * @param[out] PtrToSamplePtr    Con trỏ trỏ đến con trỏ bộ đệm kết quả  
 *
 * @return     Adc_StreamNumSampleType  Số lượng mẫu hợp lệ trên mỗi kênh  
 **************************************************************************/
Adc_StreamNumSampleType Adc_GetStreamLastPointer(Adc_GroupType Group, Adc_ValueGroupType** PtrToSamplePtr);

/**************************************************************************
 * @brief      Adc_GetStreamLastPointer
 * @param[out] versioninfo    Con trỏ đến nơi lưu trữ thông tin phiên bản của mô-đun
 * @return     None
 **************************************************************************/
void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**************************************************************************
 * @brief      Adc_SetPowerState
 * @param[out] Result  Nếu API trả về E_OK: 
 *                     - ADC_SERVICE_ACCEPTED: Thay đổi trạng thái nguồn đã được thực thi.
 *                     
 *                     Nếu API trả về E_NOT_OK: 
 *                     - ADC_NOT_INIT: Mô-đun ADC chưa được khởi tạo.  
 *                     - ADC_SEQUENCE_ERROR: Trình tự gọi API sai.  
 *                     - ADC_HW_FAILURE: Lỗi phần cứng ngăn ADC vào trạng thái nguồn yêu cầu.
 * 
 * @return     Std_ReturnType 	Trả về E_OK nếu thay đổi chế độ nguồn thành công 
 *             						   E_NOT_OK nếu yêu cầu bị từ chối  
 **************************************************************************/
Std_ReturnType Adc_SetPowerState(Adc_PowerStateRequestResultType* Result);

/**************************************************************************
 * @brief      Adc_GetCurrentPowerState
 * @param[out] CurrentPowerState  Trạng thái nguồn hiện tại của mô-đun ADC
 * @param[out] Result  
 *             Nếu API trả về E_OK: Trả về trạng thái nguồn hiện tại của ADC  
 *             Nếu API trả về E_NOT_OK: Yêu cầu bị từ chối
 * 
 * @return     Adc_StreamNumSampleType  Trạng thái nguồn hiện tại của ADC
 **************************************************************************/
Std_ReturnType Adc_GetCurrentPowerState(Adc_PowerStateType* CurrentPowerState, Adc_PowerStateRequestResultType* Result);

/**************************************************************************
 * @brief      Adc_GetTargetPowerState
 * @param[out] TargetPowerState    Trạng thái nguồn đích của bộ ADC phần cứng
 * @param[out] Result Nếu API trả về E_OK: ADC_SERVICE_ACCEPTED: Thay đổi trạng thái nguồn đã được thực hiện.
 *                                      Nếu API trả về E_NOT_OK:
 *                                          - ADC_NOT_INIT: Mô-đun ADC chưa được khởi tạo.
 *                                          - ADC_SEQUENCE_ERROR: Lỗi trình tự gọi API.
 *                                          - ADC_HW_FAILURE: Lỗi phần cứng ADC không cho phép thay đổi trạng thái nguồn.
 * @return     None
 **************************************************************************/
Std_ReturnType Adc_GetTargetPowerState(Adc_PowerStateType* TargetPowerState, Adc_PowerStateRequestResultType* Result);

/**************************************************************************
 * @brief 	   Adc_SetTargetPowerState
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
Std_ReturnType Adc_PreparePowerState(Adc_PowerStateType PowerState, Adc_PowerStateRequestResultType* Result);

#endif /* ADC_H */