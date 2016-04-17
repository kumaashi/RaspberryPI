#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdint.h>

#include "hwio.h"
#include "led.h"
#include "mailbox.h"
#include "console.h"
#include "v3d.h"

#define FONT_SIZE          16
#define FONT_RECT_SIZE     (FONT_SIZE * FONT_SIZE)
#define SIN_TABLE_MAX      256
#define SIN_TABLE_MAX_D4   64
#define SIN_TABLE_FIXED    4
#define FIXED4(x)          ((x) << 4)
#define TSIN(x)            (sintable[(x) % SIN_TABLE_MAX])
#define TCOS(x)            (sintable[(x) % (SIN_TABLE_MAX + SIN_TABLE_MAX_D4)])

extern int16_t             sintable[SIN_TABLE_MAX];

uint32_t memtest();
void     memset_t(void *dest, uint8_t c, uint32_t size);
uint32_t random();
void     usleep(int32_t count);

 uint64_t get_systime(void);
void      msleep(uint32_t ms);
void      set_char_color(uint32_t col);
void      reset_char_color();
void      uart_put_dword(uint32_t value);




#endif
