#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <stdint.h>
#include "hwio.h"

//https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
#define MAILBOX_MEM_FLAG_DISCARDABLE                                   (1 << 0)
#define MAILBOX_MEM_FLAG_NORMAL                                        (0 << 2)
#define MAILBOX_MEM_FLAG_DIRECT                                        (1 << 2)
#define MAILBOX_MEM_FLAG_COHERENT                                      (2 << 2)
#define MAILBOX_MEM_FLAG_L1_NONALLOCATING                              ((MAILBOX_MEM_FLAG_DIRECT | MAILBOX_MEM_FLAG_COHERENT))
#define MAILBOX_MEM_FLAG_ZERO                                          (1 << 4)
#define MAILBOX_MEM_FLAG_NO_INIT                                       (1 << 5)
#define MAILBOX_MEM_FLAG_HINT_PERMALOCK                                (1 << 6)


typedef struct mailbox_fb_t {
	uint32_t width;
	uint32_t height;
	uint32_t vwidth;
	uint32_t vheight;
	uint32_t pitch;
	uint32_t depth;
	uint32_t x;
	uint32_t y;
	uint32_t pointer;
	uint32_t size;
} mailbox_fb;
	
int32_t        mailbox_fb_init(uint32_t w, uint32_t h);
mailbox_fb    *mailbox_fb_getaddr();
uint8_t       *mailbox_fb_ptr();
uint32_t       mailbox_qpu_enable();
uint32_t       mailbox_read(uint32_t mask);
uint32_t       mailbox_write(uint32_t mask, uint32_t v);
uint32_t       mailbox_allocate_memory(uint32_t size, uint32_t align, uint32_t flags);
uint32_t       mailbox_free_memory(uint32_t handle);
uint32_t       mailbox_lock_memory(uint32_t handle);
uint32_t       mailbox_unlock_memory(uint32_t handle);
void           mailbox_fb_set_pixel(uint32_t val, int x, int y);
uint32_t       ArmToVc(void *p);
uint32_t       VcToArm(uint32_t p);

#endif

