#include "util.h"
#include "v3d.h"
#include <math.h>

V3DContext context  __attribute__((aligned(256)));
V3DContext *ctx = &context;
volatile int32_t frame = 0;


static int32_t offset = 0;
static int32_t float_offset = 0.0f;
void AddVertex(V3DContext *ctx) {
	V3DClearOffsetVertex(ctx);
	V3DSetOffsetBeginVertex(ctx);

	offset += 1 << 4;
	float_offset += 0.01;

	int16_t base_scale = 20;
	
	uint16_t w = (SCREEN_WIDTH  * 16);
	uint16_t h = (SCREEN_HEIGHT * 16);
	//uint16_t x = offset + (uint32_t)(FSIN(frame) * 100);
	uint16_t x0 = FIXED(256) + (int32_t)(FSIN(frame * 100) * 100);
	uint16_t x1 = FIXED(256) + (int32_t)(FCOS(frame * 100) * 100);
	
	V3DNVVertexAdd(ctx, x0, 0    , 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DNVVertexAdd(ctx, w, h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	V3DNVVertexAdd(ctx, w, 0    , 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);

	V3DNVVertexAdd(ctx, x0, 0    , 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DNVVertexAdd(ctx, x0, h / 2, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	V3DNVVertexAdd(ctx, w, h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	

	V3DNVVertexAdd(ctx, x1, h / 2, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DNVVertexAdd(ctx, x1, h / 1, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	V3DNVVertexAdd(ctx, w, h / 1, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	
	V3DNVVertexAdd(ctx, x1, h / 2, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
	V3DNVVertexAdd(ctx, w, h / 1, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
	V3DNVVertexAdd(ctx, w, h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
	
	{
		init_random();

		int angle = FIXED(frame) * 7;;
		int32_t x0 = (int32_t)(FCOS(angle + 2730 * 0) * 12.0f) * base_scale;
		int32_t y0 = (int32_t)(FSIN(angle + 2730 * 0) * 12.0f) * base_scale;
		int32_t x1 = (int32_t)(FCOS(angle + 2730 * 1) * 12.0f) * base_scale;
		int32_t y1 = (int32_t)(FSIN(angle + 2730 * 1) * 12.0f) * base_scale;
		int32_t x2 = (int32_t)(FCOS(angle + 2730 * 2) * 12.0f) * base_scale;
		int32_t y2 = (int32_t)(FSIN(angle + 2730 * 2) * 12.0f) * base_scale;
		for(int j = 0 ; j < 12; j++) {
			for(int i = 0 ; i < 16; i++) {
				int32_t x  = FIXED(2 * i * base_scale);
				int32_t y  = FIXED(2 * j * base_scale);
				
				x = (x + offset) % (640 << 4);
				
				V3DNVVertexAdd(ctx, x + x0, y + y0, 1.0f, 1.0f, 1.0, 0.0, 0.0);
				V3DNVVertexAdd(ctx, x + x1, y + y1, 1.0f, 1.0f, 0.0, 1.0, 0.0);
				V3DNVVertexAdd(ctx, x + x2, y + y2, 1.0f, 1.0f, 0.0, 0.0, 1.0);
			}
		}
	}

	V3DSetOffsetEndVertex(ctx);
}




#define FLOAT_BUF_MAX  32768
static uint32_t      last    = 0;
static uint32_t      frames  = 0;
void show_fps() {
  uint32_t           current = get_systime_ms();
  frames++;
  if(1000 <=  (current - last) ) {
      float dt = (float)(current - last) / 1000.0f;
      float fps = (float)frames / dt;
      last = current;
      frames = 0;
      uart_debug_puts("FPS : \n", (uint32_t)fps);
  }
}

void testTriangle(void *dest) {
	mailbox_fb *fb = (mailbox_fb *)dest;
	frame = 0;
	init_render_chunk_buffer();
	V3DSetBusAddress(ctx, 1 << 22);
	AddVertex(ctx);
	V3DControlListSetupBinning(ctx);
	V3DControlListSetupRendering(ctx, fb->pointer);
	V3DControlSetFragmentShader(ctx);
	V3DControlSetShaderInfo(ctx);
	while(1) {
		frame++;
		show_fps();
		AddVertex(ctx);
		V3DControlPresentBinning(ctx);
		V3DControlPresentRendering(ctx);
	}
	uart_puts("HALT");

}

