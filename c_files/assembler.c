#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_files/assembler.h"
#include "../header_files/preproc.h"
#include "../header_files/utils.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"
#include "../header_files/analyse.h"
#include "../header_files/coding.h"
/*int printword(char *token){
    switch (scan_word(token)) {
        case DATA:    return printf("DATA\n");
        case CODE:    return printf("CODE\n");
        case ENTRY:   return printf("ENTRY\n");
        case EXTERN:  return printf("EXTERN\n");
        case ARG_NUM: return printf("ARG_NUM\n");
        case ARG_REG: return printf("ARG_REG\n");
        case ARG_MAT: return printf("ARG_MAT\n");
        case LABEL:   return printf("LABEL\n");
        case STRING:  return printf("STRING\n");
        case NUMBER:  return printf("NUMBER\n");
        case MAT:     return printf("MAT\n");
        case UNKNOWN: return printf("UNKNOWN\n");
        default:      return printf("INVALID_TYPE\n");
    }
}*/

int main(int argc, char *argv[]) {
    int i;
    
    for(i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[i]);
            continue;
        }
        printf("Processing file>>>>>: %s\n", argv[i]);
        preproc(argv[i]);
        printf("Macros>>>>>:\n");
        print_mcro_list(mcro_head);
        printf("Running first pass on %s--------------------\n", argv[i]);
        first_pass(argv[i]);
        print_symbols(symbols, count_labels);

        printf("_____ print ICF__________\n");
        print_ICF(50);
        /*printf("_____ print DCF__________\n");
        print_DCF(20);*/
        fclose(file);
    }
   
    return 0;
}