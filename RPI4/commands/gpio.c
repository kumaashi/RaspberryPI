#include "hwio.h"
#include "gpio.h"

void
gpio_set(uint8_t pin)
{
	uint32_t reg = GPIO_SET + ((pin / 32) * 4);
	uint32_t value = IO_READ(reg);
	uint32_t bit = 1 << (pin % 32);
	IO_WRITE(reg, value | bit);
}

void
gpio_clr(uint8_t pin)
{
	uint32_t reg = GPIO_CLR + ((pin / 32) * 4);
	uint32_t value = IO_READ(reg);
	uint32_t bit = 1 << (pin % 32);
	IO_WRITE(reg, value | bit);
}

void
gpio_mode(uint32_t port, uint8_t func)
{
	uint32_t bank = port / 10;
	uint32_t offset = port % 10;
	uint32_t reg = GPIO_SEL0 + (bank * 4);
	uint32_t value = IO_READ(reg);
	value &= ~((uint32_t)(0x07 << (offset * 3)));
	value |= (uint32_t)(func << (offset * 3));
	IO_WRITE(reg, value);
}
