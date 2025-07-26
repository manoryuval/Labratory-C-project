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
#include "../header_files/errors.h"


int second_pass(char *file_name) {
    char line[MAX_LINE];
    char *token1, *token2;
    missing_line *current = missing_lines;
    missing_line *current_extern = missing_lines;
    int i;
    char *am_file = create_extension(file_name,".am");
    FILE *f = fopen(am_file, "r");
    FILE *input = fopen(file_name,"r");
    line_count = 0;

    if (!input || !f ) {
        printf("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }

    while (fgets(line, sizeof(line), f)) {
        line_count++;
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
                if (is_label_exists(symbols, count_labels, token2)) {
                    printf("Error: Entry label %s does not exist in the symbol table\n", token2);/*שגיאה*/
                    continue; /*continue to next line*/
                }
                /* Update the symbol table entry for the entry label */
                token2[strcspn(token2, "\r\n")] = '\0';
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
        int updated = update_missing_lines(current/*, &extern_lines*/, symbols, count_labels);
        if (updated == 0) {
            printf("No missing lines were updated.\n");
            break; /* שגיאה */
        }
        current = current->next;
    }

    if(entry_count(symbols, count_labels)) {
        char *entry_file = create_extension(file_name, ".ent");
        FILE *f = fopen(entry_file, "w+");
        for(i = 0; i < count_labels; i++) {
            if (symbols[i].type == LABEL_ENTRY) {
                fprintf(f, "%s\t", symbols[i].label);
                line_fprint(f, symbols[i].address);
                fprintf(f, "\n");
            }
        }
    }
    if(extern_count(symbols, count_labels)) 
    {
        char *extern_file = create_extension(file_name, ".ext");
        FILE *f1 = fopen(extern_file, "w+");
        while (current_extern != NULL) {
            for(i = 0; i < count_labels; i++) {
                if ((strcmp(symbols[i].label, current_extern->label) == 0) && symbols[i].type == LABEL_EXTERN) {
                    fprintf(f1, "%s\t", current_extern->label);
                    line_fprint(f1, current_extern->line+100);
                    fprintf(f1, "\n");
                    break;
                }
            }
            
            current_extern = current_extern->next;
        }
        fclose(f1);
    }

    fclose(f);
    return 0;
}
