#include "common.h"
#include "hw.h"
#include "uart.h"
#include "led.h"


//fake_vsync_isr=1
void fake_vsync(void) {
	*IRQ_GPU_ENABLE2 = IRQ_GPU_FAKE_ISR;
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
	*SMI_CS = 0;
}


void intr_handler() {
	uart_puts("call intr_handler\n");
}

void handle_hang() {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		SLEEP(0x1000000);
	}
}
