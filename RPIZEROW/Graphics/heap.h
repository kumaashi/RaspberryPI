#include <stdint.h>

#define HEAP_4K_MAX  (128)
#define HEAP_2M_MAX  (32)
#define HEAP_1M_MAX  (16)
#define HEAP_4M_MAX  (8)
#define HEAP_4K_SIZE (1024 * 4)
#define HEAP_1M_SIZE (1024 * 1024)
#define HEAP_2M_SIZE (HEAP_1M_SIZE * 2)
#define HEAP_4M_SIZE (HEAP_1M_SIZE * 4)

void heap_init(uint32_t saddr);
uint32_t *heap_4M_get();
uint32_t *heap_2M_get();
uint32_t *heap_1M_get();
uint32_t *heap_4k_get();

