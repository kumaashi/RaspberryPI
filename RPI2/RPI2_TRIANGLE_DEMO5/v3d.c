#include "v3d.h"
#include "util.h"

//const uint32_t basic_shader[] = {
//	/* 0x00000000: */ 0x958efdbf, 0xd1724823, /* mov r0, vary; mov r3.8d, 0.5 */
//	/* 0x00000008: */ 0x818e7176, 0x40024821, /* fadd r0, r0, r5; mov r1, vary; sbwait */
//	/* 0x00000010: */ 0x818e7376, 0x10024862, /* fadd r1, r1, r5; mov r2, vary */
//	/* 0x00000018: */ 0x819e7540, 0x114248a3, /* fadd r2, r2, r5; mov r3.8a, r0 */
//	/* 0x00000020: */ 0x809e7009, 0x115049e3, /* nop; mov r3.8b, r1 */
//	/* 0x00000028: */ 0x809e7012, 0x116049e3, /* nop; mov r3.8c, r2 */
//	/* 0x00000030: */ 0x158e7d80, 0x100208a7, /* mov r2, vary; nop */
//	/* 0x00000038: */ 0x158e7d80, 0x100208a7, /* mov r2, vary; nop */
//	/* 0x00000040: */ 0x158e7d80, 0x100208a7, /* mov r2, vary; nop */
//	/* 0x00000048: */ 0x159e76c0, 0x30020ba7, /* mov tlbc, r3; nop; thrend */
//	/* 0x00000050: */ 0x009e7000, 0x100009e7, /* nop */
//	/* 0x00000058: */ 0x009e7000, 0x500009e7, /* nop; nop; sbdone */
//};

const uint32_t basic_shader[] = {
///* Assembled Program */
///* entry: */
///* 0x00000000: */ 0x009e7000, 0x100009e7, /* nop             ; nop */
///* 0x00000008: */ 0x009e7000, 0x400009e7, /* nop             ; nop; sbwait */
///* 0x00000010: */ 0x158e7d80, 0x10020827, /* mov  r0, vary   ; nop */
///* 0x00000018: */ 0x019e7140, 0x10020827, /* fadd r0, r0, r5 ; nop */
///* 0x00000020: */ 0x809e7000, 0x114049e3, /* nop             ; mov  r3.8a, r0 */
///* 0x00000028: */ 0x158e7d80, 0x10020827, /* mov  r0, vary   ; nop */
///* 0x00000030: */ 0x019e7140, 0x10020827, /* fadd r0, r0, r5 ; nop */
///* 0x00000038: */ 0x809e7000, 0x115049e3, /* nop             ; mov  r3.8b, r0 */
///* 0x00000040: */ 0x158e7d80, 0x10020827, /* mov  r0, vary   ; nop */
///* 0x00000048: */ 0x019e7140, 0x10020827, /* fadd r0, r0, r5 ; nop */
///* 0x00000050: */ 0x159e7000, 0x106208e7, /* mov  r3.8c, r0  ; nop */
///* 0x00000058: */ 0x158e7d80, 0x10020827, /* mov  r0, vary   ; nop */
///* 0x00000060: */ 0x019e7140, 0x10020827, /* fadd r0, r0, r5 ; nop */
///* 0x00000068: */ 0x159e7000, 0x107208e7, /* mov  r3.8d, r0  ; nop */
///* 0x00000070: */ 0x158e7d80, 0x100208a7, /* mov  r2, vary   ; nop */
///* 0x00000078: */ 0x158e7d80, 0x100208a7, /* mov  r2, vary   ; nop */
///* 0x00000080: */ 0x159e76c0, 0x30020ba7, /* mov  tlbc, r3   ; nop; thrend */
///* 0x00000088: */ 0x009e7000, 0x100009e7, /* nop */
///* 0x00000090: */ 0x009e7000, 0x500009e7, /* nop            ; nop; sbdone */
#include "temp.txt"
};

void fake_vsync(void) {
	//NEED config.txt -> fake_vsync_isr=1
	IO_WRITE(IRQ_GPU_ENABLE2, IRQ_GPU_FAKE_ISR);
	while( (IO_READ(IRQ_GPU_PENDING2) & IRQ_GPU_FAKE_ISR) == 0 );
	IO_WRITE(SMI_CS, 0);
}


V3DShaderInfoFmt shader_info_chunk[V3D_SHADER_INFO_MAX] __attribute__((aligned(4096))); 
V3DTextureInfo   texture_info_chunk[V3D_TEXTURE_MAX]    __attribute__((aligned(4096))); 
uint8_t          bindata[0x10000]                       __attribute__((aligned(4096))); 
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

void V3DSetBusAddress(V3DContext *ctx, uint32_t bus_addr) {
	if(ctx == 0) return;
	ctx->bus_addr      = bus_addr;
	ctx->bus_addr_vc   = ArmToVc((void *)ctx->bus_addr);
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


inline void V3DWrite32Align(V3DContext *ctx, uint32_t data) 
{
	/*
	*((uint32_t *)ctx->offset) = data;
	ctx->offset += 4;
	*/
	*((float *)ctx->offset) = *((float *)&data);
	ctx->offset += 4;
}


inline void V3DFloat32Align(V3DContext *ctx, float data)  
{
	*((float *)ctx->offset) = data;
	ctx->offset += 4;
}


inline void V3DWrite16(V3DContext *ctx, uint16_t data)  
{
	uint8_t *pdata = (uint8_t *)&data;
	*(uint8_t *)(ctx->offset + 0) = pdata[0];
	*(uint8_t *)(ctx->offset + 1) = pdata[1];
	ctx->offset += 2;
}

inline void V3DWrite8(V3DContext *ctx, uint8_t data)  
{
	*(uint8_t *)(ctx->offset) = data;
	ctx->offset += 1;
}


inline void V3DControlListClearColor(V3DContext *ctx, uint32_t data0, uint32_t data1) {
	V3DWrite8(ctx,    TILE_CTRLLIST_CLEAR_COLOR);
	V3DWrite32(ctx,   data0);
	V3DWrite32(ctx,   data1);
	V3DWrite8(ctx,    0xFF);
	V3DWrite8(ctx,    0xFF);
	V3DWrite8(ctx,    0xFF);
	V3DWrite8(ctx,    0);
	V3DWrite8(ctx,    0);
}

void V3DControlListSetupRendering(V3DContext *ctx) {
	V3DSetOffset(ctx, TILE_MEM_OFFSET_RENDER_CTRL_LIST);

	uint32_t col = ctx->clear_color;
	V3DControlListClearColor(ctx, col, col);
	
	
	uint8_t cfg0 = TILE_CTRLLIST_RENDER_DEFAULT;
	cfg0 |= TILE_CTRLLIST_RENDER_CFG_MS;
	
	/*
	V3DWrite8(ctx,  TILE_CTRLLIST_PRIM_TYPE);
	V3DWrite8(ctx,  0x32);
	*/
	
	V3DWrite8(ctx,  TILE_CTRLLIST_RENDER_MODE_CONFIG);
	V3DWrite32(ctx, ctx->frame_buffer_addr);
	V3DWrite16(ctx, ctx->width);
	V3DWrite16(ctx, ctx->height);
	V3DWrite8(ctx,  cfg0);
	V3DWrite8(ctx,  0x00);
	
	V3DWrite8(ctx,  TILE_CTRLLIST_RENDER_TILE_COORD);
	V3DWrite8(ctx,  0);
	V3DWrite8(ctx,  0);
	V3DWrite8(ctx,  TILE_CTRLLIST_STORE_GENERAL);
	V3DWrite8(ctx,  0x00);
	V3DWrite8(ctx,  0x00);
	V3DWrite8(ctx,  0x00);
	V3DWrite8(ctx,  0x00);
	V3DWrite8(ctx,  0x00);
	V3DWrite8(ctx,  0x00);
	

	const int X_MAX = (ctx->width  / TILE_SIZE);
	const int Y_MAX = (ctx->height / TILE_SIZE);
	const int TILE_MAX = X_MAX * Y_MAX;
	int tile_count = 0;
	for(int y = 0; y < Y_MAX; y++) {
		for(int x = 0; x < X_MAX; x++) {
			V3DWrite8(ctx,  TILE_CTRLLIST_RENDER_TILE_COORD);
			V3DWrite8(ctx,  x);
			V3DWrite8(ctx,  y);
			V3DWrite8(ctx,  TILE_CTRLLIST_BRANCH_TO_SUBLIST);
			//V3DWrite32(ctx, ctx->bus_addr + TILE_MEM_OFFSET_BINNING_DATA + (x + y * X_MAX) * 32);
			uint32_t addr = (uint32_t)&bindata[0];
			V3DWrite32(ctx, addr + (x + y * X_MAX) * 32);
			
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
	int count = 100000;
	while(IO_READ(V3D_BFC) == 0) {
		count--;
		if(count < 0) {
			uart_dump(V3D_BASE, 0x1000);
			uart_bytedump(ctx->bus_addr + TILE_MEM_OFFSET_BINNIG_CTRL_LIST, 0x100);
			break;
		}
	}
}

void V3DControlPresentRendering(V3DContext *ctx) {
	IO_WRITE(V3D_RFC, 1);
	IO_WRITE(V3D_CT1CA, ctx->offset_c1_start);
	IO_WRITE(V3D_CT1EA, ctx->offset_c1_end);
	int count = 100000;
	while(IO_READ(V3D_RFC) == 0) {
		count--;
		if(count < 0) {
			uart_dump(V3D_BASE, 0x1000);
			uart_dump(IO_READ(0x3FC00114), 0x100);
			usleep(0x1000000);
			break;
		}
	}
	ctx->frame_count++;
}


void V3DSetBlendState(V3DContext *ctx, uint32_t state) {
	ctx->blend_state = state;
}

void V3DSetClearColor(V3DContext *ctx, uint32_t col) {
	ctx->clear_color = col;
}

void V3DSetUniformAddr(V3DContext *ctx, uint32_t addr) {
	ctx->fshader_uniform_addr = addr;
}

void V3DSetDrawPrimitive(V3DContext *ctx, uint32_t vertex_addr,  uint32_t vertex_count, uint32_t type) {
	V3DShaderInfoFmt *sifmt = ctx->shader_info[ctx->shader_info_index];
	ctx->shader_info_index++;
	
	sifmt->flag_bits            = 0;
	sifmt->vertex_stride_bytes  = sizeof(V3DVertexFmt);
	sifmt->uniform_num          = 0;
	sifmt->varying_num          = V3D_VARYING_NUM;
	sifmt->fshader_code_addr    = (uint32_t)&basic_shader[0];
	sifmt->fshader_uniform_addr = ctx->fshader_uniform_addr;
	sifmt->vertex_data_addr     = ArmToVc((void *)vertex_addr);

	V3DWrite8(ctx,  TILE_CTRLLIST_NV_SHADER_PRE);
	V3DWrite32(ctx, (uint32_t)sifmt);
	V3DWrite8(ctx,  TILE_CTRLLIST_VERTEX_ARRAY_PRIM);
	V3DWrite8(ctx,  type);
	V3DWrite32(ctx, vertex_count);
	V3DWrite32(ctx, 0);
}

uint32_t V3DBeginScene(V3DContext *ctx) {
	uint32_t ret = V3D_OK;
	
	V3DSetOffset(ctx, TILE_MEM_OFFSET_BINNIG_CTRL_LIST);

	V3DWrite8(ctx,   TILE_CTRLLIST_BINNING_CONFIG);
	uint32_t addr = (uint32_t)&bindata[0];
	
	V3DWrite32(ctx,  addr);
	V3DWrite32(ctx,  TILE_UNIT_BYTES);
	V3DWrite32(ctx,  ctx->bus_addr + TILE_MEM_OFFSET_STATE);
	V3DWrite8(ctx,   ctx->width    / TILE_SIZE);
	V3DWrite8(ctx,   ctx->height   / TILE_SIZE);

	uint8_t cfg_binning = TILE_CTRLLIST_BINNING_AUTO;
	cfg_binning |= TILE_CTRLLIST_BINNING_MS;

	V3DWrite8(ctx,   cfg_binning);
	V3DWrite8(ctx,   TILE_CTRLLIST_START_BINNING);
	V3DWrite8(ctx,   TILE_CTRLLIST_CLIP_WINDOW);
	V3DWrite16(ctx,  0);
	V3DWrite16(ctx,  0);
	V3DWrite16(ctx,  ctx->width );
	V3DWrite16(ctx,  ctx->height);

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

	ctx->shader_info_index = 0;
	return ret;
}

void V3DEndScene(V3DContext *ctx) {
	V3DWrite8(ctx,  TILE_CTRLLIST_FLUSH);
	V3DWrite8(ctx,  TILE_CTRLLIST_NOP);
	V3DSaveOffset(ctx, 0);
	V3DControlListSetupRendering(ctx);
}

void V3DPresent(V3DContext *ctx) {
	V3DControlPresentBinning(ctx);
	V3DControlPresentRendering(ctx);
}

void V3DCreateContext(V3DContext *ctx, uint32_t width, uint32_t height, uint32_t frame_buffer_addr, uint32_t bus_addr) {
	memset_t(ctx, 0, sizeof(V3DContext));
	ctx->width             = width;
	ctx->height            = height;
	ctx->frame_buffer_addr = frame_buffer_addr;
	
	V3DSetBlendState(ctx, V3D_BLEND_NONE);
	V3DSetClearColor(ctx, 0x00000000);
	V3DSetBusAddress(ctx, bus_addr);
	for(int i = 0 ; i < V3D_SHADER_INFO_MAX; i++ ) {
		ctx->shader_info[i] = &shader_info_chunk[i];
	}
	for(int i = 0 ; i < V3D_TEXTURE_MAX; i++ ) {
		ctx->texture_info[i] = &texture_info_chunk[i];
	}
}


void V3DSetupTexture(V3DContext *ctx, uint32_t num, uint32_t width, uint32_t height, void *data) {
	const uint32_t mem_align = 4096;

	V3DTextureInfo *p = ctx->texture_info[num];
	p->handle = mailbox_allocate_memory(
		4 * (width * height) + mem_align, mem_align, 0);//MAILBOX_MEM_FLAG_ZERO | MAILBOX_MEM_FLAG_L1_NONALLOCATING);
	if(p->handle) {
		p->addr = mailbox_lock_memory(p->handle);
		p->uniform_param = p->addr;
		p->addr = VcToArm(p->addr);
		p->width = width;
		p->height = height;
		
		uint32_t *dest = (uint32_t  *)(p->addr + mem_align);
		uint32_t *src = (uint32_t  *)data;
		uint32_t *param = (uint32_t *)p->addr;
		param[0] = 0;
		param[0] |= p->addr + mem_align;

		param[1] = 0;
		param[1] |= ( (width ) << 8);
		param[1] |= ( (height) << 20);
		/*
		param[1] |= (1 << 7); //MAGFILTER
		param[1] |= (1 << 4); //MINFILTER
		*/
		param[2]  = 0;
		param[3]  = 0;
		
		const uint16_t *table = GetLTFormat256x256();
		for(int i = 0 ; i < width * height; i++) {
			uint32_t offset = table[i];
			dest[i] = src[offset];
		}
		
		mailbox_unlock_memory(p->handle);
	}
}

uint32_t V3DGetTextureAddr(V3DContext *ctx, uint32_t num) {
	V3DTextureInfo *p = ctx->texture_info[num];
	return p->addr;
}
