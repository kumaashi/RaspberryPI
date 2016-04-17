#include "uart.h"

#define COMMAND_MAX 256
static uint8_t command_index = 0;
static uint8_t command[COMMAND_MAX];

void func_command(const uint8_t *str) {
	if(*str == 'd' || *str == 'D') {
		str++;
		uint32_t addr = 0;
		while(*str) {
			uint8_t c = *str++;
			addr <<= 4;
			if(c >= '0' && c <= '9') {
				addr += c - '0';
			}
			if(c >= 'A' && c <= 'F') {
				addr += (c - 'A') + 10;
			}
			if(c >= 'a' && c <= 'f') {
				addr += (c - 'a') + 10;
			}
		}
		uart_bytedump(addr, 0x1000);
	}
	if(*str == 'w' || *str == 'W') {
		str++;
		uint32_t addr = 0;
		while(*str) {
			uint8_t c = *str++;
			addr <<= 4;
			if(c >= '0' && c <= '9') {
				addr += c - '0';
			}
			if(c >= 'A' && c <= 'F') {
				addr += (c - 'A') + 10;
			}
			if(c >= 'a' && c <= 'f') {
				addr += (c - 'a') + 10;
			}
		}
		uart_dump(addr, 0x1000);
	}
}


void console_reset() {
	memset_t(command, 0, COMMAND_MAX);
	command_index = 0;
	
}

void console_update(void (*func)(uint8_t *cmd)) {
	uint8_t c = uart_getc();
	if(c) {
		if(c == '\r') {
			uart_putc('\r');
			uart_putc('\n');
			c = 0;
		} else {
			uart_putc(c);
		}
		command[command_index] = c;
		command_index = (command_index + 1) % COMMAND_MAX;
		if(c == 0 && func) {
			func(command);
			console_reset();
		}
	}
}

void core1_update() {
	uart_init();
	uart_puts("CORE1 READY\n");
	usleep(0x100000);
	while(1) {
		console_update(func_command);
	}
}

