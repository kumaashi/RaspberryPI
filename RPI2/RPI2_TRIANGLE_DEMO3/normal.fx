# Assemble with qpuasm.js from github.com/hermanhermitage/videocoreiv-qpu

.global entry

entry:
        mov  r0, vary  ; mov r3.8d, 1.0       # Load red varying AB partial to r0; set colour alpha to 1.0
        fadd r0, r0, r5; mov r1, vary; sbwait # add C component of red varying (r5); load green varying AB partial; Request access to tile buffer
        fadd r1, r1, r5; mov r2, vary         # add C component of green varying; load blue varying AB partial
        fadd r2, r2, r5; mov r3.8a, r0        # add C component of blue varying; pack red varying to color
        nop; mov r3.8b, r1                    # pack green varying to color
        nop; mov r3.8c, r2                    # pack blue varying to color
        mov r2, vary; nop
        mov r2, vary; nop
        mov r2, vary; nop
        mov tlbc, r3; nop; thrend             # write pixel colour to tile buffer; signal end of thread
        nop
        nop; nop; sbdone                      # Signal that we are finished with tile buffer
        