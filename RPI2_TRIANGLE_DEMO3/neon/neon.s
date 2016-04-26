.global _fsqrt
_fsqrt:
    vldr.f32    s0, [r0]
    vsqrt.f32   s0, s0
    vmov.f32    r0, s0
	mov         pc, lr

