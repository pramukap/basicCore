.addr 0x0
main:
    sc 0x0
    loop:
        b.u loop

.addr 0x100
sc_vect_table:
    .word 0x00004000

.addr 0x4000
    rfe 

