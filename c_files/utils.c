#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../header_files/preproc.h"
#include "../header_files/utils.h"

char *create_extension(char *filename, char *extension) {

    char *output_filename = malloc(FILENAME_MAX);
    char *dot;
    if (!output_filename) return NULL; /*שגיאת לזיכרון*/
    strcpy(output_filename, filename);
    dot = strrchr(output_filename, '.');
    if (dot) {
        strcpy(dot, extension);
    } else {
        strcat(output_filename, extension);
    }
    return output_filename;
}

void trim(char *line) {
    char *start = line;
    char *end;
    int len;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (*start == '\0') {
        line[0] = '\0';
        return;
    }

    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    len = end - start + 1;

    memmove(line, start, len);
    line[len] = '\0';
}