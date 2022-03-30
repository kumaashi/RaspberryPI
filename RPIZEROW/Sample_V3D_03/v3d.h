#ifndef _V3D_H_
#define _V3D_H_

#include "common.h"
#include "hw.h"


//SET BRANCH TO SUBLIST
typedef struct {
	uint32_t addr;
} __attribute__((__packed__)) v3d_branch_to_sublist_info;


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

//CLIP WINDOW
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} __attribute__((__packed__)) v3d_bin_clip_window_info ;

//VIEWPORT OFFSET
typedef struct {
	uint16_t x;
	uint16_t y;
} __attribute__((__packed__)) v3d_bin_viewport_offset_info;

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


//CLEAR COLORS
typedef struct {
	uint32_t color0;
	uint32_t color1;
	unsigned clear_z : 24;
	unsigned clear_vg_mask : 8;
	unsigned clear_stencil : 8;
} __attribute__((__packed__)) v3d_rendering_clear_colors_info ;

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

//DRAW PRIM
typedef struct {
	uint8_t primitive_type;
	uint32_t length;
	uint32_t index_of_first_vertex;
} __attribute__((__packed__)) v3d_vertex_array_prim_info;


//RENDERING_TILE_COORDINATES
typedef struct {
	uint8_t x;
	uint8_t y;
} __attribute__((__packed__)) v3d_rendering_tile_coordinates_info ;


enum {
	V3D_VERTEX_ARRAY_PRIM_POINTS = 0,
	V3D_VERTEX_ARRAY_PRIM_LINES,
	V3D_VERTEX_ARRAY_PRIM_LINE_LOOP,
	V3D_VERTEX_ARRAY_PRIM_LINE_STRIP,
	V3D_VERTEX_ARRAY_PRIM_TRIANGLES,
	V3D_VERTEX_ARRAY_PRIM_TRIANGLE_STRIP,
	V3D_VERTEX_ARRAY_PRIM_MAX,
};


void v3d_debug_print();
uint8_t *v3d_set_bin_clip_window(uint8_t *p, v3d_bin_clip_window_info *info);
uint8_t *v3d_set_bin_mode_config(uint8_t *p, v3d_bin_mode_config_info *info);
uint8_t *v3d_set_bin_start(uint8_t *p);
uint8_t *v3d_set_bin_start_tile_binning(uint8_t *p);
uint8_t *v3d_set_bin_state_config(uint8_t *p, v3d_bin_state_config_info *info);
uint8_t *v3d_set_bin_viewport_offset(uint8_t *p, v3d_bin_viewport_offset_info *info);
uint8_t *v3d_set_branch_to_sublist(uint8_t *p, v3d_branch_to_sublist_info *info);
uint8_t *v3d_set_branch_to_sublist_test(uint8_t *p, uint32_t addr);
uint8_t *v3d_set_flush(uint8_t *p);
uint8_t *v3d_set_flush_all(uint8_t *p);
uint8_t *v3d_set_halt(uint8_t *p);
uint8_t *v3d_set_nop(uint8_t *p);
uint8_t *v3d_set_nv_shader_state_record(uint8_t *p, uint32_t addr);
uint8_t *v3d_set_rendering_clear_colors(uint8_t *p, v3d_rendering_clear_colors_info *info);
uint8_t *v3d_set_rendering_mode_config(uint8_t *p, v3d_rendering_mode_config_info *info);
uint8_t *v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer(uint8_t *p);
uint8_t *v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer_eof(uint8_t *p);
uint8_t *v3d_set_rendering_store_tile_buffer_general(uint8_t *p, v3d_rendering_store_tile_buffer_general_info *info);
uint8_t *v3d_set_rendering_tile_coordinates(uint8_t *p, v3d_rendering_tile_coordinates_info *info);
uint8_t *v3d_set_shader_state_record(uint8_t *p, v3d_shader_state_record_info *addr);
uint8_t *v3d_set_vertex_array_prim(uint8_t *p, v3d_vertex_array_prim_info *info);
void v3d_reset();
void v3d_set_bin_exec_addr(uint32_t start, uint32_t end);
void v3d_set_rendering_exec_addr(uint32_t start, uint32_t end);
void v3d_wait_bin_exec(uint32_t timeout);
void v3d_wait_rendering_exec(uint32_t timeout);

#endif //_V3D_H_
