#ifndef _UART_H_
#define _UART_H_

#include "hwio.h"

void uart_init();
void uart_putc(uint8_t c);
void uart_puts(const char *s);
void uart_debug_puts(const char *s, uint32_t value);
void uart_dump(uint32_t addr, uint32_t count);
void uart_bytedump(uint32_t addr, uint32_t count);
uint8_t uart_getc();

#endif
