#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/first_pass.h"
#include "../header_files/preproc.h"
#include "../header_files/utils.h"
#include "../header_files/labels.h"
#include "../header_files/coding.h"
#include "../header_files/errors.h"

int add_symbol(Symbol **symbols, int *count, const char *label, Type type, Mode mode, int address) {
    Symbol *temp = realloc(*symbols, (*count + 1) * sizeof(Symbol));
    if (temp == NULL) {
        print_error(ERROR11, current_filename, line_count);
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

int is_label_exists(Symbol *symbols, int count, char *label) 
{
    int i;
    /* Check if the label exists in the symbol table */
    for ( i = 0; i < count; i++) {
        if (strcmp(symbols[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_label_start(char *line) 
{
    char *ptr = line;/*pointer to the current position in the line*/
    int count = 0; /* Count of characters in the label */
    /* Skip whitespace */
    while (*ptr == ' ' || *ptr == '\t')
    {
        ptr++;
        count ++;
    }
        /* Check for label name */
    while (isalpha(*ptr) || isdigit(*ptr) || *ptr == '_')
    {
        ptr++;
        count++;
    }
    /* Check if the next character is a colon. if true, it's a label */
    if (*(ptr) == COLON)
        {
            count++; /* Count the colon */
            if(count == strlen(line) ) {
                return 1; /* It's a label */
            }
        }
    return 0;
}

void copy_label(char *dest, char *src) {
    char *ptr = src;
    while (ptr != NULL && *ptr != COLON && *ptr != '\0') {
        *dest++ = *ptr++;
    }
    *dest = '\0'; /* Null-terminate the destination string */
   
}

int valid_label(char *label) {
    /*valid label length check*/
    if (strlen(label) > MAX_LABEL_LENGTH) {
        print_error(ERROR12, current_filename, line_count);
        return 0; 
    }
    /* Check if the label is a macro name */
    if(find_mcro_body(mcro_head,label) != NULL) {
        print_error(ERROR13,current_filename,line_count);
        return 0;
    }

    if(label_is_reserved(label)) 
    {
        print_error(ERROR32,current_filename,line_count);
        return 0;
    }

    if(!isalpha(label[0])) {
        print_error(ERROR39, current_filename, line_count);
        return 0; /* Invalid label */
    }

    return 1; /* Valid label */
}


void print_symbols(Symbol *symbols, int count) /* למחיקה לפני הגשה */{
    int i;
    printf("Symbols table:\n");
    for (i = 0; i < count; i++) {
        printf("Label: %s, Type: %d, Address: %d, mode: %d\n",
            symbols[i].label, symbols[i].type, symbols[i].address, symbols[i].mode);
    }
}


int update_symbol_address(Symbol *symbols, int count, int ICF) {
    int i;
    /* Find the symbol in the table and update its address */
    for (i = 0; i < count; i++) 
    {
        if (symbols[i].mode == LABEL_DATA) {
            symbols[i].address += ICF;
        }
    }
    return 1; 
}

int update_symbol_type(Symbol *symbols, int count,char *label,  Type type) {
    int i;
    /* Find the symbol in the table and update its type */
    for (i = 0; i < count; i++) 
    {
        if (strcmp(symbols[i].label, label) == 0) {
            symbols[i].type = type;
            return 1; /* Success */
        }
    }
    return 0; 
}

int entry_count(Symbol *symbols, int count) {
    int i;
    int entry_count = 0;
    /* Count the number of entryal symbols */
    for (i = 0; i < count; i++) {
        if (symbols[i].type == LABEL_ENTRY) {
            entry_count++;
        }
    }
    return entry_count;
}

int extern_count(Symbol *symbols, int count) {
    int i;
    int extern_count = 0;
    /* Count the number of external symbols */
    for (i = 0; i < count; i++) {
        if (symbols[i].type == LABEL_EXTERN) {
            extern_count++;
        }
    }
    return extern_count;
}

void add100(Symbol *symbols, int count_labels)/*לעשות שתעלה 100 גם בשורות חסרות*/
{
    int i;
    /* Add 100 to the address of each symbol */
    for (i = 0; i < count_labels; i++) {
        symbols[i].address += 100;
    }
 }
 
int label_is_reserved(char *label) {
    /* Check if the label is a reserved word */
    
    if ( strcmp(label, "mov" ) == 0 || strcmp(label, "cmp") == 0 || strcmp(label, "add") == 0 ||
         strcmp(label, "sub") == 0 || strcmp(label, "not") == 0 || strcmp(label, "clr") == 0 ||
         strcmp(label, "lea") == 0 || strcmp(label, "inc") == 0 || strcmp(label, "dec") == 0 ||
         strcmp(label, "jmp") == 0 || strcmp(label, "bne") == 0 || strcmp(label, "jsr") == 0 ||
         strcmp(label, "red") == 0 || strcmp(label, "prn") == 0 ||
         strcmp(label, "rts") == 0 || strcmp(label, "stop") == 0 ||
         strcmp(label, ".data") == 0 || strcmp(label, ".string") == 0 ||
         strcmp(label, ".mat") == 0 || strcmp(label, ".entry") == 0 ||
         strcmp(label, ".extern") == 0 || strcmp(label, "mcro") == 0 ||
         strcmp(label, "mcroend") == 0  || strcmp(label, "data") == 0 || strcmp(label, "string") == 0 ||
         strcmp(label, "mat") == 0 || strcmp(label, "entry") == 0 ||
         strcmp(label, "extern") == 0 || strcmp(label, "r0") == 0 || strcmp(label, "r1") == 0 ||
         strcmp(label, "r2") == 0 || strcmp(label, "r3") == 0 || strcmp(label, "r4") == 0 ||
         strcmp(label, "r5") == 0 || strcmp(label, "r6") == 0 || strcmp(label, "r7") == 0)
            return 1;  /* It's a reserved word */
        return 0; /* Not a reserved word */
    }
    
void clear_symbols() {
    free(symbols);
    symbols = NULL; /* Set the pointer to NULL after freeing */
    count_labels = 0; /* Reset the count of symbols */
}


