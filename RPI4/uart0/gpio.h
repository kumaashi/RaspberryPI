#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

void gpio_set(uint8_t pin);
void gpio_clr(uint8_t pin);
void gpio_mode(uint32_t port, uint8_t func);

#endif
