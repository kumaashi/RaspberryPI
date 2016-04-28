#include      "testdriver.h"
#include      "util.h"
#include      "v3d.h"
#include      <math.h>

#define       VERTEX_MAX          8192

V3DContext    context                 __attribute__((aligned(256)));
V3DVertexFmt  vertexdata[VERTEX_MAX]  __attribute__((aligned(256)));
V3DContext   *ctx          = &context;
int           vertex_count = 0;
int           Width        = 0;
int           Height       = 0;
int32_t       frame        = 0;


void ResetVertexFormat() {
	vertex_count = 0;
}

uint32_t GetVertexFormatCount() {
	return vertex_count;
}

void AddVertexFormat(int16_t x, int16_t y, float z, float w, float r, float g, float b) {
	
	V3DVertexFmt *p = &vertexdata[vertex_count];
	p->x = x;
	p->y = y;
	p->z = z;
	p->w = w;
	p->r = r;
	p->g = g;
	p->b = b;
	vertex_count++;
}

static int32_t offset       = 0;
static int32_t float_offset = 0.0f;
void UpdateVertex() {
	ResetVertexFormat();
	offset             += 1 << 3;
	float_offset       += 0.01;
	int16_t base_scale = 20;
	
	uint16_t w  = (Width  * 16);
	uint16_t h  = (Height * 16);
	uint16_t x0 = FIXED(256) + (int32_t)(FSIN(frame * 100) * 300);
	uint16_t x1 = FIXED(256) + (int32_t)(FCOS(frame * 100) * 300);
	
	if(1)
	{
		AddVertexFormat( x0, 0    , 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
		AddVertexFormat( w,  h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
		AddVertexFormat( w,  0    , 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);

		AddVertexFormat( x0, 0    , 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
		AddVertexFormat( x0, h / 2, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
		AddVertexFormat( w,  h  / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
		
		AddVertexFormat( x1, h / 2, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
		AddVertexFormat( x1, h / 1, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);
		AddVertexFormat( w,  h / 1, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
		
		AddVertexFormat( x1, h / 2, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f);
		AddVertexFormat( w,  h / 1, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f);
		AddVertexFormat( w,  h / 2, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f);

	}
	
	if(1)
	{
		init_random();
		
		int angle = FIXED(frame);
		int32_t x0 = ((int32_t)(FCOS(angle + 2730 * 0) * (float)base_scale)) << 4;
		int32_t y0 = ((int32_t)(FSIN(angle + 2730 * 0) * (float)base_scale)) << 4;
		int32_t x1 = ((int32_t)(FCOS(angle + 2730 * 1) * (float)base_scale)) << 4;
		int32_t y1 = ((int32_t)(FSIN(angle + 2730 * 1) * (float)base_scale)) << 4;
		int32_t x2 = ((int32_t)(FCOS(angle + 2730 * 2) * (float)base_scale)) << 4;
		int32_t y2 = ((int32_t)(FSIN(angle + 2730 * 2) * (float)base_scale)) << 4;
		for(int j = 0 ; j < 24; j++) {
			int32_t y  = FIXED(j * base_scale);
			for(int i = 0 ; i < 32; i++) {
				int32_t x  = FIXED(i * base_scale);
				x = (x + offset) % (640 << 4);
				AddVertexFormat( x + x0, y + y0, 1.0f, 1.0f, 1.0, 0.0, 0.0);
				AddVertexFormat( x + x1, y + y1, 1.0f, 1.0f, 0.0, 1.0, 0.0);
				AddVertexFormat( x + x2, y + y2, 1.0f, 1.0f, 0.0, 0.0, 1.0);
			}
		}
	}

}


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


void testTriangle(mailbox_fb *fb) {
	Width = fb->width;
	Height = fb->height;
	V3DCreateContext(ctx, fb->width, fb->height, (uint32_t)fb->pointer, 1 << 22);

	if(V3DBeginScene(ctx) == V3D_OK) {
		ResetVertexFormat();
		UpdateVertex();
		V3DSetClearColor(ctx, 0xFFFFFFFF);
		V3DSetDrawPrimitive(ctx, (uint32_t)vertexdata, GetVertexFormatCount(), V3D_VERTEX_ARRAY_PRIM_TRIANGLES);
		V3DEndScene(ctx);
		V3DPresent(ctx);
	}
	while(1) {
		frame++;
		show_fps();
		
			ResetVertexFormat();
			UpdateVertex();
			V3DPresent(ctx);
		
		//uart_debug_puts("V3D_CT0CA=\n", IO_READ(V3D_CT0CA));
		//uart_debug_puts("V3D_CT0EA=\n", IO_READ(V3D_CT0EA));
		//uart_debug_puts("V3D_CT1CA=\n", IO_READ(V3D_CT1CA));
		//uart_debug_puts("V3D_CT1EA=\n", IO_READ(V3D_CT1EA));
		
		
		//uart_bytedump((uint32_t)TILE_MEM_OFFSET_BINNIG_CTRL_LIST + (1 << 22), 0x40);

		//usleep(10000);
	}
	uart_puts("HALT");

}

