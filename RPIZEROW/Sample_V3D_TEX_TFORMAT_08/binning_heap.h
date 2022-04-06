#ifndef _BINNING_HEAP_
#define _BINNING_HEAP_

#define BINNING_HEAP_SIZE (1024 * 1024)
#define BINNING_HEAP_MAX  (32) //mustbe 2^n 

void binning_heap_init(uint32_t saddr);
uint32_t *binning_heap_get();

#endif //_BINNING_HEAP_

