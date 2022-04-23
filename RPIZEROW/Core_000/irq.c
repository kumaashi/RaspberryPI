#include <stdint.h>
#include <string.h>
#include "common.h"
#include "hw.h"
#include "uart.h"
#include "mailbox.h"
#include "led.h"

#include "irq.h"

static irq_info irq_tbl[IRQ_INFO_MAX];

void irq_init() {
	memset(irq_tbl, 0, sizeof(irq_tbl));
}

void irq_set_info(irq_info *info, int index) {
	irq_tbl[index] = *info;
}

void irq_handler() {
	for(int i = 0 ; i < IRQ_INFO_MAX; i++) {
		if(irq_tbl[i].callback)
			irq_tbl[i].callback(irq_tbl[i].data);
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
