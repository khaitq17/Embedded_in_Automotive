#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f10x.h"                 

void TIMER_Config(void);
void delay_ms(uint16_t time);
void delay_us(uint16_t time);

#endif
