#include <stdbool.h>
#include <stddef.h>
#include "uart.h"
#include "hwio.h"
#include "gpio.h"
#include "mailbox.h"
#include "std.h"

#include "v3d.h"

#define USE_MAILBOX_FB

#define CMD_MAX 256

//https://github.com/raspberrypi/linux/blob/75f1d14cee8cfb1964cbda21a30cb030a632c3c2/drivers/soc/bcm/bcm2835-power.c#L283
void
v3d_power_on()
{
	uint32_t reg = 0xFE10010C;
	IO_WRITE(reg, PM_MAGIC | IO_READ(reg) | 0x40);
	uart_wait(0x100000);
	IO_WRITE(ASB_RPIVID_S, ASB_RPIVID_MAGIC | (IO_READ(ASB_RPIVID_S) & ~1));
	uart_wait(0x100000);
	IO_WRITE(ASB_RPIVID_M, ASB_RPIVID_MAGIC | (IO_READ(ASB_RPIVID_M) & ~1));
	uart_wait(0x100000);
}

int
main()
{
	mailbox_fb *fb = mailbox_get_fb();

	//changet to tx rx for serialport.
	gpio_mode(14, 4);
	gpio_mode(15, 4);
	uart_init();

	uart_puts("\n\n");
	uart_puts("----------------------------------------------\n");
	uart_puts("START RPI4-001\n");
	uart_puts("----------------------------------------------\n");

	//V3D power on.
	v3d_power_on();

	uart_name_dword("CHECK : V3D_IDENT0:", IO_READ(V3D_IDENT0));
	uart_name_dword("CHECK : V3D_IDENTl:", IO_READ(V3D_IDENT1));
	uart_name_dword("CHECK : V3D_IDENT2:", IO_READ(V3D_IDENT2));

	uint32_t count = 0;

	enum {
		Width = 320,
		Height = 240,
	};
	mailbox_fb_init(Width, Height);
	uart_name_dword("FBADDR:", fb->pointer);
	while (1) {
		count++;
		{
			uint32_t *p = (uint32_t *)fb->pointer;
			for(int y = 0 ; y < Height; y++)
				for(int x = 0 ; x < Width; x++)
					p[x + y * Width] = ( (x + count) ^ y) << 17;
		}
		if (count > 0x100000) {
			//heart beat
			count = 0;
		}
	}
	return 0;
}

