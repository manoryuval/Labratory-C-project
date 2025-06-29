mcro     PRINTDEBUG
    MOV R0, #0xAA
    BL print_hex_val
    ; This is an inline comment within the macro body
    PUSH {LR}
mcroend

mcro CALCULATESUM
    ADD R1, R2, R3
    SUB R4, R1, #5
    ; Another line inside the macro
    LDR R0, [SP, #4]
mcroend

mcro EMPTY_MACRO
mcroend

mcro SHORT_MACRO
    NOP
mcroend

START_PROGRAM:
    PRINTDEBUG:
    
    CALCULATESUM
    CALCULATESUM: sss
    EMPTY_MACRO

    SHORT_MACRO

    LDR R7, [R8, #0]

    UNKNOWN_MACRO_CALL