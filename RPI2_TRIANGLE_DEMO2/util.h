#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

#include "hwio.h"
#include "led.h"
#include "mailbox.h"
#include "console.h"
#include "v3d.h"


#define FIXED_POINT             (4)
#define FIXED_UNIT              (1 << FIXED_POINT)
#define FIXED(x)                ((x) << FIXED_POINT)
#define UNFIXED(x)              ((x) >> FIXED_POINT)
#define ADD_FIXED(x, a, b)      ((x) = (a) + (b))
#define SUB_FIXED(x, a, b)      ((x) = (a) - (b))
#define MUL_FIXED(x, a, b)      UNFIXED((x) = (a) * (b))
#define DIV_FIXED(x, a, b)      ((x) = FIXED(a) / (b))


#define FSIN_TABLE_MAX          8192
#define TO_DEG(x)               (( (x) * FSIN_TABLE_MAX) / 360)
#define FSIN_TABLE_MAX_D4       (FSIN_TABLE_MAX / 4)
#define PI                      3.14159265358979
#define FSIN(x)                 (fsintable[( (int32_t)(x)                     ) % FSIN_TABLE_MAX])
#define FCOS(x)                 (fsintable[( (int32_t)(x) + FSIN_TABLE_MAX_D4 ) % FSIN_TABLE_MAX])
#define FONT_SIZE               16
#define FONT_RECT_SIZE          (FONT_SIZE * FONT_SIZE)

extern float fsintable[FSIN_TABLE_MAX];
extern uint8_t char_table[16];

void      memset_t(void *dest, uint8_t c, uint32_t size);
void      init_random();
uint32_t  random();
void      usleep(int32_t count);
 uint64_t get_systime(void);
void      msleep(uint32_t ms);
void      set_char_color(uint32_t col);
void      reset_char_color();
void      draw_dword(uint32_t value, int x, int y);
void      init_render_chunk_buffer();
const     uint8_t *get_fontdata16x16();
void      core1_update();
uint32_t  get_systime_ms(void);
	
#endif
