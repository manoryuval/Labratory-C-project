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

Symbol *symbols = NULL;

int count_labels = 0;
int first_pass (char *file_name) {
    int ic = 0,dc = 0, is_label = 0,line_count = 0,ICF,DCF;
    char line[MAX_LINE];
    char copy_line[MAX_LINE];
    char trimmed_line[MAX_LINE];
    char label[MAX_LABEL_LENGTH];
    char *token1, *token2, *token3, *arg, *reg1, *opcode_name, type1, type2;
    WordType arg_type;
    int L = 0; /* מספר הארגומנטים */
    int count_arg = 0; /* מספר הארגומנטים */
    int i, num, mat_arg; /* האם ארגומנט מטריצה */
    int two_reg_arg = 0; /*דגל*/
    missing_line *missing_lines = NULL; /* linked list of missing lines */


    /*open file*/
    char *am_file = create_extension(file_name,".am");
    FILE *f = fopen(am_file, "r");
    FILE *input = fopen(file_name,"r");
    if (!input || !f) {
        printf("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }
    while (fgets(line, sizeof(line), f)) {
        is_label = 0; /*reset label flag for each line*/
        line_count++;
        reg1 = "r1"; /*default register*/
        type1 = 'A'; /*default type*/
        type2 = 'A'; /*default type*/
        /*הסרת רווחים מיותרים*/
        strcpy(trimmed_line, line);
        trim(trimmed_line);
        if(trimmed_line[0] == '\0' || line[0] == COMMENT) {
            continue; /*skip empty lines*/
        }
        /*בדיקה אם יש לייבל*/
        token1 = strtok(trimmed_line, " \t"); 
        printf("Token1: %s\t", token1);
        if (is_label_start(token1)) {
            printf("label\t");
            /*האם הלייבל חוקי - פירוט בהערות */
            strncpy(label,token1, MAX_LABEL_LENGTH);
            is_label = 1; /*אחרי בדיקת ולידציה*/
            if (valid_label(label) !=1) {
                /* לעשדות שגיאה*/
                printf("3");
                continue; /*continue to next line*/

            }
            /*remove label from line*/
            token1 =  strtok(NULL, " \t"); 
        }


        printf("Token1: %s\t", token1);
        
        switch(scan_word(token1)) 
        {
            case DATA: /*האם הנחיה לאחסון נתונים*/
            { 
                printf("DATA\t");
                if(is_label) 
                {
                    printf("DATA with label\n");
                    if(!is_label_exists(symbols, count_labels, label)) {
                        printf("Adding label %s to symbols table with DC %d \n", label, dc);
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, LABEL_DATA, dc);
                    }else {
                        /*שגיאה - הלייבל כבר קיים*/
                        printf("Error: Label %s already exists.\n", label);
                        continue; /*continue to next line*/
                    }   
                }

                switch (get_data_kind(token1))
                {
                case DATA_:
                    while((token2 = strtok(NULL, ", \t")))  /*next token should be the data*/
                    {
                        if (!is_number(token2)) 
                        {
                            printf("Error: Invalid number %s\n", token2); /*שגיאה*/
                            continue; /*continue to next line*/
                        }
                        num = atoi(token2); /*convert to integer*/
                        printf("Token2: %s\t", token2);
                        num_to_code(num, dc, 'D'); 
                        dc ++; /*increment DC for each number*/
                    }
                    break;
                case STRING_:
                    token2 = strtok(NULL, " \t"); /*next token should be the string*/
                    trim(token2); /*trim the string*/
                    printf("Token2: %s\t", token2);
                    printf("String length: %d\t", alpha_count(token2));
                    for (i = 1; i < alpha_count(token2) + 1; i++)
                    {
                        char_to_code(token2[i], dc,'D');
                        dc ++; /*increment DC for each character*/
                    }
                    char_to_code(0, dc, 'D');  /*קודד אפס לסיום המחרוזת*/
                    dc ++; /*increment DC for the null terminator*/
                    break;
                case MAT_:
                    token2 = strtok(NULL, " \t"); /*next token should be the matrix*/
                    trim(token2); /*trim the matrix*/
                    printf("Token2: %s\t", token2); 
                    mat_arg = is_matrix_definition(token2); /*check if matrix definition*/
                    if (!mat_arg) 
                    {
                        printf("Error: Invalid matrix definition %s\n", token2);/*שגיאה*/
                        continue; /*continue to next line*/
                    }                    /* code */
                    for (i = 0; i < mat_arg; i++) {
                        token3 = strtok(NULL, ","); /*next token should be the matrix data*/
                        trim(token3); /*trim the matrix data*/
                        if (!token3) 
                        {
                            num_to_code(0, dc, 'D'); /*קודד אפס אם אין נתונים*/
                            dc++; /*increment DC for the null terminator*/
                            continue; /*continue to next line*/
                        }
                        num = atoi(token3); /*convert to integer*/
                        printf("Token3: %s\t", token3);
                        /*קודד מספרים - עבור כל מספר*/
                        num_to_code(num, dc, 'D');
                        dc ++; /*increment DC for each number*/
                    }

                    break;
                default:
                    break;
                }
                    /*שורה 7 באלגוריתם - זיהוי סוג הנתונים והגודל ולקדם את DC בהתאם*/
            }  
            case ENTRY:{  /*האם entry*/
                break;
            }
            case EXTERN:
            {
                printf("EXTERN\t");
                arg = strtok(NULL, " \t");
                printf("Arg1: %s\t", arg);
                /*אם אקסטרן - להכניס לטבלת סמלים עם הערך 0 ו מאפיין אקסטרן. continue*/
                if (arg && valid_label(arg)) {
                    printf("Adding extern label %s to symbols table with value 0\n", arg);
                    add_symbol(&symbols, &count_labels, arg, LABEL_EXTERN, LABEL_TBD, 0);
                } else {
                    printf("Error: Invalid extern label %s\n", arg);
                }
            break;
            }    
            case CODE:
            {/*שורת הוראה - אם יש סמל */ 
                printf("CODE\t");
                if (is_label) {
                    printf("CODE with label\t");
                    if(!is_label_exists(symbols, count_labels, label)) {
                        printf("Adding label %s to symbols table with IC \n", label);
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, LABEL_CODE, ic);
                    }else {
                        /*שגיאה - הלייבל כבר קיים*/
                        printf("Error: Label %s already exists.\n", label);
                        continue; /*continue to next line*/
                    }
                }
                L += 1; 
                opcode_name = get_opcode_name(token1);
                if (!opcode_name) {
                    printf("Error: Invalid opcode %s\n", token1);/*שגיאה*/
                    continue; /*continue to next line*/
                }
                printf("Opcode Name: %s\t", opcode_name);
                count_arg = get_opcode_arg(token1);
                i = 0; /*reset argument index*/
                /*לקודד את הפקודה*/
                /*read the arguments*/
                while ((arg = strtok(NULL, ",\t")) && i < count_arg) {/*, עשיתי לוודא שעובד טוב. תיקון - להפריד טוקנים לפי פסיקים בלבד (ואז להוריד רווחים)*/
                    trim(arg); /*trim the argument*/
                    printf("Arg1: %s\t", arg);
                    switch ((arg_type = scan_word(arg))) {
                    case ARG_NUM: 
                        printf("ARG_NUM\t");
                        if (i == 0 && count_arg > 1) type1 = 'A'; /*immediate*/
                        else type2 = 'A'; 
                        num_to_code(num_to_int(arg), ic + L, 'I'); /*convert number to code*/
                        /*לקודד מספרים  בic+i*/
                        L += 1;
                        break;
                    case ARG_REG: 
                        printf("ARG_REG\t");
                        if (i == 0 && count_arg > 1) type1 = 'D'; /*register*/
                        else type2 = 'D'; 
                        /*לקודד רישומים ic + i*/
                        if (two_reg_arg == 1){
                            two_reg_code(reg1, arg, ic + L, 'I'); /*convert register to code*/
                            continue;
                        }
                        reg1=arg;
                        two_reg_code(reg1, "r0", ic + L, 'I'); /*convert register to code*/
                        two_reg_arg++;
                        L += 1;
                        break;
                    case LABEL: 
                        printf("LABEL\t");
                        if (i == 0 && count_arg > 1) type1 = 'B'; /*label*/
                        else type2 = 'B'; 
                        L += 1;
                        break;
                    case ARG_MAT:
                        printf("ARG_MAT\t");
                        if (i == 0 && count_arg > 1) type1 = 'C'; /*matrix*/
                        else type2 = 'C'; 
                        if(get_reg1_matrix_operand(arg) && get_reg2_matrix_operand(arg)) {

                            two_reg_code( get_reg1_matrix_operand(arg), get_reg2_matrix_operand(arg), ic + L + 1, 'I'); /*convert matrix to code*/
                            } else {
                                printf("Error: Invalid matrix operand %s\n", arg); /*שגיאה*/
                        }
                        L += 2;
                        break;
                    default:
                        break;
                    }

                    i++;
                }
                op_to_code(opcode_name, type1, type2, ic, 'I'); /*convert opcode to code*/
                /*תיקון - לבדוק i ביחס ל לארגומנטים*/
                printf(">>>>>----------L: %d\t", L);
                ic += L; 
                L = 0; /*reset L for next line*/
                two_reg_arg = 0; /*reset two_reg_arg for next line*/


            break;
            }
            default: break;

        
        }
        printf("\n");
    }

    ICF = ic;
    DCF = dc;
    printf("ICF: %d, DCF: %d\n", ICF, DCF);
    update_symbol_address(symbols, count_labels, ICF);
    /*
     צריך פה להעתיק את הDCF ל ICF גם בשורות קוד וגם בלייבלים
    */
    dcf_to_icf(ICF,DCF);
    fclose(input);
    fclose(f);
    return 1;
    }

