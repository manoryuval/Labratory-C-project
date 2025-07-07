#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../header_files/preproc.h"
#include "../header_files/utils.h"

char *create_extension(char *filename, char *extension) {

    char *output_filename = malloc(FILENAME_MAX);
    char *dot;
    /* Check for memory allocation errors */
    if (!output_filename) return NULL; /*שגיאת לזיכרון*/
    /* Copy the original file name */
    strcpy(output_filename, filename);
    dot = strrchr(output_filename, '.');
    if (dot) {
        /* Replace the old extension with the new one */
        strcpy(dot, extension);
    } else {
        /* Append the new extension if no old extension was found */
        strcat(output_filename, extension);
    }
    return output_filename;
}

void trim(char *line) {
    char *start = line;
    char *end;
    int len;
    /* Find the start of the trimmed line */
    while (isspace((unsigned char)*start)) {
        start++;
    }
    /* If the line is empty after trimming, set it to an empty string */
    if (*start == '\0') {
        line[0] = '\0';
        return;
    }
    /* Find the end of the trimmed line */
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    len = end - start + 1;
    /* Move the trimmed line to the beginning of the original line */
    memmove(line, start, len);
    line[len] = '\0';
}
