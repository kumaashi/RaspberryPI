#include "util.h"

V3DContext context  __attribute__((aligned(256)));
V3DContext *ctx = &context;
volatile int frame = 0;

void V3DControlSetShaderInfo(V3DContext *ctx) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_SHADER_INFO);
	V3DWrite8(ctx,   0x00);
	V3DWrite8(ctx,   6 * 4);
	V3DWrite8(ctx,   0);
	V3DWrite8(ctx,   3);
	V3DWrite32(ctx,  ctx->bus_addr_vc + TILE_MEM_OFFSET_FSHADER);
	V3DWrite32(ctx,  0);
	V3DWrite32(ctx,  ctx->bus_addr_vc + TILE_MEM_OFFSET_VERTEX_BUFFER);

	V3DSetOffset(ctx, TILE_MEM_OFFSET_INDEX_BUFFER);
	V3DWrite8(ctx, 0); // top
	V3DWrite8(ctx, 1); // bottom left
	V3DWrite8(ctx, 2); // bottom right
}

void V3DControlSetFragmentShader(V3DContext *ctx) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_FSHADER);
	V3DWrite32(ctx, 0x958e0dbf);
	V3DWrite32(ctx, 0xd1724823); /* mov r0, vary; mov r3.8d, 1.0 */
	V3DWrite32(ctx, 0x818e7176); 
	V3DWrite32(ctx, 0x40024821); /* fadd r0, r0, r5; mov r1, vary */
	V3DWrite32(ctx, 0x818e7376); 

	V3DWrite32(ctx, 0x10024862); /* fadd r1, r1, r5; mov r2, vary */
	V3DWrite32(ctx, 0x819e7540); 

	V3DWrite32(ctx, 0x114248a3); /* fadd r2, r2, r5; mov r3.8a, r0 */
	V3DWrite32(ctx, 0x809e7009); 

	V3DWrite32(ctx, 0x115049e3); /* nop; mov r3.8b, r1 */
	V3DWrite32(ctx, 0x809e7012); 
	V3DWrite32(ctx, 0x116049e3); /* nop; mov r3.8c, r2 */
	V3DWrite32(ctx, 0x159e76c0); 

	V3DWrite32(ctx, 0x30020ba7); /* mov tlbc, r3; nop; thrend */
	V3DWrite32(ctx, 0x009e7000);
	V3DWrite32(ctx, 0x100009e7); /* nop; nop; nop */
	V3DWrite32(ctx, 0x009e7000);
	V3DWrite32(ctx, 0x500009e7); /* nop; nop; sbdone */

}

void V3DControlListFlush(V3DContext *ctx) {
	V3DWrite8(ctx,  TILE_CTRLLIST_FLUSH);
}

void V3DControlListNop(V3DContext *ctx) {
	V3DWrite8(ctx,  TILE_CTRLLIST_NOP);
}

void V3DControlListHalt(V3DContext *ctx) {
	V3DWrite8(ctx,  TILE_CTRLLIST_HALT);
}

void V3DControlListCreateBinning(V3DContext *ctx) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_BINNIG_CTRL_LIST);

	V3DWrite8(ctx,   TILE_CTRLLIST_BINNING_CONFIG);
	V3DWrite32(ctx,  ctx->bus_addr_vc + TILE_MEM_OFFSET_BINNING_DATA);
	V3DWrite32(ctx,  TILE_UNIT_BYTES);
	V3DWrite32(ctx,  ctx->bus_addr_vc + TILE_MEM_OFFSET_STATE);
	V3DWrite8(ctx,   SCREEN_WIDTH  / TILE_SIZE);
	V3DWrite8(ctx,   SCREEN_HEIGHT / TILE_SIZE);

	uint8_t cfg_binning = TILE_CTRLLIST_BINNING_AUTO;
	cfg_binning |= TILE_CTRLLIST_BINNING_MS;

	V3DWrite8(ctx,   cfg_binning);

	//Start Binnin
	V3DWrite8(ctx,   TILE_CTRLLIST_START_BINNING);

	//Clip WIndow
	V3DWrite8(ctx,   TILE_CTRLLIST_CLIP_WINDOW);
	V3DWrite16(ctx,  0);
	V3DWrite16(ctx,  0);
	V3DWrite16(ctx,  SCREEN_WIDTH);
	V3DWrite16(ctx,  SCREEN_HEIGHT);

	V3DWrite8(ctx,   TILE_CTRLLIST_VIEWPORT_OFFSET);
	V3DWrite16(ctx,  0x00);
	V3DWrite16(ctx,  0x00);

	uint8_t cfg0 = TILE_CTRLLIST_STATE_ENABLE_FORWARD_FACING_PRIM |
		TILE_CTRLLIST_STATE_ENABLE_REVERSE_FACING_PRIM |
		TILE_CTRLLIST_STATE_CLOCKWISE_PRIM;

	if(ENABLE_MSAAx4) {
		cfg0 |= TILE_CTRLLIST_STATE_RASTERISER_OVERSAMPLE_MODE_4x;
	}
	V3DWrite8(ctx,  TILE_CTRLLIST_STATE);
	V3DWrite8(ctx,  cfg0);
	V3DWrite8(ctx,  0x02);
	V3DWrite8(ctx,  0x00);


	V3DWrite8(ctx,  TILE_CTRLLIST_NV_SHADER_PRE);
	V3DWrite32(ctx, ctx->bus_addr_vc + TILE_MEM_OFFSET_SHADER_INFO);

	V3DWrite8(ctx,  TILE_CTRLLIST_VERTEX_ARRAY_PRIM);
	enum {
		VERTEX_ARRAY_PRIM_POINTS = 0,
		VERTEX_ARRAY_PRIM_LINES,
		VERTEX_ARRAY_PRIM_LINE_LOOP,
		VERTEX_ARRAY_PRIM_LINE_STRIP,
		VERTEX_ARRAY_PRIM_TRIANGLES,
		VERTEX_ARRAY_PRIM_TRIANGLE_STRIP,
		VERTEX_ARRAY_PRIM_MAX,
	};

	V3DWrite8(ctx,  VERTEX_ARRAY_PRIM_TRIANGLES);
	V3DWrite32(ctx, ctx->vertex_count);
	V3DWrite32(ctx, 0);

	V3DControlListFlush(ctx);
	V3DControlListNop(ctx);
	V3DSaveOffset(ctx, 0);
	
	
	V3DControlSetFragmentShader(ctx);
	V3DControlSetShaderInfo(ctx);
	
	
}

void V3DControlListClearColor(V3DContext *ctx, uint32_t data0, uint32_t data1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_CLEAR_COLOR);
	V3DWrite32(ctx,   data0);
	V3DWrite32(ctx,   data1);
	V3DWrite32(ctx,   0);
	V3DWrite8(ctx,    0);
}

void V3DControlListRenderModeConfig(V3DContext *ctx, void *dest, uint16_t width, uint16_t height, uint8_t cfg0, uint8_t cfg1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_RENDER_MODE_CONFIG);
	V3DWrite32(ctx,   (uint32_t)dest);
	V3DWrite16(ctx,   width);
	V3DWrite16(ctx,   height);
	V3DWrite8(ctx,    cfg0);
	V3DWrite8(ctx,    cfg1);
}

void V3DControlListRenderTileCoord(V3DContext *ctx, uint8_t x, uint8_t y) {
	V3DWrite8(ctx,    TILE_CTRLLIST_RENDER_TILE_COORD);
	V3DWrite8(ctx,    x);
	V3DWrite8(ctx,    y);
}

void V3DControlListStoreGeneral(V3DContext *ctx, uint16_t cfg0, uint32_t cfg1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_STORE_GENERAL);
	V3DWrite16(ctx,   cfg0);
	V3DWrite32(ctx,   cfg1);
}

void V3DControlListBranchToSublist(V3DContext *ctx, uint32_t addr) {
	V3DWrite8(ctx,  TILE_CTRLLIST_BRANCH_TO_SUBLIST);
	V3DWrite32(ctx, addr);
}

void V3DControlListStoreMsResolvedBufferAndEndFrame(V3DContext *ctx) {
	V3DWrite8(ctx,  TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER_AND_END_FRAME);
}

void V3DControlListStoreMsResolvedBuffer(V3DContext *ctx) {
	V3DWrite8(ctx,  TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER);
}

void V3DControlListCreateRendering(V3DContext *ctx, void *dest) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_RENDER_CTRL_LIST);

	uint32_t col = 0xFFFFFF00;
	V3DControlListClearColor(ctx, col, col);
	uint8_t cfg0 = TILE_CTRLLIST_RENDER_DEFAULT;
	cfg0 |= TILE_CTRLLIST_RENDER_CFG_MS;
	
	V3DWrite8(ctx,  TILE_CTRLLIST_PRIM_TYPE);
	V3DWrite8(ctx,  0x32);
	
	V3DControlListRenderModeConfig(ctx, dest, SCREEN_WIDTH, SCREEN_HEIGHT, cfg0, 0x00);

	V3DControlListRenderTileCoord(ctx, 0, 0);
	V3DControlListStoreGeneral(ctx, 0x0000, 0x00000000);

	const int X_MAX = (SCREEN_WIDTH  / TILE_SIZE);
	const int Y_MAX = (SCREEN_HEIGHT / TILE_SIZE);
	const int TILE_MAX = X_MAX * Y_MAX;
	int tile_count = 0;
	for(int y = 0; y < Y_MAX; y++) {
		for(int x = 0; x < X_MAX; x++) {
			V3DControlListRenderTileCoord(ctx, x, y);
			V3DControlListBranchToSublist(ctx, ctx->bus_addr_vc + TILE_MEM_OFFSET_BINNING_DATA + (x + y * X_MAX) * 32);
			if(tile_count == (TILE_MAX - 1) ) {
				V3DControlListStoreMsResolvedBufferAndEndFrame(ctx);
			} else {
				V3DControlListStoreMsResolvedBuffer(ctx);
			}
			tile_count++;
		}
	}

	V3DControlListNop(ctx);
	V3DSaveOffset(ctx, 1);
}

/*
	 3322 2222 2222 1111 1111 1100 0000 0000
	 1098 7654 3210 9876 5432 1098 7654 3210
	 */

void V3DControlReset(V3DContext *ctx) {
	IO_WRITE(V3D_CT0CS, V3D_CTnCS_CTRSTA);
	IO_WRITE(V3D_CT1CS, V3D_CTnCS_CTRSTA);
}

void V3DControlPresentBinning(V3DContext *ctx) {
	IO_WRITE(V3D_BFC, 1);
	IO_WRITE(V3D_CT0CA, ctx->bus_addr_vc + ctx->offset_c0_start);
	IO_WRITE(V3D_CT0EA, ctx->bus_addr_vc + ctx->offset_c0_end);
	while(IO_READ(V3D_BFC) == 0){}
}

void V3DControlPresentRendering(V3DContext *ctx) {
	IO_WRITE(V3D_RFC, 1);
	IO_WRITE(V3D_CT1CA, ctx->bus_addr_vc + ctx->offset_c1_start);
	IO_WRITE(V3D_CT1EA, ctx->bus_addr_vc + ctx->offset_c1_end);
	while(IO_READ(V3D_RFC) == 0) {}
	ctx->frame_count++;
}

void send_framebuffer(uint32_t addr, int size) {
	uint8_t *p = (uint8_t *)addr;
	for(int i = 0 ; i < size; i++) {
		uart_putc(*p++);
	}
}

void V3DAddPreVertex(V3DContext *ctx, int16_t x, int16_t y, float z, float w, float v0, float v1, float v2) {
	if(ctx == 0) return;
	volatile uint32_t xy = ((y & 0xFFFF) << 16) | (x & 0xFFFF);
	V3DWrite32(ctx,  xy);
	V3DFloat32(ctx,  z);
	V3DFloat32(ctx,  w);
	V3DFloat32(ctx, v0);
	V3DFloat32(ctx, v1);
	V3DFloat32(ctx, v2);
	
	ctx->vertex_count++;
}

static int offset = 0;
void AddVertex(V3DContext *ctx) {
	V3DClearOffsetVertex(ctx);
	V3DSetOffsetBeginVertex(ctx);
	if(offset > (640 << 4)) {
		offset = 0;
	}

	int16_t base_scale = 10;
	
	
	uint16_t w = (SCREEN_WIDTH  * 16);
	uint16_t h = (SCREEN_HEIGHT * 16);
	uint16_t x = offset;
	offset += 20;
	
		/*
	V3DAddPreVertex(ctx, x, 0    , 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DAddPreVertex(ctx, w, h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	V3DAddPreVertex(ctx, w, 0    , 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);

	V3DAddPreVertex(ctx, x, 0    , 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DAddPreVertex(ctx, x, h / 2, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	V3DAddPreVertex(ctx, w, h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	

	V3DAddPreVertex(ctx, x, h / 2, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DAddPreVertex(ctx, x, h / 1, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	V3DAddPreVertex(ctx, w, h / 1, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	
	V3DAddPreVertex(ctx, x, h / 2, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DAddPreVertex(ctx, w, h / 1, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	V3DAddPreVertex(ctx, w, h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	*/
	
	
	init_random();
	/*
	for(int j = 0 ; j < 15; j++) {
		for(int i = 0 ; i < 16; i++) {
			uint32_t x      = (i * base_scale * 2) << 4;
			uint32_t y      = (j * base_scale * 2) << 4;
			x += sintable[(7 * frame     ) % 256] * 15;
			y += sintable[(7 * frame + 64) % 256] * 15;
			uint32_t scale  = base_scale << 4;
			
			V3DAddPreVertex(ctx, x + 0x00 , y - scale, 1.0f, 1.0f, 1.0, 0.0, 0.0);
			V3DAddPreVertex(ctx, x - scale, y + scale, 1.0f, 1.0f, 0.0, 1.0, 0.0);
			V3DAddPreVertex(ctx, x + scale, y + scale, 1.0f, 1.0f, 0.0, 0.0, 1.0);
		}
	}
	*/
	uint32_t scale  = base_scale << 3;
	for(int j = 0 ; j < 24; j++) {
		for(int i = 0 ; i < 32; i++) {
			int32_t x  = (i * base_scale * 2) << 4;
			int32_t y  = (j * base_scale * 2) << 4;
			int32_t x0 = (0 << 4);
			int32_t y0 = -(base_scale << 4);
			
			int32_t x1 = -(base_scale << 4);
			int32_t y1 = +(base_scale << 4);
			
			int32_t x2 = +(base_scale << 4);
			int32_t y2 = +(base_scale << 4);
			
			x += sintable[frame & 0xFF] * 10;
			
			/*
			int32_t x0 = sintable[ (3 * frame +   0 + 0x40) & 0xFF ] * base_scale;
			int32_t y0 = sintable[ (3 * frame +   0 + 0x00) & 0xFF ] * base_scale;
			int32_t x1 = sintable[ (3 * frame +  85 + 0x40) & 0xFF ] * base_scale;
			int32_t y1 = sintable[ (3 * frame +  85 + 0x00) & 0xFF ] * base_scale;
			int32_t x2 = sintable[ (3 * frame + 170 + 0x40) & 0xFF ] * base_scale;
			int32_t y2 = sintable[ (3 * frame + 170 + 0x00) & 0xFF ] * base_scale;

			x += sintable[(2 * frame     ) % 256] * 15;
			y += sintable[(2 * frame + 64) % 256] * 15;
			*/
			V3DAddPreVertex(ctx, x + x0, y + y0, 1.0f, 1.0f, 1.0, 0.0, 0.0);
			V3DAddPreVertex(ctx, x + x1, y + y1, 1.0f, 1.0f, 0.0, 1.0, 0.0);
			V3DAddPreVertex(ctx, x + x2, y + y2, 1.0f, 1.0f, 0.0, 0.0, 1.0);
		}
	}
	
	
	V3DSetOffsetEndVertex(ctx);
}

void L2CacheAndMiscCacheClear() {
	IO_WRITE(V3D_L2CACTL, 4);
	IO_WRITE(V3D_SLCACTL, 0x0F0F0F0F);
	asm volatile("dmb");
}

void Present() {
	V3DControlPresentBinning(ctx);
	V3DControlPresentRendering(ctx);
}

void debug_output() {
	uart_debug_puts("ctx->V3D_ERRSTAT=\n", IO_READ(V3D_ERRSTAT));
	uart_debug_puts("ctx->V3D_SQRSV0  =\n", IO_READ(V3D_SQRSV0));
	uart_debug_puts("ctx->V3D_SRQCS   =\n", IO_READ(V3D_SRQCS));
	uart_debug_puts("ctx->V3D_VPACNTL =\n", IO_READ(V3D_VPACNTL));
	uart_debug_puts("ctx->V3D_DBGE    =\n",    IO_READ(V3D_DBGE));
	uart_debug_puts("ctx->V3D_FDBGO   =\n",    IO_READ(V3D_FDBGO));
	uart_debug_puts("ctx->V3D_FDBGS   =\n",    IO_READ(V3D_FDBGS));
	uart_debug_puts("ctx->V3D_BXCF    =\n",    IO_READ(V3D_BXCF));
}




volatile  uint32_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT] __attribute__((aligned(256))) ;

volatile uint32_t dmadata[123]; __attribute__((aligned(256))) ;
void DmaBlit(uint32_t dest, uint32_t src, uint32_t size) {
	usleep(10000);
	IO_WRITE(DMA_ENABLE, DMA_EN0);
	dmadata[0] = DMA_DEST_INC + DMA_SRC_INC;
	dmadata[1] = src;
	dmadata[2] = dest;
	dmadata[3] = size;

	dmadata[4] = 0;
	dmadata[5] = 0;
	IO_WRITE(DMA0_BASE + DMA_CONBLK_AD, dmadata);
	IO_WRITE(DMA0_BASE + DMA_CS, DMA_ACTIVE);
	/*
	do {
		uint32_t status = IO_READ(DMA0_BASE + DMA_CS);
		if( (status & 0x01) == 0 ) break;
	} while(1);
	*/
}

#define FLOAT_BUF_MAX  32768
volatile float M[FLOAT_BUF_MAX];


void testTriangle(void *dest) {
	mailbox_fb *fb = (mailbox_fb *)dest;
	frame = 0;
	init_render_chunk_buffer();

	V3DAlloc(ctx, fb->pointer);
	V3DLock(ctx);
	AddVertex(ctx);
	uart_debug_puts("AddVertex Done\n", 0);
	V3DControlListCreateBinning(ctx);
	uart_debug_puts("V3DControlListCreateBinning Done\n", 0);
	V3DControlListCreateRendering(ctx, fb->pointer_vc);
	uart_debug_puts("V3DControlListCreateRendering Done\n", 0);
	float x = 2.0f;
	float y = 2.5f;
	for(int i = 0 ; i < FLOAT_BUF_MAX; i++)
	{
		M[i] = M[i] * x * y;
	}
	V3DUnlock(ctx);

	while(1) {
		frame++;

		/*
		if(frame & 1) {
			off_status_led();
		} else {
			on_status_led();
		}
		*/

		V3DLock(ctx);
		AddVertex(ctx);
		Present();
		V3DUnlock(ctx);

		//DmaBlit(dest, buffer, SCREEN_WIDTH * SCREEN_HEIGHT * 4);

		/*
		uint32_t *src = buffer;
		uint32_t *d   = (uint32_t *)dest;
		for(int i = 0 ; i < 640 * 200; i++) {
			d[i] = src[i];
		}
		*/
	}
	V3DUnlock(ctx);
	uart_puts("HALT");

}

