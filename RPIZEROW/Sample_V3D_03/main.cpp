// For Raspberry Pi Zero W 
// 2022 yasai kumaashi (gyaboyan@gmail.com)

extern "C" {
#include "common.h"
#include "heap.h"
#include "tmath.h"
#include "uart.h"
#include "hw.h"
#include "led.h"
#include "mailbox.h"
#include "dma.h"
#include "irq.h"
#include "v3d.h"
#include "tmath.h"
#include "vecmath.h"
} //EXTERN C

#define WIDTH             640
#define HEIGHT            480
#define BUFNUM            2
#define TILE_SIZE         32

const uint32_t test_copy_shader[] = {
#include "copy.h"
};

struct vertex_format_clip {
	float xc, yc, zc, wc;
	uint16_t xs;
	uint16_t ys;
	float zs;
	float inv_wc;
	float r, g, b;
} __attribute__((__packed__));

void ndc_to_screen(vertex_format_clip *fmt, vec4 & vdc, int width, int height)
{
	fmt->xc = vdc.v[0] / vdc.v[3];
	fmt->yc = vdc.v[1] / vdc.v[3];
	fmt->zc = vdc.v[2] / vdc.v[3];
	fmt->wc = vdc.v[3] / vdc.v[3];
	fmt->zs = fmt->zc * 0.5f + 0.5f;
	fmt->inv_wc = 1.0f / fmt->wc;
	float xx = ((fmt->xc + 1.0f) * width  * 0.5f);
	float yy = ((fmt->yc + 1.0f) * height * 0.5f);
	fmt->xs = (int16_t)(xx * 16.0f); //12.4
	fmt->ys = (int16_t)(yy * 16.0f); //12.4
}


int maincpp(void) {
	uint32_t *v3d_cmdlist0;
	uint32_t *v3d_cmdlist1;
	uint32_t *v3d_bin_buffer0;
	uint32_t *v3d_bin_buffer1;
	uint32_t *v3d_shader_state_record;
	uint32_t *v3d_shader_code;
	uint32_t *v3d_vertex_data;

	led_init();
	uart_init();
	heap_init(0x400000);

	int heap_index = 0;
	v3d_cmdlist0 = heap_get(heap_index++);
	v3d_cmdlist1 = heap_get(heap_index++);
	v3d_bin_buffer0 = heap_get(heap_index++);
	v3d_bin_buffer1 = heap_get(heap_index++);
	v3d_shader_state_record = heap_get(heap_index++);
	v3d_shader_code = heap_get(heap_index++);
	v3d_vertex_data = heap_get(heap_index++);

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

	int count = 0;
	float fcount = 0.0;
	float fcount_t = 0.0;
	while(1) {
		matrix ident = matrix_ident();
		matrix view = matrix_lookat(
				0.0f, 0.0f, -7.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);
		matrix proj = matrix_projection(45.0f, (float)HEIGHT / (float)WIDTH, 0.125, 10.0f);
		matrix trans = matrix_rotationf(
			fcount_t * -0.89,
			fcount_t * 0.5,
			fcount_t * -0.7);
		matrix view_proj = matrix_mult(proj, view);
		view_proj = matrix_mult(view_proj, trans);

		fcount += 0.01666666f;
		fcount_t += 0.01666666f;
		if(fcount > 1.0f)
			fcount -= 1.0f;
		float offset = fcount * WIDTH * 16;

		static const vec4 cube_vertex[] = {
			// front
			{-1, -1, 1, 1}, //0
			{1, -1, 1, 1},  //1
			{1, 1, 1, 1},   //2
			{-1, 1, 1, 1},  //3
			// back
			{-1, -1, -1, 1}, //4
			{1, -1, -1, 1},  //5
			{1, 1, -1, 1},   //6
			{-1, 1, -1, 1},  //7
		};

		static const int indexs[] = {
			// front
			0, 1, 2, 2, 3, 0, //0
			// top
			3, 2, 6, 6, 7, 3, //1
			// back
			7, 6, 5, 5, 4, 7, //2
			// bottom
			4, 5, 1, 1, 0, 4, //3
			// left
			4, 0, 3, 3, 7, 4, //4
			// right
			1, 5, 6, 6, 2, 1, //5
		};


		//bar vertex for detect to tearing.
		{
			vertex_format_clip *vfmt = (vertex_format_clip *)v3d_vertex_data;
			vec4 color[4] = {
				{1,0,0,1},
				{1,1,0,1},
				{1,0,1,1},
				{0,1,1,1},
			};
			for(int i = 0 ; i < 36; i++) {
				int idx = indexs[i];
				vec4 v = cube_vertex[idx];
				vec4 vdc = matrix_vec4_mult(v, view_proj);
				ndc_to_screen(vfmt, vdc, WIDTH, HEIGHT);

				vfmt->r = color[i % 4].v[0];
				vfmt->g = color[i % 4].v[1];
				vfmt->b = color[i % 4].v[2];
				vfmt++;
			}
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
			//info.shaded_vertex_data_stride = 6 * sizeof(uint32_t);
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
			info.primitive_type = V3D_VERTEX_ARRAY_PRIM_TRIANGLES;
			info.length = 36;
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
			v3d_rendering_clear_colors_info info = {};
			memset(&info, 0, sizeof(info));
			if(count & 1) {
				info.color0 = 0xFFFF0000;
				info.color1 = 0xFFFF0000;
			} else {
				info.color0 = 0xFF00FFFF;
				info.color1 = 0xFF00FFFF;
			}
			info.color0 = 0xFF001111;
			info.color1 = 0xFF001111;
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

			//Enable this memory when using it as a texture. TMU interprets T-format.
			//info.memory_format = 1;
			rcl = v3d_set_rendering_mode_config(rcl, &info);
		}

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
		rcl = v3d_set_flush(rcl);
		rcl = v3d_increment_sem(rcl);
		rcl = v3d_wait_sem(rcl);
		rcl = v3d_set_nop(rcl);

		//submit cl
		v3d_set_bin_exec_addr((uint32_t)v3d_cmdlist0, (uint32_t)bcl);
		v3d_wait_bin_exec(0x1000000);

		//blit
		v3d_set_rendering_exec_addr((uint32_t)v3d_cmdlist1, (uint32_t)rcl);
		v3d_wait_rendering_exec(0x1000000);

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
