#include <stdint.h>
#include <string.h>
#include "common.h"
#include "hw.h"
#include "uart.h"
#include "mailbox.h"
#include "led.h"

#include "irq.h"

static irq_info irq_tbl[IRQ_INFO_MAX];

//todo contextilization
uint32_t g_count = 0;
uint32_t g_count_prepare = 0;
uint32_t g_frame_index = 0;


void irq_init() {
	memset(irq_tbl, 0, sizeof(irq_tbl));
}

void irq_set_info(irq_info *info, int index) {
	irq_tbl[index] = *info;
}

uint32_t irq_get_flip_count() {
	return g_count;
}

void irq_enable_flip_isr(uint32_t frame_index, uint32_t count) {
	g_count_prepare = count;
	g_frame_index = frame_index;
	*IRQ_GPU_ENABLE2 |= IRQ_GPU_FAKE_ISR;
}

void irq_fake_vsync(void) {
	*IRQ_GPU_ENABLE2 |= IRQ_GPU_FAKE_ISR;
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
	*SMI_CS = 0;
	*IRQ_GPU_ENABLE2 &= ~IRQ_GPU_FAKE_ISR;
}

void irq_handler() {
	for(int i = 0 ; i < IRQ_INFO_MAX; i++) {
		if(irq_tbl[i].callback)
			irq_tbl[i].callback(irq_tbl[i].data);
	}
	if(*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) {
		*SMI_CS = 0;
		g_count = g_count_prepare;
		mailbox_fb_flip(g_frame_index);
	}
	return;
}

void irq_handle_hang(const char *name) {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		uart_puts(name);
		SLEEP(0x1000000);
	}
}

void irq_handle_undef() {
	irq_handle_hang("UNDEF ");
}

void irq_handle_prefa() {
	irq_handle_hang("PREFETCH ABORT ");
}

void irq_handle_dataa() {
	irq_handle_hang("DATA ABORT ");
}
