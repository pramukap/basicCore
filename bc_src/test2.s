.addr   0x0
main:
    add.w   r1, r0, 0d0
    add.w   r2, r0, 0d1
    add.w   r3, r0, 0d0
    
    main_loop0:
        shl.w   r2, r2, r1
        or.w    r3, r3, r2
        add.w   r1, r1, 0d1
        add.ws  r0, r1, -0d5
        b.ne    main_loop0 

end_loop:
b.u     end_loop

