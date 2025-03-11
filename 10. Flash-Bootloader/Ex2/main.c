#include "stm32f10x.h"          

#define ADDR_STR_BLINK	0x08008000

void Boot(void);

int main(void)
{
	Boot();
	
	while(1) {}
}

void Boot(void)
{
	// Thiết lập lại hệ thống clock
	RCC_DeInit();

	// Vô hiệu hóa các lỗi ngắt để tránh lỗi trong quá trình chuyển giao
	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

	// Cập nhật Stack Pointer từ bảng vector ngắt của chương trình chính
	__set_MSP(*(__IO uint32_t*) (ADDR_STR_BLINK));

	// Cập nhật thanh ghi SCB->VTOR để trỏ đến bảng vector ngắt của chương trình chính
	SCB->VTOR = ADDR_STR_BLINK;

	// Lấy địa chỉ Reset Handler của chương trình chính	
	uint32_t jumpAddress = *(__IO uint32_t*) (ADDR_STR_BLINK + 4);
	
	// Tạo con trỏ hàm đến Reset Handler
	void (*reset_handler)(void) = (void (*) (void)) jumpAddress;

	// Nhảy vào Reset Handler của chương trình chính
	reset_handler();
}