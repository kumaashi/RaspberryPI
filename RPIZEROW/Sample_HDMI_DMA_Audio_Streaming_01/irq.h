#ifndef _IRQ_H_
#define _IRQ_H_

#include <stdint.h>

#define IRQ_INFO_MAX 32

typedef struct {
	int (*callback)(void *);
	void *data;
} irq_info;

void irq_init();
void irq_fake_vsync(void);
void irq_enable_flip_isr(uint32_t frame_index, uint32_t count);
void irq_set_info(irq_info *info, int index);
uint32_t irq_get_flip_count();


#endif //_IRQ_H_
