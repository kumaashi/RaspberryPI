#include "mailbox.h"
#include "util.h"
#include "led.h"

#define GET_FIRMWARE_REVISION 0x00000001 // VideoCore: Get Firmware Revision (Response: Firmware Revision)
#define GET_BOARD_MODEL       0x00010001 // Hardware: Get Board Model (Response: Board Model)
#define GET_BOARD_REVISION    0x00010002 // Hardware: Get Board Revision (Response: Board Revision)
#define GET_BOARD_MAC_ADDRESS 0x00010003 // Hardware: Get Board MAC Address (Response: MAC Address In Network Byte Order)
#define GET_BOARD_SERIAL      0x00010004 // Hardware: Get Board Serial (Response: Board Serial)
#define GET_ARM_MEMORY        0x00010005 // Hardware: Get ARM Memory (Response: Base Address In Bytes, Size In Bytes)
#define GET_VC_MEMORY         0x00010006 // Hardware: Get VC Memory (Response: Base Address In Bytes, Size In Bytes)
#define GET_CLOCKS            0x00010007 // Hardware: Get Clocks (Response: Parent Clock ID (0 For A Root Clock), Clock ID)
#define GET_POWER_STATE       0x00020001 // Power: Get Power State (Response: Device ID, State)
#define GET_TIMING            0x00020002 // Power: Get Timing (Response: Device ID, Enable Wait Time In Microseconds)
#define SET_POWER_STATE       0x00028001 // Power: Set Power State (Response: Device ID, State)
#define GET_CLOCK_STATE       0x00030001 // Clocks: Get Clock State (Response: Clock ID, State)
#define GET_CLOCK_RATE        0x00030002 // Clocks: Get Clock Rate (Response: Clock ID, Rate In Hz)
#define GET_VOLTAGE           0x00030003 // Voltage: Get Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_MAX_CLOCK_RATE    0x00030004 // Clocks: Get Max Clock Rate (Response: Clock ID, Rate In Hz)
#define GET_MAX_VOLTAGE       0x00030005 // Voltage: Get Max Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_TEMPERATURE       0x00030006 // Voltage: Get Temperature (Response: Temperature ID, Value In Degrees C)
#define GET_MIN_CLOCK_RATE    0x00030007 // Clocks: Get Min Clock Rate (Response: Clock ID, Rate In Hz)
#define GET_MIN_VOLTAGE       0x00030008 // Voltage: Get Min Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define GET_TURBO             0x00030009 // Clocks: Get Turbo (Response: ID, Level)
#define GET_MAX_TEMPERATURE   0x0003000A // Voltage: Get Max Temperature (Response: Temperature ID, Value In Degrees C)
#define ALLOCATE_MEMORY       0x0003000C // Memory: Allocates Contiguous Memory On The GPU (Response: Handle)
#define LOCK_MEMORY           0x0003000D // Memory: Lock Buffer In Place, And Return A Bus Address (Response: Bus Address)
#define UNLOCK_MEMORY         0x0003000E // Memory: Unlock Buffer (Response: Status)
#define RELEASE_MEMORY        0x0003000F // Memory: Free The Memory Buffer (Response: Status)
#define EXECUTE_CODE          0x00030010 // Memory: Calls The Function At Given (Bus) Address And With Arguments Given
#define EXECUTE_QPU           0x00030011 // QPU: Calls The QPU Function At Given (Bus) Address And With Arguments Given (Response: Number Of QPUs, Control, No Flush, Timeout In ms)
#define ENABLE_QPU            0x00030012 // QPU: Enables The QPU (Response: Enable State)
#define GET_EDID_BLOCK        0x00030020 // HDMI: Read Specificed EDID Block From Attached HDMI/DVI Device (Response: Block Number, Status, EDID Block (128 Bytes))
#define SET_CLOCK_STATE       0x00038001 // Clocks: Set Clock State (Response: Clock ID, State)
#define SET_CLOCK_RATE        0x00038002 // Clocks: Set Clock Rate (Response: Clock ID, Rate In Hz)
#define SET_VOLTAGE           0x00038003 // Voltage: Set Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
#define SET_TURBO             0x00038009 // Clocks: Set Turbo (Response: ID, Level)
#define ALLOCATE_BUFFER       0x00040001 // Frame Buffer: Allocate Buffer (Response: Frame Buffer Base Address In Bytes, Frame Buffer Size In Bytes)
#define BLANK_SCREEN          0x00040002 // Frame Buffer: Blank Screen (Response: State)
#define GET_PHYSICAL_DISPLAY  0x00040003 // Frame Buffer: Get Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
#define GET_VIRTUAL_BUFFER    0x00040004 // Frame Buffer: Get Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
#define GET_DEPTH             0x00040005 // Frame Buffer: Get Depth (Response: Bits Per Pixel)
#define GET_PIXEL_ORDER       0x00040006 // Frame Buffer: Get Pixel Order (Response: State)
#define GET_ALPHA_MODE        0x00040007 // Frame Buffer: Get Alpha Mode (Response: State)
#define GET_PITCH             0x00040008 // Frame Buffer: Get Pitch (Response: Bytes Per Line)
#define GET_VIRTUAL_OFFSET    0x00040009 // Frame Buffer: Get Virtual Offset (Response: X In Pixels, Y In Pixels)
#define GET_OVERSCAN          0x0004000A // Frame Buffer: Get Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
#define GET_PALETTE           0x0004000B // Frame Buffer: Get Palette (Response: RGBA Palette Values (Index 0 To 255))
#define TEST_PHYSICAL_DISPLAY 0x00044003 // Frame Buffer: Test Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
#define TEST_VIRTUAL_BUFFER   0x00044004 // Frame Buffer: Test Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
#define TEST_DEPTH            0x00044005 // Frame Buffer: Test Depth (Response: Bits Per Pixel)
#define TEST_PIXEL_ORDER      0x00044006 // Frame Buffer: Test Pixel Order (Response: State)
#define TEST_ALPHA_MODE       0x00044007 // Frame Buffer: Test Alpha Mode (Response: State)
#define TEST_VIRTUAL_OFFSET   0x00044009 // Frame Buffer: Test Virtual Offset (Response: X In Pixels, Y In Pixels)
#define TEST_OVERSCAN         0x0004400A // Frame Buffer: Test Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
#define TEST_PALETTE          0x0004400B // Frame Buffer: Test Palette (Response: RGBA Palette Values (Index 0 To 255))
#define RELEASE_BUFFER        0x00048001 // Frame Buffer: Release Buffer (Response: Releases And Disables The Frame Buffer)
#define SET_PHYSICAL_DISPLAY  0x00048003 // Frame Buffer: Set Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
#define SET_VIRTUAL_BUFFER    0x00048004 // Frame Buffer: Set Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
#define SET_DEPTH             0x00048005 // Frame Buffer: Set Depth (Response: Bits Per Pixel)
#define SET_PIXEL_ORDER       0x00048006 // Frame Buffer: Set Pixel Order (Response: State)
#define SET_ALPHA_MODE        0x00048007 // Frame Buffer: Set Alpha Mode (Response: State)
#define SET_VIRTUAL_OFFSET    0x00048009 // Frame Buffer: Set Virtual Offset (Response: X In Pixels, Y In Pixels)
#define SET_OVERSCAN          0x0004800A // Frame Buffer: Set Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
#define SET_PALETTE           0x0004800B // Frame Buffer: Set Palette (Response: RGBA Palette Values (Index 0 To 255))
#define GET_COMMAND_LINE      0x00050001 // Config: Get Command Line (Response: ASCII Command Line String)
#define GET_DMA_CHANNELS      0x00060001 // Shared Resource Management: Get DMA Channels (Response: Bits 0-15: DMA channels 0-15)

uint32_t ArmToVc(void *p)    { return ((uint32_t)p) +   VCADDR_BASE; }
uint32_t VcToArm(uint32_t p) { return (uint32_t)(p) & ~(VCADDR_BASE) ; }

static int        is_fb_init = 0;
static mailbox_fb mailbox_fb_data __attribute__((aligned(256))) ;
static uint32_t   p[256]          __attribute__((aligned(256))) ;


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
	p[i++] = 5;          // V3D
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
