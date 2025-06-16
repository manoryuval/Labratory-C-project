#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preproc.h"
#include "utils.h"


int main(int argc, char *argv[]) {
    int i;
    
    for(i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[i]);
            continue;
        }
        preproc(argv[i]);
        fclose(file);
    }
    return 0;
}