.addr 0x0

add.hs r0, r0, 0d1
add.hs r1, r0, 0d2
b.lu add_num
end:
b.u end

add_num:
    add.hs r0, r1, r2
    b.r

    
