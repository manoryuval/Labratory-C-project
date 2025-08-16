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
    if (!output_filename) return NULL;
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
    /* Check for empty strings */
    if (len < 2) return -1;
    /* Check for quotes */
    if (token[0] != '"' || token[len-1] != '"') {
        return -1; 
    }
    /* Return the length of the string without the quotes */
    return len-2; 
}

void remove_spaces(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
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

int is_number(char *token) 
{
    int i;
    /* Check for valid signs */
    if (token[0] == MINUS || token[0] == PLUS || isdigit(token[0])) 
    {
        for(i = 1; i < strlen(token); i++) {
            /* Check if the character is a digit */
            if(!isdigit(token[i])) 
            {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int num_to_int(char *token) {
    char *ptr = token;
    ptr++;
    return atoi(ptr);
}

int multiple_consecutive_commas(char *line) {
    int i;
    int comma_count = 0;
    int line_length = strlen(line);
    for (i = 0; i < line_length; i++) {
        if (line[i] == ' ' || line[i] == '\t') {
            continue; 
        }
        /* Check for multiple consecutive commas */
        if (line[i] == ',') {
            comma_count++;
            if (comma_count > 1) {
                return 1;  /* Found multiple consecutive commas*/
            }
        } else {
            comma_count = 0;  /* Reset count if not a comma */
        }
    }
    return 0;  /* No multiple consecutive commas found */
}

int check_missing_commas(const char* input) {
    int i = 0;
    int arg_index = 0;         
    int last_was_comma = 0;

    while (input[i] != '\0') {
        /* Skip spaces and tabs */
        while (input[i] == ' ' || input[i] == '\t') {
            i++;
        }
        /* Check for end of input */
        if (input[i] == '\0') break;

        arg_index++;

        /* Read the command */
        if (arg_index == 1) {
            while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != ',') {
                i++;
            }

            /* Skip spaces and tabs after the command */
            while (input[i] == ' ' || input[i] == '\t') {
                i++;
            }

            /* Check if there is a comma directly after the command */
            if (input[i] == ',') {
                return 2; /* Invalid - comma directly after command */
            }

            continue;
        }

        /* Check if a comma is missing between arguments (except for the second one) */
        if (!last_was_comma && arg_index > 2) {
            return 1; /* Missing comma */
        }

        /* Skip the token (until a space, tab, or comma) */
        while (input[i] != '\0' && input[i] != ' ' && input[i] != '\t' && input[i] != ',') {
            i++;
        }

        /* Skip spaces and tabs after the token */
        while (input[i] == ' ' || input[i] == '\t') {
            i++;
        }

        /* Check if there is a comma */
        if (input[i] == ',') {
            last_was_comma = 1;
            i++;
        } else {
            last_was_comma = 0;
        }
    }

    return 0; /* Valid */
}