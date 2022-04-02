#include <stdint.h>

#define HEAP_SIZE    (1024 * 1024)
#define HEAP_SIZE_4K (1024 * 4)
#define HEAP_MAX     (256)
void heap_init(uint32_t saddr);
uint32_t *heap_get(int index);
uint32_t *heap4k_get(int index);

