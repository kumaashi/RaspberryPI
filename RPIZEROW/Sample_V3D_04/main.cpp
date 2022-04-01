// For Raspberry Pi Zero W V3D test
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

//roughrand
struct frand {
	int32_t a, b, c;
	void reset() {
		a = 1;
		b = 1234578;
		c = 78918673;
	}
	int32_t get() {
		a += b;
		b += c;
		c += a;
		return (a >> 16);
	}
	float getfloat() {
		return (float)(get()) / (float)0x7FFF;
	}
};

const uint32_t fs_normal_z[] = {
	#include "fs_normal_z.h"
};

const uint32_t fs_add[] = {
	#include "fs_add.h"
};

struct vertex_format_nv {
	int16_t xs;
	int16_t ys;
	float zs;
	float inv_wc;
	float r, g, b;
} __attribute__((__packed__));

int ndc_to_screen(vertex_format_nv *fmt, vec4 & vdc, int width, int height)
{
	float xc = vdc.v[0] / vdc.v[3];
	float yc = vdc.v[1] / vdc.v[3];
	float zc = vdc.v[2] / vdc.v[3];
	float wc = vdc.v[3] / vdc.v[3];
	
	fmt->zs = (zc * 0.5f + 0.5f);
	fmt->inv_wc = 1.0f / wc;
	float xx = ((xc + 1.0f) * width  * 0.5f);
	float yy = ((yc + 1.0f) * height * 0.5f);
	//fmt->xs = (int16_t)(xx * 16.0f); //12.4
	//fmt->ys = (int16_t)(yy * 16.0f); //12.4
	int32_t xs = (int16_t)(xx * 16.0f); //12.4
	int32_t ys = (int16_t)(yy * 16.0f); //12.4
	if(xs < -16384 || xs > 16384)
		return 1;
	if(ys < -16384 || ys > 16384)
		return 1;
	fmt->xs = (int16_t)(xx * 16.0f); //12.4
	fmt->ys = (int16_t)(yy * 16.0f); //12.4
	return 0;
}

int calc_matrix(vertex_format_nv *vfmt, int mesh_count, uint32_t count, float fcount_t) {
	int processed_vertex = 0;
	uint32_t time_matrix_start  = get_systime_ms();
	frand rnd;
	rnd.reset();
	//Calc view and proj.
	float posradius = 20.0f;
	matrix ident = matrix_ident();
	matrix view = matrix_lookat(
			tcos(fcount_t * 0.3) * posradius,
			-tsin(fcount_t * 0.1) * posradius,
			tsin(fcount_t * 0.3) * posradius,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
	matrix proj = matrix_projection(90.0f, (float)HEIGHT / (float)WIDTH, 0.125, 64.0f);
	matrix view_proj = matrix_mult(proj, view);

	//---------------------------------------------------------------------------
	// Only T&L
	//---------------------------------------------------------------------------
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

	float radius = 13.0f;
	for(int i = 0 ; i < mesh_count; i++) {
		vec4 color[4] = {
			{1,0,0,1},
			{1,1,0,1},
			{1,0,1,1},
			{0,1,1,1},
		};
		matrix rot;
		matrix rot2;
		matrix trans;
		matrix tmp = view_proj;

		matrix_rotationf2(
			rot,
			rnd.getfloat() + fcount_t * -0.789,
			rnd.getfloat() + fcount_t *  0.555,
			rnd.getfloat() + fcount_t * -0.872);
		matrix_rotationf2(
			rot2,
			rnd.getfloat() + fcount_t * -1.789,
			rnd.getfloat() + fcount_t *  2.555,
			rnd.getfloat() + fcount_t * -3.872);
		matrix_translate2(
			trans,
			rnd.getfloat() * radius,
			rnd.getfloat() * radius,
			rnd.getfloat() * radius);
		
		trans = matrix_mult(trans, rot2);
		//for test
		if(count & 0x1000)
			tmp = matrix_mult(tmp, rot);

		tmp = matrix_mult(tmp, trans);
		for(int i = 0 ; i < 12; i++) {
			int reject = 0;
			for(int ti = 0 ; ti < 3; ti++) {
				int idx = indexs[i * 3 + ti];
				vec4 v = cube_vertex[idx];
				vec4 vdc;
				matrix_vec4_mult2(vdc, v, tmp);
				int ret = ndc_to_screen(&vfmt[ti], vdc, WIDTH, HEIGHT);
				if(ret) {
					reject = 1;
					break;
				}
				int color_index = i * 3 + ti;
				vfmt[ti].r = color[(color_index >> 2) % 4].v[0];
				vfmt[ti].g = color[(color_index >> 2) % 4].v[1];
				vfmt[ti].b = color[(color_index >> 2) % 4].v[2];
			}

			if(reject == 0) {
				processed_vertex += 3;
				vfmt += 3;
			}
		}
	}
	uint32_t time_matrix_end  = get_systime_ms();
	//uart_debug_puts("calc matrix time : ", time_matrix_end - time_matrix_start);
	return processed_vertex;
}

int maincpp(void) {
	uint32_t *v3d_cmdlist0;
	uint32_t *v3d_cmdlist1;
	uint32_t *v3d_bin_buffer0;
	uint32_t *v3d_bin_buffer1;
	uint32_t *v3d_shader_state_record;
	uint32_t *v3d_shader_code_normal_z;
	uint32_t *v3d_shader_code_add;
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
	v3d_shader_code_normal_z = heap_get(heap_index++);
	v3d_shader_code_add = heap_get(heap_index++);
	v3d_vertex_data = heap_get(heap_index++);
	memcpy(v3d_shader_code_normal_z, fs_normal_z, sizeof(fs_normal_z));
	memcpy(v3d_shader_code_add, fs_add, sizeof(fs_add));

	mailbox_qpu_enable();
	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	mailbox_fb *fb = mailbox_fb_get();

	//fillup xor checker board for reasonable to init tile.
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

	int count = 0;
	float fcount = 0.0;
	float fcount_t = 0.0;
	
	//--------------------------------------------------------------------
	// main loop
	//--------------------------------------------------------------------
	while(1) {
		led_set(count & 1);
		
		uint32_t time_start = get_systime_ms();

		//calc object matrix and vertex
		int mesh_count = 192;
		int processed_vertex = calc_matrix((vertex_format_nv *)v3d_vertex_data, mesh_count, count, fcount_t);

		const int tile_w = WIDTH / TILE_SIZE;
		const int tile_h = HEIGHT / TILE_SIZE;
		const int tile_inc_size = 1;

		uint32_t *frame_buffer_addr = (uint32_t *)fb->pointer;
		frame_buffer_addr += WIDTH * HEIGHT * (count & 1);
		v3d_reset();

		//PREP BINNING CONTROL LIST
		uint8_t *bcl = (uint8_t *)v3d_cmdlist0;

		//PREP RENDERING CONTROL LIST
		uint8_t *rcl = (uint8_t *)v3d_cmdlist1;

		uint32_t binning_addr = (uint32_t)ArmToVc(v3d_bin_buffer0);

		//-----------------------------------------------------------------------
		//BINNING
		//-----------------------------------------------------------------------
		{
			v3d_bin_mode_config_info info = {};
			memset(&info, 0, sizeof(info));
			info.mem_addr = binning_addr;
			info.mem_size = HEAP_SIZE;
			info.mem_tile_array_addr = (uint32_t)ArmToVc(v3d_bin_buffer1);
			info.tile_width = WIDTH / TILE_SIZE;
			info.tile_height = HEIGHT / TILE_SIZE;
			info.msaa = 1;
			info.auto_init_tile_array = 1;
			bcl = v3d_set_bin_mode_config(bcl, &info);
		}

		//START BINNING
		bcl = v3d_set_bin_start_tile_binning(bcl);
		
		//CLIP WINDOW
		{
			v3d_bin_clip_window_info info = {};
			memset(&info, 0, sizeof(info));
			info.w = WIDTH ;
			info.h = HEIGHT;
			bcl = v3d_set_bin_clip_window(bcl, &info);
		}
		
		//VIEWPORT OFFSET
		{
			v3d_bin_viewport_offset_info info = {};
			memset(&info, 0, sizeof(info));
			bcl = v3d_set_bin_viewport_offset(bcl, &info);
		}

		//CONFIG
		{
			v3d_bin_state_config_info info = {};
			memset(&info, 0, sizeof(info));

			info.rasteriser_oversample_mode = 1; //x4
			info.clockwise_primitives = 1;
			info.enable_forward_facing_primitive = 1;
			info.depth_test_function = 3; //LE
			info.enable_z_updates = 1;
			info.enable_early_z = 1;
			info.enable_early_z_updates = 1;
			bcl = v3d_set_bin_state_config(bcl, &info);
		}

		//draw start
		{
			v3d_nv_shader_state_record_info info;
			memset(&info, 0, sizeof(info));

			//xy, z, w, rgb
			info.shaded_vertex_data_stride = 6 * sizeof(uint32_t);
			info.fs_number_of_uniforms = 0;
			info.fs_number_of_varyings = 3; //rgb
			info.fs_code_addr = (uint32_t)ArmToVc(
				(count & 0x400) ?
					v3d_shader_code_add :
					v3d_shader_code_normal_z
			);
			info.fs_uniform_addr = 0;
			info.shaded_vertex_data_addr = (uint32_t)ArmToVc(v3d_vertex_data);

			memcpy(v3d_shader_state_record, &info, sizeof(info));
			bcl = v3d_set_nv_shader_state_record(bcl, (uint32_t)v3d_shader_state_record);
		}

		//DRAW
		{
			v3d_vertex_array_prim_info info;
			memset(&info, 0, sizeof(info));
			info.primitive_type = V3D_VERTEX_ARRAY_PRIM_TRIANGLES;
			info.length = processed_vertex;
			info.index_of_first_vertex = 0;
			bcl = v3d_set_vertex_array_prim(bcl, &info);
		}
		{
			bcl = v3d_set_flush(bcl);
			bcl = v3d_set_nop(bcl);
		}
		//BINNING END


		//-----------------------------------------------------------------------
		//RENDERING
		//-----------------------------------------------------------------------
		//CLEAR
		{
			v3d_rendering_clear_colors_info info = {};
			memset(&info, 0, sizeof(info));
			info.color0 = 0xFF000511;
			info.color1 = 0xFF000511;
			info.clear_z = 0xFFFFFF;
			info.clear_vg_mask = 0xFF;
			rcl = v3d_set_rendering_clear_colors(rcl, &info);
		}

		{
			v3d_rendering_mode_config_info info = {};
			memset(&info, 0, sizeof(info));
			info.mem_addr = (uint32_t)ArmToVc(frame_buffer_addr);
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
		rcl = v3d_set_nop(rcl);
		

		//submit binning
		uint32_t time_bin_start  = get_systime_ms();
		v3d_set_bin_exec_addr((uint32_t)ArmToVc(v3d_cmdlist0), (uint32_t)ArmToVc(bcl));
		v3d_wait_bin_exec(0x1000000);
		uint32_t time_bin_end  = get_systime_ms();
		//uart_debug_puts("bin time : ", time_bin_end - time_bin_start);

		//submit rendering
		uint32_t time_rcl_start  = get_systime_ms();
		v3d_set_rendering_exec_addr((uint32_t)ArmToVc(v3d_cmdlist1), (uint32_t)ArmToVc(rcl));
		v3d_wait_rendering_exec(0x1000000);
		uint32_t time_rcl_end  = get_systime_ms();
		//uart_debug_puts("rc time : ", time_rcl_end - time_rcl_start);

		//vsync (todo make the blitter with using DMA when trigger irq)
		fake_vsync();
		
		//Flip
		mailbox_fb_flip( (count) & 1);

		uint32_t time_end = get_systime_ms();
		//uart_debug_puts("all time : ", time_end - time_start);
		//uart_puts("-----------------------------------------------------------\n");

		count++;
		fcount += 0.01666666f;
		fcount_t += 0.01666666f;
		if(fcount > 1.0f)
			fcount -= 1.0f;
	}
	return(0);
}


extern "C" {
	int notmain() {
		maincpp();
		return 0;
	}
}
