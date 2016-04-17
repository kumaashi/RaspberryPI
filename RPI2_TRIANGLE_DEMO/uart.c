#include "uart.h"

static uart_wait(int32_t count) {
	while(count > 0) {
		count--;
	}
}

void uart_init() {
	IO_WRITE(UART0_CR,   0x00000000);
	IO_WRITE(GPIO_GPPUD, 0x00000000);
	uart_wait(1000);
	IO_WRITE(GPIO_GPPUDCLK0, (1 << 14) | (1 << 15));
	uart_wait(1000);
	IO_WRITE(GPIO_GPPUDCLK0, 0x00000000);
	uart_wait(1000);
	IO_WRITE(UART0_ICR, 0x7FF);
    IO_WRITE(UART0_IBRD, 1);
    IO_WRITE(UART0_FBRD, 40);
	IO_WRITE(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    IO_WRITE(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
						(1 << 6) | (1 << 7) | (1 << 8) |
						(1 << 9) | (1 << 10));
	IO_WRITE(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(uint8_t c) {
	while (1) {
		if (!(IO_READ(UART0_FR) & (1 << 5))) {
			break;
		}
	}
	IO_WRITE(UART0_DR, c);
}

uint8_t uart_getc() {
	uint8_t c = 0;
	if(IO_READ(UART0_FR) & (1 << 4)) {
		c = IO_READ(UART0_DR);
	}
	return c;
}

void uart_puts(const char *s) {
	while(*s) {
		uart_putc(*s);
		if(*s == '\n') {
			uart_putc('\r');
		}
		s++;
	}
}

void uart_debug_puts(const char *s, uint32_t value) {
	while(*s) {
		if(*s == '\n') {
			uart_putc(' ');
			uart_put_dword(value);
			uart_putc('\r');
		}
		uart_putc(*s);
		s++;
	}
}

void uart_dump(uint32_t addr, uint32_t count) {
	if(0x3 & addr) {
		return;
	}
	uint32_t *p = (uint32_t *)addr;
	int index = 0;
	for(int i = 0 ; i < count/4; i++) {
		if( (i % 4) == 0) {
			uart_puts("\n");
			uart_put_dword(addr + index * 4);
			uart_puts(" : ");
		}
		uart_put_dword(p[index++]);
		uart_putc(' ');
	}
	uart_puts("\n");
	uart_puts("\n");
	
}

void uart_bytedump(uint32_t addr, uint32_t count) {
	uint8_t *p = (uint8_t *)addr;
	int index = 0;
	for(int i = 0 ; i < count; i++) {
		if( (i % 16) == 0) {
			uart_puts("\n");
			uart_put_dword(addr + index);
			uart_puts(" : ");
		}
		uart_char_putc(p[index++]);
		uart_putc(' ');
	}
	uart_puts("\n");
	uart_puts("\n");
	
}




