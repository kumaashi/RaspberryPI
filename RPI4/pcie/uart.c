#include "uart.h"
#include "hwio.h"

extern void _cpu_nop();

static const uint8_t char_table[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F'
};

void
uart_wait(volatile int32_t count)
{
	while (count > 0) {
		//_cpu_nop();
		count--;
	}
}

void
uart_init()
{
	IO_WRITE(UART0_CR,   0x00000000);
	IO_WRITE(GPIO_GPPUD, 0x00000000);
	uart_wait(0x100000);
	IO_WRITE(GPIO_GPPUDCLK0, (1 << 14) | (1 << 15));
	uart_wait(0x100000);
	IO_WRITE(GPIO_GPPUDCLK0, 0x00000000);
	IO_WRITE(UART0_ICR,  0x000007FF);
	IO_WRITE(UART0_IBRD, 1);
	IO_WRITE(UART0_FBRD, 40);
	IO_WRITE(UART0_LCRH, 0x00000060);
	IO_WRITE(UART0_CR,   0x00000301);
}

void
uart_putc(volatile uint8_t c)
{
	while ( (IO_READ(UART0_FR) & (1 << 5) ) );
	IO_WRITE(UART0_DR, c);
}

uint8_t
uart_getc()
{
	volatile uint8_t c = 0;
	if ( (IO_READ(UART0_FR) & (1 << 6)) ) {
		c = IO_READ(UART0_DR);
	}
	return c;
}

void
uart_puts(const char *s)
{
	while (*s) {
		uart_putc(*s);
		if (*s == '\n') {
			uart_putc('\r');
		}
		s++;
	}
}

void
uart_debug_puts(const char *s, uint32_t value)
{
	while (*s) {
		if (*s == '\n') {
			uart_putc(' ');
			uart_put_dword(value);
			uart_putc('\r');
		}
		uart_putc(*s);
		s++;
	}
}

void
uart_dump(uint32_t addr, uint32_t count)
{
	if (0x3 & addr) {
		uart_puts("not aligened\n");
		return;
	}
	uint32_t *p = (uint32_t *)(uintptr_t)addr;
	int index = 0;
	for (int i = 0 ; i < count / 4; i++) {
		if ( (i % 4) == 0) {
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

void
uart_char_putc(uint8_t value)
{
	uart_putc( char_table[(value >>  4) & 0xF]);
	uart_putc( char_table[(value >>  0) & 0xF]);
}

void
uart_put_dword(uint32_t value)
{
	uart_putc( char_table[(value >> 28) & 0xF]);
	uart_putc( char_table[(value >> 24) & 0xF]);
	uart_putc( char_table[(value >> 20) & 0xF]);
	uart_putc( char_table[(value >> 16) & 0xF]);
	uart_putc( char_table[(value >> 12) & 0xF]);
	uart_putc( char_table[(value >>  8) & 0xF]);
	uart_putc( char_table[(value >>  4) & 0xF]);
	uart_putc( char_table[(value >>  0) & 0xF]);
}

void
uart_bytedump(uint32_t addr, uint32_t count)
{
	uint8_t *p = (uint8_t *)addr;
	int index = 0;
	for (int i = 0 ; i < count; i++) {
		if ( (i % 16) == 0) {
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


void
uart_name_dword(const char* name, uint32_t value)
{
	uart_puts(name);
	uart_put_dword(value);
	uart_puts("\n");
}
