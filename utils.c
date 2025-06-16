#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "preproc.h"
#include "utils.h"
    
char *create_extension(char *filename, char *extension) {
    char *output_filename = malloc(FILENAME_MAX);
    if (!output_filename) return NULL; /*שגיאת לזיכרון*/
    strcpy(output_filename, filename);
    char *dot = strrchr(output_filename, '.');
    printf("%s,%s,%s\n",filename,extension,dot);
    if (dot) {
        strcpy(dot, extension);
    } else {
        strcat(output_filename, extension);
    }
    return output_filename;
}