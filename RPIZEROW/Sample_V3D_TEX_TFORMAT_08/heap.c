#include "heap.h"

static uint32_t *heaps[HEAP_MAX];
static uint32_t *heaps4k[HEAP_MAX];
static int heap_index = 0;
static int heap4k_index = 0;


void heap_init(uint32_t saddr) {
	heap_index = 0;
	heap4k_index = 0;
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
	return heaps[heap_index++];
}

uint32_t *heap4k_get(int index) {
	return heaps4k[heap4k_index++];
}
