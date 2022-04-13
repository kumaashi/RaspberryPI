#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdint.h>

//#define RGB32_BITS
#define RGB16_BITS

#ifdef RGB32_BITS
#define RGB_SIZE (sizeof(uint32_t))
#define RGB_BITS 32
#define surface_t uint32_t
#endif

#ifdef RGB16_BITS
#define RGB_SIZE (sizeof(uint16_t))
#define RGB_BITS 16
#define surface_t uint16_t
#endif

#endif


