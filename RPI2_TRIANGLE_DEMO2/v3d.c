#include "v3d.h"

void V3DSetOffset(V3DContext *ctx, uint32_t offset) 
{
	if(ctx == 0) return;
	ctx->offset         = offset;
	ctx->offset_start   = offset;
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



void V3DLock(V3DContext *ctx)  
{
	if(ctx == 0) return;
	if(ctx->handle == 0) return;
	ctx->bus_addr_vc   = mailbox_lock_memory(ctx->handle);
	ctx->bus_addr      = VcToArm(ctx->bus_addr_vc);
	//if(ctx-> & 1) {
	//	ctx->bus_addr   = (uint32_t)&renderchunk_buffer0[0];
	//}
}

void V3DUpdate(V3DContext *ctx)  
{
	if(ctx == 0) return;
	if(ctx->handle == 0) return;
	//if(ctx->frame_count & 1) {
	//	ctx->bus_addr   = (uint32_t)&renderchunk_buffer0[0];
	//}
}

void V3DUnlock(V3DContext *ctx)  
{
	if(ctx == 0) return;
	if(ctx->handle == 0) return;
	mailbox_unlock_memory(ctx->handle);
	ctx->bus_addr_vc = 0;
	ctx->bus_addr = 0;
}


void V3DWrite32(V3DContext *ctx, uint32_t data) 
{
	volatile uint32_t num_addr = (ctx->bus_addr + ctx->offset);
	volatile uint8_t *addr = (uint8_t *)(num_addr);
	volatile uint8_t *d    = (uint8_t *)(&data);
	addr[0] = d[0];
	addr[1] = d[1];
	addr[2] = d[2];
	addr[3] = d[3];
	
	ctx->offset += 4;
}


void V3DFloat32(V3DContext *ctx, float data)  
{
	uint32_t udata    = *((uint32_t *)&data);
	uint32_t num_addr = (ctx->bus_addr + ctx->offset);
	volatile uint8_t *addr = (uint8_t *)(num_addr);
	volatile uint8_t *d    = (uint8_t *)(&udata);
	addr[0] = d[0];
	addr[1] = d[1];
	addr[2] = d[2];
	addr[3] = d[3];
	
	ctx->offset += 4;
}


void V3DWrite16(V3DContext *ctx, uint16_t data)  
{
	volatile uint8_t *addr = (uint8_t *)(ctx->bus_addr + ctx->offset);
	volatile uint8_t *d    = (uint8_t *)(&data);
	addr[0] = d[0];
	addr[1] = d[1];
	ctx->offset += 2;
}

void V3DWrite8(V3DContext *ctx, uint8_t data)  
{
	volatile uint8_t *addr = (uint8_t *)(ctx->bus_addr + ctx->offset);
	volatile uint8_t *d    = (uint8_t *)(&data);
	addr[0] = d[0];
	ctx->offset += 1;
}


