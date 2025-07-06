#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_files/assembler.h"
#include "../header_files/utils.h"
#include "../header_files/preproc.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"
#include "../header_files/analyse.h"


Symbol *symbols = NULL;
int count_labels = 0;
int first_pass (char *file_name) {
    int IC = 0,DC = 0, is_label = 0,line_count = 0;
    char line[MAX_LINE];
    char copy_line[MAX_LINE];
    char trimmed_line[MAX_LINE];
    char label[MAX_LABEL_LENGTH];
    char *token1, *token2, *arg1;
    /*open file*/
    char *am_file = create_extension(file_name,".am");
    FILE *f = fopen(am_file, "r");
    FILE *input = fopen(file_name,"r");
    if (!input || !f) {
        perror("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }
    while (fgets(line, sizeof(line), f)) {
        is_label = 0; /*reset label flag for each line*/
        line_count++;
        /*הסרת רווחים מיותרים*/
        strcpy(trimmed_line, line);
        trim(trimmed_line);
        if(trimmed_line[0] == '\0' || line[0] == COMMENT) {
            continue; /*skip empty lines*/
        }
        /*בדיקה אם יש לייבל*/
        token1 = strtok(trimmed_line, " \t\n"); 
        printf("Token1: %s\t", token1);
        if (is_label_start(token1)) {
            printf("label\t");
            /*האם הלייבל חוקי - פירוט בהערות */
            strncpy(label,token1, MAX_LABEL_LENGTH);
            is_label = 1; 
            if (valid_label(label) !=1) {
                /* לעשדות שגיאה*/
                printf("3");
                continue; /*continue to next line*/

            }
            token2 = strtok(NULL, " \t\n"); 
            token1 = token2;
        }
        printf("Token2: %s\t", token1);
        switch(scan_word(token1)) {
            case DATA:{ /*האם הנחיה לאחסון נתונים*/
                printf("DATA\n");
                if(is_label) {
                    printf("DATA with label\n");
                    if(!is_label_exists(symbols, count_labels, label)) {
                        printf("Adding label %s to symbols table with DC \n", label);
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, DATA, DC);
                    }else {
                        /*שגיאה - הלייבל כבר קיים*/
                        printf("Error: Label %s already exists.\n", label);
                        continue; /*continue to next line*/
                    }
                    
                    /*זיהוי סוג הנתונים והגודל ולקדם את DC בהתאם*/

                }
                break;
            }  
            case ENTRY:{  /*האם entry*/
                break;
            }
            case EXTERN:
            {
                printf("EXTERN\t");
                arg1 = strtok(NULL, " \t\n");
                printf("Arg1: %s\t", arg1);
                /*אם אקסטרן - להכניס לטבלת סמלים עם הערך 0 ו מאפיין אקסטרן. continue*/
                if (arg1 && valid_label(arg1)) {
                    printf("Adding extern label %s to symbols table with value 0\n", arg1);
                    add_symbol(&symbols, &count_labels, arg1, LABEL_EXTERN, EXTERN, 0);
                } else {
                    printf("Error: Invalid extern label %s\n", arg1);
                }
            break;
            }    
            case CODE:
            {/*שורת הוראה - אם יש סמל */ 
                printf("CODE\n");
                if (is_label) {
                    printf("CODE with label\n");
                    if(!is_label_exists(symbols, count_labels, label)) {
                        printf("Adding label %s to symbols table with IC \n", label);
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, CODE, IC);
                    }else {
                        /*שגיאה - הלייבל כבר קיים*/
                        printf("Error: Label %s already exists.\n", label);
                        continue; /*continue to next line*/
                    }
                }
                arg1 = strtok(NULL, " \t\n");
                /*שורה 12 באלגוריתם - לבדוק האם הוראה חוקית*/
            break;
            }
            default: break;
        
        }
    }
    /*אם יש לייבל*/
    /*
    print_symbols(symbols, count_labels);
    printf("%d\n",is_label_exists(symbols, count_labels, "END"));
    */
        print_symbols(symbols, count_labels);

    fclose(input);
    fclose(f);
    return 1;
    }

