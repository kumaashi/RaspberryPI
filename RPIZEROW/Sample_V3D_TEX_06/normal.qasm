#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------
mov  r0, vary      ; mov r3.8d, 1.0
fadd r0, r0, r5    ; mov r1, vary ; sbwait
fadd r1, r1, r5    ; mov r2, vary
fadd r2, r2, r5    ; mov r3.8a, r0
nop                ; mov r3.8b, r1
nop                ; mov r3.8c, r2 ; loadc 

#write to Z (TLB_Z)
#mov tlb_z, rb15; nop
v8adds r3, r3, r4  ; nop

mov tlbc, r3       ; nop           ; thrend
nop                ; nop           ; nop
nop                ; nop           ; sbdone

