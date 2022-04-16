#include <stdint.h>

#define HEAP_SIZE    (1024 * 1024 * 2)
#define HEAP_SIZE_4K (1024 * 4)
#define HEAP_MAX     (64)
void heap_init(uint32_t saddr);
uint32_t *heap_get();
uint32_t *heap4k_get();
