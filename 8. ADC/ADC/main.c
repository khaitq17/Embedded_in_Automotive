#include "stm32f10x.h"                
#include "stm32f10x_rcc.h"             
#include "stm32f10x_gpio.h"         
#include "stm32f10x_adc.h"          
#include "stm32f10x_tim.h" 
#include "kalman.h"

void RCC_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
void TIMER_Config(void);
void delay_ms(uint32_t time);
uint16_t val;
float valupdate;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	ADC_Config();
	TIMER_Config();
	SimpleKalmanFilter(1, 2, 0.001f);
	
	while(1)
	{
		// Đọc giá trị chuyển đổi từ ADC
		val = ADC_GetConversionValue(ADC1);

		// Sử dụng bộ lọc Kalman để giảm nhiễu
		valupdate = (float)updateEstimate((float)val);

		delay_ms(100);
	}
}

void RCC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ADC_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // Chế độ đơn kênh
	ADC_InitStruct.ADC_NbrOfChannel = 1; // 1 kênh
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; // Chế độ chuyển đổi liên tục
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; // Căn lề phải
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // Không sử dụng nguồn kích hoạt ngoài
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; // Không quét lần lượt từng kênh

	ADC_Init(ADC1, &ADC_InitStruct);

	// Cấu hình kênh 0 của ADC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// Bật ADC1
	ADC_Cmd(ADC1, ENABLE);
	
	// Bật chuyển đổi ADC bằng phần mềm
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void TIMER_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;
	TIM_InitStruct.TIM_Period = 0xFFFF - 1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t time)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < time * 10) {}
}