// For Raspberry Pi Zero W V3D
// 2022 yasai kumaashi (gyaboyan@gmail.com)
#include "v3d.h"
#include "uart.h"

#define V3D_CTnCS_CTRUN   (0x20)
#define V3D_CTnCS_CTRSTA  (1 << 15)

void v3d_debug_print() {
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

//INCREMENT_SEM
uint8_t *v3d_increment_sem(uint8_t *p) {
	*p = 7;
	p++;
	return p;
}

//WAIT SEM
uint8_t *v3d_wait_sem(uint8_t *p) {
	*p = 8;
	p++;
	return p;
}

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

uint8_t *v3d_set_bin_clip_window(uint8_t *p, v3d_bin_clip_window_info *info) {
	*p = 102;
	p += 1;

	*(v3d_bin_clip_window_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_bin_viewport_offset(uint8_t *p, v3d_bin_viewport_offset_info *info) {
	*p = 103;
	p += 1;

	*(v3d_bin_viewport_offset_info *)p = *info;
	p += sizeof(*info);
	return p;
}


uint8_t *v3d_set_bin_state_config(uint8_t *p, v3d_bin_state_config_info *info) {
	*p = 96;
	p += 1;

	*(v3d_bin_state_config_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_rendering_clear_colors(uint8_t *p, v3d_rendering_clear_colors_info *info) {
	*p = 114;
	p += 1;

	*(v3d_rendering_clear_colors_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_rendering_store_tile_buffer_general(uint8_t *p, v3d_rendering_store_tile_buffer_general_info *info) {
	*p = 28;
	p += 1;

	*(v3d_rendering_store_tile_buffer_general_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_rendering_mode_config(uint8_t *p, v3d_rendering_mode_config_info *info) {
	*p = 113;
	p += 1;

	*(v3d_rendering_mode_config_info *)p = *info;
	p += sizeof(*info);
	return p;
}


uint8_t *v3d_set_shader_state_record(uint8_t *p, v3d_shader_state_record_info *addr) {
	*p = 64;
	p++;

	uint32_t value = (uint32_t)addr;
	value >>= 4;
	memcpy(p, &value, 4);

	p += sizeof(uint32_t);
	return p;
}

uint8_t *v3d_set_nv_shader_state_record(uint8_t *p, uint32_t addr) {
	*p = 65;
	p++;

	uint32_t value = addr;
	memcpy(p, &value, 4);
	//*(uint32_t *)p = addr;

	p += sizeof(uint32_t);
	return p;
}



uint8_t *v3d_set_vertex_array_prim(uint8_t *p, v3d_vertex_array_prim_info *info) {
	*p = 33;
	p++;

	*(v3d_vertex_array_prim_info *)p = *info;

	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_rendering_tile_coordinates(uint8_t *p, v3d_rendering_tile_coordinates_info *info) {
	*p = 115;
	p++;

	*(v3d_rendering_tile_coordinates_info *)p = *info;
	p += sizeof(*info);
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
