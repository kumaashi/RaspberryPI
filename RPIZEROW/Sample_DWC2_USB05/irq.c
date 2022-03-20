#include "common.h"

//https://github.com/raspberrypi/firmware/issues/67
#define IRQ_BASE                                               (SUBSYSTEM_BASE + 0xB200)
#define IRQ_GPU_PENDING1                                       ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB204))
#define IRQ_GPU_PENDING2                                       ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB208))
#define IRQ_GPU_ENABLE2                                        ((volatile uint32_t *)(SUBSYSTEM_BASE + 0xB214))
#define IRQ_GPU_FAKE_ISR                                       0x10000
#define SMI_BASE                                               (SUBSYSTEM_BASE + 0x600000)
#define SMI_CS                                                 ((volatile uint32_t *)(SMI_BASE + 0x00))

//fake_vsync_isr=1
void fake_vsync(void) {
	*IRQ_GPU_ENABLE2 = IRQ_GPU_FAKE_ISR;
	*SMI_CS = 0;
	while( (*IRQ_GPU_PENDING2 & IRQ_GPU_FAKE_ISR) == 0 );
}

