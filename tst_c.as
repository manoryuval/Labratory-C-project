; קובץ בדיקה עם מאקרו ולייבלים
END:        stop

.extern MAINjj
pTEND:        stop

MAIN:       .data     r3,  LENGTH
EEND:        stop

            add     LOOP[2][3],  STR
pEND:        stop
            .data     r3,  LENGTH

            prn     #5
            jsr     LOOP
            stop

LOOP:       .data     r3,  #0
            bne     END
            sub     r1,  r4
            bne     LOOP
            rts


STR:        .string "abcdef"
LENGTH:     .data   6

mcro M1
    mov r1, r2
    add r1, #0
mcroend

            M1
