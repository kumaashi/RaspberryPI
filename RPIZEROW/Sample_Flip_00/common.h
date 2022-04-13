#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "color.h"

//#define SUBSYSTEM_BASE 0x20000000  //PI1
#define SUBSYSTEM_BASE 0x20000000  //PI1
#define VCADDR_BASE    0x40000000
#define PERI_BASE      0x7E000000
#define WAIT_CNT       0x00100000

extern void PUT32 (uint32_t, uint32_t);
extern uint32_t GET32 (uint32_t);
extern void ENABLE_IRQ(void );
extern void DISABLE_IRQ(void );
extern void SLEEP (int32_t );
extern int32_t FTOI (float );
extern void MemoryBarrier( );
extern void InvalidateData( );
extern uint32_t common_random();
void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);
uint32_t get_systime_ms(void);
uint32_t get_systime32(void);
void msleep(uint32_t ms);
void msleep32(uint32_t ms);

typedef struct {
	int32_t dt;
	int32_t frames;
} fps_info;
fps_info show_fps_raw();
float show_fps();


inline uint32_t ArmToVc(void *p) {
	return ((uint32_t)p) + VCADDR_BASE;
}

inline uint32_t VcToArm(uint32_t p) {
	return (uint32_t)(p) & ~(VCADDR_BASE) ;
}
