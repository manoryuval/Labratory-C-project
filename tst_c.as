; קובץ בדיקה עם מאקרו ולייבלים
END:        stop

STR12: .extern MAINjj
pTEND:        stop

MAIN:       .data     7,8
EEND:        stop

             add     STR,  STR
pEND:        stop
            .data    67,5

            prn     #5
            jsr     LOOP
            stop

LOOP:       .data     4
            bne     END
            sub     r1,  r4
            bne     LOOP
            rts


STR:        .string "abcdef"
LENGTH:     .data   6

mcro M1
    mov r1, r2
    add r1, r5
mcroend

            M1
