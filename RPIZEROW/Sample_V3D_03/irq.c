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

void handle_hang(const char *name) {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		uart_puts(name);
		SLEEP(0x1000000);
	}
}

void handle_undef() {
	handle_hang("UNDEF ");
}

void handle_prefa() {
	handle_hang("PREFETCH ABORT ");
}

void handle_dataa() {
	handle_hang("DATA ABORT ");
}
