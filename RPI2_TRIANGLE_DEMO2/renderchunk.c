#include <stdint.h>
#include "hwio.h"
#include "util.h"

volatile uint8_t renderchunk_buffer0[TILE_MEM_ALLOC_SIZE] __attribute__((aligned(256))) ;

void init_render_chunk_buffer() {
	memset_t((void *)renderchunk_buffer0, 0x5A, sizeof(renderchunk_buffer0));
}
