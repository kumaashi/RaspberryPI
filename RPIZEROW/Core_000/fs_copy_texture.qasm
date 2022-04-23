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

#uv
mov  r0, vary       ; nop           ; sbwait
fmul r0, r0, r3     ; nop
fadd r0, r0, r5     ; nop
mov t0t, r0         ; nop           ; nop

mov  r0, vary       ; nop
fmul r0, r0, r3     ; nop
fadd r0, r0, r5     ; nop
mov t0s, r0         ; nop           ; nop

#ignore rgb
mov r0, vary        ; nop
mov r0, vary        ; nop
mov r0, vary        ; nop
nop                 ; nop           ; ldtmu0

#COPY
mov r3, r4          ; nop

#DEBUG
#mov r3.8a, 1.0      ; nop
#mov r3.8b, 0.0      ; nop
#mov r3.8c, 1.0      ; nop
#mov r3.8d, 1.0      ; nop

#export
mov tlbc, r3        ; nop           ; thrend
nop                 ; nop           ; nop
nop                 ; nop           ; sbdone

