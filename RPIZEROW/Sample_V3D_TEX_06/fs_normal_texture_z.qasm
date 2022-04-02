#---------------------------------------
#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------
nop                 ; nop
nop                 ; nop

# VideoCoreIV-AG100-R Section 6
# The varyings are interpolated using an equation of the form (A*(x-x0)+B*(y-y0))*W+C.
# The partial varying result VP=(A*(x-x0)+B*(y-y0)) is calculated in hardware,
# with VP*W+C calculated in QPU code.
#r5 = C
#r3 = w
mov  r3, ra15       ; nop

#v0
mov  r0, vary       ; nop
fmul r0, r0, r3     ; nop
fadd r0, r0, r5     ; mov r1, vary  ; sbwait

#v1
fmul r1, r1, r3     ; nop
fadd r1, r1, r5     ; mov r2, vary

#v2
fmul r2, r2, r3     ; nop
fadd r2, r2, r5     ; mov r3.8a, r0
nop                 ; mov r3.8b, r1
nop                 ; mov r3.8c, r2

#write to Z (TLB_Z)
mov tlb_z, rb15     ; nop

#fetch texture -> r4
mov t0t, r0         ; nop           ; nop
mov t0s, r1         ; nop           ; nop

#wait
nop                 ; nop           ; ldtmu0

#col = col * texcol;
v8muld r3, r3, r4   ; nop

#col = col + texcol;
v8adds r3, r3, r4   ; nop

#export
mov tlbc, r3        ; nop           ; thrend
nop                 ; nop           ; nop
nop                 ; nop           ; sbdone

