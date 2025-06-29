#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_files/assembler.h"
#include "../header_files/preproc.h"
#include "../header_files/utils.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"


int main(int argc, char *argv[]) {
    int i;
    
    for(i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[i]);
            continue;
        }
        preproc(argv[i]);
        
        first_pass(argv[i]);
        fclose(file);
    }
    /*first_pass(argv[1]);*/
    return 0;
}