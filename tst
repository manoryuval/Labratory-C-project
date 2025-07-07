; קובץ בדיקה עם מאקרו ולייבלים
.extern MAINjj
MAIN:       .data     r3,  LENGTH
            add     r2,  STR
            prn     #5
            jsr     LOOP
            stop

LOOP:       .data     r3,  #0
            bne     END
            sub     r1,  r4
            bne     LOOP
            rts

END:        stop

STR:        .string "abcdef"
LENGTH:     .data   6

mcro M1
    mov r1, r2
    add r1, #1
mcroend

            M1
