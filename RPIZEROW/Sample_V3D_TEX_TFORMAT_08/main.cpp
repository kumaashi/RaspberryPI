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
#include "binning_heap.h"
#include "v3dx.h"
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

const uint32_t fs_copy_shader[] = {
	#include "fs_copy_texture.h"
};

const uint32_t fs_normal_z[] = {
	#include "fs_normal_z.h"
};

const uint32_t fs_normal_texture_z[] = {
	#include "fs_normal_texture_z.h"
};

const uint32_t fs_add[] = {
	#include "fs_add.h"
};

const uint32_t fs_depth[] = {
	#include "fs_depth.h"
};

const uint32_t nan_32bit_raw[] = {
	#include "nan.h"
};

struct vertex_format_nv {
	int16_t xs;
	int16_t ys;
	float zs;
	float inv_wc;
	float u, v;
	float r, g, b;
} __attribute__((__packed__));

int ndc_to_screen(vertex_format_nv *fmt, vec4 & vdc, int width, int height)
{
	fmt->inv_wc = 1.0f / vdc.v[3];
	float xc = vdc.v[0] * fmt->inv_wc;
	float yc = vdc.v[1] * fmt->inv_wc;
	float zc = vdc.v[2] * fmt->inv_wc;
	float wc = vdc.v[3] * fmt->inv_wc;
	fmt->zs = (zc * 0.5f + 0.5f);

	float xx = ((xc + 1.0f) * width  * 0.5f);
	float yy = ((yc + 1.0f) * height * 0.5f);
	int32_t xs = (int16_t)(xx * 16.0f); //12.4
	int32_t ys = (int16_t)(yy * 16.0f); //12.4

	/*
	if(xs < -16384 || xs > 16384)
		return 1;
	if(ys < -16384 || ys > 16384)
		return 1;
	*/

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
	float posradius = 13.0f;
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
		{-1, -1,  1, 1}, //0
		{ 1, -1,  1, 1}, //1
		{ 1,  1,  1, 1}, //2
		{-1,  1,  1, 1}, //3

		// back
		{-1, -1, -1, 1}, //4
		{ 1, -1, -1, 1}, //5
		{ 1,  1, -1, 1}, //6
		{-1,  1, -1, 1}, //7
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

	float radius = 7.0f;
	for(int i = 0 ; i < mesh_count; i++) {
		vec4 color[6] = {
			{0,0,0,1}, // {-1, -1}
			{1,0,0,1}, // { 1, -1}
			{1,1,0,1}, // { 1,  1}

			{1,1,0,1}, // { 1,  1}
			{0,1,1,1}, // {-1,  1}
			{0,0,0,1}, // {-1, -1}
		};
		matrix rot;
		matrix rot2;
		matrix trans;
		matrix scale;
		matrix tmp = view_proj;

		matrix_rotationf2(
			rot,
			rnd.getfloat() + fcount_t * -0.789 * 0.01,
			rnd.getfloat() + fcount_t *  0.555 * 0.01,
			rnd.getfloat() + fcount_t * -0.872 * 0.01);
		matrix_rotationf2(
			rot2,
			rnd.getfloat() + fcount_t * -1.789 * 0.01,
			rnd.getfloat() + fcount_t *  2.555 * 0.01,
			rnd.getfloat() + fcount_t * -3.872 * 0.01);
		float key = ((float)i * 2) * 3.141592 / (float)mesh_count;
		matrix_translate2(
			trans,
			 tcos(key * 2.2 + fcount_t * 0.75) * radius,
			 tsin(key * 3.1 + fcount_t * 0.75) * radius,
			-tcos(key * 5.3 + fcount_t * 0.75) * radius);

		matrix_scalling2(scale, 0.2, 1, 1);
		rot = matrix_mult(rot2, scale);
		trans = matrix_mult(trans, rot);

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
				vfmt[ti].u = color[color_index % 6].v[0];
				vfmt[ti].v = color[color_index % 6].v[1];

				vfmt[ti].r = 0.2 * color[color_index % 6].v[0];
				vfmt[ti].g = 0.8 * color[color_index % 6].v[1];
				vfmt[ti].b = 0.9 * color[color_index % 6].v[2];
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

extern "C" {
void v3dx_convert_texture_to_tformat(v3d_texture_param & tex);
}

void v3dx_convert_texture_to_tformat(v3d_texture_param & tex) {

	static int is_init = 0;
	static uint8_t *bcl = NULL;
	static uint8_t *rcl = NULL;
	static uint32_t *binning_heap0 = NULL;
	static uint32_t *binning_heap1 = NULL;
	static uint32_t *copy_shader = NULL;
	static vertex_format_nv *vfmt = NULL;
	static v3d_texture_param *ptex = NULL;
	static uint32_t *pbuffer = NULL;
	static v3d_nv_shader_state_record_info *precord = NULL;
	//if(is_init == 0)
	{
		uart_puts("v3dx_convert_texture_to_tformat : init\n");
		is_init = 1;
		bcl = (uint8_t *)heap4k_get();
		rcl = (uint8_t *)heap4k_get();
		binning_heap0 = (uint32_t *)heap4k_get();
		binning_heap1 = (uint32_t *)heap4k_get();
		copy_shader = (uint32_t *)heap4k_get();
		vfmt = (vertex_format_nv *)heap4k_get();
		ptex = (v3d_texture_param *)heap4k_get();
		precord = (v3d_nv_shader_state_record_info *)heap4k_get();

		//max 2048 x 2048 x hibit -> 16M or something.
		pbuffer = (uint32_t *)heap_get();

		memcpy(copy_shader, fs_copy_shader, sizeof(fs_copy_shader));
		static const vec4 plane_index[] = {
			{-1, -1, 0, 1}, //0
			{ 1, -1, 0, 1}, //1
			{ 1,  1, 0, 1}, //2
			{-1,  1, 0, 1}, //3
		};

		static const int indexs[] = {
			// front
			0, 1, 2, 2, 3, 0, //0
			1, 5, 6, 6, 2, 1, //5
		};
		static vec4 color[6] = {
			{0,0,0,1}, // {-1, -1}
			{1,0,0,1}, // { 1, -1}
			{1,1,0,1}, // { 1,  1}

			{1,1,0,1}, // { 1,  1}
			{0,1,1,1}, // {-1,  1}
			{0,0,0,1}, // {-1, -1}
		};
		for(int i = 0 ; i < 2; i++) {
			for(int ti = 0 ; ti < 3; ti++) {
				int vertex_index = i * 3 + ti;
				int idx = indexs[i * 3 + ti];
				vec4 v = plane_index[idx];
				ndc_to_screen(&vfmt[vertex_index], v, tex.width, tex.height);
				vfmt[vertex_index].u = v.v[0] * 0.5f + 0.5;
				vfmt[vertex_index].v = v.v[1] * 0.5f + 0.5;
				vfmt[vertex_index].r = 1.0f;
				vfmt[vertex_index].g = 1.0f;
				vfmt[vertex_index].b = 1.0f;
			}
		}
		uart_puts("v3dx_convert_texture_to_tformat : init done\n");
	}
	const int is_tformat = 1;
	uint32_t texbase = v3dx_get_texture_param_baseaddr(tex);
	v3dx_render_target rt = v3dx_create_render_target(texbase, (uint32_t)binning_heap0, (uint32_t)binning_heap1, tex.width, tex.height, is_tformat);

	//temp //type4 = 1
	*ptex = v3dx_create_texture_param((uint32_t)pbuffer, tex.width, tex.height, (void *)texbase);

	//create cl
	v3dx_cl_record rec;
	rec.bcl = bcl;
	rec.rcl = rcl;

	v3d_reset();

	rec.bcl = v3d_set_bin_start_tile_binning(rec.bcl);
	v3dx_clear_render_target(rec, rt, 0xFF001155);
	v3dx_set_render_target(rec, rt, TILE_SIZE);

	//CONFIG
	{
		v3d_bin_state_config_info info = {};
		memset(&info, 0, sizeof(info));
		info.rasteriser_oversample_mode = 1; //x4
		info.clockwise_primitives = 0;
		info.enable_forward_facing_primitive = 1;
		rec.bcl = v3d_set_bin_state_config(rec.bcl, &info);
	}

	//set shader
	{
		v3d_nv_shader_state_record_info info;
		memset(&info, 0, sizeof(info));

		//xy, z, w, [u, v, r, g, b]
		info.shaded_vertex_data_stride = 8 * sizeof(uint32_t);
		info.fs_number_of_uniforms = 1; //t0
		info.fs_number_of_varyings = 5; //uv rgb
		info.fs_code_addr = (uint32_t)ArmToVc(copy_shader);
		info.fs_uniform_addr = (uint32_t)ptex;
		info.shaded_vertex_data_addr = (uint32_t)ArmToVc((void *)vfmt);
		memcpy((void *)precord, &info, sizeof(info));
		rec.bcl = v3d_set_nv_shader_state_record(rec.bcl, (uint32_t)precord);
	}

	//DRAW
	{
		v3d_vertex_array_prim_info info;
		memset(&info, 0, sizeof(info));
		info.primitive_type = V3D_VERTEX_ARRAY_PRIM_TRIANGLES;
		info.length = 6;
		info.index_of_first_vertex = 0;
		rec.bcl = v3d_set_vertex_array_prim(rec.bcl, &info);
		rec.bcl = v3d_set_flush(rec.bcl);
		rec.bcl = v3d_set_nop(rec.bcl);
	}

	//submit binning
	v3d_set_bin_exec_addr((uint32_t)bcl, (uint32_t)rec.bcl);
	v3d_wait_bin_exec(0x1000000);
	v3d_set_rendering_exec_addr((uint32_t)rcl, (uint32_t)rec.rcl);
	v3d_wait_rendering_exec(0x1000000);

	//promotion to tformat.
	tex.type4 = 0;
}

int maincpp(void) {
	struct v3d_cl_context {
		uint32_t bcl_head;
		uint32_t rcl_head;
		uint32_t bcl_tail;
		uint32_t rcl_tail;
		uint32_t binbuf0;
		uint32_t binbuf1;
		uint32_t vertex_buffer;
		uint32_t shader_state_record;
	};
	v3d_cl_context cl_ctx[BUFNUM];

	const int SHADER_ARRAY_MAX = 4;
	uint32_t *v3d_shader_code_array[SHADER_ARRAY_MAX];

	led_init();
	uart_init();
	heap_init(0x400000);
	binning_heap_init(0x1000000);

	for(int i = 0 ; i < BUFNUM; i++) {
		cl_ctx[i].bcl_head = (uint32_t)ArmToVc((void *)heap_get());
		cl_ctx[i].rcl_head = (uint32_t)ArmToVc((void *)heap_get());
		cl_ctx[i].binbuf0 = (uint32_t)ArmToVc((void *)heap_get());
		cl_ctx[i].binbuf1 = (uint32_t)ArmToVc((void *)heap_get());
		cl_ctx[i].bcl_tail = cl_ctx[i].bcl_head;
		cl_ctx[i].rcl_tail = cl_ctx[i].rcl_head;
		cl_ctx[i].vertex_buffer = (uint32_t)heap_get();
		cl_ctx[i].shader_state_record = (uint32_t)heap4k_get();
	}

	//copy to heap of shaders
	int shader_count = 0;
	int shader_index = 0;
	for(int i = 0; i < SHADER_ARRAY_MAX; i++)
		v3d_shader_code_array[i] = heap4k_get();
	memcpy(v3d_shader_code_array[0], fs_normal_z, sizeof(fs_normal_z));
	memcpy(v3d_shader_code_array[1], fs_normal_texture_z, sizeof(fs_normal_texture_z));
	memcpy(v3d_shader_code_array[2], fs_add, sizeof(fs_add));
	memcpy(v3d_shader_code_array[3], fs_depth, sizeof(fs_depth));

	//init gpu resources
	mailbox_qpu_enable();
	mailbox_fb_init(WIDTH, HEIGHT, BUFNUM);
	mailbox_fb *fb = mailbox_fb_get();


	//Create Framebuffer
	v3dx_render_target framebuffer[BUFNUM];
	for(int i = 0 ; i < BUFNUM; i++) {
		uint32_t *fbaddr = (uint32_t *)fb->pointer;
		fbaddr += WIDTH * HEIGHT * i;
		framebuffer[i] = v3dx_create_render_target(
			(uint32_t)fbaddr, (uint32_t)binning_heap_get(), (uint32_t)binning_heap_get(), WIDTH, HEIGHT, 0);
	}
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

	//-------------------------------------------------------------------------
	//Create Texture
	//-------------------------------------------------------------------------
	uint32_t texture_data = (uint32_t)heap_get();
	v3d_texture_param *texture_param = (v3d_texture_param *)heap4k_get();
	*texture_param = v3dx_create_texture_param(texture_data, 256, 256, (void *)nan_32bit_raw);

	v3dx_convert_texture_to_tformat(*texture_param);

	int count = 0;
	float fcount = 0.0;
	float fcount_t = 0.0;
	
	//--------------------------------------------------------------------
	// main loop
	//--------------------------------------------------------------------
	while(1) {
		led_set(count & 1);
		//uart_puts("START------------------------------------------------------------------------------\n");

		int frame_rendering = (count + 0) % BUFNUM;
		int frame_binning   = (count + 1) % BUFNUM;
		v3dx_cl_record rec;
		{
			uint8_t *bcl = (uint8_t *)cl_ctx[frame_binning].bcl_head;
			uint8_t *rcl = (uint8_t *)cl_ctx[frame_binning].rcl_head;
			rec.bcl = bcl;
			rec.rcl = rcl;
		}
		
		//calc object matrix and vertex
		int mesh_count = 128;
		
		vertex_format_nv *vertex_buffer = (vertex_format_nv *)cl_ctx[frame_binning].vertex_buffer;
		int processed_vertex = calc_matrix(vertex_buffer, mesh_count, count, fcount_t);


		//start rendering
		v3dx_render_target &rtfb = framebuffer[frame_binning];
		v3d_reset();

		//PREP RENDERING CONTROL LIST
		if(cl_ctx[frame_rendering].rcl_tail != cl_ctx[frame_rendering].rcl_head) {
			v3d_set_rendering_exec_addr(
				(uint32_t)cl_ctx[frame_rendering].rcl_head,
				(uint32_t)cl_ctx[frame_rendering].rcl_tail);
		}

		rec.bcl = v3d_set_bin_start_tile_binning(rec.bcl);
		v3dx_clear_render_target(rec, rtfb, 0xFF000511);
		v3dx_set_render_target(rec, rtfb, TILE_SIZE);

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
			rec.bcl = v3d_set_bin_state_config(rec.bcl, &info);
		}

		//set shader
		{
			v3d_nv_shader_state_record_info info;
			memset(&info, 0, sizeof(info));

			//xy, z, w, [u, v, r, g, b]
			//info.flag_bits = 1 << 2;
			info.shaded_vertex_data_stride = 8 * sizeof(uint32_t);
			info.fs_number_of_uniforms = 1; //t0
			info.fs_number_of_varyings = 5; //uv rgb

			shader_count++;
			if(shader_count > 60 * 10) {
				shader_count = 0;
				shader_index++;
				shader_index %= SHADER_ARRAY_MAX;
				uart_debug_puts("shader_index=", shader_index);
			}
			info.fs_code_addr = (uint32_t)ArmToVc(v3d_shader_code_array[shader_index % SHADER_ARRAY_MAX]);

			info.fs_uniform_addr = (uint32_t)texture_param;
			info.shaded_vertex_data_addr = (uint32_t)ArmToVc(vertex_buffer);

			memcpy((void *)cl_ctx[frame_binning].shader_state_record, &info, sizeof(info));
			rec.bcl = v3d_set_nv_shader_state_record(rec.bcl, cl_ctx[frame_binning].shader_state_record);
		}

		//DRAW
		{
			v3d_vertex_array_prim_info info;
			memset(&info, 0, sizeof(info));
			info.primitive_type = V3D_VERTEX_ARRAY_PRIM_TRIANGLES;
			info.length = processed_vertex;
			info.index_of_first_vertex = 0;
			rec.bcl = v3d_set_vertex_array_prim(rec.bcl, &info);
		}
		{
			rec.bcl = v3d_set_flush(rec.bcl);
			rec.bcl = v3d_set_nop(rec.bcl);

			cl_ctx[frame_binning].bcl_tail = (uint32_t)rec.bcl;
		}

		//submit binning
		cl_ctx[frame_binning].rcl_tail = (uint32_t)rec.rcl;

#ifdef __DEBUG__
		uart_debug_puts("cl_ctx[frame_binning].bcl_head = ", (uint32_t)cl_ctx[frame_binning].bcl_head);
		uart_debug_puts("cl_ctx[frame_binning].bcl_tail = ", (uint32_t)cl_ctx[frame_binning].bcl_tail);
		uart_debug_puts("cl_ctx[frame_binning].rcl_head = ", (uint32_t)cl_ctx[frame_binning].rcl_head);
		uart_debug_puts("cl_ctx[frame_binning].rcl_tail = ", (uint32_t)cl_ctx[frame_binning].rcl_tail);
		uart_debug_puts("cl_ctx[frame_binning].binning_addr0 = ", (uint32_t)cl_ctx[frame_binning].binbuf0);
		uart_debug_puts("cl_ctx[frame_binning].binning_addr1 = ", (uint32_t)cl_ctx[frame_binning].binbuf1);
		uart_debug_puts("cl_ctx[frame_rendering].bcl_head = ", (uint32_t)cl_ctx[frame_rendering].bcl_head);
		uart_debug_puts("cl_ctx[frame_rendering].bcl_tail = ", (uint32_t)cl_ctx[frame_rendering].bcl_tail);
		uart_debug_puts("cl_ctx[frame_rendering].rcl_head = ", (uint32_t)cl_ctx[frame_rendering].rcl_head);
		uart_debug_puts("cl_ctx[frame_rendering].rcl_tail = ", (uint32_t)cl_ctx[frame_rendering].rcl_tail);
		uart_debug_puts("cl_ctx[frame_rendering].binning_addr0 = ", (uint32_t)cl_ctx[frame_rendering].binbuf0);
		uart_debug_puts("cl_ctx[frame_rendering].binning_addr1 = ", (uint32_t)cl_ctx[frame_rendering].binbuf1);
#endif //__DEBUG__
		{
			if(cl_ctx[frame_binning].bcl_tail != cl_ctx[frame_binning].bcl_head) {
				v3d_set_bin_exec_addr(
					(uint32_t)cl_ctx[frame_binning].bcl_head,
					(uint32_t)cl_ctx[frame_binning].bcl_tail);
				v3d_wait_bin_exec(0x1000000);
				cl_ctx[frame_binning].bcl_tail = cl_ctx[frame_binning].bcl_head;
			}
		}

		//vsync (todo make the blitter with using DMA when trigger irq)
		if(cl_ctx[frame_rendering].rcl_tail != cl_ctx[frame_rendering].rcl_head) {
#ifdef __DEBUG__
			uart_debug_puts("RCL HEAD:", cl_ctx[frame_rendering].rcl_head);
			uart_debug_puts("RCL TAIL:", cl_ctx[frame_rendering].rcl_tail);
#endif //__DEBUG__
			v3d_wait_rendering_exec(0x1000000);
			cl_ctx[frame_rendering].rcl_tail = cl_ctx[frame_rendering].rcl_head;
		}
		fake_vsync();
		mailbox_fb_flip(frame_rendering);
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
