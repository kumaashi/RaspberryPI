# Assemble with qpuasm.js from github.com/hermanhermitage/videocoreiv-qpu

.global entry

entry:
        nop             ; nop
        nop             ; nop; sbwait
        mov  r1, unif   ; nop
        nop             ; nop
        mov  unif_addr, r1

        mov  r0, vary   ; nop
        mov  r0, vary   ; nop
        mov  r0, vary   ; nop
        mov  r0, vary   ; nop

        #U
        mov  r0, vary   ; nop
        fadd r0, r0, r5 ; nop

        #V
        mov  r1, vary   ; nop
        fadd r1, r1, r5 ; nop
        
        #U
        mov  t0t, r1    ; nop

        #V
        mov  t0s, r0    ; nop
        
        #Get Texture Color
        nop             ; nop ; ldtmu0
        mov   ra0, r4.8a; nop
        mov   ra1, r4.8b; nop
        mov   ra2, r4.8c; nop
        
        #Get Back Color
        nop             ; nop ; loadc
        
        mov  ra5, 0.25  ; nop
        fsub r2, 1.0, ra5  ; nop
        
        #Blend 0
        mov  r0, r4.8a  ; nop
        fmul r0, r0, ra5 ; nop
        fmul r1, ra0, r2 ; nop
        fadd r0, r0, r1 ; nop
        nop             ; mov  r3.8a, r0
        
        #Blend 1
        mov  r0, r4.8b  ; nop
        fmul r0, r0, ra5 ; nop
        fmul r1, ra1, r2 ; nop
        fadd r0, r0, r1 ; nop
        nop             ; mov  r3.8b, r0
        
        #Blend 2
        mov  r0, r4.8c  ; nop
        fmul r0, r0, ra5 ; nop
        fmul r1, ra2, r2 ; nop
        fadd r0, r0, r1 ; nop
        nop             ; mov  r3.8c, r0
        
        mov  tlbc, r3   ; nop ; thrend
        nop             ; nop
        nop             ; nop ; sbdone
