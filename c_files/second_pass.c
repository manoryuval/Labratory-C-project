#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_files/assembler.h"
#include "../header_files/utils.h"
#include "../header_files/preproc.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"
#include "../header_files/analyse.h"
#include "../header_files/coding.h"
#include "../header_files/second_pass.h"


int second_pass(char *file_name) {
    char line[MAX_LINE];
    char *token1, *token2;
    missing_line *current = missing_lines;

    char *am_file = create_extension(file_name,".am");
    FILE *f = fopen(am_file, "r");
    FILE *input = fopen(file_name,"r");
    if (!input || !f ) {
        printf("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }

    while (fgets(line, sizeof(line), f)) {
        token1 = strtok(line, " \t");
        if (!token1) continue;
        if (is_label_start(token1)) {
            token1 = strtok(NULL, " \t"); /* Skip the label */
        }

        switch (scan_word(token1)) {
            case DATA:
                break;
            case EXTERN:
                break;
            case ENTRY:
                token2 = strtok(NULL, " \t");
                if (!token2 || !valid_label(token2)) {
                    printf("Error: Invalid entry label %s\n", token2);/*שגיאה*/
                    continue; /*continue to next line*/
                }
                if (!is_label_exists(symbols, count_labels, token2)) {
                    printf("Error: Entry label %s does not exist in the symbol table\n", token2);/*שגיאה*/
                    continue; /*continue to next line*/
                }
                /* Update the symbol table entry for the entry label */
                update_symbol_type(symbols, count_labels, token2, LABEL_ENTRY);
                break;
            case CODE:
                break;
            default:
                break;
        }
    }
    while (current != NULL)
    {
        int updated = update_missing_lines(current, symbols, count_labels);
        if (updated == 0) {
            printf("No missing lines were updated.\n");
            break; /* שגיאה */
        }
        current = current->next;
    }

    fclose(f);
    return 0;
}