/***************************************************************************
 * @file    Adc_Cfg.h
 * @brief   Cấu hình các thông số cho ADC 
 * @details File này chứa cấu hình các thông số cho ADC và các macro để định
 * 	 		nghĩa các nhóm kênh sử dụng.
 * @version 1.0
 * @date    2024-12-02
 * @author  Tran Quang Khai
 ***************************************************************************/
#ifndef ADC_CFG_H
#define ADC_CFG_H

#include "Adc_Types.h"

/**************************************************************************
 * @brief Định nghĩa các nhóm kênh ADC sử dụng
 **************************************************************************/
#define ADC_MAX_GROUPS	2    /* Số lượng nhóm kênh ADC tối đa có thể cấu hình */

#define ADC_BUFFER_SIZE_GROUP_1   4    /* Kích thước bộ đệm ADC cho nhóm kênh 1 */
#define ADC_BUFFER_SIZE_GROUP_2   4    /* Kích thước bộ đệm ADC cho nhóm kênh 2 */

#define ADC_GROUP_1		(Adc_GroupType)0    /* Nhóm kênh 1 */
#define ADC_GROUP_2		(Adc_GroupType)1    /* Nhóm kênh 2 */

/**************************************************************************
 * @brief Định nghĩa các kênh ADC trong từng nhóm
 **************************************************************************/
#define ADC_GROUP_1_CHANNELS     { ADC_CHANNEL_0, ADC_CHANNEL_1 }	/* Kênh 0, 1 thuộc nhóm ADC1 */
#define ADC_GROUP_2_CHANNELS     { ADC_CHANNEL_2 }    				/* Kênh 2 thuộc nhóm ADC2 */

/**************************************************************************
 * @brief Mảng xác định nhóm kênh thuộc bộ ADC nào
 **************************************************************************/
const Adc_InstanceType adcInstance[ADC_MAX_GROUPS] = {
	ADC_INSTANCE_1, /* Nhóm kênh 1 thuộc ADC1 */
	ADC_INSTANCE_2	/* Nhóm kênh 2 thuộc ADC2 */
};

/**************************************************************************
 * @brief Cấu hình nhóm kênh ADC (ADC1, ADC2)
 **************************************************************************/
const Adc_ConfigType Adc_Configs[ADC_MAX_GROUPS] = {
	// Nhóm kênh 1 cho ADC1
	{
		.ClockPrescaler = ADC_CLOCK_PRESCALER_8,   	/* Chia tần số xung clock cho ADC */
		.ConversionMode = ADC_CONV_MODE_ONESHOT,	/* Chế độ chuyển đổi 1 lần */
		.Resolution = ADC_RESOLUTION_12BIT,  		/* Độ phân giải 12 bit */
		.TriggerSource = ADC_TRIGG_SRC_SW,   		/* Kích hoạt bằng phần mềm */
		.NumChannels = 2,    						/* 2 kênh */
		.AdcInstance = ADC_INSTANCE_1,  			/* ADC1 */

		.Channels = {
			{
				.Channel = ADC_CHANNEL_0,					/* Kênh 0 của ADC1 */
				.SamplingTime = ADC_SAMPLETIME_55CYCLE5, 	/* Thời gian lấy mẫu 55,5 chu kỳ */
				.Rank = 1	 								/* Ưu tiên cao nhất */
			},
			{
				.Channel = ADC_CHANNEL_1, 					/* Kênh 1 của ADC1 */
				.SamplingTime = ADC_SAMPLETIME_55CYCLE5,	/* Thời gian lấy mẫu 55,5 chu kỳ */
				.Rank = 2  									/* Ưu tiên thấp hơn */
			} 
		},
		
		.InitCallback = NULL_PTR	/* Không sử dụng callback khi khởi tạo */
	},

	// Nhóm kênh 2 cho ADC2
	{
		.ClockPrescaler = ADC_CLOCK_PRESCALER_4, 	/* Chia tần số xung clock cho ADC */
		.ConversionMode = ADC_CONV_MODE_CONTINUOUS,	/* Chế độ chuyển đổi liên tục */
		.Resolution = ADC_RESOLUTION_12BIT, 		/* Độ phân giải 12 bit */
		.TriggerSource = ADC_TRIGG_SRC_HW, 			/* Kích hoạt bằng phần cứng */
		.NumChannels = 1, 							/* 1 kênh */
		.AdcInstance = ADC_INSTANCE_2, 				/* ADC2 */
		
		.Channels = {
			{
				.Channel = ADC_CHANNEL_2, 					/* Kênh 2 của ADC2 */
				.SamplingTime = ADC_SAMPLETIME_28CYCLE5, 	/* Thời gian lấy mẫu 28,5 chu kỳ */
				.Rank = 1 									/* Ưu tiên cao nhất */
			}
		},

		.InitCallback = NULL_PTR	/* Không sử dụng callback khi khởi tạo */
	}
};


#endif	/* ADC_CFG_H */