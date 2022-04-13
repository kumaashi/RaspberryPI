#pragma once

#include <stdint.h>

#define DMA_CB_MAX 2560

typedef struct dma_control_block_t {
	uint32_t ti;
	uint32_t s_addr;
	uint32_t d_addr;
	uint32_t txfr_len;
	uint32_t stride;
	struct dma_control_block_t *next_cb;
	uint32_t debug;
	uint32_t reserved;
} __attribute__((__packed__)) dma_control_block;
dma_control_block *dma_get_cb();
void dma_cb_set_addr(dma_control_block *p, uint32_t dst, uint32_t src);
void dma_cb_set_stride_d(dma_control_block *p, uint16_t value);
void dma_cb_set_stride_s(dma_control_block *p, uint16_t value);
void dma_cb_set_ti_dst_inc(dma_control_block *p, int value);
void dma_cb_set_ti_src_inc(dma_control_block *p, int value);
void dma_cb_set_ti_tdmode(dma_control_block *p, int value);
void dma_cb_set_txfr_len(dma_control_block *p, uint32_t value);
void dma_cb_set_txfr_len_xlength(dma_control_block *p, uint16_t value);
void dma_cb_set_txfr_len_ylength(dma_control_block *p, uint16_t value);
void dma_clear_cb(dma_control_block *ret);
void dma_debug(int x);
void dma_init();
void dma_submit_cb(int ch);
void dma_wait(int ch);
