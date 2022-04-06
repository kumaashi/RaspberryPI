// For Raspberry Pi Zero W V3D
// 2022 yasai kumaashi (gyaboyan@gmail.com)
#include "v3d.h"
#include "uart.h"

#define V3D_CTnCS_CTRUN   (0x20)
#define V3D_CTnCS_CTRSTA  (1 << 15)

#define _V3D_DEBUG_

void v3d_debug_print() {
	uart_puts("---------------------------------------------------------------------\n");
	uart_debug_puts("V3D_IDENT0       =", *V3D_IDENT0         ); 
	uart_debug_puts("V3D_IDENT1       =", *V3D_IDENT1         ); 
	uart_debug_puts("V3D_IDENT2       =", *V3D_IDENT2         ); 
	uart_debug_puts("V3D_SCRATCH      =", *V3D_SCRATCH        ); 
	uart_debug_puts("V3D_L2CACTL      =", *V3D_L2CACTL        ); 
	uart_debug_puts("V3D_SLCACTL      =", *V3D_SLCACTL        ); 
	uart_debug_puts("V3D_INTCTL       =", *V3D_INTCTL         ); 
	uart_debug_puts("V3D_INTENA       =", *V3D_INTENA         ); 
	uart_debug_puts("V3D_INTDIS       =", *V3D_INTDIS         ); 
	uart_debug_puts("V3D_CT0CS        =", *V3D_CT0CS          ); 
	uart_debug_puts("V3D_CT1CS        =", *V3D_CT1CS          ); 
	uart_debug_puts("V3D_CT0EA        =", *V3D_CT0EA          ); 
	uart_debug_puts("V3D_CT1EA        =", *V3D_CT1EA          ); 
	uart_debug_puts("V3D_CT0CA        =", *V3D_CT0CA          ); 
	uart_debug_puts("V3D_CT1CA        =", *V3D_CT1CA          ); 
	uart_debug_puts("V3D_CT00RA0      =", *V3D_CT00RA0        ); 
	uart_debug_puts("V3D_CT01RA0      =", *V3D_CT01RA0        ); 
	uart_debug_puts("V3D_CT0LC        =", *V3D_CT0LC          ); 
	uart_debug_puts("V3D_CT1LC        =", *V3D_CT1LC          ); 
	uart_debug_puts("V3D_CT0PC        =", *V3D_CT0PC          ); 
	uart_debug_puts("V3D_CT1PC        =", *V3D_CT1PC          ); 
	uart_debug_puts("V3D_PCS          =", *V3D_PCS            ); 
	uart_debug_puts("V3D_BFC          =", *V3D_BFC            ); 
	uart_debug_puts("V3D_RFC          =", *V3D_RFC            ); 
	uart_debug_puts("V3D_BPCA         =", *V3D_BPCA           ); 
	uart_debug_puts("V3D_BPCS         =", *V3D_BPCS           ); 
	uart_debug_puts("V3D_BPOA         =", *V3D_BPOA           ); 
	uart_debug_puts("V3D_BPOS         =", *V3D_BPOS           ); 
	uart_debug_puts("V3D_BXCF         =", *V3D_BXCF           ); 
	uart_debug_puts("V3D_SQRSV0       =", *V3D_SQRSV0         ); 
	uart_debug_puts("V3D_SQRSV1       =", *V3D_SQRSV1         ); 
	uart_debug_puts("V3D_SQCNTL       =", *V3D_SQCNTL         ); 
	uart_debug_puts("V3D_SRQPC        =", *V3D_SRQPC          ); 
	uart_debug_puts("V3D_SRQUA        =", *V3D_SRQUA          ); 
	uart_debug_puts("V3D_SRQUL        =", *V3D_SRQUL          ); 
	uart_debug_puts("V3D_SRQCS        =", *V3D_SRQCS          ); 
	uart_debug_puts("V3D_VPACNTL      =", *V3D_VPACNTL        ); 
	uart_debug_puts("V3D_VPMBASE      =", *V3D_VPMBASE        ); 
	uart_debug_puts("V3D_PCTRC        =", *V3D_PCTRC          ); 
	uart_debug_puts("V3D_PCTRE        =", *V3D_PCTRE          ); 
	uart_debug_puts("V3D_PCTR0        =", *V3D_PCTR0          ); 
	uart_debug_puts("V3D_PCTRS0       =", *V3D_PCTRS0         ); 
	uart_debug_puts("V3D_PCTR1        =", *V3D_PCTR1          ); 
	uart_debug_puts("V3D_PCTRS1       =", *V3D_PCTRS1         ); 
	uart_debug_puts("V3D_PCTR2        =", *V3D_PCTR2          ); 
	uart_debug_puts("V3D_PCTRS2       =", *V3D_PCTRS2         ); 
	uart_debug_puts("V3D_PCTR3        =", *V3D_PCTR3          ); 
	uart_debug_puts("V3D_PCTRS3       =", *V3D_PCTRS3         ); 
	uart_debug_puts("V3D_PCTR4        =", *V3D_PCTR4          ); 
	uart_debug_puts("V3D_PCTRS4       =", *V3D_PCTRS4         ); 
	uart_debug_puts("V3D_PCTR5        =", *V3D_PCTR5          ); 
	uart_debug_puts("V3D_PCTRS5       =", *V3D_PCTRS5         ); 
	uart_debug_puts("V3D_PCTR6        =", *V3D_PCTR6          ); 
	uart_debug_puts("V3D_PCTRS6       =", *V3D_PCTRS6         ); 
	uart_debug_puts("V3D_PCTR7        =", *V3D_PCTR7          ); 
	uart_debug_puts("V3D_PCTRS7       =", *V3D_PCTRS7         ); 
	uart_debug_puts("V3D_PCTR8        =", *V3D_PCTR8          ); 
	uart_debug_puts("V3D_PCTRS8       =", *V3D_PCTRS8         ); 
	uart_debug_puts("V3D_PCTR9        =", *V3D_PCTR9          ); 
	uart_debug_puts("V3D_PCTRS9       =", *V3D_PCTRS9         ); 
	uart_debug_puts("V3D_PCTR10       =", *V3D_PCTR10         ); 
	uart_debug_puts("V3D_PCTRS10      =", *V3D_PCTRS10        ); 
	uart_debug_puts("V3D_PCTR11       =", *V3D_PCTR11         ); 
	uart_debug_puts("V3D_PCTRS11      =", *V3D_PCTRS11        ); 
	uart_debug_puts("V3D_PCTR12       =", *V3D_PCTR12         ); 
	uart_debug_puts("V3D_PCTRS12      =", *V3D_PCTRS12        ); 
	uart_debug_puts("V3D_PCTR13       =", *V3D_PCTR13         ); 
	uart_debug_puts("V3D_PCTRS13      =", *V3D_PCTRS13        ); 
	uart_debug_puts("V3D_PCTR14       =", *V3D_PCTR14         ); 
	uart_debug_puts("V3D_PCTRS14      =", *V3D_PCTRS14        ); 
	uart_debug_puts("V3D_PCTR15       =", *V3D_PCTR15         ); 
	uart_debug_puts("V3D_PCTRS15      =", *V3D_PCTRS15        ); 
	uart_debug_puts("V3D_DBGE         =", *V3D_DBGE           ); 
	uart_debug_puts("V3D_FDBGO        =", *V3D_FDBGO          ); 
	uart_debug_puts("V3D_FDBGB        =", *V3D_FDBGB          ); 
	uart_debug_puts("V3D_FDBGR        =", *V3D_FDBGR          ); 
	uart_debug_puts("V3D_FDBGS        =", *V3D_FDBGS          ); 
	uart_debug_puts("V3D_ERRSTAT      =", *V3D_ERRSTAT        ); 
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

uint8_t *v3d_set_z_min_and_max_clipping_planes(uint8_t *p, v3d_z_min_and_max_clipping_planes_info *info) {
	*p = 104;
	p++;

	*(v3d_z_min_and_max_clipping_planes_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_bin_clipper_xy_scaling(uint8_t *p, v3d_bin_clipper_xy_scaling_info *info) {
	*p = 105;
	p++;

	*(v3d_bin_clipper_xy_scaling_info *)p = *info;
	p += sizeof(*info);
	return p;
}

uint8_t *v3d_set_bin_clipper_z_scale_and_offset(uint8_t *p, v3d_bin_clipper_z_scale_and_offset_info *info) {
	*p = 106;
	p++;

	*(v3d_bin_clipper_z_scale_and_offset_info *)p = *info;
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
#ifdef _V3D_DEBUG_
	uart_debug_puts("bin exe start:", start);
	uart_debug_puts("bin exe end  :", end);
#endif
	*V3D_CT0CA = start;
	*V3D_CT0EA = end;
}

void v3d_set_rendering_exec_addr(uint32_t start, uint32_t end) {
#ifdef _V3D_DEBUG_
	uart_debug_puts("rendering exe start:", start);
	uart_debug_puts("rendering exe end  :", end);
#endif
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
#ifdef _V3D_DEBUG_
		if( (count % 10000) == 0) {
			uart_puts("HANG : v3d_wait_binning_exec\n");
			v3d_debug_print();
		}
		SLEEP(100);
#endif
	}
	*V3D_BFC = 1;
	
#ifdef _V3D_DEBUG_
	uart_puts("OK : v3d_wait_binning_exec\n");
#endif
}

void v3d_wait_rendering_exec(uint32_t timeout) {
	uint32_t count = 0;
	while(*V3D_RFC == 0) {
		count++;
#ifdef _V3D_DEBUG_
		if( (count % 10000) == 0) {
			uart_puts("HANG : v3d_wait_rendering_exec\n");
			v3d_debug_print();
		}
#endif
		SLEEP(100);
	}
	*V3D_RFC = 1;
#ifdef _V3D_DEBUG_
	uart_puts("OK : v3d_wait_rendering_exec\n");
#endif
}
