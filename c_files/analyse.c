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
#include "../header_files/coding.h"
#include "../header_files/errors.h"


/* array of opcodes */
op_code OPCODES[] = {
        {"mov",  2, "aa"},
        {"cmp",  2, "ab"},
        {"add",  2, "ac"},
        {"sub",  2, "ad"},
        {"lea",  2, "ba"},
        {"clr",  1, "bb"},
        {"not",  1, "bc"},
        {"inc",  1, "bd"},
        {"dec",  1, "ca"},
        {"jmp",  1, "cb"},
        {"bne",  1, "cc"},
        {"jsr",  1, "cd"},
        {"red",  1, "da"},
        {"prn",  1, "db"},
        {"rts",  0, "dc"},
        {"stop", 0, "dd"},
};

/* array of registers */
regs_code REGS[] = {
    {"r0", "aa"},
    {"r1", "ab"},
    {"r2", "ac"},
    {"r3", "ad"},
    {"r4", "ba"},
    {"r5", "bb"},
    {"r6", "bc"},
    {"r7", "bd"}
};

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
        if(!isalpha(token[i]) && !isdigit(token[i])) {
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

char *get_opcode_code(char *token) {
    int i;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(token, OPCODES[i].opcode) == 0) {
            /*if opcode found, return opcode code*/
            return OPCODES[i].code;
        }
    }
    return NULL;
}

char *get_register_name(char *token) {
    int i;
    for (i = 0; i < sizeof(REGS) / sizeof(REGS[0]); i++) {
        if (strcmp(token, REGS[i].reg) == 0) {
            return REGS[i].reg;
        }
    }
    return NULL;
}

char *get_register_code(char *token) {
    int i;
    if (!token || strlen(token) == 0) {
        return NULL; /* Check for NULL or empty token */
    }
    for (i = 0; i < sizeof(REGS) / sizeof(REGS[0]); i++) {
        if (strcmp(token, REGS[i].reg) == 0) {
            return REGS[i].code;
        }
    }
    return NULL;
}

int is_matrix_operand( char *str) {
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

char *get_matrix_name(char *str) {
    int matched;
    char name[31];
    char *result;
    matched = sscanf(str, "%30[^[]", name);

    if (matched != 1) {
        return NULL;
    }

    /* הקצאת זיכרון חדש והחזרת עותק */
    result = malloc(strlen(name) + 1);
    if (!result) return NULL;
    strcpy(result, name);
    return result;
}

char *get_reg1_matrix_operand( char *str) {
    int matched;
    char reg1[4];
    remove_spaces(str); /* Remove spaces from the string */
    /* printf("\n\n%s\n\n", str); */
    /* Check for matrix operand format */
    matched = sscanf(str, "%*[^[][%3[^]]]", reg1);
    /* Check for valid register */
    if (matched != 1 || !get_register_name(reg1)) {
        return NULL; 
    }
    return get_register_name(reg1); /* Return the register name */
}

char *get_reg2_matrix_operand( char *str) {
    int matched;
    char reg2[4];
    /* Check for matrix operand format */
    matched = sscanf(str, "%*[^[][%*[^]]][%3[^]]]", reg2);/*לא עובד*/
    /* Check for valid register */
    if (matched != 1 )
        return NULL;
    if(!get_register_name(reg2))             
        return NULL; 
    
    return get_register_name(reg2); /* Return the register name */
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

    return num1*num2;
}

int get_opcode_arg(char *token) {
    int i;
    for (i = 0; i < sizeof(OPCODES) / sizeof(OPCODES[0]); i++) {
        if (strcmp(token, OPCODES[i].opcode) == 0) {
            /*if opcode found, return opcode name*/
            return OPCODES[i].arg_num;
        }
    }
    return -1;
}



DataType get_data_kind(char *token) {
    if (strcmp(token, ".data") == 0) {
        return DATA_;
    } else if (strcmp(token, ".string") == 0) {
        return STRING_;
    } else if (strcmp(token, ".mat") == 0) {
        return MAT_;
    }
    return UNKNOWN_;
}




void add_missing_line(int line, char *label, missing_line **head)

{
    /*check if label exists*/
    
    if (*head == NULL)
    {
        *head = malloc(sizeof(missing_line));
        (*head)->line = line;
        (*head)->label = malloc(strlen(label) + 1);
        strcpy((*head)->label, label);
        (*head)->next = NULL;
        (*head)->prev = NULL; /* Initialize prev pointer to NULL */
    }
    else
    {
        missing_line *current = *head;
        while (current->next != NULL) 
        {
            current = current->next;
        }
        current->next = malloc(sizeof(missing_line));
        current->next->line = line;
        current->next->label = malloc(strlen(label) + 1);
        current->next->prev = current; 
        strcpy(current->next->label, label);
        current->next->next = NULL;
    }
}

void print_missing_lines(missing_line *head)
{
    missing_line *current = head;
    if(current == NULL) {
        printf("No missing lines.\n");
        return;
    }
    printf("Missing lines:\n");
    while (current != NULL) {
        printf("Line: %d, Label: %s\n", current->line, current->label);
        current = current->next;
    }
}

void remove_node(missing_line **head_ref, missing_line *current) {
    if (current == NULL) return;

    if (current->prev == NULL && current->next == NULL) {
        /* צומת בודד ברשימה */
        *head_ref = NULL;
    } else if (current->prev == NULL) {
        /* הסרה מראש הרשימה */
        *head_ref = current->next;
        (*head_ref)->prev = NULL;
    } else if (current->next == NULL) {
        /* הסרה מהסוף */
        current->prev->next = NULL;
    } else {
        /* הסרה באמצע */
        current->next->prev = current->prev;
        current->prev->next = current->next;
    }

    free(current->label);
    free(current);
}

int update_missing_lines(missing_line *head/*, extern_line **extern_lines*/ , Symbol *symbols, int count)
{
    missing_line *current = head;
    
    int i, updated = 0;
    while (current != NULL) {
        if (is_label_exists(symbols, count, current->label)) {
            /* If the label exists, update the address */
            for (i = 0; i < count; i++) {
                if (strcmp(symbols[i].label, current->label) == 0) {
                    if( symbols[i].type == LABEL_EXTERN) {
                        extern_to_code(current->line);
                    } else {
                    line_to_code(symbols[i].address, current->line,'I'); /* Update the address in IC */
                    }
                    updated++;
                    break;
                }
            }
        }
        current = current->next;
    }
    if (updated > 0) {
        
        remove_node(&head, current); 
    } else {
        printf("No missing lines were updated.\n");/*שגיאה*/
    }
    return updated; /* Return the number of updated labels */
}

void clear_missing_lines()
{
    missing_line *current = missing_lines;
    while (current != NULL) {
        missing_line *temp = current;
        current = current->next;
        free(temp->label);
        free(temp);
    }
    missing_lines = NULL; /* Reset the head pointer */
}

int is_valid_argument(char *cmd, int arg_num, WordType type) {
    
    switch(type) {
        case ARG_NUM:
            if ((arg_num == 0) && (strcmp(cmd, "mov") == 0 || strcmp(cmd, "cmp") == 0 || strcmp(cmd, "add") == 0 || strcmp(cmd, "sub") == 0 || strcmp(cmd, "prn") == 0)) {
                return 1;
            }
            if ((arg_num == 1) && (strcmp(cmd, "cmp") == 0 )) {
                return 1;
            }
            break;
        case ARG_REG:
            if ((arg_num == 0) && (strcmp(cmd, "lea") != 0 )) {
                return 1;
            }
            if ((arg_num == 1) && (strcmp(cmd, "cmp") == 0 )) {
                return 1;
            }
            break;
        case ARG_MAT:
            return 1;            
            break;
        case LABEL:
            return 1;
            break;
        default:
            return 0; /* Invalid argument type */
        }
    return 0; /* Invalid argument type */
}