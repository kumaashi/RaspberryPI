#ifndef _UART_H_
#define _UART_H_

#include "hwio.h"

void uart_init();
void uart_putc(uint8_t c);
void uart_puts(const char *s);
void uart_debug_puts(const char *s, uint32_t value);
void uart_dump(uint32_t addr, uint32_t count);
void uart_bytedump(uint32_t addr, uint32_t count);
void uart_put_dword(uint32_t value);
uint8_t uart_getc();
void uart_char_putc(uint8_t value);

#endif
