#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

//#define SUBSYSTEM_BASE 0x20000000  //PI1
#define SUBSYSTEM_BASE 0x20000000  //PI1
#define WAIT_CNT               0x100000

extern void PUT32 ( uint32_t, uint32_t );
extern uint32_t GET32 ( uint32_t );
extern void ENABLE_IRQ( void );
extern void SLEEP (int32_t );
extern void FLUSH_CACHE ( );
extern void MemoryBarrier ( );
extern void InvalidateData ( );

