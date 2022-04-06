#include <stdint.h>

#include "binning_heap.h"

static uint32_t *binning_heaps[BINNING_HEAP_MAX];
static int binning_heap_index = 0;
void binning_heap_init(uint32_t saddr) {
	uint32_t start_addr = saddr;
	for(int i = 0 ; i < BINNING_HEAP_MAX; i++) {
		binning_heaps[i] = (uint32_t *)start_addr;
		start_addr += BINNING_HEAP_SIZE;
	}
}

uint32_t *binning_heap_get() {
	return binning_heaps[binning_heap_index++ % BINNING_HEAP_MAX];
}

