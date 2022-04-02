#---------------------------------------
#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------
nop                 ; nop
nop                 ; nop
mov  r0, vary       ; mov r3.8d, 1.0
fmul r0, r0, r5     ; mov r1, vary  ; sbwait
fmul r1, r1, r5     ; mov r2, vary
fmul r2, r2, r5     ; mov r3.8a, r0
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

