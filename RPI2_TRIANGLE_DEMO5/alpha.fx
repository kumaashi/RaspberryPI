# ALPHA BLEND
# todo opt

.global entry

entry:
        nop                ; mov r3.8d, 1.0
        mov  ra5, 0.5      ; nop
        nop                ; nop            ; sbwait
        fsub r2, 1.0, ra5  ; nop
        mov  r0, vary      ; nop
        fadd r0, r0,  r5   ; nop
        fmul r0, r0,  ra5  ; nop            ; loadc
        mov  r1, r4.8a     ; nop
        fmul r1, r1,  r2   ; nop
        fadd r0, r0,  r1   ; nop 
        fmin r0, r0,  1.0  ; nop  # clamp
        fmax r0, r0,  0.0  ; nop  # clamp
        nop                ; mov  r3.8a, r0 
        
        mov  r0, vary      ; nop
        fadd r0, r0,  r5   ; nop
        fmul r0, r0,  ra5  ; nop
        mov  r1, r4.8b     ; nop
        fmul r1, r1,  r2   ; nop
        fadd r0, r0,  r1   ; nop 
        fmin r0, r0,  1.0  ; nop  # clamp
        fmax r0, r0,  0.0  ; nop  # clamp
        nop                ; mov  r3.8b, r0 

        mov  r0, vary      ; nop
        fadd r0, r0,  r5   ; nop
        fmul r0, r0,  ra5  ; nop
        mov  r1, r4.8c     ; nop
        fmul r1, r1,  r2   ; nop
        fadd r0, r0,  r1   ; nop 
        fmin r0, r0,  1.0  ; nop  # clamp
        fmax r0, r0,  0.0  ; nop  # clamp
        nop                ; mov  r3.8c, r0 

        mov  tlbc, r3      ; nop           ; thrend
        nop                ; nop
        nop                ; nop           ; sbdone
        