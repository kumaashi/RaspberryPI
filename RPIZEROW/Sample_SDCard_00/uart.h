#include <stdint.h>

void uart_init();
void uart_putc(uint8_t c);
uint8_t uart_getc();
void uart_puts(const char *s);

//ex
void uart_debug_puts(const char *s, uint32_t value);
void uart_put_dword(uint32_t a);
void uart_putc_hex(uint8_t value);
void uart_dump(uint32_t addr, size_t size);
