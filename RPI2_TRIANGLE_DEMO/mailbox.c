#include "mailbox.h"
#include "led.h"

uint32_t ArmToVc(void *p)    { return ((uint32_t)p) +   VCADDR_BASE; }
uint32_t VcToArm(uint32_t p) { return (uint32_t)(p) & ~(VCADDR_BASE) ; }

int is_fb_init = 0;
static int32_t stick = 0;
static mailbox_fb mailbox_fb_data __attribute__((aligned(256))) ;
volatile static uint32_t p[256]  __attribute__((aligned(256))) ;
int32_t mailbox_fb_test(int shift) {
	mailbox_fb_init(1280, 720);
	if(is_fb_init) {
		volatile mailbox_fb *fb = (volatile mailbox_fb *)(1 << 22);
		uint32_t *buf = (uint32_t *)fb->pointer;
		int x, y;
		stick++;
		for(y = 0; y < fb->height; y++) {
			for(x = 0; x < fb->width; x++) {
				uint32_t offset = (x + y * fb->width);
				buf[offset] = ((x + stick)^ (y + stick)) << shift;
			}
		}
	}
	return 0;
}

mailbox_fb *mailbox_fb_getaddr() {
	return &mailbox_fb_data;
}

uint32_t mailbox_read(uint32_t mask) {
	uint32_t result = 0;
	volatile int count = 0;
	on_status_led();
	while(1) {
		MemoryBarrier();
		while(IO_READ(MAILBOX_STATUS) & MAILBOX_EMPTY) {
			usleep(0x10000);
			count++;
			if(count > 0x5000) {
				off_status_led();
				return 0;
			}
			continue;
		}
		MemoryBarrier();
		result = IO_READ(MAILBOX_READ);
		if( (result & 0xF) == mask) {
			break;
		}
	}
	off_status_led();
	return result & 0xFFFFFFF0;
}

uint32_t mailbox_write(uint32_t mask, uint32_t v) {
	MemoryBarrier();
	on_status_led();
	while(IO_READ(MAILBOX_STATUS) & MAILBOX_FULL) {
		usleep(0x10000);
		continue;
	}
	off_status_led();
	MemoryBarrier();
	IO_WRITE(MAILBOX_WRITE, mask | (v & 0xFFFFFFF0));
	return 0;
}

//https://github.com/phire/hackdriver/blob/master/test.cpp
uint32_t mailbox_qpu_enable() {
	int i  = 0;
	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request
	p[i++] = SET_CLOCK_RATE; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 5; // (the tag id)
	p[i++] = 250 * 1000 * 1000;
	p[i++] = ENABLE_QPU;    // (the tag id)
	p[i++] = 4;          // (size of the buffer)
	p[i++] = 4;          // (size of the data)
	p[i++] = 1;
	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size

	do {
		mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		mailbox_read(MAILBOX_ARM_TO_VC);
		if(p[1] == 0x80000000) {
			break;
		}
		uart_debug_puts("mailbox_qpu_enable FAILED p[1]=\n", p[1]);
	} while(1);
	uart_debug_puts("mailbox_qpu_enable OK \n", p[1]);
	return p[1];

}

uint32_t mailbox_allocate_memory(uint32_t size, uint32_t align, uint32_t flags)
{
	int i  = 0;
	p[i++] = 0;              // size
	p[i++] = 0x00000000;     // process request
	p[i++] = ALLOCATE_MEMORY;     // (the tag id)
	p[i++] = 12;             // (size of the buffer)
	p[i++] = 12;             // (size of the data)
	p[i++] = size;           // (num bytes? or pages?)
	p[i++] = align;          // (alignment)
	p[i++] = flags;          // (MEM_FLAG_L1_NONALLOCATING)

	p[i++] = 0x00000000;     // end tag
	p[0]   = i * sizeof(uint32_t); // actual size

	do {
		mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		mailbox_read(MAILBOX_ARM_TO_VC);
		if(p[1] == 0x80000000) {
			break;
		}
		uart_debug_puts("mailbox_unlock_memory FAILED p[1]=\n", p[1]);
	} while(1);
	uart_debug_puts("mailbox_allocate_memory handle=\n", p[5]);
	return p[5];
}

uint32_t mailbox_free_memory(uint32_t handle)
{
	int i  = 0;
	p[i++] = 0;
	p[i++] = 0x00000000;
	p[i++] = RELEASE_MEMORY;
	p[i++] = 4;
	p[i++] = 4;
	p[i++] = handle;
	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size

	do {
		mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		mailbox_read(MAILBOX_ARM_TO_VC);
		if(p[1] == 0x80000000) {
			break;
		}
		uart_debug_puts("mailbox_unlock_memory FAILED p[1]=\n", p[1]);
	} while(1);
	return p[5];
}

uint32_t mailbox_lock_memory(uint32_t handle)
{
	int i  = 0;
	p[i++] = 0;
	p[i++] = 0x00000000;
	p[i++] = LOCK_MEMORY;
	p[i++] = 4;
	p[i++] = 4;
	p[i++] = handle;

	p[i++] = 0x00000000;
	p[0]   = i * sizeof(uint32_t);

	do {
		mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		mailbox_read(MAILBOX_ARM_TO_VC);
		if(p[1] == 0x80000000) {
			break;
		}
		uart_debug_puts("mailbox_lock_memory FAILED p[1]=\n", p[1]);
	} while(1);
	//uart_debug_puts("mailbox_lock_memory addr=\n", p[5]);
	return p[5];
}

uint32_t mailbox_unlock_memory(uint32_t handle)
{
	int i  = 0;
	p[i++] = 0;
	p[i++] = 0x00000000;
	p[i++] = UNLOCK_MEMORY;
	p[i++] = 4;
	p[i++] = 4;
	p[i++] = handle;

	p[i++] = 0x00000000;
	p[0]   = i * sizeof(uint32_t);

	do {
		mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		mailbox_read(MAILBOX_ARM_TO_VC);
		if(p[1] == 0x80000000) {
			break;
		}
		uart_debug_puts("mailbox_unlock_memory FAILED p[1]=\n", p[1]);
	} while(1);
	//uart_debug_puts("mailbox_unlock_memory=\n", p[5]);
	return p[5];
}

int32_t mailbox_fb_init(uint32_t w, uint32_t h) {
	volatile mailbox_fb *fb = mailbox_fb_getaddr();
	fb->width   = w;
	fb->height  = h;
	fb->vwidth  = fb->width;
	fb->vheight = fb->height;
	fb->pitch   = 0;
	fb->depth   = 32;
	fb->x       = 0;
	fb->y       = 0;
	fb->pointer = 0;
	fb->size    = 0;
	
	fb->pointer_vc = 0;

#if 0
	int i  = 0;
	p[i++] = 0;
	p[i++] = 0x00000000; 
	
	//set disp
	p[i++] = SET_PHYSICAL_DISPLAY; 
	p[i++] = 8;
	p[i++] = 8;
	p[i++] = fb->width;
	p[i++] = fb->height;
	
	//vbuffer
	p[i++] = SET_VIRTUAL_BUFFER;
	p[i++] = 8;
	p[i++] = 8;
	p[i++] = fb->width;
	p[i++] = fb->height;
	
	//set depth
	p[i++] = SET_DEPTH;
	p[i++] = 0x00000004;
	p[i++] = 0x00000004;
	p[i++] = fb->depth;
	
	//set voffset
	p[i++] = SET_VIRTUAL_OFFSET;
	p[i++] = 0x00000008;
	p[i++] = 0x00000008;
	p[i++] = 0x00000000;
	p[i++] = 0x00000000;
	
	//allocate buffer
	p[i++] = ALLOCATE_BUFFER;
	p[i++] = 0x00000008;
	p[i++] = 0x00000008;
	
	uint32_t framebuffer_index = i;
	p[i++] = 0x00000000;
	uint32_t buffersize_index = i;
	p[i++] = 0x00000000;
	
	//end
	p[i++] = 0x00000000;
	p[0] = i * sizeof(uint32_t);  // actual size


	int count = 0;
	do {
		mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		//uart_debug_puts("mailbox_fb_init=\n", p[1]);
		mailbox_read(MAILBOX_ARM_TO_VC);
		if(p[framebuffer_index]) {
			break;
		}
		if(count & 1) {
			on_status_led();
		} else {
			off_status_led();
		}
		count++;
		usleep(100);
	} while(1);
	fb->pointer = VcToArm(p[framebuffer_index]);
	is_fb_init = 1;
	return p[buffersize_index];
#endif

#if 1
	int count = 0;
	do {
		mailbox_write(MAILBOX_FRAMEBUFFER, ArmToVc((void *)fb));
		mailbox_read(MAILBOX_FRAMEBUFFER);
		if(fb->pointer) {
			break;
		}
		if(count & 1) {
			on_status_led();
		} else {
			off_status_led();
		}
		count++;
		usleep(0x1000);
		uart_debug_puts("MAILBOX_FRAMEBUFFER count=\n", count);
	} while(1);
	uint32_t vc_pointer = fb->pointer;
	fb->pointer = VcToArm((void *)fb->pointer);
	fb->pointer_vc = vc_pointer;
	is_fb_init = 1;
	return 0;
#endif 
}

uint8_t *mailbox_fb_ptr() {
	if(is_fb_init) {
		volatile mailbox_fb *fb = mailbox_fb_getaddr();
		return (uint8_t *)fb->pointer;
	}
	return 0;
}

void mailbox_fb_set_pixel(uint32_t val, int x, int y) {
	if(is_fb_init) {
		volatile mailbox_fb *fb = mailbox_fb_getaddr();
		if(x < 0 || y < 0) return ;
		if(x >= fb->width || y >= fb->height) return ;
		uint32_t offset = (x + y * fb->width);
		uint32_t *buf = (uint32_t *)fb->pointer;
		buf[offset] = val;
	}
}
