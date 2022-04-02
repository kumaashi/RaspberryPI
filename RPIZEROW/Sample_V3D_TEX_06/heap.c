#include "heap.h"

uint32_t *heaps[HEAP_MAX];
uint32_t *heaps4k[HEAP_MAX];

void heap_init(uint32_t saddr) {
	uint32_t start_addr = saddr;
	for(int i = 0 ; i < HEAP_MAX; i++) {
		heaps[i] = (uint32_t *)start_addr;
		start_addr += HEAP_SIZE;
	}
	for(int i = 0 ; i < HEAP_MAX; i++) {
		heaps4k[i] = (uint32_t *)start_addr;
		start_addr += HEAP_SIZE_4K;
	}
}

uint32_t *heap_get(int index) {
	return heaps[index];
}

uint32_t *heap4k_get(int index) {
	return heaps4k[index];
}
