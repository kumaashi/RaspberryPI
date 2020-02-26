#include <stdbool.h>
#include <stddef.h>
#include "uart.h"
#include "hwio.h"
#include "gpio.h"

#include "mailbox.h"

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
#define GET_DOMAIN_STATE      0x00030030
#define SET_DOMAIN_STATE      0x00038030
#define NOTIFY_REBOOT         0x00030048
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

//https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface
#define MAILBOX_MEM_FLAG_DISCARDABLE                                   (1 << 0)
#define MAILBOX_MEM_FLAG_NORMAL                                        (0 << 2)
#define MAILBOX_MEM_FLAG_DIRECT                                        (1 << 2)
#define MAILBOX_MEM_FLAG_COHERENT                                      (2 << 2)
#define MAILBOX_MEM_FLAG_L1_NONALLOCATING                              ((MAILBOX_MEM_FLAG_DIRECT | MAILBOX_MEM_FLAG_COHERENT))
#define MAILBOX_MEM_FLAG_ZERO                                          (1 << 4)
#define MAILBOX_MEM_FLAG_NO_INIT                                       (1 << 5)
#define MAILBOX_MEM_FLAG_HINT_PERMALOCK                                (1 << 6)


const char *power_domain_names[] = {
	"RPI_POWER_DOMAIN_NONE",
	"RPI_POWER_DOMAIN_I2C0",
	"RPI_POWER_DOMAIN_I2C1",
	"RPI_POWER_DOMAIN_I2C2",
	"RPI_POWER_DOMAIN_VIDEO_SCALER",
	"RPI_POWER_DOMAIN_VPU1",
	"RPI_POWER_DOMAIN_HDMI",
	"RPI_POWER_DOMAIN_USB",
	"RPI_POWER_DOMAIN_VEC",
	"RPI_POWER_DOMAIN_JPEG",
	"RPI_POWER_DOMAIN_H264",
	"RPI_POWER_DOMAIN_V3D",
	"RPI_POWER_DOMAIN_ISP",
	"RPI_POWER_DOMAIN_UNICAM0",
	"RPI_POWER_DOMAIN_UNICAM1",
	"RPI_POWER_DOMAIN_CCP2RX",
	"RPI_POWER_DOMAIN_CSI2",
	"RPI_POWER_DOMAIN_CPI",
	"RPI_POWER_DOMAIN_DSI0",
	"RPI_POWER_DOMAIN_DSI1",
	"RPI_POWER_DOMAIN_TRANSPOSER",
	"RPI_POWER_DOMAIN_CCP2TX",
	"RPI_POWER_DOMAIN_CDP",
	"RPI_POWER_DOMAIN_ARM",
	NULL,
};

const char *clk_block_name[] = {
	"0x000000000: reserved ",
	"0x000000001: EMMC",
	"0x000000002: UART",
	"0x000000003: ARM",
	"0x000000004: CORE",
	"0x000000005: V3D",
	"0x000000006: H264",
	"0x000000007: ISP",
	"0x000000008: SDRAM",
	"0x000000009: PIXEL",
	"0x00000000a: PWM",
	"0x00000000c: EMMC2",
	NULL,
};

const char *voltage_names[] = {
	"reserved",
	"Core",
	"SDRAM_C",
	"SDRAM_P",
	"SDRAM_I",
	NULL,
};

static volatile mailbox_fb fbdata __attribute__ ((aligned (256))) = {0};
static volatile uint32_t mailbox_data __attribute__ ((aligned (256))) = {0};

static uint32_t
ArmToVc(void *p)
{
	return ((uint32_t)p) +   V3D_ADDR_BASE;
}

static uint32_t
VcToArm(uint32_t p)
{
	return (uint32_t)(p) & ~(V3D_ADDR_BASE) ;
}

mailbox_fb *
mailbox_get_fb(void)
{
	return &fbdata;
}

uint32_t
mailbox_read(uint32_t mask)
{
	uint32_t result = 0;
	volatile int count = 0;

	while (1) {
		while (IO_READ(MAILBOX_STATUS) & MAILBOX_EMPTY) {
			uart_wait(0x10000);
			continue;
		}
		result = IO_READ(MAILBOX_READ);
		if ( (result & 0xF) == mask) {
			break;
		}
	}
	return result & 0xFFFFFFF0;
}

uint32_t
mailbox_write(uint32_t mask, uint32_t v)
{
	while (IO_READ(MAILBOX_STATUS) & MAILBOX_FULL) {
		uart_wait(0x10000);
		continue;
	}
	IO_WRITE(MAILBOX_WRITE, mask | (v & 0xFFFFFFF0));
	return 0;
}

void
mailbox_print(const uint32_t *p, int len)
{
	for (int off = 0; off < len; off++) {
		uart_put_dword(p[off]);
		uart_puts(", ");
	}
	uart_puts("\n");
}

uint32_t
mailbox_send(uint32_t *p, int len)
{
	int count = 4;

	uart_puts("SEND : ");
	mailbox_print(p, len);
	do {
		//_memory_barrier();
		//mailbox_write(MAILBOX_ARM_TO_VC, ArmToVc((void *)p));
		mailbox_write(MAILBOX_ARM_TO_VC, (uint32_t)p);
		//_memory_barrier();
		mailbox_read(MAILBOX_ARM_TO_VC);
		if (p[1] == 0x80000000) {
			break;
		}
		uart_debug_puts("FAILED p[1]=\n", p[1]);
	} while (count-- > 0);
	uart_puts("RECV : ");
	mailbox_print(p, len);
	return p[1];
}

uint32_t
mailbox_qpu_enable()
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request
	p[i++] = SET_CLOCK_RATE; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 0x00000008; // (the tag id)
	p[i++] = 5;          // V3D
	p[i++] = 250 * 1000 * 1000;
	p[i++] = ENABLE_QPU;    // (the tag id)
	p[i++] = 0x00000004;          // (size of the buffer)
	p[i++] = 0x00000004;          // (size of the data)
	p[i++] = 0x00000001;
	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return p[1];
}

uint32_t
mailbox_notify_reboot()
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = NOTIFY_REBOOT; // (the tag id)
	p[i++] = 0x00000004; // length + 4
	p[i++] = 0x00000000; // size of the data
	p[i++] = 0x00000000; // request

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_fw_version()
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_FIRMWARE_REVISION; // (the tag id)
	p[i++] = 0x00000004; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = 0x00000000; // request

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_set_voltage(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = SET_VOLTAGE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000008; // size of the data
	p[i++] = id; // request
	p[i++] = value; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_voltage(int id)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_VOLTAGE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = id; // request
	p[i++] = 0x00000000; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_set_clock_state(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request
	p[i++] = SET_CLOCK_STATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = id; // request
	p[i++] = value;
	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_set_clock_rate(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = SET_CLOCK_RATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = id; // request
	p[i++] = value; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_clock_state(int id)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request
	p[i++] = GET_CLOCK_STATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = id; // request
	p[i++] = 0x00000000; // pad
	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_clock_rate(int id)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_CLOCK_RATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000004; // size of the data
	p[i++] = id; // request
	p[i++] = 0x00000000; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t mailbox_get_arm_memory()
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;
	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_ARM_MEMORY; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000000; // size of the data
	p[i++] = 0x00000000; // request
	p[i++] = 0x00000000; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_vc_memory()
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_VC_MEMORY; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000000; // size of the data
	p[i++] = 0x00000000; // request
	p[i++] = 0x00000000; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_set_power_state(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;
	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = SET_POWER_STATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000008; // size of the data
	p[i++] = id; // request
	p[i++] = value; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_power_state(int id)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;
	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_POWER_STATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000008; // size of the data
	p[i++] = id; // request
	p[i++] = 0xFFFFFFFF; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_set_domain_state(int id, uint32_t value)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = SET_DOMAIN_STATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000008; // size of the data
	p[i++] = id; // request
	p[i++] = value; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

uint32_t
mailbox_get_domain_state(int id)
{
	int i = 0;
	uint32_t *p = (uint32_t *)(uintptr_t)mailbox_data;

	p[i++] = 0;          // size
	p[i++] = 0x00000000; // process request

	p[i++] = GET_DOMAIN_STATE; // (the tag id)
	p[i++] = 0x00000008; // length + 4
	p[i++] = 0x00000008; // size of the data
	p[i++] = id; // request
	p[i++] = 0xFFFFFFFF; // pad

	p[i++] = 0x00000000; // end tag
	p[0]   = i * sizeof(uint32_t); // actual size
	mailbox_send(p, i);
	return 0;
}

int32_t
mailbox_fb_init(uint32_t w, uint32_t h)
{
	volatile mailbox_fb *fb = &fbdata;

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

	int count = 0;
	do {
		mailbox_write(MAILBOX_FRAMEBUFFER, ArmToVc((void *)fb));
		mailbox_read(MAILBOX_FRAMEBUFFER);
		if (fb->pointer) {
			break;
		}
		count++;
		uart_wait(0x1000);
		uart_debug_puts("MAILBOX_FRAMEBUFFER count=\n", count);
	} while (1);
	uint32_t vc_pointer = fb->pointer;
	fb->pointer = VcToArm(fb->pointer);
	fb->pointer_vc = vc_pointer;
	return 0;
}
