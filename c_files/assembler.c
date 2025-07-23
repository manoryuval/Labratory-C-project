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
#include "../header_files/second_pass.h"
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
        char *as_name=NULL;
        FILE *file = NULL;
        printf("file name: %s\n", argv[i]);
        as_name = create_extension(argv[i], ".as");
        printf("Processing file: %s\n", as_name);
        file = fopen(as_name, "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s\n", as_name);/*שגיאה*/
            continue;
        }
        /*printf("Processing file>>>>>: %s\n", argv[i]);*/
        preproc(as_name);
        /*printf("Macros>>>>>:\n");
        print_mcro_list(mcro_head);
        printf("Running first pass on %s--------------------\n", argv[i]);*/
        first_pass(as_name);

        printf("_____ print ICF__________\n");
        printf("ICF: %d ,DCF: %d\n", ICF, DCF);
        second_pass(as_name);
        /*print_ICF(ICF+DCF);*/
        fprint_ICF(argv[i], ICF+DCF);
        
        print_symbols(symbols, count_labels);


        /*printf("_____ print DCF__________\n");
        print_DCF(20);*/
        fclose(file);
    }
   
    return 0;
}