# Assemble with qpuasm.js from github.com/hermanhermitage/videocoreiv-qpu

.global entry

entry:
        #R
        mov  r0, vary   ; nop
        fadd r0, r0, r5 ; nop; sbwait
        nop             ; mov  r3.8a, r0

        #G
        mov  r0, vary   ; nop
        fadd r0, r0, r5 ; nop
        nop             ; mov  r3.8b, r0

        #B
        mov  r0, vary   ; nop
        fadd r0, r0, r5 ; nop
        nop             ; mov  r3.8c, r0

        #A
        mov  r0, vary   ; nop
        fadd r0, r0, r5 ; nop
        nop             ; mov  r3.8d, r0

        #UV
        mov  r2, 0.0    ; nop
        nop             ; mov  unif_addr, r2
        mov  r2, 0.0    ; nop
        nop             ; mov  unif_addr_rel, r2

        fmul r0, vary, ra15
        fadd r0, r0, r5 ; nop
        
        fmul r1, vary, ra15
        fadd r1, r1, r5 ; nop
                
        mov  t0t, r1    ; nop
        mov  t0s, r0    ; nop
        nop             ; nop ; ldtmu0
        nop             ; nop ; sbwait
        
        mov  tlbc, r4   ; nop; thrend # write pixel colour to tile buffer; signal end of thread
        nop
        nop             ; nop; sbdone # Signal that we are finished with tile buffer
        