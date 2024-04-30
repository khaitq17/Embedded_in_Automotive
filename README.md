# Embedded-Automotive

Bài 2: GPIO

1. C?p xung clock cho GPIO
S? d?ng các API ???c cung c?p s?n cho t?ng Bus. Các ngo?i vi trên Bus ???c c?p xung thông qua vi?c truy?n các tham s? vào API này.

Module RCC (Reset and Clock control) cung c?p các hàm ?? c?u hình xung clock.
RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
RCC_APB2Periph: RCC_APB2Periph_GPIOA
                               RCC_APB2Periph_TIM2
                              RCC_APB2Periph_ADC1
                                           ……….
FunctionalState NewState: ENABLE or DISABLE
                           
2. C?u hình GPIO
2.1. C?u hình Pin GPIO
M?t chân trong GPIO có th? ???c c?u hình thông qua struct GPIO_InitTypeDef g?m các thông s?:
GPIO_Pin: Ch?n Pin
GPIO_Mode: Ch?n Mode 
GPIO_Speed: T?c ?? ?áp ?ng
2.2. Kh?i t?o GPIO
Hàm GPIO_Init() dùng ?? kh?i t?o GPIO g?m 2 thông s?:
GPIO_TypeDef: GPIO c?n c?u hình
&GPIO_InitStruct: Con tr? t?i bi?n TypeDef v?a ???c t?o











3. Các hàm GPIO thông d?ng 
??c giá tr? 1 bit chân input
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

??c giá tr? t?t c? các bit chân input 
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);

??c giá tr? 1 bit chân output
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

??c giá tr? t?t c? các bit chân output
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);

Set bit lên m?c 1
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

Reset bit v? m?c 0
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

Ghi giá tr? cho 1 bit 
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

Ghi giá tr? cho 1 chân
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);



