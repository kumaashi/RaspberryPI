#---------------------------------------
#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------
nop                 ; nop
nop                 ; nop

#ignore uv rgb
mov r0, vary
mov r0, vary
mov r0, vary
mov r0, vary
mov r0, vary

#r0 = itof(depth24bit)
itof r0, rb15       ; nop

#r1 = (1.0 / 256.0)
mov  r1, 0x3B800000 ; nop 

#r0 / (1 << 24)
fmul r0, r0, r1     ; nop
fmul r0, r0, r1     ; nop
fmul r0, r0, r1     ; nop 

#tekito
fmul r0, r0, r0     ; nop
fmul r0, r0, r0     ; nop
fmul r0, r0, r0     ; nop
fmul r0, r0, r0     ; nop

#write to Z (TLB_Z)
mov tlb_z, rb15     ; nop

nop                 ; mov r3.8a, r0
nop                 ; mov r3.8b, r0
nop                 ; mov r3.8c, r0
nop                 ; mov r3.8d, 1.0

#export
mov tlbc, r3        ; nop           ; thrend
nop                 ; nop           ; nop
nop                 ; nop           ; sbdone

