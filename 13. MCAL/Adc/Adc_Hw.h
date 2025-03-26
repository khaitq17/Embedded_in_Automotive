/***************************************************************************
 * @file    Adc_Hw.h
 * @brief   Quản lý phần cứng ADC 
 * @details File này chứa các macro và hàm để quản lý phần cứng ADC trên 
 * 			vi điều khiển STM32F103.
 * @version 1.0
 * @date    2024-12-02
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef ADC_HW_H
#define ADC_HW_H

#include "stm32f10x_gpio.h"  
#include "stm32f10x_adc.h"          
#include "Adc_Types.h"
#include "Adc_Cfg.h"

/**************************************************************************
 * @brief Cấp clock và cấu hình GPIO cho ADC
 * @param RCC_GPIO	 	Clock cho GPIO
 * @param GPIO_PORT 	Cổng GPIO
 * @param GPIO_PIN 		Chân GPIO
 **************************************************************************/
#define SETUP_ADC_GPIO(RCC_GPIO, GPIO_PORT, GPIO_PIN)	\
	do {												\
		/* Cấp clock cho GPIO */						\
		RCC_APB2PeriphClockCmd((RCC_GPIO), ENABLE);		\
		/* Cấu hình GPIO cho ADC */						\
		GPIO_InitTypeDef GPIO_InitStruct;				\
		GPIO_InitStruct.GPIO_Pin = (GPIO_PIN);			\
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		\
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	\
		GPIO_Init((GPIO_PORT), &GPIO_InitStruct);		\
	} while(0)

/**************************************************************************
 * @brief  Khởi tạo phần cứng GPIO sử dụng cho ADC
 * @param  ConfigPtr 	Con trỏ cấu hình ADC 
 * @return None
 **************************************************************************/
static inline void Adc_Hw_SetupGPIO(const Adc_ConfigType* ConfigPtr)
{
	for (uint8 i = 0; i < ConfigPtr->NumChannels; i++) {
		Adc_ChannelType channel = ConfigPtr->Channels[i].Channel;
		
		if (channel < ADC_CHANNEL_7) {
			// Từ ADC_CHANNEL_0 đến ADC_CHANNEL_7 nằm trong GPIOA, tương ứng từ PA0 đến PA7
			SETUP_ADC_GPIO(RCC_APB2Periph_GPIOA, GPIOA, (1 << channel));	// Cấu hình GPIOA cho ADC
			
		} else if (channel == ADC_CHANNEL_8 || channel == ADC_CHANNEL_9) {
			// ADC_CHANNEL_8 và ADC_CHANNEL_9 nằm trong GPIOB, tương ứng PB0 và PB1 
			SETUP_ADC_GPIO(RCC_APB2Periph_GPIOB, GPIOB, (1 << ((channel) - 8)));	// Cấu hình GPIOA cho ADC
			
		} else if (channel >= ADC_CHANNEL_10 && channel <= ADC_CHANNEL_15) {
			// Từ ADC_CHANNEL_10 đến ADC_CHANNEL_15 nằm trong GPIOC, tương ứng từ PC0 đến PC5 
			SETUP_ADC_GPIO(RCC_APB2Periph_GPIOC, GPIOC, (1 << ((channel) - 10)));	// Cấu hình GPIOC cho ADC
		} 
	}
}

/**************************************************************************
 * @brief  Khởi tạo phần cứng ADC1 
 * @param  ConfigPtr 	Con trỏ cấu hình ADC1
 * @return None
 **************************************************************************/
static inline void Adc_Hw_Init_ADC1(const Adc_ConfigType* ConfigPtr)
{
	// Cấp clock cho ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitTypeDef ADC_InitStruct;
	
	// Chế độ hoạt động độc lập
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	
	// Kiểm tra chế độ chuyển đổi
	ADC_InitStruct.ADC_ContinuousConvMode = (ConfigPtr->ConversionMode == ADC_CONV_MODE_CONTINUOUS) ? ENABLE : DISABLE;
	
	// Không sử dụng chế độ quét
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	// Số kênh ADC
	ADC_InitStruct.ADC_NbrOfChannel = ConfigPtr->NumChannels;
	
	// Kiểm tra nguồn kích hoạt
	ADC_InitStruct.ADC_ExternalTrigConv = (ConfigPtr->TriggerSource == ADC_TRIGG_SRC_SW) 
																					? ADC_ExternalTrigConv_None : ADC_ExternalTrigConv_T1_CC1;
	
	// Căn lề phải (Mặc định)
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// Hiệu chuẩn ADC1
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	// Bật ADC1 and bật chuyển đổi ADC bằng phần mềm 
	ADC_Cmd(ADC1, ENABLE);
	if (ConfigPtr->TriggerSource == ADC_TRIGG_SRC_SW) {
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
}

/**************************************************************************
 * @brief  Khởi tạo phần cứng ADC2 
 * @param  ConfigPtr 	Con trỏ cấu hình ADC2
 * @return None
 **************************************************************************/
static inline void Adc_Hw_Init_ADC2(const Adc_ConfigType* ConfigPtr)
{
	// Cấp clock cho ADC2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	ADC_InitTypeDef ADC_InitStruct;
	
	// Chế độ hoạt động độc lập
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	
	// Kiểm tra chế độ chuyển đổi
	ADC_InitStruct.ADC_ContinuousConvMode = (ConfigPtr->ConversionMode == ADC_CONV_MODE_CONTINUOUS) ? ENABLE : DISABLE;
	
	// Không sử dụng chế độ quét
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	// Số kênh ADC
	ADC_InitStruct.ADC_NbrOfChannel = ConfigPtr->NumChannels;
	
	// Kiểm tra nguồn kích hoạt
	ADC_InitStruct.ADC_ExternalTrigConv = (ConfigPtr->TriggerSource == ADC_TRIGG_SRC_SW) 
																					? ADC_ExternalTrigConv_None : ADC_ExternalTrigConv_T1_CC1;
	
	// Căn lề phải (Mặc định)
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	// Hiệu chuẩn ADC2
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
	
	// Bật ADC2 and bật chuyển đổi ADC bằng phần mềm 
	ADC_Cmd(ADC2, ENABLE);
	if (ConfigPtr->TriggerSource == ADC_TRIGG_SRC_SW) {
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
	}
}

/**************************************************************************
 * @brief 	Cấu hình các kênh ADC
 * @param	ConfigPtr		Con trỏ cấu hình các kênh ADC 
 * @return	None
 **************************************************************************/
static inline void Adc_Hw_SetupChannels(const Adc_ConfigType* ConfigPtr)
{
	for (uint8 i = 0; i < ConfigPtr->NumChannels; i++) {
		Adc_ChannelType channel = ConfigPtr->Channels[i].Channel;
		Adc_SamplingTimeType samplingTime = ConfigPtr->Channels[i].SamplingTime;
		uint8 rank = ConfigPtr->Channels[i].Rank;
		
		if (ConfigPtr->AdcInstance == ADC_INSTANCE_1) {
			ADC_RegularChannelConfig(ADC1, channel, rank, samplingTime);	// Cấu hình cho ADC1
		} else if (ConfigPtr->AdcInstance == ADC_INSTANCE_2) {
			ADC_RegularChannelConfig(ADC2, channel, rank, samplingTime);	// Cấu hình cho ADC2
		}
	}
}

/**************************************************************************
 * @brief  Bật phần cứng ADC
 * @param  ConfigPtr 	Con trỏ cấu hình để bật ADC 
 * @return None
 **************************************************************************/
static inline void Adc_Hw_EnableADC(const Adc_ConfigType* ConfigPtr)
{
	if (ConfigPtr->AdcInstance == ADC_INSTANCE_1) {
		ADC_Cmd(ADC1, ENABLE);
	} else if (ConfigPtr->AdcInstance == ADC_INSTANCE_2) {
		ADC_Cmd(ADC2, ENABLE);
	}
}

/**************************************************************************
 * @brief  Tắt phần cứng ADC
 * @param  ConfigPtr 	Con trỏ cấu hình để tắt ADC 
 * @return None
 **************************************************************************/
static inline void Adc_Hw_DisableADC(void)
{
	// Tắt phần cứng ADC1
	ADC_Cmd(ADC1, DISABLE);
	
	// Tắt phần cứng ADC2
	ADC_Cmd(ADC2, DISABLE);
}

/**************************************************************************
 * @brief  Bắt đầu chuyển đổi một nhóm kênh ADC 
 * @param  Group 	ID của nhóm kênh ADC 
 * @return None
 **************************************************************************/
static inline void Adc_Hw_StartConversion(Adc_InstanceType AdcInstance)
{
	if (AdcInstance == ADC_INSTANCE_1) {
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	} else if (AdcInstance == ADC_INSTANCE_2) {
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
	}
}

/**************************************************************************
 * @brief  Dừng chuyển đổi một nhóm kênh ADC 
 * @param  Group 	ID của nhóm kênh ADC
 * @return None
 **************************************************************************/
static inline void Adc_Hw_StopConversion(Adc_InstanceType AdcInstance)
{
	if (AdcInstance == ADC_INSTANCE_1) {
		// Dừng cả trigger phần mềm và phần cứng của ADC1
		ADC_SoftwareStartConvCmd(ADC1, DISABLE);
		ADC_ExternalTrigConvCmd(ADC1, DISABLE);
	} else if (AdcInstance == ADC_INSTANCE_2) {
		// Dừng cả trigger phần mềm và phần cứng của ADC2
		ADC_SoftwareStartConvCmd(ADC2, DISABLE);
		ADC_ExternalTrigConvCmd(ADC2, DISABLE);
	}
}

/**************************************************************************
 * @brief  Đọc giá trị chuyển đổi của nhóm kênh ADC
 * @param  Group 			ID của nhóm kênh ADC 
 * @param  DataBufferPtr 	Con trỏ lưu giá trị chuyển đổi đọc được của nhóm kênh
 * @return Std_ReturnType 	Trả về E_OK nếu đọc giá trị và lưu vào trong bộ đệm thành công
 *						   		   E_NOT_OK nếu đọc giá trị và lưu vào trong bộ đệm không thành công
 **************************************************************************/
static inline Std_ReturnType Adc_Hw_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
	// Lấy cấu hình ADC của nhóm kênh ADC đang xét
	const Adc_ConfigType* adcConfig = &Adc_Configs[Group]; 
	
	// Lấy số kênh trong nhóm
	uint8 numChannels = adcConfig->NumChannels;
	
	// Lấy bộ ADC sử dụng
	ADC_TypeDef* ADCx = (adcConfig->AdcInstance == ADC_INSTANCE_1) ? ADC1 : ADC2;
	
	// Đọc giá trị ADC cho từng kênh trong nhóm
	for (uint8 i = 0; i < numChannels; i++) {
		DataBufferPtr[i] = ADC_GetConversionValue(ADCx);
	}
		
	return E_OK;
}

/**************************************************************************
 * @brief  Bật kích hoạt phần cứng cho nhóm kênh ADC
 * @param  Group 	ID của nhóm kênh ADC
 * @return None
 **************************************************************************/
static inline void Adc_Hw_EnableHardwareTrigger(Adc_GroupType Group)
{
	if (Group == ADC_INSTANCE_1) {
		ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	} else if (Group == ADC_INSTANCE_2) {
		ADC_ExternalTrigConvCmd(ADC2, ENABLE);
	} 
}

/**************************************************************************
 * @brief  Tắt kích hoạt phần cứng cho nhóm kênh ADC
 * @param  Group 	ID của nhóm kênh ADC
 * @return None
 **************************************************************************/
static inline void Adc_Hw_DisableHardwareTrigger(Adc_GroupType Group)
{
	ADC_ExternalTrigConvCmd(ADC1, DISABLE);
	ADC_ExternalTrigConvCmd(ADC2, DISABLE);
}

/**************************************************************************
 * @brief  Bật thông báo cho nhóm kênh ADC
 * @param  Group 	ID của nhóm kênh ADC
 * @return None
 **************************************************************************/
static inline void Adc_Hw_EnableNotification(Adc_GroupType Group)
{
	
}

/**************************************************************************
 * @brief  Tắt thông báo cho nhóm kênh ADC
 * @param  Group 	ID của nhóm kênh ADC
 * @return None
 **************************************************************************/
static inline void Adc_Hw_DisableNotification(Adc_GroupType Group)
{
	
}

/**************************************************************************
 * @brief  Trả về trạng thái của nhóm kênh ADC 
 * @param  Group 	ID của nhóm kênh ADC
 * @return None
 **************************************************************************/
static inline Adc_StatusType Adc_Hw_GetGroupStatus(Adc_GroupType Group)
{
	if (adcInstance[Group] == ADC_INSTANCE_1) {
		if (ADC1->SR & ADC_SR_STRT) {
			return ADC_BUSY;  // Đang trong quá trình chuyển đổi
		} else if (ADC1->SR & ADC_SR_EOC) {
			return ADC_COMPLETED; // Chuyển đổi hoàn tất
		}
	} else if (adcInstance[Group] == ADC_INSTANCE_2) {
		if (ADC2->SR & ADC_SR_STRT) {
			return ADC_BUSY;  // Đang trong quá trình chuyển đổi
		} else if (ADC2->SR & ADC_SR_EOC) {
			return ADC_COMPLETED; // Chuyển đổi hoàn tất
		}
	} 
	
	return ADC_IDLE;
}

#endif	/* ADC_HW_H */