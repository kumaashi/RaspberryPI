#include "common.h"
#include "uart.h"

#define GPIO_BASE       (SUBSYSTEM_BASE + 0x200000)
#define GPIO_SEL0       (GPIO_BASE + (4 * 0))
#define GPIO_SEL1       (GPIO_BASE + (4 * 1))
#define GPIO_SEL2       (GPIO_BASE + (4 * 2))
#define GPIO_SEL3       (GPIO_BASE + (4 * 3))
#define GPIO_SEL4       (GPIO_BASE + (4 * 4))
#define GPIO_SEL5       (GPIO_BASE + (4 * 5))
#define GPIO_SET0       (GPIO_BASE + 0x1C)
#define GPIO_SET1       (GPIO_BASE + 0x20)
#define GPIO_CLR0       (GPIO_BASE + 0x28)
#define GPIO_CLR1       (GPIO_BASE + 0x2C)
#define GPIO_LEV0       (GPIO_BASE + 0x34)
#define GPIO_LEV1       (GPIO_BASE + 0x38)
#define GPIO_GPPUD      (GPIO_BASE + 0x94)
#define GPIO_GPPUDCLK0  (GPIO_BASE + 0x98)

#define UART0_BASE      (SUBSYSTEM_BASE + 0x00201000)
#define UART0_DR        (UART0_BASE + 0x00)
#define UART0_RSRECR    (UART0_BASE + 0x04)
#define UART0_FR        (UART0_BASE + 0x18)
#define UART0_ILPR      (UART0_BASE + 0x20)
#define UART0_IBRD      (UART0_BASE + 0x24)
#define UART0_FBRD      (UART0_BASE + 0x28)
#define UART0_LCRH      (UART0_BASE + 0x2C)
#define UART0_CR        (UART0_BASE + 0x30)
#define UART0_IFLS      (UART0_BASE + 0x34)
#define UART0_IMSC      (UART0_BASE + 0x38)
#define UART0_RIS       (UART0_BASE + 0x3C)
#define UART0_MIS       (UART0_BASE + 0x40)
#define UART0_ICR       (UART0_BASE + 0x44)
#define UART0_DMACR     (UART0_BASE + 0x48)
#define UART0_ITCR      (UART0_BASE + 0x80)
#define UART0_ITIP      (UART0_BASE + 0x84)
#define UART0_ITOP      (UART0_BASE + 0x88)
#define UART0_TDR       (UART0_BASE + 0x8C)

void uart_init() {
	//gpio14 : alt0
	//gpio15 : alt0
	PUT32(GPIO_SEL1, GET32(GPIO_SEL1) | (4 << 12) | (4 << 15));
	SLEEP(150);
	PUT32(UART0_CR, 0x00000000);
	PUT32(GPIO_GPPUD, 0x00000001);
	SLEEP(150);
	PUT32(GPIO_GPPUDCLK0, GET32(GPIO_GPPUDCLK0) | (1 << 14) | (1 << 15));
	SLEEP(150);
	PUT32(GPIO_GPPUDCLK0, 0x00000000);
	//PUT32(UART0_ICR,  0x7FF);
	PUT32(UART0_IBRD, 1);
	PUT32(UART0_FBRD, 40);
	PUT32(UART0_LCRH, 0x00000070);
	//PUT32(UART0_IMSC,
	//		(1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	//		(1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
	PUT32(UART0_CR, 0x00000301);
}

void uart_putc(uint8_t c) {
	while (GET32(UART0_FR) & (1 << 5) )
		SLEEP(1);
	PUT32(UART0_DR, c);
}

uint8_t uart_getc() {
	while ( GET32(UART0_FR) & (1 << 4) )
		SLEEP(1);
	return GET32(UART0_DR);
}

void uart_puts(const char *s) {
	while(*s) {
		uart_putc(*s);
		if(*s == '\n')
			uart_putc('\r');
		s++;
	}
}

static uint8_t char_table[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

void uart_debug_puts(const char *s, uint32_t value) {
	uart_puts(s);
	uart_put_dword(value);
	uart_puts("\n");
}

void uart_put_dword(uint32_t value) {
	uart_putc( char_table[(value >> 28) & 0xF]);
	uart_putc( char_table[(value >> 24) & 0xF]);
	uart_putc( char_table[(value >> 20) & 0xF]);
	uart_putc( char_table[(value >> 16) & 0xF]);
	uart_putc( char_table[(value >> 12) & 0xF]);
	uart_putc( char_table[(value >>  8) & 0xF]);
	uart_putc( char_table[(value >>  4) & 0xF]);
	uart_putc( char_table[(value >>  0) & 0xF]);
}

void uart_putc_hex(uint8_t value) {
	uart_putc( char_table[(value >>  4) & 0xF]);
	uart_putc( char_table[(value >>  0) & 0xF]);
}


void uart_dump(uint32_t addr, size_t size) {
	uint8_t *p = (uint8_t *)addr;
	for(int i = 0 ; i < size; i++) {
		if( (i % 16) == 0) {
			uart_puts("\n");
			uart_put_dword((uint32_t)p);
			uart_puts(" : ");
		}
		uart_putc_hex(*p++);
		uart_puts(" ");
	}
	uart_puts("\n");
}

