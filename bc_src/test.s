.addr   0x0
func_add:
ld.hs   r1, data
b.gt    skip
add.hs  r3, r1, 0d1
b.lt    loop
skip:
add.h   r4, r0, 0d1
loop:
b.u     loop

.addr   0x100
data:
.hword  0xfffe


