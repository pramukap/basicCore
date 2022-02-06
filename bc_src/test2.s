.addr   0x0
main:
    // loop counter 0x0
    add.w   r1, r0, 0d0
    // bit mask 0x4
    add.w   r2, r0, 0d1
    // result 0x8
    add.w   r3, r0, 0d1
    // bit shift 0xc
    add.w   r4, r0, 0d1
    
    main_loop0:
        // 0x10
        shl.w   r2, r2, r4
        // 0x14
        or.w    r3, r3, r2
        // 0x18
        add.w   r1, r1, 0d1
        // 0x1c
        add.ws  r0, r1, -0d5
        // 0x20
        b.ne    main_loop0 

end_loop:
// 0x24
b.u     end_loop

