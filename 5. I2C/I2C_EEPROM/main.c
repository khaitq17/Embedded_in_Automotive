#include "stm32f10x.h"              

#define I2C_SCL             GPIO_Pin_6
#define I2C_SDA             GPIO_Pin_7
#define I2C_GPIO            GPIOB

#define WRITE_SDA_0         GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1         GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0         GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1         GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL        GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum {
    NOT_OK = 0,
    OK = !NOT_OK
} status;

typedef enum {
    ACK = 0,
    NOT_ACK = !ACK
} ACK_Bit;

void RCC_Config(void);
void GPIO_Config(void);
void TIMER_Config(void);
void delay_us(uint32_t timedelay);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
status I2C_Write(uint8_t u8Data);
uint8_t I2C_Read(ACK_Bit _ACK);
status EEPROM_Write(uint16_t memAddr, uint8_t slaveAddr, uint8_t numByte, uint8_t *pData);
status EEPROM_Read(uint16_t memAddr, uint8_t slaveAddr, uint8_t numByte, uint8_t *pData);

uint8_t data[10] = {0x01, 0x03, 0x06, 0x07, 0x10, 0x1c, 0x35, 0x58, 0x8d, 0x17};
uint8_t rcv[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int main(void)
{
    RCC_Config();
    GPIO_Config();
    TIMER_Config();
    I2C_Init();
    
    if (EEPROM_Write(0x0045, 0x57, 10, data) == NOT_OK)
    {
        while(1) { }
    }
    
    delay_us(50000);
    
    while(1)
    {
        if (EEPROM_Read(0x0045, 0x57, 10, rcv) == NOT_OK)
        {
            while(1) { }
        }
    }
}

void RCC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Pin = I2C_SCL | I2C_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}

void TIMER_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStruct.TIM_Prescaler = 72 - 1;
    TIM_InitStruct.TIM_Period = 0xFFFF - 1;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint32_t timedelay)
{
    TIM_SetCounter(TIM2, 0);
    while(TIM_GetCounter(TIM2) < timedelay) { }
}

void I2C_Init(void)
{
    WRITE_SCL_1;
    delay_us(1);
    WRITE_SDA_1;
    delay_us(1);
}

void I2C_Start(void)
{
    WRITE_SCL_1;
    delay_us(3);
    WRITE_SDA_1;
    delay_us(3);
    
    WRITE_SDA_0;
    delay_us(3);
    WRITE_SCL_0;
    delay_us(3);
}

void I2C_Stop(void)
{
    WRITE_SDA_0;
    delay_us(3);
    
    WRITE_SCL_1;
    delay_us(3);
    WRITE_SDA_1;
    delay_us(3);
}

status I2C_Write(uint8_t u8Data)
{
    uint8_t i;
    status stRet;
	
    for (i = 0; i < 8; i++)
    {
        if (u8Data & 0x80) {    
            WRITE_SDA_1;
        } else {
            WRITE_SDA_0;
        }
        
        delay_us(3);
        WRITE_SCL_1;
        delay_us(5);
        WRITE_SCL_0;
        delay_us(2);
        u8Data <<= 1;
    }
    
    WRITE_SDA_1;
    delay_us(3);
    WRITE_SCL_1;
    delay_us(3);
    
    if (READ_SDA_VAL) {    
        stRet = NOT_OK;                
    } else {
        stRet = OK;                    
    }
    delay_us(2);
    WRITE_SCL_0;
    delay_us(5);
    
    return stRet;
}

uint8_t I2C_Read(ACK_Bit _ACK)
{
    uint8_t i;
    uint8_t u8Ret = 0x00;
    
    WRITE_SDA_1;
    delay_us(3);
    
    for (i = 0; i < 8 ; i++)
    {
        u8Ret <<= 1;
        WRITE_SCL_1;
        delay_us(3);
        
        if (READ_SDA_VAL) 
        {
            u8Ret |= 0x01;
        }
        
        delay_us(2);
        WRITE_SCL_0;
        delay_us(5);
    }
    
    if (_ACK) {    
        WRITE_SDA_0;
    } else {
        WRITE_SDA_1;
    }
    delay_us(3);
    
    WRITE_SCL_1;
    delay_us(5);
    WRITE_SCL_0;
    delay_us(5);

    return u8Ret;
}

status EEPROM_Write(uint16_t memAddr, uint8_t slaveAddr, uint8_t numByte, uint8_t *pData)
{
    uint8_t i;
    
    I2C_Start();
    
    if (I2C_Write((uint8_t)(slaveAddr << 1)) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    if (I2C_Write((uint8_t)(memAddr >> 8)) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    if (I2C_Write((uint8_t)(memAddr)) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    for (i = 0; i < numByte; i++)
    {
        if (I2C_Write(pData[i]) == NOT_OK)
        {
            I2C_Stop();
            return NOT_OK;
        }
    }
    
    I2C_Stop();
    delay_us(10000);
    
    return OK;
}

status EEPROM_Read(uint16_t memAddr, uint8_t slaveAddr, uint8_t numByte, uint8_t *pData)
{
    uint8_t i;
    
    I2C_Start();
        
    if (I2C_Write((uint8_t)(slaveAddr << 1)) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    if (I2C_Write((uint8_t)(memAddr >> 8)) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    if (I2C_Write((uint8_t)(memAddr)) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    I2C_Start();
    
    if (I2C_Write((uint8_t)(slaveAddr << 1) | 1) == NOT_OK)
    {
        I2C_Stop();
        return NOT_OK;
    }
    
    for (i = 0; i < numByte - 1; ++i)
    {
        pData[i] = I2C_Read(ACK);
    }
    pData[i] = I2C_Read(NOT_ACK);
    
    I2C_Stop();
    return OK;
}
