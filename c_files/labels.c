#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_files/labels.h"


int add_symbol(Symbol **symbols, int *size, const char *label, int address, int is_data) {
    Symbol *temp = realloc(*symbols, (*size + 1) * sizeof(Symbol));
    if (temp == NULL) {
        return 0;
    }

    *symbols = temp;

    strncpy((*symbols)[*size].label, label, 30);
    (*symbols)[*size].label[30] = '\0'; 
    (*symbols)[*size].address = address;
    (*symbols)[*size].is_data = is_data;

    (*size)++;
    return 1; 
}
