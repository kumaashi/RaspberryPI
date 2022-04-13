#pragma once

void fake_vsync(void);
void enable_irq_flip_isr(uint32_t frame_index, uint32_t count);
uint32_t get_irq_flip_count();
