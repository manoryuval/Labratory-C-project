#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "preproc.h"
#include "utils.h"

char *create_extension(char *filename, char *extension) {

    char *output_filename = malloc(FILENAME_MAX);
    if (!output_filename) return NULL; /*שגיאת לזיכרון*/
    strcpy(output_filename, filename);
    char *dot = strrchr(output_filename, '.');
    if (dot) {
        strcpy(dot, extension);
    } else {
        strcat(output_filename, extension);
    }
    return output_filename;
}


void trim(char *line) {
    char *end;
    while (isspace((unsigned char)*line)) line++; /* leading spaces*/
    end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) *end-- = '\0';
}