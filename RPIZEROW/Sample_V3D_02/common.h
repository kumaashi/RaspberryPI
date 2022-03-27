#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

//#define SUBSYSTEM_BASE 0x20000000  //PI1
#define SUBSYSTEM_BASE 0x20000000  //PI1
#define VCADDR_BASE    0x40000000
#define PERI_BASE      0x7E000000
#define WAIT_CNT       0x00100000

extern void PUT32 (uint32_t, uint32_t);
extern uint32_t GET32 (uint32_t);
extern void ENABLE_IRQ(void );
extern void SLEEP (int32_t );
extern void MemoryBarrier( );
extern void InvalidateData( );
extern uint32_t random();
void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);
