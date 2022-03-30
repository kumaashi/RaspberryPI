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
} //EXTERN C

#define WIDTH             640
#define HEIGHT            480
#define BUFNUM            2
#define TILE_SIZE         32

const uint32_t test_copy_shader[] = {
#include "copy.h"
};

struct vertex_format_clip {
	//float xc, yc, zc, wc;
	int16_t xs;
	int16_t ys;
	float zs;
	float inv_wc;
	float r, g, b;
} __attribute__((__packed__));


struct vec4 {
	float v[4];
} __attribute__((__packed__));

struct matrix {
	float m[16];
} __attribute__((__packed__));

//ref : https://github.com/unia-sik/emsbench/blob/master/embedded/tg/sqrtf.c
float sqrtf_quake3( float x ) {
	const float xhalf = 0.5f * x;
	union {
		float x;
		int i;
	} u;
	u.x = x;
	u.i = 0x5f3759df - ( u.i >> 1 );
	return x * u.x * (1.5f - xhalf * u.x * u.x);
}

vec4 vec4_set(float x, float y, float z, float w) {
	vec4 a;
	a.v[0] = x;
	a.v[1] = y;
	a.v[2] = z;
	a.v[3] = w;
	return a;
}

float vec4_dot(vec4 &v0, vec4 &v1) {
	return v0.v[0] * v1.v[0] + v0.v[1] * v1.v[1] + v0.v[2] * v1.v[2];
}

float vec4_length(vec4 v) {
	float a = sqrtf_quake3(vec4_dot(v, v));
	return a;
}

vec4 vec4_normalize(vec4 &v)
{
	float len = vec4_length(v);
	if (len == 0)
		len = 1.0f;
	len = 1.0f / len;
	return vec4_set(
			v.v[0] * len,
			v.v[1] * len,
			v.v[2] * len,
			v.v[3] * len);
}

vec4 vec4_cross(vec4 &a, vec4 &b)
{
	return vec4_set(
			a.v[1] * b.v[2] - a.v[2] * b.v[1],
			a.v[2] * b.v[0] - a.v[0] * b.v[2],
			a.v[0] * b.v[1] - a.v[1] * b.v[0],
			0.0f);
}

void vec4_uart_puts(vec4 & v) {
	uart_puts("------------------------------------------------------\n");
	uart_debug_puts("v.x:", *(uint32_t *)&v.v[0]);
	uart_debug_puts("v.y:", *(uint32_t *)&v.v[1]);
	uart_debug_puts("v.z:", *(uint32_t *)&v.v[2]);
	uart_debug_puts("v.w:", *(uint32_t *)&v.v[3]);
	uart_puts("------------------------------------------------------\n");
}

void matrix_uart_puts(matrix & m) {
	uart_puts("------------------------------------------------------\n");
	uart_debug_puts("ret.m[0 + 4 * 0]=", *(uint32_t *)&m.m[0 + 4 * 0]);
	uart_debug_puts("ret.m[1 + 4 * 0]=", *(uint32_t *)&m.m[1 + 4 * 0]);
	uart_debug_puts("ret.m[2 + 4 * 0]=", *(uint32_t *)&m.m[2 + 4 * 0]);
	uart_debug_puts("ret.m[3 + 4 * 0]=", *(uint32_t *)&m.m[3 + 4 * 0]);
	uart_debug_puts("ret.m[0 + 4 * 1]=", *(uint32_t *)&m.m[0 + 4 * 1]);
	uart_debug_puts("ret.m[1 + 4 * 1]=", *(uint32_t *)&m.m[1 + 4 * 1]);
	uart_debug_puts("ret.m[2 + 4 * 1]=", *(uint32_t *)&m.m[2 + 4 * 1]);
	uart_debug_puts("ret.m[3 + 4 * 1]=", *(uint32_t *)&m.m[3 + 4 * 1]);
	uart_debug_puts("ret.m[0 + 4 * 2]=", *(uint32_t *)&m.m[0 + 4 * 2]);
	uart_debug_puts("ret.m[1 + 4 * 2]=", *(uint32_t *)&m.m[1 + 4 * 2]);
	uart_debug_puts("ret.m[2 + 4 * 2]=", *(uint32_t *)&m.m[2 + 4 * 2]);
	uart_debug_puts("ret.m[3 + 4 * 2]=", *(uint32_t *)&m.m[3 + 4 * 2]);
	uart_debug_puts("ret.m[0 + 4 * 3]=", *(uint32_t *)&m.m[0 + 4 * 3]);
	uart_debug_puts("ret.m[1 + 4 * 3]=", *(uint32_t *)&m.m[1 + 4 * 3]);
	uart_debug_puts("ret.m[2 + 4 * 3]=", *(uint32_t *)&m.m[2 + 4 * 3]);
	uart_debug_puts("ret.m[3 + 4 * 3]=", *(uint32_t *)&m.m[3 + 4 * 3]);
	uart_puts("------------------------------------------------------\n");
}

vec4 matrix_vec4_mult(vec4 & v, matrix & m)
{
	return vec4_set(
			v.v[0] * m.m[0x0] + v.v[1] * m.m[0x4] + v.v[2] * m.m[0x8] + v.v[3] * m.m[0xC],
			v.v[0] * m.m[0x1] + v.v[1] * m.m[0x5] + v.v[2] * m.m[0x9] + v.v[3] * m.m[0xD],
			v.v[0] * m.m[0x2] + v.v[1] * m.m[0x6] + v.v[2] * m.m[0xA] + v.v[3] * m.m[0xE],
			v.v[0] * m.m[0x3] + v.v[1] * m.m[0x7] + v.v[2] * m.m[0xB] + v.v[3] * m.m[0xF]);
}

matrix matrix_ident()
{
	matrix ret;
	ret.m[0 + 4 * 0] = 1;
	ret.m[1 + 4 * 0] = 0;
	ret.m[2 + 4 * 0] = 0;
	ret.m[3 + 4 * 0] = 0;
	ret.m[0 + 4 * 1] = 0;
	ret.m[1 + 4 * 1] = 1;
	ret.m[2 + 4 * 1] = 0;
	ret.m[3 + 4 * 1] = 0;
	ret.m[0 + 4 * 2] = 0;
	ret.m[1 + 4 * 2] = 0;
	ret.m[2 + 4 * 2] = 1;
	ret.m[3 + 4 * 2] = 0;
	ret.m[0 + 4 * 3] = 0;
	ret.m[1 + 4 * 3] = 0;
	ret.m[2 + 4 * 3] = 0;
	ret.m[3 + 4 * 3] = 1;
	//matrix_uart_puts(ret);
	return ret;
}

matrix matrix_translate(float x, float y, float z)
{
	matrix ret = matrix_ident();
	ret.m[0 * 4 * 3] = x;
	ret.m[1 * 4 * 3] = y;
	ret.m[2 * 4 * 3] = z;
	return ret;
}

matrix matrix_rotationf(float x, float y, float z)
{
	matrix m = matrix_ident();
	float sin_a = tsin(x);
	float cos_a = tcos(x);
	float sin_b = tsin(y);
	float cos_b = tcos(y);
	float sin_c = tsin(z);
	float cos_c = tcos(z);
	m.m[0 + 0] = cos_b * cos_c;
	m.m[0 + 1] = -cos_b * sin_c;
	m.m[0 + 2] = sin_b;
	m.m[4 + 0] = sin_a * sin_b * cos_c + cos_a * sin_c;
	m.m[4 + 1] = -sin_a * sin_b * sin_c + cos_a * cos_c;
	m.m[4 + 2] = -sin_a * cos_b;
	m.m[8 + 0] = -cos_a * sin_b * cos_c + sin_a * sin_c;
	m.m[8 + 1] = cos_a * sin_b * sin_c + sin_a * cos_c;
	m.m[8 + 2] = cos_a * cos_b;
	return m;
}

matrix matrix_mult(matrix &a, matrix &b)
{
	matrix m = matrix_ident();
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			int yi = y * 4;
			float value = 0;
			value += b.m[yi + 0] * a.m[x + 0];
			value += b.m[yi + 1] * a.m[x + 4];
			value += b.m[yi + 2] * a.m[x + 8];
			value += b.m[yi + 3] * a.m[x + 12];
			m.m[x + y * 4] = value;
		}
	}
	return m;
}


matrix matrix_lookat(
		float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz)
{
	vec4 Z = vec4_set(centerx - eyex, centery - eyey, centerz - eyez, 0);
	Z = vec4_normalize(Z);

	vec4 Y = vec4_set(upx, upy, upz, 0);
	//vec4 X = vec4_cross(Y, Z);
	vec4 X = vec4_cross(Z, Y);
	X = vec4_normalize(X);
	Y = vec4_cross(Z, X);

	vec4 pos = vec4_set(eyex, eyey, eyez, 0);
	matrix ret = matrix_ident();

	ret.m[0 + 4 * 0] = X.v[0];
	ret.m[1 + 4 * 0] = Y.v[0];
	ret.m[2 + 4 * 0] = Z.v[0];
	ret.m[3 + 4 * 0] = 0;
	ret.m[0 + 4 * 1] = X.v[1];
	ret.m[1 + 4 * 1] = Y.v[1];
	ret.m[2 + 4 * 1] = Z.v[1];
	ret.m[3 + 4 * 1] = 0;
	ret.m[0 + 4 * 2] = X.v[2];
	ret.m[1 + 4 * 2] = Y.v[2];
	ret.m[2 + 4 * 2] = Z.v[2];
	ret.m[3 + 4 * 2] = 0;
	ret.m[0 + 4 * 3] = vec4_dot(X, pos);
	ret.m[1 + 4 * 3] = vec4_dot(Y, pos);
	ret.m[2 + 4 * 3] = vec4_dot(Z, pos);
	ret.m[3 + 4 * 3] = 1;
	return ret;
}

matrix matrix_projection(float fovy, float aspect, float znear, float zfar)
{
	float tanhfov = 0.5;//ttan(fovy / 2.0f);
	float a = 1.0f / (aspect * tanhfov);
	float b = 1.0f / tanhfov;
	float c = (zfar + znear) / (zfar - znear);
	float d = 1.0f;
	float e = -(2 * zfar * znear) / (zfar - znear);

	matrix ret = matrix_ident();
	ret.m[0 + 4 * 0] = a;
	ret.m[1 + 4 * 0] = 0;
	ret.m[2 + 4 * 0] = 0;
	ret.m[3 + 4 * 0] = 0;

	ret.m[0 + 4 * 1] = 0;
	ret.m[1 + 4 * 1] = b;
	ret.m[2 + 4 * 1] = 0;
	ret.m[3 + 4 * 1] = 0;

	ret.m[0 + 4 * 2] = 0;
	ret.m[1 + 4 * 2] = 0;
	ret.m[2 + 4 * 2] = c;
	ret.m[3 + 4 * 2] = d;

	ret.m[0 + 4 * 3] = 0;
	ret.m[1 + 4 * 3] = 0;
	ret.m[2 + 4 * 3] = e;
	ret.m[3 + 4 * 3] = 0;
	return ret;
}

void ndc_to_screen(vertex_format_clip *fmt, vec4 & vdc, int width, int height)
{
	/*
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
	uart_debug_puts("------------------\nxs:", fmt->xs);
	uart_debug_puts("ys:", fmt->ys);
*/
	float xc = vdc.v[0] / vdc.v[3];
	float yc = vdc.v[1] / vdc.v[3];
	float zc = vdc.v[2] / vdc.v[3];
	float wc = vdc.v[3] / vdc.v[3];
	fmt->zs = zc * 0.5f + 0.5f;
	fmt->inv_wc = 1.0f / wc;
	float xx = ((xc + 1.0f) * width  * 0.5f);
	float yy = ((yc + 1.0f) * height * 0.5f);

	fmt->xs = (int16_t)(xx * 16.0f); //12.4
	fmt->ys = (int16_t)(yy * 16.0f); //12.4
	//uart_debug_puts("------------------\nxs:", fmt->xs);
	//uart_debug_puts("ys:", fmt->ys);
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
				0.0f, 0.0f, 3.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);
		matrix proj = matrix_projection(
				45.0f, (float)WIDTH / (float)HEIGHT, 0.125, 10.0f);
		//matrix_uart_puts(ident);
		matrix trans = matrix_translate(tsin(fcount_t), -tcos(fcount_t), -tsin(fcount_t));
		//matrix trans = matrix_ident();//matrix_translate(0, 0, 0);
		matrix view_proj = matrix_mult(proj, view);
		//view_proj = matrix_mult(trans, view_proj);
		//view_proj = trans;//matrix_mult(trans, view_proj);
		//matrix_uart_puts(view_proj);

		fcount += 0.01666666f * 0.25;
		fcount_t += 0.01666666f * 0.25;
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
			//info.flag_bits = (1 << 3) | (1 << 2);

			//clip.xyzw, xsys, zs, inv_w, rgb
			//info.shaded_vertex_data_stride = 10 * sizeof(uint32_t);
			info.shaded_vertex_data_stride = 6 * sizeof(uint32_t);
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
			info.length = 12;
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
