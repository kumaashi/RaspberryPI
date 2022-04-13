#include "common.h"
#include "hw.h"
#include "uart.h"
#include "mailbox.h"
#include "led.h"
#include <stdint.h>
#include <string.h>

//todo contextilization
uint32_t g_count = 0;
uint32_t g_count_prepare = 0;
uint32_t g_frame_index = 0;
uint32_t get_irq_flip_count() {
	return g_count;
}
void enable_irq_flip_isr(uint32_t frame_index, uint32_t count) {
	g_count_prepare = count;
	g_frame_index = frame_index;
	*IRQ_GPU_ENABLE2 |= IRQ_GPU_FAKE_ISR;
}

void fake_vsync(void) {
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
	*SMI_CS = 0;
	*IRQ_GPU_ENABLE2 &= ~IRQ_GPU_FAKE_ISR;
}

void intr_handler() {
	uart_puts("call intr_handler\n");
	if(*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) {
		*SMI_CS = 0;
		g_count = g_count_prepare;
		mailbox_fb_flip(g_frame_index);
	}
	return;
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
