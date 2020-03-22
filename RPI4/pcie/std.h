#ifndef _STD_H_
#define _STD_H_

#include <stddef.h>
#include <stdint.h>

//cpp tukai tai.
#define MAX(x, y) ((x < y) ? (y) : (x))
#define MIN(x, y) ((x > y) ? (y) : (x))

int strlen(const char *a);
int strcmp(const char *a, const char *b);
void strcpy(char *dest, const char *src);
void memset(void *a, uint8_t c, const size_t size);
void memcpy(void *dest, const void *src, const size_t size);
void usleep(volatile int n);


#endif /* _STD_H_ */

