#ifndef _STD_H_
#define _STD_H_

#include <stddef.h>
#include <stdint.h>

int strlen(const char *a);
int strcmp(const char *a, const char *b);
void strcpy(char *dest, const char *src);
void memset(void *a, uint8_t c, const size_t size);
void memcpy(void *dest, const void *src, const size_t size);

#endif /* _STD_H_ */

