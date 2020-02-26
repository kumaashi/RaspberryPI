#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include <stdint.h>

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
	uint32_t pointer_vc;
} mailbox_fb;

uint32_t mailbox_qpu_enable();
uint32_t mailbox_notify_reboot();
uint32_t mailbox_get_fw_version();
uint32_t mailbox_set_voltage(int id, uint32_t value);
uint32_t mailbox_get_voltage(int id);
uint32_t mailbox_set_clock_state(int id, uint32_t value);
uint32_t mailbox_set_clock_rate(int id, uint32_t value);
uint32_t mailbox_get_clock_state(int id);
uint32_t mailbox_get_clock_rate(int id);
uint32_t mailbox_get_arm_memory();
uint32_t mailbox_get_vc_memory();
uint32_t mailbox_set_power_state(int id, uint32_t value);
uint32_t mailbox_get_power_state(int id);
uint32_t mailbox_set_domain_state(int id, uint32_t value);
uint32_t mailbox_get_domain_state(int id);
int32_t mailbox_fb_init(uint32_t w, uint32_t h);
mailbox_fb *mailbox_get_fb(void);

extern const char *power_domain_names[];
extern const char *clk_block_name[];
extern const char *voltage_names[];

#endif /* _MAILBOX_H_ */
