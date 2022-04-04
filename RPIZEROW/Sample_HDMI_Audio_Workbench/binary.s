.org 0x20000
.balign 256
.global ipaaddr
ipaaddr:
.incbin "./asset/SevillaAlbeniz.wav"

.global ipaaddr_al
ipaaddr_al:
.incbin "./asset/orientex.wav"

.global ipaaddr_size
.set ipaaddr_size, . - ipaaddr

