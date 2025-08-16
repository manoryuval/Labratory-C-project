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

/* Current file name */
char *current_filename= NULL;


void clear()
{
    clear_IC_DC();
    clear_symbols();
    clear_missing_lines();
    clear_macros();
    error_count = 0;
}

int main(int argc, char *argv[]) {
    int i;
    /* Run through all input files */
    for(i = 1; i < argc; i++) 
    {
        /* Create output file names */
        char *as_name = create_extension(argv[i], ".as");
        char *ent_file = create_extension(argv[i], ".ent");
        char *ext_file = create_extension(argv[i], ".ext");
        char *ob_file = create_extension(argv[i], ".ob");
        FILE *file = NULL;
        current_filename = argv[i];
        printf("Processing file: %s\n", as_name);
        file = fopen(as_name, "r");
        if (!file) {/* If .as file not found */
            print_error(ERROR20, current_filename, 0);
            remove(ent_file);
            remove(ext_file);
            remove(ob_file);
            free(ent_file);
            free(ext_file);
            free(ob_file);
            free(as_name);
            clear();
            printf("\n");
            continue;
        }
        /* Run the preprocessing stage */
        preproc(as_name);
        /* Run the first pass */
        first_pass(as_name);
        /* Run the second pass */
        second_pass(as_name);
        /* If errors were found, remove output files */
        if(error_count > 0) {
            remove(ent_file);
            remove(ext_file);
            remove(ob_file);
            printf("Errors found in file %s, Skipping output files.\n\n", argv[i]);
        }
        else {/* If no errors were found, generate output files*/
            fprint_ICF(argv[i], ICF+DCF);
            printf("No errors found in file %s, Generating output files.\n\n", argv[i]);
        }
        /* Clear all global variables and structures */
        clear();
        free(ent_file);
        free(ext_file);
        free(ob_file);
        free(as_name);
        fclose(file);
       
    }
    
    return 0;
}