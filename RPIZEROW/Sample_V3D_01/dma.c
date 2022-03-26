/*
 RPA DMA apis.
 DREQ Peripheral
 0 DREQ = 1 This is always on so use this channel if no DREQ is required. 
 1 DSI
 2 PCM TX
 3 PCM RX
 4 SMI
 5 PWM
 6 SPI TX
 7 SPI RX
 8 BSC/SPI Slave TX
 9 BSC/SPI Slave RX
 10 unused
 11 e.MMC
 12 UART TX
 13 SD HOST
 14 UART RX.
 15 DSI
 16 SLIMBUS MCTX.
 17 HDMI
 18 SLIMBUS MCRX
 19 SLIMBUS DC0
 20 SLIMBUS DC1
 21 SLIMBUS DC2
 22 SLIMBUS DC3
 23 SLIMBUS DC4
 24 Scaler FIFO 0 & SMI *
 25 Scaler FIFO 1 & SMI *
 26 Scaler FIFO 2 & SMI *
 27 SLIMBUS DC5
 28 SLIMBUS DC6
 29 SLIMBUS DC7
 30 SLIMBUS DC8
 31 SLIMBUS DC9
*/
#include "common.h"
#include "hw.h"
#include "uart.h"
#include "dma.h"

#define DMA_BASE                   (SUBSYSTEM_BASE + 0x7000)
#define DMA_CS(x)                  ((volatile uint32_t *)(DMA_BASE + 0x00 + (0x100 * (x))))
#define DMA_CB_ADDR(x)             ((volatile uint32_t *)(DMA_BASE + 0x04 + (0x100 * (x))))
#define DMA_TI(x)                  ((volatile uint32_t *)(DMA_BASE + 0x08 + (0x100 * (x))))
#define DMA_S_ADDR(x)              ((volatile uint32_t *)(DMA_BASE + 0x0c + (0x100 * (x))))
#define DMA_D_ADDR(x)              ((volatile uint32_t *)(DMA_BASE + 0x10 + (0x100 * (x))))
#define DMA_TXFR_LEN(x)            ((volatile uint32_t *)(DMA_BASE + 0x14 + (0x100 * (x))))
#define DMA_STRIDE(x)              ((volatile uint32_t *)(DMA_BASE + 0x18 + (0x100 * (x))))
#define DMA_NEXTCONBK(x)           ((volatile uint32_t *)(DMA_BASE + 0x1c + (0x100 * (x))))
#define DMA_DEBUG(x)               ((volatile uint32_t *)(DMA_BASE + 0x20 + (0x100 * (x))))
#define DMA_INT_STATUS             ((volatile uint32_t *)(DMA_BASE + 0xFE0))
#define DMA_ENABLE                 ((volatile uint32_t *)(DMA_BASE + 0xFF0))

static dma_control_block g_dma_cbs[DMA_CB_MAX]  __attribute__ ((aligned (32))) ;
struct dma_control_block_t *g_dma_cb_head;
static int dma_cb_index;

void dma_clear_cb(dma_control_block *ret) {
	ret->ti = 0;
	ret->s_addr = 0;
	ret->d_addr = 0;
	ret->txfr_len = 0;
	ret->stride = 0;
	ret->next_cb = 0;
	ret->debug = 0;
	ret->reserved = 0;
}

void dma_cb_dup(dma_control_block *dst, dma_control_block *src)
{
	dst->ti = src->ti;
	dst->s_addr = src->s_addr;
	dst->d_addr =src->d_addr;
	dst->txfr_len = src->txfr_len;
	dst->stride = src->stride;
	dst->next_cb = src->next_cb;
	dst->debug = src->debug;
	dst->reserved =src->reserved;
}
void dma_init() {
	dma_cb_index = 0;
	g_dma_cb_head = g_dma_cbs;
	for(int i = 0 ; i < DMA_CB_MAX; i++) {
		dma_clear_cb(&g_dma_cbs[i]);
	}
}

void dma_cb_set_addr(dma_control_block *p, uint32_t dst, uint32_t src) {
	p->d_addr = (uint32_t)dst;
	p->s_addr = (uint32_t)src;

	if(p->s_addr < VCADDR_BASE)
		p->s_addr += VCADDR_BASE;
	if(p->d_addr < VCADDR_BASE)
		p->d_addr += VCADDR_BASE;
}

void dma_cb_set_ti_permap(dma_control_block *p, uint32_t value) {
	p->ti |= value << 16;
}

void dma_cb_set_ti_burst_length(dma_control_block *p, uint32_t value) {
	p->ti |= (value & 0xF) << 12;
}

void dma_cb_set_ti_src_dreq(dma_control_block *p, int value) {
	p->ti |= ((value ? 1 : 0) << 10);
}

void dma_cb_set_ti_src_inc(dma_control_block *p, int value) {
	p->ti |= ((value ? 1 : 0) << 8);
}

void dma_cb_set_ti_dst_dreq(dma_control_block *p, int value) {
	p->ti |= ((value ? 1 : 0) << 6);
}

void dma_cb_set_ti_dst_inc(dma_control_block *p, int value) {
	p->ti |= ((value ? 1 : 0) << 4);
}

void dma_cb_set_ti_tdmode(dma_control_block *p, int value) {
	p->ti |= ((value ? 1 : 0) << 1);
}

void dma_cb_set_txfr_len(dma_control_block *p, uint32_t value) {
	p->txfr_len = value;
}

void dma_cb_set_txfr_len_xlength(dma_control_block *p, uint16_t value) {
	p->txfr_len &= 0xFFFF0000;
	p->txfr_len |= value;
}

void dma_cb_set_txfr_len_ylength(dma_control_block *p, uint16_t value) {
	p->txfr_len &= 0x0000FFFF;
	p->txfr_len |= value << 16;
}

void dma_cb_set_stride_d(dma_control_block *p, uint16_t value) {
	p->stride &= 0x0000FFFF;
	p->stride |= value << 16;
}

void dma_cb_set_stride_s(dma_control_block *p, uint16_t value) {
	p->stride &= 0xFFFF0000;
	p->stride |= value;
}

dma_control_block *dma_get_cb() {
	dma_control_block * ret = &g_dma_cbs[dma_cb_index % DMA_CB_MAX];
	dma_clear_cb(ret);
	dma_cb_index++;

	ret->next_cb = &g_dma_cbs[dma_cb_index % DMA_CB_MAX];
	dma_clear_cb(ret->next_cb);
	return ret;
}

void dma_debug(int x) {
	uart_puts("----------------------------------------------------------------\n");
	uart_debug_puts("DMA : DMA_INT_STATUS             : ", *DMA_INT_STATUS);
	uart_debug_puts("DMA : DMA_ENABLE                 : ", *DMA_ENABLE);
	uart_debug_puts("DMA : DMA_CS(x)                  : ", *DMA_CS(x)             );
	uart_debug_puts("DMA : DMA_CB_ADDR(x)             : ", *DMA_CB_ADDR(x)        );
	uart_debug_puts("DMA : DMA_TI(x)                  : ", *DMA_TI(x)             );
	uart_debug_puts("DMA : DMA_S_ADDR(x)              : ", *DMA_S_ADDR(x)         );
	uart_debug_puts("DMA : DMA_D_ADDR(x)              : ", *DMA_D_ADDR(x)         );
	uart_debug_puts("DMA : DMA_TXFR_LEN(x)            : ", *DMA_TXFR_LEN(x)       );
	uart_debug_puts("DMA : DMA_STRIDE(x)              : ", *DMA_STRIDE(x)         );
	uart_debug_puts("DMA : DMA_NEXTCONBK(x)           : ", *DMA_NEXTCONBK(x)      );
	uart_debug_puts("DMA : DMA_DEBUG(x)               : ", *DMA_DEBUG(x)          );
	uart_debug_puts("DMA : g_dma_cb_head              : ", (uint32_t)g_dma_cb_head);
	uart_puts("----------------------------------------------------------------\n");

}

void dma_submit_cb(int ch) {
	*DMA_CS(ch) = (1 << 31);
	*DMA_CB_ADDR(ch) = (uint32_t)g_dma_cb_head;

	InvalidateData();
	*DMA_CS(ch) |= (1 << 28);
	*DMA_CS(ch) |= (1 << 0);

	dma_cb_index++;
	g_dma_cb_head = &g_dma_cbs[dma_cb_index % DMA_CB_MAX];
}

void dma_wait(int ch) {
	int count = 0;
	while((*DMA_CS(ch)) & 0x01) {
		count++;
		if(count > 0x100000) {
			uart_debug_puts("DMA : hang DMA_CS=", *DMA_CS(ch));
			uart_debug_puts("DMA : hang ch=", ch);
			break;
		}
	}
}

