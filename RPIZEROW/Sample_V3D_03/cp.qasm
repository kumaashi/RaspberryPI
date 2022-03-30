#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------

#mov r0.8d, 0.5 ; nop
#mov r0.8c, 1.0 ; nop
#mov r0.8b, 0.5 ; nop
#mov r0.8a, 1.0 ; nop
#
#mov tlbc, r0 ; nop

        #nop
        #.long 0xd1f24823958e0dbf
        #fadd r0, r0, r5
        #mov r1, vary;  sbwait
        #fadd r1, r1, r5
        #mov r2, vary
        #fadd r2, r2, r5
        #.long 0x11c049e3809e7000
        #.long 0x11d049e3809e7009
        #.long 0x11e049e3809e7012
        #mov tlbc, r3;  thrend
        #nop
        #nop;  sbdone
        #nop;  sbdone
        #nop;  thrend
        #nop
        #nop
mov r0, vary;mov r3.8d, 1.0
fadd r0, r0, r5; mov r1, vary ; sbwait
fadd r1, r1, r5; mov r2, vary
fadd r2, r2, r5; mov r3.8a, r0

nop ; mov r3.8b, r1
nop ; mov r3.8c, r2
mov tlbc, r3 ; nop ; thrend
nop; nop; nop

nop ; nop ; sbdone

