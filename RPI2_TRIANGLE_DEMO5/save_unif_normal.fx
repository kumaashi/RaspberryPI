# Assemble with qpuasm.js from github.com/hermanhermitage/videocoreiv-qpu

.global entry

entry:
        nop             ; nop
        mov  ra5, 0.5   ; nop            #
        fsub r2, 1.0, ra5  ; nop
        nop             ; nop; sbwait
        mov  r1, unif   ; nop
        nop             ; nop
        mov  unif_addr, r1

        mov  r0, vary   ; nop
        mov  r0, vary   ; nop
        mov  r0, vary   ; nop
        mov  r0, vary   ; nop
        #mov  tmurs,  1  ; nop
        #U
        mov  r0, vary   ; nop
        fadd r0, r0, r5 ; nop
        mov  ra0, r0    ; nop

        #V
        mov  r1, vary   ; nop
        fadd r1, r1, r5 ; nop
        mov  ra1, r1    ; nop
        
        mov  t0t, r1    ; nop #U
        mov  t0s, r0    ; nop #V

        nop             ; nop ; ldtmu0
        
        #alpha blend
        fmul r0, r4.8a, r2     ; nop
        nop             ; mov  r3.8a, r0
        
        fmul r0, r4.8b, r2     ; nop
        nop             ; mov  r3.8b, r0

        fmul r0, r4.8c, r2     ; nop
        nop             ; mov  r3.8c, r0
        
        #mov  r3, r4     ; nop
        
        #nop             ; mov  r0, r4.8a
        #nop             ; mov  r1, r4.8b
        #fadd r0, ra0, r0 ; nop
        #fadd r1, ra1, r1 ; nop
        #nop             ; mov  r3.8a, r0
        #nop             ; mov  r3.8b, r1
        #mov tlbz, rb15  ; nop
        mov  tlbc, r3   ; nop; thrend
        nop
        nop             ; nop; sbdone
