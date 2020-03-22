#include "std.h"

int
strlen(const char *a)
{
	int count = 0;
	while (*a) {
		count++;
		a++;
	}
	return count;
}

int
strcmp(const char *a, const char *b)
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
strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = 0;
}

void
memset(void *a, uint8_t c, const size_t size)
{
	uint8_t *p = (uint8_t *)a;
	for (size_t i = 0; i < size; i++)
		p[i] = c;
}

void
memcpy(void *dest, const void *src, const size_t size)
{
	uint8_t *pd = (uint8_t *)dest;
	uint8_t *ps = (uint8_t *)src;
	for (size_t i = 0; i < size; i++)
		pd[i] = ps[i];
}


void
usleep(volatile int n)
{
	while (n > 0)
		n--;
}
