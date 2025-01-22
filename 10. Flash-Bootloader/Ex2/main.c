#include "stm32f10x.h"          

#define ADDR_STR_BLINK	0x08008000

void Boot(void);

int main(void)
{
	Boot();
	
	while(1)
	{
		
	}
}

void Boot(void)
{
	RCC_DeInit();
	
	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
	
	__set_MSP(*(__IO uint32_t*) (ADDR_STR_BLINK));
	
	SCB->VTOR = ADDR_STR_BLINK;
	
	uint32_t jumpAddress = *(__IO uint32_t*) (ADDR_STR_BLINK + 4);
	
	void (*reset_handler)(void) = (void (*) (void)) jumpAddress;
	
	reset_handler();
}



