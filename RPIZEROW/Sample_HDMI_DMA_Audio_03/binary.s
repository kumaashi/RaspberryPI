.org 0x20000
.balign 256
.global ipaaddr
ipaaddr:
.incbin "SevillaAlbeniz.wav"

.global ipaaddr_size
.set ipaaddr_size, . - ipaaddr

.balign 256
.global sound_buffer
sound_buffer:
