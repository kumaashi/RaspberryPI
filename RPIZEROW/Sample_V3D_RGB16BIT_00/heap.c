#include "heap.h"

static uint32_t *heaps4k[HEAP_4K_MAX];
static uint32_t *heaps1m[HEAP_1M_MAX];
static uint32_t *heaps2m[HEAP_2M_MAX];
static uint32_t *heaps4m[HEAP_4M_MAX];

static int heap4k_index = 0;
static int heap1m_index = 0;
static int heap2m_index = 0;
static int heap4m_index = 0;

void heap_init(uint32_t saddr) {
	uint32_t start_addr = saddr;
	heap4k_index = 0;
	heap1m_index = 0;
	heap2m_index = 0;
	heap4m_index = 0;

	for(int i = 0 ; i < HEAP_4K_MAX; i++) {
		heaps4k[i] = (uint32_t *)start_addr;
		start_addr += HEAP_4K_SIZE;
	}
	for(int i = 0 ; i < HEAP_1M_MAX; i++) {
		heaps1m[i] = (uint32_t *)start_addr;
		start_addr += HEAP_1M_SIZE;
	}
	for(int i = 0 ; i < HEAP_2M_MAX; i++) {
		heaps2m[i] = (uint32_t *)start_addr;
		start_addr += HEAP_2M_SIZE;
	}
	for(int i = 0 ; i < HEAP_4M_MAX; i++) {
		heaps4m[i] = (uint32_t *)start_addr;
		start_addr += HEAP_4M_SIZE;
	}
}

uint32_t *heap_4k_get(int index) {
	return heaps4k[heap4k_index++];
}

uint32_t *heap_1M_get(int index) {
	return heaps1m[heap1m_index++];
}

uint32_t *heap_2M_get(int index) {
	return heaps2m[heap2m_index++];
}

uint32_t *heap_4M_get(int index) {
	return heaps4m[heap4m_index++];
}
