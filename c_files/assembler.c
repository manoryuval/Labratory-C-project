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
#include "../header_files/errors.h"

char *current_filename= NULL;

void clear()
{
    clear_IC_DC();
    clear_symbols();
    clear_missing_lines();
    clear_macros();
    
}

int main(int argc, char *argv[]) {
    int i;
    
    for(i = 1; i < argc; i++) {
        char *as_name=NULL;
        char *ent_file = create_extension(argv[i], ".ent");
        char *ext_file = create_extension(argv[i], ".ext");
        char *ob_file = create_extension(argv[i], ".ob");
        FILE *file = NULL;
        current_filename = argv[i];
        printf("file name: %s\n", argv[i]);
        as_name = create_extension(argv[i], ".as");
        printf("Processing file: %s\n", as_name);
        file = fopen(as_name, "r");
        if (!file) {
            print_error(ERROR1, current_filename, 0); 
            continue;
        }
        preproc(as_name);
        first_pass(as_name);
        second_pass(as_name);
        if(error_count > 0) {
            remove(ent_file);
            remove(ext_file);
            remove(ob_file);
            printf("Errors found in file %s. Skipping output files.\n", argv[i]);
        }
        else{
            fprint_ICF(argv[i], ICF+DCF);
            printf("No errors found in file %s. Generating output files.\n\n", argv[i]);
        }
        /*printf("_____ print ICF__________\n");
        printf("ICF: %d ,DCF: %d\n", ICF, DCF);*/
        /*print_symbols(symbols, count_labels);*/
        clear();
        fclose(file);
    }
   
    return 0;
}