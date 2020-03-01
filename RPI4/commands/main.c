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
static uint8_t cmdstr[CMD_MAX];
char token[16][32];
static uint32_t cmd_offset = 0;

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

void
reset()
{
	uint32_t r = IO_READ(PM_RSTS);
	r &= ~0xfffffaaa;
	IO_WRITE(PM_RSTS, PM_MAGIC | r);
	IO_WRITE(PM_WDOG, PM_MAGIC | 0x0A);
	IO_WRITE(PM_RSTC, PM_MAGIC | 0x20);
}

void
cmd_reset()
{
	cmd_offset = 0;
	memset(cmdstr, 0, sizeof(cmdstr));
	memset(token, 0, sizeof(token));

	uint32_t sp = _get_stack_pointer();
	uart_puts("SP:");
	uart_put_dword(sp);
	uart_puts("\n");

	uart_puts(">");
}

void
cmd_setc(uint8_t c)
{
	cmdstr[cmd_offset++] = c;
}

uint32_t
hex2value(char *s)
{
	uint32_t value = 0;

	while (*s) {
		char c = *s;
		uint8_t v = 0;
		if (c >= '0' && c <= '9') v = c - '0';
		if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
		if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
		value <<= 4;
		value |= v;
		s++;
	}
	return value;
}


bool
issep(char c, char *s)
{
	while (*s)
		if (c == *s++)
			return true;
	return false;
}

void
parse_token(char *str)
{
	char temp[128];
	char *p = temp;
	int index = 0;

	while (*str) {
		int c = *str++;
		if (issep(c, ": \t\r\n")) {
			if (p != temp) {
				*p = 0;
				strcpy(token[index], temp);
				uart_puts("TOKEN:");
				uart_puts(token[index]);
				uart_puts("\n");
				index++;
			}
			p = temp;
		} else {
			*p++ = c;
		}
	}
}

void
cmd_process()
{
	cmd_setc(0);
	if (strlen(cmdstr) == 0)
		return;
	uart_puts("CMD : ");
	uart_puts(cmdstr);
	uart_puts("\n");
	parse_token(cmdstr);

	int cmd = token[0][0];
	char *strcmd = token[0];
	char *arg0 = token[1];
	char *arg1 = token[1];

	if (strcmp("reboot", strcmd) == 0) {
		uart_puts("command NOTIFY_REBOOT\n");
		mailbox_notify_reboot();
		reset();
	}
	if (strcmp("domain", strcmd) == 0) {
		uart_puts("command get_domain_state:");
		for (int i = 0 ; power_domain_names[i] ; i++) {
			uart_puts(power_domain_names[i]);
			uart_puts("\n");
			mailbox_get_domain_state(i);
		}
		uart_puts("command set_domain_state:");
		for (int i = 0 ; power_domain_names[i] ; i++) {
			uart_puts(power_domain_names[i]);
			uart_puts("\n");
			if(strcmp("V3D", power_domain_names[i]) == 0) {
				uart_puts("V3D domain power on will take time...\n");
			}
			mailbox_set_domain_state(i, 1);
		}
	}
	if (strcmp("power", strcmd) == 0) {
		uart_puts("command get_power_state:");
		for (int i = 0 ; power_domain_names[i] ; i++) {
			uart_puts(power_domain_names[i]);
			uart_puts("\n");
			mailbox_get_power_state(i);
		}
		uart_puts("command set_power_state:");
		for (int i = 0 ; power_domain_names[i] ; i++) {
			uart_puts(power_domain_names[i]);
			uart_puts("\n");
			mailbox_set_power_state(i, 1);
		}
	}

	if (strcmp("setdomain", strcmd) == 0) {
		uart_puts("command set domain:");
		uint32_t index = hex2value(arg0);
		uint32_t value = hex2value(arg1);
		if (index == 0) {
			for (int i = 0 ; power_domain_names[i]; i++) {
				uart_put_dword(i);
				uart_puts(":");
				uart_puts(power_domain_names[i]);
				uart_puts("\n");
			}
		} else {
			uart_puts(power_domain_names[index]);
			uart_puts("\n");
			mailbox_set_domain_state(index, value);
		}
	}

	if (cmd == 'r' || cmd == 'R') {
		char *p = token[1];
		uint32_t addr = hex2value(p);
		uint32_t value = IO_READ(addr);
		uart_puts("command READ:ADDR:\n");
		uart_put_dword(addr);
		uart_puts("\ncommand READ:VALUE:\n");
		uart_put_dword(value);
	}

	if (cmd == 'w' || cmd == 'W') {
		uart_puts("command WRITE:\n");
		char *p = token[1];
		char *pv = token[2];
		uint32_t addr = hex2value(p);
		uint32_t value = hex2value(pv);

		uart_puts("WRITE:addr\n");
		uart_put_dword(value);
		uart_puts("\nVALUE:\n");
		uart_put_dword(value);
		uart_puts("\n");
		IO_WRITE(addr, value);
	}

	if (strcmp("getvoltage", strcmd) == 0) {
		uart_puts("command GET_VOLTAGE:\n");
		for (int i = 0 ; voltage_names[i]; i++)
			mailbox_get_voltage(i);
	}

	if (strcmp("setvoltage", strcmd) == 0) {
		uint32_t index = hex2value(arg0);
		uint32_t value = hex2value(arg1);
		uart_puts("command SET_VOLTAGE:\n");
		mailbox_set_voltage(index, value);
	}

	if (strcmp("qpu", strcmd) == 0) {
		uart_puts("command QPU_ENABLE:\n");
		mailbox_qpu_enable();
	}

	if (strcmp("version", strcmd) == 0) {
		uart_puts("command GET_FIRMWARE_REVISION:\n");
		mailbox_get_fw_version();
	}

	if (strcmp("getclock", strcmd) == 0) {
		for (int i = 1; clk_block_name[i]; i++) {
			uart_puts("command GET_CLOCK_STATE ");
			uart_puts(clk_block_name[i]);
			uart_puts("\n");
			mailbox_get_clock_state(i);
			mailbox_get_clock_rate(i);
		}
	}

	if (strcmp("setclock", strcmd) == 0) {
		uint32_t index = hex2value(arg0);
		uint32_t value = hex2value(arg1);
		uart_puts("command SET_CLOCK_STATE ");
		uart_puts(clk_block_name[index]);
		uart_puts("\n");
		mailbox_set_clock_state(index, 1);
		mailbox_set_clock_rate(index, value);
	}

	if (cmd == 'a' || cmd == 'A') {
		uart_puts("command GET_MEMORY INFO:\n");
		mailbox_get_arm_memory();
		mailbox_get_vc_memory();
	}
	uart_puts("\n");
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
	cmd_reset();


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
		uint8_t c = 0;
		do {
			bool is_process = false;
			c = uart_getc();
			if (c) {
				cmd_setc(c);
				if (c == '\n') {
					is_process = true;
					cmd_setc(0);
				}
				uart_putc(c);
			}
			if (is_process) {
				cmd_process();
				cmd_reset();
			}
		} while(c);
	}
	return 0;
}

