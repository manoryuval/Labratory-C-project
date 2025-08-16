MAIN: mov M1[ r2 ][ r7 ],Wmjh
add r2,STR
prn #-5
sub r1, r4
inc K 
mov M1[r3][r3],r3
bne L3
END: stop
M1: .mat [ 4   ][ 3 ] 1,2,3,3
STR: .string   "abcde"
LENGTH: .data 6,-9,15
K: .data 22
.entry LENGTH
.extern L3
.extern W


