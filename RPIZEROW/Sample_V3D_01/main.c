// Raspberry Pi Zero W V3D 01(Clear Only)
// 2022 yasai kumaashi (gyaboyan@gmail.com)
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "dma.h"

#define WIDTH     640
#define HEIGHT    480
#define BUFNUM    2

void handle_hang() {
	led_init();
	uart_init();
	while(1) {
		uart_puts("HANG!!!!!!!!!\n");
		SLEEP(0x10000);
	}
}

void intr_handler() {
	uart_puts("call intr_handler\n");
}

uint32_t random() {
	static uint32_t a = 1;
	static uint32_t b = 1235666;
	static uint32_t c = 9786423;
	a += b;
	b += c;
	c += a;
	return (a >> 16);
}

//mimic posix
void *memcpy(void *dst, const void *src, size_t n) {
	uint8_t *d = (uint8_t *)dst;
	uint8_t *s = (uint8_t *)src;
	for(int i = 0; i < n; i++) {
		*d++ = *s++;
	}
	return dst;
}

void *memset(void *dst, int c, size_t n) {
	uint8_t *d = (uint8_t *)dst;
	for(int i = 0; i < n; i++) {
		*d++ = c;
	}
	return dst;
}

#define TILE_CTRLLIST_HALT                                     0
#define TILE_CTRLLIST_NOP                                      1
#define TILE_CTRLLIST_FLUSH                                    4
#define TILE_CTRLLIST_FLUSH_ALL                                5
#define TILE_CTRLLIST_START_BINNING                            6
#define TILE_CTRLLIST_BRANCH_TO_SUBLIST                        17
#define TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER                 24
#define TILE_CTRLLIST_STORE_MS_RESOLVED_BUFFER_AND_END_FRAME   25
#define TILE_CTRLLIST_STORE_GENERAL                            28
#define TILE_CTRLLIST_PRIM_INDEX_LIST                          32
#define TILE_CTRLLIST_VERTEX_ARRAY_PRIM                        33
#define TILE_CTRLLIST_PRIM_TYPE                                56
#define TILE_CTRLLIST_NV_SHADER_PRE                            65
#define TILE_CTRLLIST_NV_SHADER                                66
#define TILE_CTRLLIST_STATE                                    96
#define TILE_CTRLLIST_CLIP_WINDOW                              102
#define TILE_CTRLLIST_VIEWPORT_OFFSET                          103
#define TILE_CTRLLIST_BINNING_CONFIG                           112
#define TILE_CTRLLIST_RENDER_MODE_CONFIG                       113
#define TILE_CTRLLIST_CLEAR_COLOR                              114
#define TILE_CTRLLIST_RENDER_TILE_COORD                        115
#define TILE_SIZE                                              32

//BINNING CONFIG
typedef struct {
	uint32_t mem_addr;
	uint32_t mem_size;
	uint32_t mem_tile_array_addr;
	uint8_t tile_width;
	uint8_t tile_height;
	unsigned msaa : 1;
	unsigned color_depth_64bit : 1;
	unsigned auto_init_tile_array : 1;
	unsigned tile_init_block_size : 2;
	unsigned tile_block_size : 2;
	unsigned double_buffer : 1;
} __attribute__((__packed__)) v3d_bin_mode_config_info ;

uint8_t *v3d_set_bin_mode_config(uint8_t *p, v3d_bin_mode_config_info *info) {
	//#define TILE_CTRLLIST_BINNING_CONFIG  112
	*p = 112;
	p += 1;

	*(v3d_bin_mode_config_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//BINNING START
uint8_t *v3d_set_bin_start(uint8_t *p) {
	//#define TILE_CTRLLIST_START_BINNING  6
	*p = 6;
	p += 1;
	return p;
}

//CLIP WINDOW
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} __attribute__((__packed__)) v3d_bin_clip_window_info ;

uint8_t *v3d_set_bin_clip_window(uint8_t *p, v3d_bin_clip_window_info *info) {
	//#define TILE_CTRLLIST_CLIP_WINDOW  102
	*p = 102;
	p += 1;

	*(v3d_bin_clip_window_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//VIEWPORT OFFSET
typedef struct {
	uint16_t x;
	uint16_t y;
} __attribute__((__packed__)) v3d_bin_viewport_offset_info;

uint8_t *v3d_set_bin_viewport_offset(uint8_t *p, v3d_bin_viewport_offset_info *info) {
	//#define TILE_CTRLLIST_VIEWPORT_OFFSET 103
	*p = 103;
	p += 1;

	*(v3d_bin_viewport_offset_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//STATE CONFIG
typedef struct {
	unsigned enable_forward_facing_primitive : 1;
	unsigned enable_reverse_facing_primitive : 1;
	unsigned clockwise_primitives : 1;
	unsigned enable_depth_offset : 1;
	unsigned antialiased_points_and_lines : 1; //not supported on VC4
	unsigned coverage_read_type : 1;
	unsigned rasteriser_oversample_mode : 2;
	unsigned coverage_pipe_select : 1;
	unsigned coverage_update_mode : 2;
	unsigned coverage_read_mode : 1;
	unsigned depth_test_function : 3;
	unsigned enable_z_updates : 1;
	unsigned enable_early_z : 1;
	unsigned enable_early_z_updates : 1;
	unsigned unused : 6;
} __attribute__((__packed__)) v3d_bin_state_config_info ;
uint8_t *v3d_set_bin_state_config(uint8_t *p, v3d_bin_state_config_info *info) {
	//#define TILE_CTRLLIST_STATE  96
	*p = 96;
	p += 1;

	*(v3d_bin_state_config_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//----------------------------------------------------------------------
//CLEAR COLORS
typedef struct {
	uint32_t color0;
	uint32_t color1;
	unsigned clear_z : 24;
	unsigned clear_vg_mask : 8;
	unsigned clear_stencil : 8;
} __attribute__((__packed__)) v3d_rendering_clear_colors_info ;
uint8_t *v3d_set_rendering_clear_colors(uint8_t *p, v3d_rendering_clear_colors_info *info) {
	//#define TILE_CTRLLIST_CLEAR_COLOR 114
	*p = 114;
	p += 1;

	*(v3d_rendering_clear_colors_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//RENDERING_MODE_CONFIG
typedef struct {
	uint32_t mem_addr;
	uint16_t width;
	uint16_t height;
	unsigned msaa : 1;
	unsigned tile_buffer_64bit_color_depth : 1;
	unsigned non_hdr_frame_buffer_color_format : 2;
	unsigned decimate_mode : 2;
	unsigned memory_format : 2;
	unsigned enable_vg_mask_buffer : 1;
	unsigned select_coverrage_mode : 1;
	unsigned early_z_update_direction : 1;
	unsigned early_z_early_cov_disable_: 1;
	unsigned double_buffer : 1;
	unsigned unused : 3;
} __attribute__((__packed__)) v3d_rendering_mode_config_info;
uint8_t *v3d_set_rendering_mode_config(uint8_t *p, v3d_rendering_mode_config_info *info) {
	*p = 113;
	p += 1;

	*(v3d_rendering_mode_config_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//SHADER STATE RECORD
typedef struct {
	uint8_t flag_bits[2];

	uint8_t fs_number_of_uniforms;
	uint8_t fs_number_of_varyings;
	uint32_t fs_code_addr;
	uint32_t fs_uniform_addr;

	uint16_t vs_num_of_uniforms;
	uint8_t vs_attr_array_select_bits;
	uint8_t vs_total_attr_size;
	uint32_t vs_shader_code_addr;
	uint32_t vs_uniform_addr;

	uint16_t xs_num_of_uniforms;
	uint8_t cs_attr_array_select_bits;
	uint8_t cs_total_attr_size;
	uint32_t cs_sahder_code_addr;
	uint32_t cs_uniform_addr;
	//todo attr
} __attribute__((__packed__)) v3d_shader_state_record_info ;
uint8_t *v3d_set_shader_state_record(uint8_t *p, v3d_shader_state_record_info *addr) {
	*p = 64;
	p++;
	*(uint32_t *)p = (uint32_t)addr;
	p += sizeof(uint32_t);
	return p;
}

//rendering_tile_coordinates
typedef struct {
	uint8_t x;
	uint8_t y;
} __attribute__((__packed__)) v3d_rendering_tile_coordinates_info ;
uint8_t *v3d_set_rendering_tile_coordinates(uint8_t *p, v3d_rendering_tile_coordinates_info *info) {
	//#define TILE_CTRLLIST_RENDER_TILE_COORD 115
	*p = 115;
	p++;

	*(v3d_rendering_tile_coordinates_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_halt(uint8_t *p) {
	*p = 0; //HALT
	p++;
	return p;
}

uint8_t *v3d_set_nop(uint8_t *p) {
	*p = 1; //NOP
	p++;
	return p;
}

uint8_t *v3d_set_flush(uint8_t *p) {
	*p = 4; //FLUSH
	p++;
	return p;
}

uint8_t *v3d_set_flush_all(uint8_t *p) {
	*p = 5; //FLUSH_ALL
	p++;
	return p;
}

uint8_t *v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer(uint8_t *p) {
	*p = 24; //NOP
	p++;
	return p;
}

uint8_t *v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer_eof(uint8_t *p) {
	*p = 25; //NOP
	p++;
	return p;
}

void v3d_set_bin_exec_addr(uint32_t start, uint32_t end) {
	*V3D_CT0CA = start;
	*V3D_CT0EA = end;
}


void v3d_set_rendering_exec_addr(uint32_t start, uint32_t end) {
	*V3D_CT1CA = start;
	*V3D_CT1EA = end;
}

void v3d_wait_rendering_exec(uint32_t timeout) {
	uint32_t count = 0;
	InvalidateData();
	while(*V3D_RFC == 0) {
		count++;
		if(count > timeout) {
			uart_puts("v3d_wait_rendering_exec TIMEOUT\n");
			uart_debug_puts("V3D_PCS=", *V3D_PCS);
			uart_debug_puts("V3D_RFC=", *V3D_RFC);
			uart_debug_puts("V3D_CT0CS=", *V3D_CT0CS);
			uart_debug_puts("V3D_CT1CS=", *V3D_CT1CS);
			break;
		}
	}
}

void v3d_reset() {
	*V3D_CT0CS = V3D_CTnCS_CTRSTA;
	*V3D_CT1CS = V3D_CTnCS_CTRSTA;
}

extern uint32_t v3dcmdlist0[];
extern uint32_t v3dcmdlist1[];
extern uint32_t v3dbinbuf0[];
extern uint32_t v3dbinbuf1[];

extern void fake_vsync(void);
int notmain(void) {
	led_init();
	uart_init();
	mailbox_qpu_enable();

	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	mailbox_fb *fb = mailbox_fb_get();

	//check framebuffer
	{
		uint32_t *screen0 = (uint32_t *)fb->pointer;
		uint32_t *screen1 = screen0 + (WIDTH * HEIGHT);
		for(int y = 0 ; y < HEIGHT; y++) {
			for(int x = 0 ; x < WIDTH; x++) {
				screen0[x + y * WIDTH] = x ^ y;
				screen1[x + y * WIDTH] = screen0[x + y * WIDTH] << 8;
			}
		}
	}

	int count = 0;

	//check qpu status
	uart_debug_puts("V3D_IDENT0:", *V3D_IDENT0);
	uart_debug_puts("V3D_IDENT1:", *V3D_IDENT1);
	uart_debug_puts("V3D_IDENT2:", *V3D_IDENT2);
	uart_debug_puts("V3D_PCS=", *V3D_PCS);

	uart_debug_puts("v3d_bin_clip_window_info        =", sizeof(v3d_bin_clip_window_info));
	uart_debug_puts("v3d_bin_mode_config_info        =", sizeof(v3d_bin_mode_config_info));
	uart_debug_puts("v3d_bin_state_config_info       =", sizeof(v3d_bin_state_config_info));
	uart_debug_puts("v3d_bin_viewport_offset_info    =", sizeof(v3d_bin_viewport_offset_info));
	uart_debug_puts("v3d_rendering_clear_colors_info =", sizeof(v3d_rendering_clear_colors_info));
	uart_debug_puts("v3d_rendering_mode_config_info  =", sizeof(v3d_rendering_mode_config_info));

	uart_debug_puts("v3dcmdlist0=", (uint32_t)v3dcmdlist0);
	uart_debug_puts("v3dcmdlist1=", (uint32_t)v3dcmdlist1);
	uart_debug_puts("v3dbinbuf0 =", (uint32_t)v3dbinbuf0);
	uart_debug_puts("v3dbinbuf1 =", (uint32_t)v3dbinbuf1);

	while(1) {
		led_set(count & 1);

		uint32_t *frame_buffer_addr = (uint32_t *)fb->pointer;
		frame_buffer_addr += WIDTH * HEIGHT * (count & 1);
		//uint8_t *bcl = (uint8_t *)v3dcmdlist0;
		uint8_t *rcl = (uint8_t *)v3dcmdlist1;
		v3d_reset();

		{
			v3d_rendering_clear_colors_info info = {};
			memset(&info, 0, sizeof(info));
			info.color0 = (count & 1) ? 0xFFFFFFFF : 0xFFFF00FF;
			info.color1 = 0xFFFFFFFF;
			rcl = v3d_set_rendering_clear_colors(rcl, &info);
		}
		{
			v3d_rendering_mode_config_info info = {};
			memset(&info, 0, sizeof(info));
			info.mem_addr = (uint32_t)frame_buffer_addr;
			info.width = WIDTH;
			info.height = HEIGHT;
			info.msaa = 1;
			info.non_hdr_frame_buffer_color_format = 1;
			rcl = v3d_set_rendering_mode_config(rcl, &info);
		}

		rcl = v3d_set_nop(rcl);
		rcl = v3d_set_nop(rcl);
		rcl = v3d_set_nop(rcl);
		for(int y = 0 ; y < HEIGHT / TILE_MAX; y++) {
			for(int x = 0 ; x < WIDTH / TILE_MAX; x++) {
				v3d_rendering_tile_coordinates_info info;
				memset(&info, 0, sizeof(info));
				info.x = x;
				info.y = y;
				rcl = v3d_set_rendering_tile_coordinates(rcl, &info);
				rcl = v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer(rcl);
			}
		}
		rcl = v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer_eof(rcl);
		rcl = v3d_set_nop(rcl);

		uart_debug_puts("rcl start=", (uint32_t)v3dcmdlist1);
		uart_debug_puts("rcl end  =", (uint32_t)rcl);
		v3d_set_rendering_exec_addr((uint32_t)v3dcmdlist1, (uint32_t)rcl);
		v3d_wait_rendering_exec(0x1000000);

		fake_vsync();
		mailbox_fb_flip(count & 1);
		count++;
	}
	return(0);
}

