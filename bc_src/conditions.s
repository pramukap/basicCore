.addr 0x0

ld.ws   r1, data
b.gt    q2
b.u     fail

q2:
st.ws   r1, data
b.gt    q3
b.u     fail

q3:
add.ws  r1, r0, 0d1
b.gt    q4
b.u     fail

q4:
mul.ws  r1, r1, 0d1
b.gt    q5
b.u     fail

q5:
and.ws  r1, r1, r1
b.gt    q6
b.u     fail

q6:
or.ws   r1, r1, r1
b.gt    q7
b.u     fail

q7:
shl.ws  r1, r1, r1
b.gt    q8
b.u     fail

q8:
shr.ws  r1, r1, 0d1
b.gt    finish
b.u     fail

finish:
b.u finish

fail:
b.u fail

.addr 0x100
data:
.word 0x00000001
