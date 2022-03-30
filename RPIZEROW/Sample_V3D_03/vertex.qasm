mov vr_setup, 0x00000000
mov ra0, vpm #X
mov ra1, vpm #Y
mov ra2, vpm #Z
mov -, vr_wait

#to coodinate shader
mov vw_setup, (2 << 30) | (7 << 23) | (1 << 16) | (1 << 14)
mov vpm, ra0
mov vpm, ra1
mov vpm, ra2
mov vpm, ra0
mov vpm, ra1
mov vpm, 1.0 #ra2
mov vpm, 1.0 #ra2
mov -, vw_wait

#end
nop; thrend
nop
nop

