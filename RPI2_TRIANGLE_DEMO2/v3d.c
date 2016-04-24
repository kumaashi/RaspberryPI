#include "v3d.h"

void V3DSetOffset(V3DContext *ctx, uint32_t offset) 
{
	if(ctx == 0) return;
	ctx->offset         = ctx->bus_addr + offset;
	ctx->offset_start   = ctx->bus_addr + offset;
}

void V3DSaveOffset(V3DContext *ctx, uint32_t cn) {
	if(ctx == 0) return;
	if(cn == 0) {
		ctx->offset_c0_start = ctx->offset_start;
		ctx->offset_c0_end   = ctx->offset - 1;
	}
	if(cn == 1) {
		ctx->offset_c1_start = ctx->offset_start;
		ctx->offset_c1_end   = ctx->offset - 1;
	}
}

void V3DFree(V3DContext *ctx)  
{
	if(ctx == 0) return;
	if(ctx->handle) {
		mailbox_free_memory(ctx->handle);
	}
	ctx->handle   = 0;
	ctx->bus_addr = 0;
}

void V3DAlloc(V3DContext *ctx, uint32_t pointer) 
{
	if(ctx == 0) return;
	uint32_t handle = mailbox_allocate_memory(
		TILE_MEM_ALLOC_SIZE,
		TILE_MEM_ALLOC_ALIGN,
		MAILBOX_MEM_FLAG_COHERENT | 
		MAILBOX_MEM_FLAG_DISCARDABLE |
		MAILBOX_MEM_FLAG_L1_NONALLOCATING |
		MAILBOX_MEM_FLAG_L1_NONALLOCATING
	);
	if(handle) {
		ctx->handle          = handle;
		ctx->pointer         = pointer;
		ctx->bus_addr        = 0;
		ctx->offset          = 0;
		ctx->offset_start    = 0;
		ctx->offset_c0_start = 0;
		ctx->offset_c0_end   = 0;
		ctx->offset_c1_start = 0;
		ctx->offset_c1_end   = 0;
		
		ctx->vertex_offset   = 0;
		ctx->vertex_count    = 0;
		ctx->frame_count     = 0;
	}
}

void V3DClearOffsetVertex(V3DContext *ctx) {
	if(ctx == 0) return;
	ctx->vertex_offset = 0;
	ctx->vertex_count  = 0;
}

void V3DSetOffsetBeginVertex(V3DContext *ctx) {
	if(ctx == 0) return;
	V3DSetOffset(ctx, ctx->vertex_offset + TILE_MEM_OFFSET_VERTEX_BUFFER);
}

void V3DSetOffsetEndVertex(V3DContext *ctx) {
	if(ctx == 0) return;
	ctx->vertex_offset = ctx->offset;
}


void V3DSetBusAddress(V3DContext *ctx, uint32_t bus_addr) {
	if(ctx == 0) return;
	ctx->bus_addr      = bus_addr;
	ctx->bus_addr_vc   = ArmToVc(ctx->bus_addr);
}

void V3DLock(V3DContext *ctx)  
{
	if(ctx == 0) return;
	if(ctx->handle == 0) return;
	ctx->bus_addr_vc   = mailbox_lock_memory(ctx->handle);
	ctx->bus_addr      = VcToArm(ctx->bus_addr_vc);
	
	//ctx->bus_addr    = (uint32_t)&renderchunk_buffer0[0];
	//ctx->bus_addr_vc = ArmToVc(ctx->bus_addr);
}

void V3DUnlock(V3DContext *ctx)  
{
	if(ctx == 0) return;
	if(ctx->handle == 0) return;
	mailbox_unlock_memory(ctx->handle);
	ctx->bus_addr_vc = 0;
	ctx->bus_addr = 0;
}


inline void V3DWrite32(V3DContext *ctx, uint32_t data) 
{
	volatile uint32_t num_addr = (ctx->offset);
	volatile uint8_t *addr = (uint8_t *)(num_addr);
	volatile uint8_t *d    = (uint8_t *)(&data);
	addr[0] = d[0];
	addr[1] = d[1];
	addr[2] = d[2];
	addr[3] = d[3];
	
	ctx->offset += 4;
}


inline void V3DFloat32(V3DContext *ctx, float data)  
{
	uint32_t udata    = *((uint32_t *)&data);
	uint32_t num_addr = (ctx->offset);
	volatile uint8_t *addr = (uint8_t *)(num_addr);
	volatile uint8_t *d    = (uint8_t *)(&udata);
	addr[0] = d[0];
	addr[1] = d[1];
	addr[2] = d[2];
	addr[3] = d[3];
	
	ctx->offset += 4;
}


inline void V3DWrite16(V3DContext *ctx, uint16_t data)  
{
	volatile uint8_t *addr = (uint8_t *)(ctx->offset);
	volatile uint8_t *d    = (uint8_t *)(&data);
	addr[0] = d[0];
	addr[1] = d[1];
	ctx->offset += 2;
}

inline void V3DWrite8(V3DContext *ctx, uint8_t data)  
{
	volatile uint8_t *addr = (uint8_t *)(ctx->offset);
	volatile uint8_t *d    = (uint8_t *)(&data);
	addr[0] = d[0];
	ctx->offset += 1;
}



void V3DControlSetShaderInfo(V3DContext *ctx) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_SHADER_INFO);
	V3DWrite8(ctx,   0x00);
	V3DWrite8(ctx,   6 * 4);
	V3DWrite8(ctx,   0);
	V3DWrite8(ctx,   3);
	V3DWrite32(ctx,  ctx->bus_addr + TILE_MEM_OFFSET_FSHADER);
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

void V3DControlListSetupBinning(V3DContext *ctx) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_BINNIG_CTRL_LIST);

	V3DWrite8(ctx,   TILE_CTRLLIST_BINNING_CONFIG);
	V3DWrite32(ctx,  ctx->bus_addr + TILE_MEM_OFFSET_BINNING_DATA);
	V3DWrite32(ctx,  TILE_UNIT_BYTES);
	V3DWrite32(ctx,  ctx->bus_addr + TILE_MEM_OFFSET_STATE);
	V3DWrite8(ctx,   SCREEN_WIDTH  / TILE_SIZE);
	V3DWrite8(ctx,   SCREEN_HEIGHT / TILE_SIZE);

	uint8_t cfg_binning = TILE_CTRLLIST_BINNING_AUTO;
	cfg_binning |= TILE_CTRLLIST_BINNING_MS;

	V3DWrite8(ctx,   cfg_binning);

	//Start Binning
	V3DWrite8(ctx,   TILE_CTRLLIST_START_BINNING);

	//Clip Window
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
	V3DWrite32(ctx, ctx->bus_addr + TILE_MEM_OFFSET_SHADER_INFO);
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

	V3DWrite8(ctx,  TILE_CTRLLIST_FLUSH);
	V3DWrite8(ctx,  TILE_CTRLLIST_NOP);
	V3DSaveOffset(ctx, 0);
}

inline void V3DControlListClearColor(V3DContext *ctx, uint32_t data0, uint32_t data1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_CLEAR_COLOR);
	V3DWrite32(ctx,   data0);
	V3DWrite32(ctx,   data1);
	V3DWrite32(ctx,   0);
	V3DWrite8(ctx,    0);
}

inline void V3DControlListRenderModeConfig(V3DContext *ctx, uint32_t dest, uint16_t width, uint16_t height, uint8_t cfg0, uint8_t cfg1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_RENDER_MODE_CONFIG);
	V3DWrite32(ctx,   (uint32_t)dest);
	V3DWrite16(ctx,   width);
	V3DWrite16(ctx,   height);
	V3DWrite8(ctx,    cfg0);
	V3DWrite8(ctx,    cfg1);
}

inline void V3DControlListRenderTileCoord(V3DContext *ctx, uint8_t x, uint8_t y) {
	V3DWrite8(ctx,    TILE_CTRLLIST_RENDER_TILE_COORD);
	V3DWrite8(ctx,    x);
	V3DWrite8(ctx,    y);
}

inline void V3DControlListStoreGeneral(V3DContext *ctx, uint16_t cfg0, uint32_t cfg1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_STORE_GENERAL);
	V3DWrite16(ctx,   cfg0);
	V3DWrite32(ctx,   cfg1);
}

inline void V3DControlListBranchToSublist(V3DContext *ctx, uint32_t addr) {
	V3DWrite8(ctx,  TILE_CTRLLIST_BRANCH_TO_SUBLIST);
	V3DWrite32(ctx, addr);
}

void V3DControlListSetupRendering(V3DContext *ctx, uint32_t dest) {
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
			V3DControlListBranchToSublist(ctx, ctx->bus_addr + TILE_MEM_OFFSET_BINNING_DATA + (x + y * X_MAX) * 32);
			if(tile_count == (TILE_MAX - 1) ) {
				V3DWrite8(ctx,  TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER_AND_END_FRAME);
			} else {
				V3DWrite8(ctx,  TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER);
			}
			tile_count++;
		}
	}

	V3DWrite8(ctx,  TILE_CTRLLIST_NOP);
	V3DSaveOffset(ctx, 1);
}

void V3DControlReset(V3DContext *ctx) {
	IO_WRITE(V3D_CT0CS, V3D_CTnCS_CTRSTA);
	IO_WRITE(V3D_CT1CS, V3D_CTnCS_CTRSTA);
}

void V3DControlPresentBinning(V3DContext *ctx) {
	IO_WRITE(V3D_BFC, 1);
	IO_WRITE(V3D_CT0CA, ctx->offset_c0_start);
	IO_WRITE(V3D_CT0EA, ctx->offset_c0_end);
	while(IO_READ(V3D_BFC) == 0){}
}

void V3DControlPresentRendering(V3DContext *ctx) {
	IO_WRITE(V3D_RFC, 1);
	IO_WRITE(V3D_CT1CA, ctx->offset_c1_start);
	IO_WRITE(V3D_CT1EA, ctx->offset_c1_end);
	while(IO_READ(V3D_RFC) == 0) {}
	ctx->frame_count++;
}


void V3DNVVertexAdd(V3DContext *ctx, int16_t x, int16_t y, float z, float w, float v0, float v1, float v2) {
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

