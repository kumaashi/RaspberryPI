#---------------------------------------
#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------
nop                 ; nop
nop                 ; nop

mov  r3, ra15       ; nop
nop                 ; mov r0, vary

#V0
fmul r0, r0, r3     ; nop
fadd r0, r0, r5     ; mov r1, vary  ; sbwait

#V1
fmul r1, r1, r3     ; nop
fadd r1, r1, r5     ; mov r2, vary

#V2
fmul r2, r2, r3     ; nop
nop                 ; mov r3.8d, 1.0
fadd r2, r2, r5     ; mov r3.8a, r0
nop                 ; mov r3.8b, r1
nop                 ; mov r3.8c, r2 ; loadc

#ADD
v8adds r3, r3, r4   ; nop

#export
mov tlbc, r3        ; nop           ; thrend
nop                 ; nop           ; nop
nop                 ; nop           ; sbdone

