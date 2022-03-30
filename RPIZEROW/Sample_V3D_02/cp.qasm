#3322 2222 2222 1111 1111 1100 0000 0000
#1098 7654 3210 9876 5432 1098 7654 3210
#---------------------------------------

#mov r0.8d, 0.5 ; nop
#mov r0.8c, 1.0 ; nop
#mov r0.8b, 0.5 ; nop
#mov r0.8a, 1.0 ; nop
#
#mov tlbc, r0 ; nop
nop; nop; nop;
ldi tlbc, 0xFFFFFFFF

nop ; nop ; sbdone
nop ; nop ; thrend

nop; nop; nop;
nop; nop; nop;

