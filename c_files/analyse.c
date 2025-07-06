#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/utils.h"
#include "../header_files/preproc.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"
#include "../header_files/analyse.h"

/* array of opcodes */
op_code OPCODES[] = {
        {"mov",  2},
        {"cmp",  2},
        {"add",  2},
        {"sub",  2},
        {"lea",  2},
        {"clr",  1},
        {"not",  1},
        {"inc",  1},
        {"dec",  1},
        {"jmp",  1},
        {"bne",  1},
        {"jsr",  1},
        {"red",  1},
        {"prn",  1},
        {"rts",  0},
        {"stop", 0},
};

/* array of registers */
char *REGS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

WordType scan_word(char *token) {
    int token_length, i;
    /* Check for NULL or empty token */
    if(!token || strlen(token) == 0) {
        return UNKNOWN; 
    }
    /* Get the length of the token */
    token_length= strlen(token);
    /* Check for data directives */
    if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || strcmp(token, ".mat") == 0) {
        return DATA;
    }
    /* Check for entry directive */
    if (strcmp(token, ".entry") == 0) {
        return ENTRY;
    }
    /* Check for extern directive */
    if (strcmp(token, ".extern") == 0) {
        return EXTERN;
    }
    /* Check for opcode */
    if(get_opcode_name(token)) {
        return CODE;
    }
    /* Check for register */
    if(get_register_name(token)) {
        return ARG_REG;
    }
    /* Check for immediate value */
    if(token[0] == HASH) {
        if(token[1] == MINUS || token[1] == PLUS || isdigit(token[1]) ) {
            for(i = 2; i < token_length; i++) {
                if(!isdigit(token[i])) {
                    return UNKNOWN;
                }
            }
            return ARG_NUM;
        }
    }
    /* Check for number */
    if(token[0] == MINUS || token[0] == PLUS || isdigit(token[0])) {
            for(i = 1; i < token_length; i++) {
                if(!isdigit(token[i])) {
                    return UNKNOWN;
                }
            }
            return NUMBER;
        }
    /* Check for string */
    if(token[0] == '"' && token[token_length - 1] == '"') {
        return STRING;
    }
    /*check for matrix operand*/
    if(is_matrix_operand(token)) {
        return ARG_MAT;
    }
    /*check for matrix definition*/
    if(is_matrix_definition(token)) {
        return MAT;
    }
    /* Check for label */
    for(i = 0; i < token_length; i++) {
        if(!isalpha(token[i])) {
            return UNKNOWN;
        }
    } return LABEL;
    /* If none of the above, return UNKNOWN */
    return UNKNOWN;
}

char *get_opcode_name(char *token) {
    int i;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(token, OPCODES[i].opcode) == 0) {
            /*if opcode found, return opcode name*/
            return OPCODES[i].opcode;
        }
    }
    return NULL;
}

char *get_register_name(char *token) {
    int i;
    for (i = 0; i < sizeof(REGS) / sizeof(REGS[0]); i++) {
        if (strcmp(token, REGS[i]) == 0) {
            return REGS[i];
        }
    }
    return NULL;
}

int is_matrix_operand(const char *str) {
    int matched;
    char name[31], reg1[4], reg2[4], extra;
    /* Check for matrix operand format */
    matched = sscanf(str, "%30[^[][%3[^]]][%3[^]]]%c", name, reg1, reg2, &extra);
    /* Check for extra characters */
    if (matched != 3) {
        return 0; 
    }
    /* Check for valid registers */
    if (!get_register_name(reg1) || !get_register_name(reg2)) {
        return 0;
    }

    return 1;
}

int is_matrix_definition(const char *str) {
    int num1, num2;
    char temp;
    /*Check for matrix definition format*/
    int matched = sscanf(str, "[%d][%d]%c", &num1, &num2, &temp);

    /* Check for valid dimensions */
    if (matched != 2) return 0;
    /* Check for valid ranges */
    if (num1 < 0 || num2 < 0 ) {
        return 0;
    }

    return 1;
}