#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/first_pass.h"
#include "../header_files/preproc.h"
#include "../header_files/utils.h"
#include "../header_files/labels.h"


int add_symbol(Symbol **symbols, int *count, const char *label, Type type, Mode mode, int address) {
    Symbol *temp = realloc(*symbols, (*count + 1) * sizeof(Symbol));
    if (temp == NULL) {
        perror("malloc failed"); /*להוסיף שגיאה*/
        return 0;
    }

    *symbols = temp;

    /*add symbol to table*/
    strncpy((*symbols)[*count].label, label, 30);
    (*symbols)[*count].label[30] = '\0'; 
    (*symbols)[*count].type = type;
    (*symbols)[*count].address = address;
    (*symbols)[*count].mode = mode;
    /* Increment the symbol count */
    (*count)++;
    return 1; 
}

int is_label_exists(Symbol *symbols, int count, char *label) {
    int i;
    /* Check if the label exists in the symbol table */
    for ( i = 0; i < count; i++) {
        if (strcmp(symbols[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_label_start(char *line) {
    char *ptr = line;/*pointer to the current position in the line*/
    /* Skip whitespace */
    while (*ptr == ' ' || *ptr == '\t')
        ptr++;
    /* Check for label name */
    while (isalpha(*ptr))
        ptr++;
    /* Check if the next character is a colon. if true, it's a label */
    if (*(ptr) == COLON)
        return 1;
    return 0;
}

int valid_label(char *label) {
    /*valid label length check*/
    if (strlen(label) > MAX_LABEL_LENGTH) {
        printf("Error: Label '%s' is too long.\n", label);
        return 0; /* Label too long שגיאה*/
    }
    /* Check if the label is a macro name */
    if(find_mcro_body(mcro_head,label) != NULL) {
        printf("Error: Label '%s' is a macro name.\n", label);
        return 0; /* Label is a macro  שגיאה*/
    }

    return 1; /* Valid label */
}


void print_symbols(Symbol *symbols, int count) {
    int i;
    printf("Symbols table:\n");
    for (i = 0; i < count; i++) {
        printf("Label: %s, Type: %d, Address: %d, data: %d\n",
               symbols[i].label, symbols[i].type, symbols[i].address, symbols[i].mode);
    }
}

void free_symbols(Symbol *symbols) {
    free(symbols);
}

