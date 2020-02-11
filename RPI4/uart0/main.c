#include "uart.h"
#include "hwio.h"
#include "gpio.h"
#include <stdbool.h>
#include <stddef.h>

int strlen(char *a)
{
	int count = 0;
	while (*a) {
		count++;
		a++;
	}
	return count;
}

int strcmp(char *a, char *b)
{
	int count = 0;
	while (*a) {
		if ( (*a - *b) != 0)
			return -1;
		a++;
		b++;
	}
	return 0;
}

void
memset(void *a, uint8_t c, size_t size)
{
	uint8_t *p = (uint8_t *)a;
	for (size_t i = 0; i < size; i++)
		p[i] = c;
}

#define CMD_MAX 256
volatile static uint8_t cmdstr[CMD_MAX];
volatile static uint32_t cmd_offset = 0;

void cmd_reset()
{
	cmd_offset = 0;

	uint64_t sp = _get_stack_pointer();
	uart_puts("SP:");
	uart_put_dword(sp);
	uart_puts("\n");

	uart_puts(">");
}

void cmd_setc(uint8_t c)
{
	cmdstr[cmd_offset++] = c;
}

uint64_t hex2value(char *s)
{
	uint64_t value = 0;
	while (*s) {
		char c = *s;
		uint8_t v = 0;
		if (c >= '0' && c <= '9') v = c - '0';
		if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
		if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
		value <<= 4;
		value |= v;
		s++;
	}
	return value;
}

void cmd_process()
{

	cmd_setc(0);
	if (strlen(cmdstr) == 0)
		return;
	uart_puts("CMD : ");
	uart_puts(cmdstr);
	uart_puts("\n");
	int cmd = cmdstr[0];
	if (cmd == 'r' || cmd == 'R') {
		uart_puts("command READ:\n");
		char *p = &cmdstr[1];
		uint64_t addr = hex2value(p);
		uint32_t value = IO_READ(addr);
		uart_put_dword(value);
	}
	uart_puts("\n");
}

int main()
{
	gpio_mode(14, 4);
	gpio_mode(15, 4);
	uart_init();
	uart_puts("\n\n");
	uart_puts("----------------------------------------------\n");
	uart_puts("START RPI4-001\n");
	uart_puts("----------------------------------------------\n");
	uint64_t count = 0;

	cmd_reset();
	while (1) {
		//gpio_set(42);
		//uart_wait(0x1000);
		//gpio_clr(42);
		//uart_wait(0x1000);
		//uart_putc('B');
		count++;
		if (count > 0x100000) {
			//heart beat
			count = 0;
		}
		uint8_t c = uart_getc();
		bool is_process = false;
		if (c) {
			if (c == '\n') {
				is_process = true;
			} else {
				cmd_setc(c);
			}
			uart_putc(c);
		}
		if (is_process) {
			cmd_process();
			cmd_reset();
		}
	}
	return 0;
}

