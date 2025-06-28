#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/first_pass.h"
#include "../header_files/preproc.h"
#include "../header_files/utils.h"
#include "../header_files/labels.h"


int add_symbol(Symbol **symbols, int *count, const char *label, Type type, Address address, int is_data) {
    Symbol *temp = realloc(*symbols, (*count + 1) * sizeof(Symbol));
    if (temp == NULL) {
        free(*symbols);
        *symbols = NULL;
        return 0;
    }

    *symbols = temp;

    strncpy((*symbols)[*count].label, label, 30);
    (*symbols)[*count].label[30] = '\0'; 
    (*symbols)[*count].type = type;
    (*symbols)[*count].address = address;
    (*symbols)[*count].is_data = is_data;

    (*count)++;
    return 1; 
}

int is_label_exists(Symbol *symbols, int count, const char *label) {
    int i;
    for ( i = 0; i < count; i++) {
        if (strcmp(symbols[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_label_start(char *line) {
    char *ptr = line;
    while (*ptr == ' ' || *ptr == '\t')
        ptr++;
    while (isalpha(*ptr))
        ptr++;

    if (*(ptr) == COLON)
        return 1;
    return 0;
}

int valid_label(char *label) {
    if (strlen(label) > MAX_LABEL_LENGTH) {
        return 0; /* Label too long שגיאה*/
    }
    if(find_mcro_body(mcro_head,label) != NULL) {
        return 0; /* Label is a macro  שגיאה*/
    }
    return 1; /* Valid label */
}

void free_symbols(Symbol *symbols) {
    free(symbols);
}

