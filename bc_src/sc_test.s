.addr   0x0
func_add:
ldi.hs   r1, data
sc 0x0
loop0:
b.u     loop0

.addr 0x200
addi.hs  r3, r1, 0d1
rfe
b.lt    loop1
skip:
addi.h   r4, r0, 0d1
loop1:
b.u     loop1

.addr 0x50
data:
.hword 0xfffe

.addr   0x100
sc_vec_0:
.word  0x200



