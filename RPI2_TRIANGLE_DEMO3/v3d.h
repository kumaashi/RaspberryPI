#ifndef _V3D_H_
#define _V3D_H_

#include "hwio.h"

// Defines for v3d register offsets
#define V3D_BASE                       (SUBSYSTEM_BASE + 0xC00000)
#define V3D_IDENT0                     (V3D_BASE + (0x000)) // V3D Identification 0 (V3D block identity)
#define V3D_IDENT1                     (V3D_BASE + (0x004)) // V3D Identification 1 (V3D Configuration A)
#define V3D_IDENT2                     (V3D_BASE + (0x008)) // V3D Identification 1 (V3D Configuration B)

#define V3D_SCRATCH                    (V3D_BASE + (0x010)) // Scratch Register

#define V3D_L2CACTL                    (V3D_BASE + (0x020)) // 2 Cache Control
#define V3D_SLCACTL                    (V3D_BASE + (0x024)) // Slices Cache Control

#define V3D_INTCTL                     (V3D_BASE + (0x030)) // Interrupt Control
#define V3D_INTENA                     (V3D_BASE + (0x034)) // Interrupt Enables
#define V3D_INTDIS                     (V3D_BASE + (0x038)) // Interrupt Disables

#define V3D_CT0CS                      (V3D_BASE + (0x100)) // Control List Executor Thread 0 Control and Status.
#define V3D_CT1CS                      (V3D_BASE + (0x104)) // Control List Executor Thread 1 Control and Status.
#define V3D_CT0EA                      (V3D_BASE + (0x108)) // Control List Executor Thread 0 End Address.
#define V3D_CT1EA                      (V3D_BASE + (0x10c)) // Control List Executor Thread 1 End Address.
#define V3D_CT0CA                      (V3D_BASE + (0x110)) // Control List Executor Thread 0 Current Address.
#define V3D_CT1CA                      (V3D_BASE + (0x114)) // Control List Executor Thread 1 Current Address.
#define V3D_CT00RA0                    (V3D_BASE + (0x118)) // Control List Executor Thread 0 Return Address.
#define V3D_CT01RA0                    (V3D_BASE + (0x11c)) // Control List Executor Thread 1 Return Address.
#define V3D_CT0LC                      (V3D_BASE + (0x120)) // Control List Executor Thread 0 List Counter
#define V3D_CT1LC                      (V3D_BASE + (0x124)) // Control List Executor Thread 1 List Counter
#define V3D_CT0PC                      (V3D_BASE + (0x128)) // Control List Executor Thread 0 Primitive List Counter
#define V3D_CT1PC                      (V3D_BASE + (0x12c)) // Control List Executor Thread 1 Primitive List Counter

#define V3D_PCS                        (V3D_BASE + (0x130)) // V3D Pipeline Control and Status
#define V3D_BFC                        (V3D_BASE + (0x134)) // Binning Mode Flush Count
#define V3D_RFC                        (V3D_BASE + (0x138)) // Rendering Mode Frame Count

#define V3D_BPCA                       (V3D_BASE + (0x300)) // Current Address of Binning Memory Pool
#define V3D_BPCS                       (V3D_BASE + (0x304)) // Remaining Size of Binning Memory Pool
#define V3D_BPOA                       (V3D_BASE + (0x308)) // Address of Overspill Binning Memory Block
#define V3D_BPOS                       (V3D_BASE + (0x30c)) // Size of Overspill Binning Memory Block
#define V3D_BXCF                       (V3D_BASE + (0x310)) // Binner Debug

#define V3D_SQRSV0                     (V3D_BASE + (0x410)) // Reserve QPUs 0-7
#define V3D_SQRSV1                     (V3D_BASE + (0x414)) // Reserve QPUs 8-15
#define V3D_SQCNTL                     (V3D_BASE + (0x418)) // QPU Scheduler Control

#define V3D_SRQPC                      (V3D_BASE + (0x430)) // QPU User Program Request Program Address
#define V3D_SRQUA                      (V3D_BASE + (0x434)) // QPU User Program Request Uniforms Address
#define V3D_SRQUL                      (V3D_BASE + (0x438)) // QPU User Program Request Uniforms Length
#define V3D_SRQCS                      (V3D_BASE + (0x43c)) // QPU User Program Request Control and Status

#define V3D_VPACNTL                    (V3D_BASE + (0x500)) // VPM Allocator Control
#define V3D_VPMBASE                    (V3D_BASE + (0x504)) // VPM base (user) memory reservation

#define V3D_PCTRC                      (V3D_BASE + (0x670)) // Performance Counter Clear
#define V3D_PCTRE                      (V3D_BASE + (0x674)) // Performance Counter Enables

#define V3D_PCTR0                      (V3D_BASE + (0x680)) // Performance Counter Count 0
#define V3D_PCTRS0                     (V3D_BASE + (0x684)) // Performance Counter Mapping 0
#define V3D_PCTR1                      (V3D_BASE + (0x688)) // Performance Counter Count 1
#define V3D_PCTRS1                     (V3D_BASE + (0x68c)) // Performance Counter Mapping 1
#define V3D_PCTR2                      (V3D_BASE + (0x690)) // Performance Counter Count 2
#define V3D_PCTRS2                     (V3D_BASE + (0x694)) // Performance Counter Mapping 2
#define V3D_PCTR3                      (V3D_BASE + (0x698)) // Performance Counter Count 3
#define V3D_PCTRS3                     (V3D_BASE + (0x69c)) // Performance Counter Mapping 3
#define V3D_PCTR4                      (V3D_BASE + (0x6a0)) // Performance Counter Count 4
#define V3D_PCTRS4                     (V3D_BASE + (0x6a4)) // Performance Counter Mapping 4
#define V3D_PCTR5                      (V3D_BASE + (0x6a8)) // Performance Counter Count 5
#define V3D_PCTRS5                     (V3D_BASE + (0x6ac)) // Performance Counter Mapping 5
#define V3D_PCTR6                      (V3D_BASE + (0x6b0)) // Performance Counter Count 6
#define V3D_PCTRS6                     (V3D_BASE + (0x6b4)) // Performance Counter Mapping 6
#define V3D_PCTR7                      (V3D_BASE + (0x6b8)) // Performance Counter Count 7
#define V3D_PCTRS7                     (V3D_BASE + (0x6bc)) // Performance Counter Mapping 7 
#define V3D_PCTR8                      (V3D_BASE + (0x6c0)) // Performance Counter Count 8
#define V3D_PCTRS8                     (V3D_BASE + (0x6c4)) // Performance Counter Mapping 8
#define V3D_PCTR9                      (V3D_BASE + (0x6c8)) // Performance Counter Count 9
#define V3D_PCTRS9                     (V3D_BASE + (0x6cc)) // Performance Counter Mapping 9
#define V3D_PCTR10                     (V3D_BASE + (0x6d0)) // Performance Counter Count 10
#define V3D_PCTRS10                    (V3D_BASE + (0x6d4)) // Performance Counter Mapping 10
#define V3D_PCTR11                     (V3D_BASE + (0x6d8)) // Performance Counter Count 11
#define V3D_PCTRS11                    (V3D_BASE + (0x6dc)) // Performance Counter Mapping 11
#define V3D_PCTR12                     (V3D_BASE + (0x6e0)) // Performance Counter Count 12
#define V3D_PCTRS12                    (V3D_BASE + (0x6e4)) // Performance Counter Mapping 12
#define V3D_PCTR13                     (V3D_BASE + (0x6e8)) // Performance Counter Count 13
#define V3D_PCTRS13                    (V3D_BASE + (0x6ec)) // Performance Counter Mapping 13
#define V3D_PCTR14                     (V3D_BASE + (0x6f0)) // Performance Counter Count 14
#define V3D_PCTRS14                    (V3D_BASE + (0x6f4)) // Performance Counter Mapping 14
#define V3D_PCTR15                     (V3D_BASE + (0x6f8)) // Performance Counter Count 15
#define V3D_PCTRS15                    (V3D_BASE + (0x6fc)) // Performance Counter Mapping 15

#define V3D_DBGE                       (V3D_BASE + (0xf00)) // PSE Error Signals
#define V3D_FDBGO                      (V3D_BASE + (0xf04)) // FEP Overrun Error Signals
#define V3D_FDBGB                      (V3D_BASE + (0xf08)) // FEP Interface Ready and Stall Signals, FEP Busy Signals
#define V3D_FDBGR                      (V3D_BASE + (0xf0c)) // FEP Internal Ready Signals
#define V3D_FDBGS                      (V3D_BASE + (0xf10)) // FEP Internal Stall Input Signals

#define V3D_ERRSTAT                    (V3D_BASE + (0xf20)) // Miscellaneous Error Signals (VPM, VDW, VCD, VCM, L2C)

#define V3D_CTnCS_CTRUN                0x20
#define V3D_CTnCS_CTRSTA               (1 << 15)

#define V3D_SHADER_INFO_MAX            128

typedef struct V3DShaderInfoFmt_t {
	union {
		struct {
			uint8_t flag_bits;
			uint8_t vertex_stride_bytes;
			uint8_t uniform_num;
			uint8_t varying_num;
		};
		uint32_t data;
	};
	uint32_t fshader_code_addr;
	uint32_t fshader_uniform_addr;
	uint32_t vertex_data_addr;
} V3DShaderInfoFmt;

typedef struct V3DContext_t {
	uint32_t width            ;
	uint32_t height           ;
	uint32_t blend_state      ;
	uint32_t primitive_type   ;
	uint32_t clear_color      ;
	uint32_t frame_buffer_addr;
	
	V3DShaderInfoFmt shader_info[V3D_SHADER_INFO_MAX];
	uint32_t shader_info_index;

	uint32_t handle           ;
	uint32_t bus_addr         ;
	uint32_t bus_addr_vc      ;
	uint32_t offset           ;
	uint32_t offset_start     ;
	uint32_t pointer          ;
	uint32_t offset_c0_start  ;
	uint32_t offset_c0_end    ;
	uint32_t offset_c1_start  ;
	uint32_t offset_c1_end    ;
	
	uint32_t vertex_handle    ;
	uint32_t vertex_addr      ;
	uint32_t vertex_offset    ;
	uint32_t vertex_count     ;
	
	uint32_t frame_count      ;
} V3DContext;



typedef struct V3DVertexFmt_t {
	union {
		struct {
			uint16_t x, y;
		};
		uint32_t xy;
	};
	float z;
	float w;
	
	//varying
	//todo u32
	float r;
	float g;
	float b;
	float a;
	
	float u;
	float v;
} V3DVertexFmt;



enum {
	V3D_OK = 0,
	V3D_NG,
	V3D_MAX = 0x7FFFFFFF,
};

enum {
	V3D_BLEND_NONE = 0,
	V3D_BLEND_BLEND,
	V3D_BLEND_ADD,
	V3D_BLEND_SUB,
	V3D_BLEND_MAX,
};

enum {
	V3D_VERTEX_ARRAY_PRIM_POINTS = 0,
	V3D_VERTEX_ARRAY_PRIM_LINES,
	V3D_VERTEX_ARRAY_PRIM_LINE_LOOP,
	V3D_VERTEX_ARRAY_PRIM_LINE_STRIP,
	V3D_VERTEX_ARRAY_PRIM_TRIANGLES,
	V3D_VERTEX_ARRAY_PRIM_TRIANGLE_STRIP,
	V3D_VERTEX_ARRAY_PRIM_MAX,
};
void V3DSetPrimitiveType(V3DContext *ctx, uint32_t type);
void V3DSetBlendState(V3DContext *ctx, uint32_t state);
void V3DCreateContext(V3DContext *ctx, uint32_t width, uint32_t height, uint32_t frame_buffer_addr, uint32_t bus_addr);
void V3DSetClearColor(V3DContext *ctx, uint32_t col);
uint32_t V3DBeginScene(V3DContext *ctx);
void V3DEndScene(V3DContext *ctx);
void V3DPresent(V3DContext *ctx);
void V3DSetDrawPrimitive(V3DContext *ctx, uint32_t vertex_addr,  uint32_t vertex_count, uint32_t type);


//internal

void V3DSetBusAddress(V3DContext *ctx, uint32_t bus_addr);
void V3DWrite8(V3DContext *ctx, uint8_t data);
void V3DWrite16(V3DContext *ctx, uint16_t data);
void V3DWrite32(V3DContext *ctx, uint32_t data);
void V3DFloat32(V3DContext *ctx, float data);
void V3DWrite32Align(V3DContext *ctx, uint32_t data);
void V3DFloat32Align(V3DContext *ctx, float data);
void V3DAlloc(V3DContext *ctx, uint32_t pointer);
void V3DFree(V3DContext *ctx);
void V3DLock(V3DContext *ctx);
void V3DUnlock(V3DContext *ctx);
void V3DNVVertexAdd(V3DContext *ctx, int16_t x, int16_t y, float z, float w, float r, float g, float b, float a, float u, float v);
void V3DSetOffset(V3DContext *ctx, uint32_t offset);
void V3DSaveOffset(V3DContext *ctx, uint32_t cn);
void V3DClearOffsetVertex(V3DContext *ctx);
void V3DSetOffsetBeginVertex(V3DContext *ctx);
void V3DSetOffsetEndVertex(V3DContext *ctx);

void V3DControlReset(V3DContext *ctx);
void V3DControlSetShaderInfo(V3DContext *ctx);
void V3DControlSetFragmentShader(V3DContext *ctx);
void V3DControlListSetupBinning(V3DContext *ctx);
void V3DControlListSetupRendering(V3DContext *ctx);
void V3DControlPresentBinning(V3DContext *ctx);
void V3DControlPresentRendering(V3DContext *ctx);

	
#endif //_V3D_H_

