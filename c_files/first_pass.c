#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header_files/assembler.h"
#include "../header_files/utils.h"
#include "../header_files/preproc.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"


int first_pass (char *file_name) {
    Symbol *symbols = NULL;
    int count_labels = 0,IC = 0,DC = 0, is_label = 0;
    char line[MAX_LINE];
    char copy_line[MAX_LINE];
    char trimmed_line[MAX_LINE];
    char label[MAX_LABEL_LENGTH];
    /*open file*/
    char *am_file = create_extension(file_name,".ob");
    FILE *f = fopen(am_file, "w+");
    FILE *input = fopen(file_name,"r");
    if (!input || !f) {
        perror("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }
    
    while (fgets(line, sizeof(line), input)) {
        /*הסרת רווחים מיותרים*/
        strcpy(trimmed_line, line);
        trim(trimmed_line);
        /*בדיקה אם יש לייבל*/
        if (is_label_start(line)) {
            /*האם הלייבל חוקי - פירוט בהערות */
            strncpy(label, strtok(trimmed_line, ":"), MAX_LABEL_LENGTH);
            is_label = 1;
            if (valid_label(trimmed_line) !=1) {
                /* לעשדות שגיאה*/
                continue; /*continue to next line*/

            }
            
            
        }
            /*האם הנחיה לאחסון נתונים*/
                /*אם זה לייבל*/
                    /*לבדוק שהלייבל לא קיים ולהכניס לטבלה*/        
                /*זיהוי סוג הנתונים והגודל ולקדם את DC בהתאם*/
            /*האם extern או entry*/
                /*אם אנטרי - continue*/
                /*אם אקסטרן - להכניס לטבלת סמלים עם הערך 0 ו מאפיין אקסטרן. continue*/
            /*שורת הוראה - אם יש סמל */
                /*אם יש כבר אז שגיאה)להכניס אותו לטבלת הסמלים עם )code וIC*/
                /*לחפש בטבלת שמות הפעולה*/
                /*להבין כמות אופרנדים ולשים בL*/
                /*לבנות קוד בינארי של מיעון מיידי*/            

        
    }
    /*אם יש לייבל*/
    add_symbol(&symbols, &count_labels, "START", REGULAR, CODE, 0);
    add_symbol(&symbols, &count_labels, "LOOP", REGULAR, CODE, 0);
    add_symbol(&symbols, &count_labels, "DATA", REGULAR, DATA, 1);
    add_symbol(&symbols, &count_labels, "END", REGULAR, CODE, 0);
    printf("%d",is_label_exists(symbols, count_labels, "END"));


    close(input);
    close(f);
    free(symbols);
    return 1;
}