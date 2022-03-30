// For Raspberry Pi Zero W V3D
// 2022 yasai kumaashi (gyaboyan@gmail.com)
extern "C" {
#include "common.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "dma.h"
#include "irq.h"
}

#define WIDTH             640
#define HEIGHT            480
#define BUFNUM            2
#define TILE_SIZE         32
#define V3D_CTnCS_CTRUN   (0x20)
#define V3D_CTnCS_CTRSTA  (1 << 15)

static void v3d_debug_print() {
	uart_puts("---------------------------------------------------------------------\n");
	uart_debug_puts("V3D_CT0CA   =", *V3D_CT0CA);
	uart_debug_puts("V3D_CT0EA   =", *V3D_CT0EA);
	uart_debug_puts("V3D_CT1CA   =", *V3D_CT1CA);
	uart_debug_puts("V3D_CT1EA   =", *V3D_CT1EA);
	uart_debug_puts("V3D_ERRSTAT =", *V3D_ERRSTAT);
	uart_debug_puts("V3D_DBGE    =", *V3D_DBGE);
	uart_debug_puts("V3D_PCS     =", *V3D_PCS);
	uart_debug_puts("V3D_BFC     =", *V3D_BFC);
	uart_debug_puts("V3D_RFC     =", *V3D_RFC);
	uart_debug_puts("V3D_CT0CS   =", *V3D_CT0CS);
	uart_debug_puts("V3D_CT1CS   =", *V3D_CT1CS);
	uart_puts("---------------------------------------------------------------------\n");
}

//HALT
uint8_t *v3d_set_halt(uint8_t *p) {
	*p = 0;
	p++;
	return p;
}

//NOP
uint8_t *v3d_set_nop(uint8_t *p) {
	*p = 1;
	p++;
	return p;
}

//FLUSH
uint8_t *v3d_set_flush(uint8_t *p) {
	*p = 4;
	p++;
	return p;
}

//FLUSH ALL
uint8_t *v3d_set_flush_all(uint8_t *p) {
	*p = 5;
	p++;
	return p;
}

//START TILE BINNING
uint8_t *v3d_set_bin_start_tile_binning(uint8_t *p) {
	*p = 6;
	p++;

	return p;
}

//SET BRANCH TO SUBLIST
typedef struct {
	uint32_t addr;
} __attribute__((__packed__)) v3d_branch_to_sublist_info;
uint8_t *v3d_set_branch_to_sublist(uint8_t *p, v3d_branch_to_sublist_info *info) {
	*p = 17;
	p++;

	*(v3d_branch_to_sublist_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//SET BRANCH TO SUBLIST
uint8_t *v3d_set_branch_to_sublist_test(uint8_t *p, uint32_t addr) {
	*p = 17;
	p++;

	//*(uint32_t *)p = addr;
	memcpy(p, &addr, 4);
	p += 4;
	return p;
}

//RENDERING STORE MULTI SAMPLE RESOLVED TILE COLOR BUFFER
uint8_t *v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer(uint8_t *p) {
	*p = 24;

	p++;
	return p;
}

//RENDERING STORE MULTI SAMPLE RESOLVED TILE COLOR BUFFER EOF
uint8_t *v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer_eof(uint8_t *p) {
	*p = 25; 
	p++;
	return p;
}

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
	*p = 112;
	p += 1;

	*(v3d_bin_mode_config_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//BINNING START
uint8_t *v3d_set_bin_start(uint8_t *p) {
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
	*p = 114;
	p += 1;

	*(v3d_rendering_clear_colors_info *)p = *info;
	p += sizeof(*info);
	return p;
}

//STORE TILE BUFFER GENERAL
typedef struct {
	unsigned buffer_to_store                                     :  3;
	unsigned unused0                                             :  1;
	unsigned format                                              :  2;
	unsigned mode                                                :  2;
	unsigned pixel_color_format                                  :  2;
	unsigned unused1                                             :  2;
	unsigned disable_double_buffer_swap_in_double_buffer_mode    :  1;
	unsigned disable_color_buffer_clear_on_store_dump            :  1;
	unsigned disable_z_stencil_buffer_clear_on_store_dump        :  1;
	unsigned disable_vg_mask_buffer_clear_on_store_dump          :  1;
	unsigned disable_color_buffer_dump                           :  1;
	unsigned disable_z_stencil_buffer_dump                       :  1;
	unsigned disable_vg_mask_buffer_dump                         :  1;
	unsigned last_tile_of_frame                                  :  1;
	unsigned mem_addr                                            : 28;
} __attribute__((__packed__)) v3d_rendering_store_tile_buffer_general_info;
uint8_t *v3d_set_rendering_store_tile_buffer_general(uint8_t *p, v3d_rendering_store_tile_buffer_general_info *info) {
	*p = 28;
	p += 1;

	*(v3d_rendering_store_tile_buffer_general_info *)p = *info;
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

	uint32_t value = (uint32_t)addr;
	value >>= 4;
	memcpy(p, &value, 4);

	p += sizeof(uint32_t);
	return p;
}

//NV SHADER STATE RECORD
typedef struct {
	uint8_t flag_bits;
	uint8_t shaded_vertex_data_stride;
	uint8_t fs_number_of_uniforms;
	uint8_t fs_number_of_varyings;
	uint32_t fs_code_addr;
	uint32_t fs_uniform_addr;
	uint32_t shaded_vertex_data_addr;
} __attribute__((__packed__)) v3d_nv_shader_state_record_info ;
uint8_t *v3d_set_nv_shader_state_record(uint8_t *p, uint32_t addr) {
	*p = 65;
	p++;

	uint32_t value = addr;
	memcpy(p, &value, 4);
	//*(uint32_t *)p = addr;

	p += sizeof(uint32_t);
	return p;
}

enum {
	V3D_VERTEX_ARRAY_PRIM_POINTS = 0,
	V3D_VERTEX_ARRAY_PRIM_LINES,
	V3D_VERTEX_ARRAY_PRIM_LINE_LOOP,
	V3D_VERTEX_ARRAY_PRIM_LINE_STRIP,
	V3D_VERTEX_ARRAY_PRIM_TRIANGLES,
	V3D_VERTEX_ARRAY_PRIM_TRIANGLE_STRIP,
	V3D_VERTEX_ARRAY_PRIM_MAX,
};

//DRAW PRIM
typedef struct {
	uint8_t primitive_type;
	uint32_t length;
	uint32_t index_of_first_vertex;
} __attribute__((__packed__)) v3d_vertex_array_prim_info;
uint8_t *v3d_set_vertex_array_prim(uint8_t *p, v3d_vertex_array_prim_info *info) {
	*p = 33;
	p++;

	*(v3d_vertex_array_prim_info *)p = *info;

	p += sizeof(*info);
	return p;
}


//RENDERING_TILE_COORDINATES
typedef struct {
	uint8_t x;
	uint8_t y;
} __attribute__((__packed__)) v3d_rendering_tile_coordinates_info ;
uint8_t *v3d_set_rendering_tile_coordinates(uint8_t *p, v3d_rendering_tile_coordinates_info *info) {
	*p = 115;
	p++;

	*(v3d_rendering_tile_coordinates_info *)p = *info;
	p += sizeof(*info);
	return p;
}

void v3d_set_bin_exec_addr(uint32_t start, uint32_t end) {
	//uart_debug_puts("binexec start=", start);
	//uart_debug_puts("binexec end  =", end);
	*V3D_CT0CA = start;
	*V3D_CT0EA = end;
}

void v3d_set_rendering_exec_addr(uint32_t start, uint32_t end) {
	//uart_debug_puts("renderingexec start=", start);
	//uart_debug_puts("renderingexec end  =", end);
	*V3D_CT1CA = start;
	*V3D_CT1EA = end;
}

void v3d_reset() {
	*V3D_CT0CS = 0;
	*V3D_CT1CS = 0;
	*V3D_CT0CS = V3D_CTnCS_CTRSTA;
	*V3D_CT1CS = V3D_CTnCS_CTRSTA;
	*V3D_BFC = 0;
	*V3D_RFC = 0;
	*V3D_PCS = 0;
}

void v3d_wait_bin_exec(uint32_t timeout) {
	uint32_t count = 0;
	while(*V3D_BFC == 0) {
		count++;
		if(count > timeout) {
			uart_puts("v3d_wait_binning_exec TIMEOUT\n");
			v3d_debug_print();
			break;
		}
	}
	//uart_puts("OK : v3d_wait_binning_exec\n");
}

void v3d_wait_rendering_exec(uint32_t timeout) {
	uint32_t count = 0;
	while(*V3D_RFC == 0) {
		count++;
		if(count > timeout) {
			uart_puts("v3d_wait_rendering_exec TIMEOUT\n");
			v3d_debug_print();
			break;
		}
	}
	//uart_puts("OK : v3d_wait_rendering_exec\n");
}

extern uint32_t v3d_cmdlist0[];
extern uint32_t v3d_cmdlist1[];
extern uint32_t v3d_bin_buffer0[];
extern uint32_t v3d_bin_buffer1[];

extern uint32_t v3d_shader_state_record[];
extern uint32_t v3d_shader_code[];
extern uint32_t v3d_vertex_data[];

const uint32_t test_copy_shader[] = {
#include "copy.h"
};


//https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}


int maincpp(void) {
	led_init();
	uart_init();
	mailbox_qpu_enable();

	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	mailbox_fb *fb = mailbox_fb_get();

	//fillup xor checker board for reasonable to tile.
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

	//check qpu status and misc
	uart_debug_puts("V3D_IDENT0      =", *V3D_IDENT0);
	uart_debug_puts("V3D_IDENT1      =", *V3D_IDENT1);
	uart_debug_puts("V3D_IDENT2      =", *V3D_IDENT2);
	uart_debug_puts("fb->pointer     =", fb->pointer);


	//setup test vertex
	int index =0 ;
	float *fv = (float *)v3d_vertex_data;
	struct vertex_format_clip {
		float xc, yc, zc, wc;
		int16_t xs;
		int16_t ys;
		float zs;
		float inv_wc;
		float r, g, b;
	} __attribute__((__packed__));

	int count = 0;
	float fcount = 0.0;
	while(1) {
		fcount += 0.01666666f * 0.25;
		if(fcount > 1.0f)
			fcount -= 1.0f;
		float offset = fcount * WIDTH * 16;

		//bar vertex for detect to tearing.
		{
			vertex_format_clip *v = (vertex_format_clip *)v3d_vertex_data;
			const float BarWidth = 32.0f;

			//v0
			v->xc = -1.000f;
			v->yc =  1.000f;
			v->zc =  1.000f;
			v->wc =  1.000f;

			v->xs =  0 * 16 + offset;
			v->ys =  HEIGHT * 16;
			v->zs =  1.000f;
			v->inv_wc = 1.0f / v->wc;
			v->r = 0.0;
			v->g = 0.0;
			v->b = 1.0;
			v++;

			//v1
			v->xc = -1.000f;
			v->yc = -1.000f;
			v->zc =  1.000f;
			v->wc =  1.000f;

			v->xs =  0 * 16 + offset;
			v->ys =  0 * 16;
			v->zs =  0.300f;
			v->inv_wc = 1.0f / v->wc;
			v->r = 1.0;
			v->g = 0.0;
			v->b = 0.0;
			v++;

			//v2
			v->xc =  1.000f;
			v->yc =  1.000f;
			v->zc =  1.000f;
			v->wc =  1.000f;

			v->xs =  BarWidth * 16 + offset;
			v->ys =  HEIGHT * 16;
			v->zs =  1.000f;
			v->inv_wc = 1.0f / v->wc;
			v->r = 0.0;
			v->g = 1.0;
			v->b = 1.0;
			v++;

			//v3
			v->xc =  1.000f;
			v->yc = -1.000f;
			v->zc =  1.000f;
			v->wc =  1.000f;

			v->xs =  BarWidth * 16 + offset;
			v->ys =  0 * 16;
			v->zs =  1.0f;
			v->inv_wc = 1.0f / v->wc;
			v->r = 0.0;
			v->g = 1.0;
			v->b = 0.0;
			v++;


		}
		led_set(count & 1);

		const int tile_w = WIDTH / TILE_SIZE;
		const int tile_h = HEIGHT / TILE_SIZE;
		const int tile_inc_size = 1;

		uint32_t *frame_buffer_addr = (uint32_t *)fb->pointer;
		frame_buffer_addr += WIDTH * HEIGHT * (count & 1);
		v3d_reset();

		//PREP BINNING CONTROL LIST
		uint8_t *bcl = (uint8_t *)v3d_cmdlist0;
		uint32_t binning_addr = (uint32_t)v3d_bin_buffer0;
		{
			v3d_bin_mode_config_info info = {};
			memset(&info, 0, sizeof(info));
			info.mem_addr = binning_addr;
			info.mem_size = 0x20000; //todo how to calc of size when outside binner
			info.mem_tile_array_addr = (uint32_t)v3d_bin_buffer1;
			info.tile_width = WIDTH / TILE_SIZE;
			info.tile_height = HEIGHT / TILE_SIZE;
			info.msaa = 1;
			info.auto_init_tile_array = 1;
			bcl = v3d_set_bin_mode_config(bcl, &info);
		}

		bcl = v3d_set_bin_start_tile_binning(bcl);
		{
			v3d_bin_clip_window_info info = {};
			memset(&info, 0, sizeof(info));
			info.x = 0;
			info.y = 0;
			info.w = WIDTH;
			info.h = HEIGHT;
			bcl = v3d_set_bin_clip_window(bcl, &info);
		}

		//CONFIG
		{
			v3d_bin_state_config_info info = {};
			memset(&info, 0, sizeof(info));

			info.rasteriser_oversample_mode = 1; //x4
			info.enable_forward_facing_primitive = 1;
			info.enable_reverse_facing_primitive = 1;
			info.enable_early_z_updates = 1;
			bcl = v3d_set_bin_state_config(bcl, &info);
		}


		{
			v3d_bin_viewport_offset_info info = {};
			memset(&info, 0, sizeof(info));

			bcl = v3d_set_bin_viewport_offset(bcl, &info);
		}

		//draw start
		{
			memcpy(v3d_shader_code, test_copy_shader, sizeof(test_copy_shader));

			v3d_nv_shader_state_record_info info;
			memset(&info, 0, sizeof(info));

			//enable clipping
			info.flag_bits = (1 << 3) | (1 << 2);

			//clip.xyzw, xsys, zs, inv_w, rgb
			info.shaded_vertex_data_stride = 10 * sizeof(uint32_t);
			info.fs_number_of_uniforms = 0;
			info.fs_number_of_varyings = 3; //rgb
			info.fs_code_addr = (uint32_t)v3d_shader_code;
			info.fs_uniform_addr = 0;
			info.shaded_vertex_data_addr = (uint32_t)v3d_vertex_data;

			memcpy(v3d_shader_state_record, &info, sizeof(info));
			bcl = v3d_set_nv_shader_state_record(bcl, (uint32_t)v3d_shader_state_record);
		}

		//DRAW!
		{
			v3d_vertex_array_prim_info info;
			memset(&info, 0, sizeof(info));
			info.primitive_type = V3D_VERTEX_ARRAY_PRIM_TRIANGLE_STRIP;
			info.length = 4;
			info.index_of_first_vertex = 0;
			bcl = v3d_set_vertex_array_prim(bcl, &info);
		}
		//draw end

		{
			bcl = v3d_set_flush(bcl);
			bcl = v3d_set_nop(bcl);
		}

		//PREP RENDERING CONTROL LIST
		uint8_t *rcl = (uint8_t *)v3d_cmdlist1;

		{
			v3d_rendering_mode_config_info info = {};
			memset(&info, 0, sizeof(info));
			info.mem_addr = (uint32_t)frame_buffer_addr;
			info.width = WIDTH;
			info.height = HEIGHT;
			info.msaa = 1;
			info.non_hdr_frame_buffer_color_format = 1;

			//Enable this memory when using it as a texture. TMU interprets T-format.
			//info.memory_format = 1;
			rcl = v3d_set_rendering_mode_config(rcl, &info);
		}
		/*
		{
			v3d_rendering_clear_colors_info info = {};
			memset(&info, 0, sizeof(info));
			if(count & 1) {
				info.color0 = 0xFFFF0000;
				info.color1 = 0xFFFF0000;
			} else {
				info.color0 = 0xFF00FFFF;
				info.color1 = 0xFF00FFFF;
			}
				info.color0 = 0xFF00FFFF;
				info.color1 = 0xFF00FFFF;
			rcl = v3d_set_rendering_clear_colors(rcl, &info);
		}
		*/

		{
			v3d_rendering_tile_coordinates_info info = {};
			memset(&info, 0, sizeof(info));
			info.x = 0;
			info.y = 0;
			rcl = v3d_set_rendering_tile_coordinates(rcl, &info);
		}
		{
			v3d_rendering_store_tile_buffer_general_info info = {};
			memset(&info, 0, sizeof(info));
			rcl = v3d_set_rendering_store_tile_buffer_general(rcl, &info);
		}

		for(int y = 0 ; y < tile_h; y += tile_inc_size) {
			for(int x = 0 ; x < tile_w; x += tile_inc_size) {
				v3d_rendering_tile_coordinates_info info = {};
				memset(&info, 0, sizeof(info));
				info.x = x;
				info.y = y;
				rcl = v3d_set_rendering_tile_coordinates(rcl, &info);
				{
					int offset = ((y * tile_w + x) * 32);
					v3d_branch_to_sublist_info info = {};
					info.addr = binning_addr + offset;
					rcl = v3d_set_branch_to_sublist(rcl, &info);
				}
				if(x == (tile_w - 1) && y == (tile_h - 1)) {
					rcl = v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer_eof(rcl);
				} else {
					rcl = v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer(rcl);
				}
			}
		}
		rcl = v3d_set_nop(rcl);

		//submit cl
		v3d_set_bin_exec_addr((uint32_t)v3d_cmdlist0, (uint32_t)bcl);
		v3d_wait_bin_exec(0x1000000);

		//blit
		v3d_set_rendering_exec_addr((uint32_t)v3d_cmdlist1, (uint32_t)rcl);
		v3d_wait_rendering_exec(0x1000000);

#if 0
		int wait_loop = 1000000;
		while( *V3D_PCS & 0x4 && wait_loop) {
			if((wait_loop % 1000) == 0)
				v3d_debug_print();
			wait_loop--;
		}
		if(wait_loop == 0) {
			uart_puts("-----------------------------------------------------------\n");
			uart_puts("T I M E O U T ! ! !\n");
			uart_puts("-----------------------------------------------------------\n");
		}

		//DEBUG
		if((count % 33) == 0) {
			//uart_debug_puts("v3d_cmdlist0=",(uint32_t)v3d_cmdlist0);
			//uart_debug_puts("v3d_cmdlist0=",(uint32_t)bcl);
			//uart_debug_puts("v3d_cmdlist0=",(uint32_t)v3d_cmdlist1);
			//uart_debug_puts("v3d_cmdlist0=",(uint32_t)rcl);
			//uart_dump((uint32_t)v3d_vertex_data, 0x80);
			//uart_dump((uint32_t)v3d_shader_code, 0x80);
			v3d_debug_print();
		}
#endif
		fake_vsync();
		mailbox_fb_flip( (count) & 1);
		count++;

		//uart_debug_puts("show fps raw =", show_fps_raw().frames);
	}
	return(0);
}


extern "C" {

	int notmain() {
		maincpp();
		return 0;
	}

}
