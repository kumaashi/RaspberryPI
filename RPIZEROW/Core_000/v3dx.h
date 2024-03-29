#ifndef _V3DX_H_
#define _V3DX_H_


struct v3dx_cl_record {
	uint8_t *bcl;
	uint8_t *rcl;
};

struct v3dx_render_target {
	uint32_t addr;
	int width;
	int height;
	int is_tformat;
	v3d_texture_param tex;
	uint32_t heap0;
	uint32_t heap1;
};

inline v3d_texture_param v3dx_create_texture_param(uint32_t addr, int width, int height, void *data) {
	v3d_texture_param ret;
	memset(&ret, 0, sizeof(ret));
	ret.base = addr >> 12;
	ret.width = width;
	ret.height = height;
	ret.type4 = 1;
	if(data) {
		memcpy((void *)addr, data, width * height * sizeof(uint32_t));
	}
	//ret.magfilt = 0;
	//ret.minfilt = 0;
	//ret.param2 = 0;
	//ret.param3 = 0;
	return ret;
}

inline uint32_t v3dx_get_texture_param_baseaddr(v3d_texture_param & tex) {
	return (uint32_t)(tex.base << 12);
}

inline v3dx_render_target v3dx_create_render_target(uint32_t addr, uint32_t heap0, uint32_t heap1, int width, int height, int is_tformat) {
	v3dx_render_target ret;
	ret.addr = addr;
	ret.width = width;
	ret.height = height;
	ret.is_tformat = is_tformat;
	ret.tex = v3dx_create_texture_param(addr, width, height, NULL);
	ret.heap0 = heap0;
	ret.heap1 = heap1;
	return ret;
}

inline void v3dx_clear_render_target(v3dx_cl_record & rec, v3dx_render_target & rt, uint32_t col) {
	v3d_rendering_clear_colors_info info = {};
	memset(&info, 0, sizeof(info));
	info.color0 = col;
	info.color1 = col;
	info.clear_z = 0xFFFFFF;
	info.clear_vg_mask = 0xFF;
	rec.rcl = v3d_set_rendering_clear_colors(rec.rcl, &info);
}

inline void v3dx_set_render_target(v3dx_cl_record & rec, v3dx_render_target & rt, int tile_size) {
	//----------------------------------------------------------------------------
	// Binning
	//----------------------------------------------------------------------------
	v3d_bin_mode_config_info bininfo = {};
	{
		memset(&bininfo, 0, sizeof(bininfo));
		bininfo.mem_size = HEAP_2M_SIZE;
		bininfo.mem_addr = rt.heap0;
		bininfo.mem_tile_array_addr = rt.heap1;
		bininfo.tile_width = rt.width / tile_size;
		bininfo.tile_height = rt.height / tile_size;
		bininfo.msaa = 1;
		bininfo.auto_init_tile_array = 1;
		rec.bcl = v3d_set_bin_mode_config(rec.bcl, &bininfo);
	}

	rec.bcl = v3d_set_bin_start_tile_binning(rec.bcl);

	//CLIPPER XY SCALE
	{
		v3d_bin_clipper_xy_scaling_info info = {};
		memset(&info, 0, sizeof(info));
		info.half_width  = (float)((rt.width / 2) * 16) * 0.5;
		info.half_height = (float)((rt.height / 2) * 16) * 0.5;
		rec.bcl = v3d_set_bin_clipper_xy_scaling(rec.bcl, &info);
	}

	//min max CLIPPING PLANE
	{
		v3d_z_min_and_max_clipping_planes_info info = {};
		memset(&info, 0, sizeof(info));
		info.min_zw = 0.0f;
		info.max_zw = 1.0f;
		rec.bcl = v3d_set_z_min_and_max_clipping_planes(rec.bcl, &info);
	}

	//CLIP WINDOW
	{
		v3d_bin_clip_window_info info = {};
		memset(&info, 0, sizeof(info));
		info.x = 0;
		info.y = 0;
		info.w = rt.width - info.x;
		info.h = rt.height - info.y;
		rec.bcl = v3d_set_bin_clip_window(rec.bcl, &info);
	}

	//VIEW PORT OFFSET
	{
		v3d_bin_viewport_offset_info info = {};
		memset(&info, 0, sizeof(info));
		info.x = (rt.width / 2 ) * 16;
		info.y = (rt.height / 2) * 16;
		rec.bcl = v3d_set_bin_viewport_offset(rec.bcl, &info);
	}

	//----------------------------------------------------------------------------
	// Rendering
	//----------------------------------------------------------------------------
	{
		v3d_rendering_mode_config_info info = {};
		memset(&info, 0, sizeof(info));
		info.mem_addr = (uint32_t)ArmToVc((void *)rt.addr);
		info.width = rt.width;
		info.height = rt.width;
		info.msaa = 1;
		info.non_hdr_frame_buffer_color_format = RGB_BITS == 32 ? 1 : 2;

		info.memory_format = rt.is_tformat;
		rec.rcl = v3d_set_rendering_mode_config(rec.rcl, &info);
	}

	{
		v3d_rendering_tile_coordinates_info info = {};
		memset(&info, 0, sizeof(info));
		rec.rcl = v3d_set_rendering_tile_coordinates(rec.rcl, &info);
	}

	{
		v3d_rendering_store_tile_buffer_general_info info = {};
		memset(&info, 0, sizeof(info));
		info.pixel_color_format = RGB_BITS == 32 ? 0 : 2;
		rec.rcl = v3d_set_rendering_store_tile_buffer_general(rec.rcl, &info);
	}

	int tile_w = bininfo.tile_width;
	int tile_h = bininfo.tile_height;
	for(int y = 0 ; y < tile_h; y++) {
		for(int x = 0 ; x < tile_w; x++) {
			v3d_rendering_tile_coordinates_info info = {};
			memset(&info, 0, sizeof(info));
			info.x = x;
			info.y = y;
			rec.rcl = v3d_set_rendering_tile_coordinates(rec.rcl, &info);
			{
				int offset = ((y * tile_w + x) * 32);
				v3d_branch_to_sublist_info info = {};
				info.addr = rt.heap0 + offset;
				rec.rcl = v3d_set_branch_to_sublist(rec.rcl, &info);
			}
			if(x == (tile_w - 1) && y == (tile_h - 1)) {
				rec.rcl = v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer_eof(rec.rcl);
			} else {
				rec.rcl = v3d_set_rendering_store_multi_sample_resolved_tile_color_buffer(rec.rcl);
			}
		}
	}
	rec.rcl = v3d_set_nop(rec.rcl);
}

#endif //_V3DX_H_
