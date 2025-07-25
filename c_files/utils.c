#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/utils.h"
#include "../header_files/preproc.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"
#include "../header_files/analyse.h"
#include "../header_files/coding.h"
#include "../header_files/errors.h"

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


int alpha_count(char *token){
    int len = strlen(token);
    int i;
    int count = 0;
    if (len < 2) return 0; 

    if (token[0] != '"' || token[len-1] != '"') {
        return 0; 
    }

    for (i = 1; i < len - 1; i++) {
        char c = token[i];
        if (!isalpha((unsigned char)c)) {
            return 0;
        }
        if (c == ' ' || c == '\t' ) {
            continue;
        }
        count++;    
    }

    return count; 
}


int valid_mat(char *token) {
    int len = strlen(token);
    int i;
    int count = 0;
    if (len < 2) return 0;

    if (token[0] != '[' || token[len-1] != ']') {
        return 0;
    }

    for (i = 1; i < len - 1; i++) {
        char c = token[i];
        if (!isdigit((unsigned char)c)) {
            return 0;
        }
        if (c == ' ' || c == '\t' ) {
            continue;
        }
        count++;
    }

    return count;
}

void remove_spaces(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0'; /* Null-terminate the string */
}

char lowercase(char c) 
{
    switch (c)
    {
    case 'A':
        return 'a';
    case 'B':
        return 'b';
    case 'C':
        return 'c';
    case 'D':
        return 'd';
    }
    return 'E';
}
