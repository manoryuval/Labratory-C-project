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
#include "../header_files/errors.h"

Symbol *symbols = NULL;
missing_line *missing_lines = NULL; /* linked list of missing lines */
int ICF = 0;
int DCF = 0;
int count_labels = 0;
int first_pass (char *file_name) 
{
    int ic = 0,dc = 0, is_label = 0;
    char line[MAX_LINE];
    char trimmed_line[MAX_LINE];
    char copy_trimmed_line[MAX_LINE];
    char label[MAX_LABEL_LENGTH];
    char *token1, *token2, *token3, *token4, *arg, *reg1, *opcode_name, type1, type2, *end_of_line = NULL;
    WordType arg_type;
    char *am_file;
    FILE *f;
    FILE *input;
    int L = 0; /* מספר הארגומנטים */
    int count_arg = 0; /* מספר הארגומנטים */
    int i , num, mat_arg, comma_index; /* האם ארגומנט מטריצה */
    int two_reg_arg = 0; /*דגל*/
    line_count = 0;
    
    /*open file*/
    am_file = create_extension(file_name,".am");
    f = fopen(am_file, "r");
    input = fopen(file_name,"r");
    if (!input || !f ) {
        print_error(ERROR1, current_filename, 0);
        return 1;
    }

    /*read file line by line*/
    while (fgets(line, sizeof(line), f)) {
        is_label = 0; /*reset label flag for each line*/
        line_count++;
        reg1 = "r1"; /*default register*/
        type1 = 'A'; /*default type*/
        type2 = 'A'; /*default type*/
        /*הסרת רווחים מיותרים*/
        strcpy(trimmed_line, line);
        trim(trimmed_line);
        strcpy(copy_trimmed_line, trimmed_line);
        if(trimmed_line[0] == '\0' || line[0] == COMMENT) {
            continue; /*skip empty lines*/
        }
        if(multiple_consecutive_commas(trimmed_line)) {
            print_error(ERROR26, current_filename, line_count); /*multiple commas*/
        }
        comma_index= check_missing_commas(trimmed_line);
        if(comma_index == 2) {
            print_error(ERROR27, current_filename, line_count); /*missing comma*/
        }
        /*בדיקה אם יש לייבל*/
        token1 = strtok(trimmed_line, " \t"); 
        /* printf("Token1: %s\t", token1); */
        if (is_label_start(token1)) {
            /* printf("label\t"); */
            /* האם הלייבל חוקי - פירוט בהערות */
            copy_label(label, token1); /*copy label to label variable*/
            if (valid_label(label) !=1) {
                continue; /*continue to next line*/

            }
            is_label = 1; /*אחרי בדיקת ולידציה*/
            /*remove label from line*/
            token1 =  strtok(NULL, " \t"); 
        }


        /* printf("Token1: %s\t", token1); */
        
        switch(scan_word(token1)) 
        {
            case DATA: /*האם הנחיה לאחסון נתונים*/
            { 
                /* printf("DATA\t"); */
                if(is_label) 
                {
                    /* printf("DATA with label\n"); */
                    if(!is_label_exists(symbols, count_labels, label)) {
                        /* printf("Adding label %s to symbols table with DC %d \n", label, dc); */
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, LABEL_DATA, dc);
                    }else {
                        print_error(ERROR3, current_filename, line_count);
                        continue; /*continue to next line*/
                    }   
                }

                switch (get_data_kind(token1))
                {
                case DATA_:
                while((token2 = strtok(NULL, ",")))  /*next token should be the data*/
                    {
                        trim(token2); /*trim the data*/
                        if (token2[0] == '\0') 
                        {
                            continue; /*continue to next line*/
                        }                        
                        if (!is_number(token2)) 
                        {
                            print_error(ERROR4, current_filename, line_count);
                            continue; /*continue to next line*/
                        }
                        num = atoi(token2); /*convert to integer*/
                        /* printf("Token2: %s\t", token2); */
                        num_to_code(num, dc, 'D'); 
                        dc ++; /*increment DC for each number*/
                    }
                    if(copy_trimmed_line[strlen(copy_trimmed_line)-1] == ',') {
                        print_error(ERROR30, current_filename, line_count); 
                    }
                    token4 = strtok(copy_trimmed_line, " \t");
                    token4 = strtok(NULL, " \t");
                    token4 = strtok(NULL, " \t");
                    if (token4[0] == ',') {
                        print_error(ERROR31, current_filename, line_count); 
                    }
                    break;

                    
                case STRING_:
                    token2 = strtok(NULL,"\n"); /*next token should be the string*/
                    trim(token2); /*trim the string*/
                    /*printf("Token2: %s\t", token2); 
                    printf("String length: %d\t", alpha_count(token2)); */
                    if (alpha_count(token2) < 0) 
                    {
                        print_error(ERROR29, current_filename, line_count);
                        continue; /*continue to next line*/
                    }
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
                    /* printf("Token2: %s\t", token2);  */
                    mat_arg = is_matrix_definition(token2); /*check if matrix definition*/
                    if (!mat_arg) 
                    {
                        print_error(ERROR5, current_filename, line_count);
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
                        /* printf("Token3: %s\t", token3); */
                        /*קודד מספרים - עבור כל מספר*/
                        num_to_code(num, dc, 'D');
                        dc ++; /*increment DC for each number*/
                    }

                    break;
                default:{
                    print_error(ERROR21, current_filename, line_count); /*unknown word*/
                    break;
                    } 
                }
            }  
            case ENTRY:{  /*האם entry*/
                break;
            }
            case EXTERN:
            {
                arg = strtok(NULL, " \t");
                if (arg && valid_label(arg)) 
                {
                    if(!is_label_exists(symbols, count_labels, arg)) {
                        /* printf("Adding extern label %s to symbols table with value 0\n", arg); */
                       add_symbol(&symbols, &count_labels, arg, LABEL_EXTERN, LABEL_TBD, 0);
                    }else {
                        print_error(ERROR3, current_filename, line_count);
                        continue; 
                    }
                    
                    
                } 
                
            break;
            }    
            case CODE:
            {/*שורת הוראה - אם יש סמל */ 
                /* printf("CODE\t"); */
                if (is_label) {
                    /*printf("Found label %s in code line\n", label);*/
                    /* printf("CODE with label\t"); */
                    if(!is_label_exists(symbols, count_labels, label)) {
                        /* printf("Adding label %s to symbols table with IC \n", label); */
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, LABEL_CODE,ic);
                    }else {
                        print_error(ERROR3, current_filename, line_count);
                        continue; 
                    }
                }
                L += 1; 
                opcode_name = get_opcode_name(token1);
                if (!opcode_name) {
                    print_error(ERROR6, current_filename, line_count);
                    continue; /*continue to next line*/
                }
                /* printf("Opcode Name: %s\t", opcode_name); */
                count_arg = get_opcode_arg(token1);
                i = 0; /*reset argument index*/
                /*לקודד את הפקודה*/
                /*read the arguments*/
                while (i < count_arg && (arg = strtok(NULL, ",\t"))) {/*, עשיתי לוודא שעובד טוב. תיקון - להפריד טוקנים לפי פסיקים בלבד (ואז להוריד רווחים)*/
                    trim(arg); /*trim the argument*/
                    /* printf("Arg1: %s\t", arg); */
                    switch ((arg_type = scan_word(arg))) {
                    case ARG_NUM: 
                        if(!is_valid_argument(opcode_name, i, arg_type)){
                            print_error(ERROR22, current_filename, line_count);
                            break; /*continue to next line*/
                        }                        /* printf("ARG_NUM\t"); */
                        if (i == 0 && count_arg > 1) type1 = 'A'; /*immediate*/
                        else type2 = 'A'; 
                        num_to_code8(num_to_int(arg), ic + L, 'I'); /*convert number to code*/
                        L += 1;
                        break;
                    case ARG_REG: 
                        if(!is_valid_argument(opcode_name, i, arg_type)){
                            print_error(ERROR8, current_filename, line_count);
                            break; 
                        }            
                        if (i == 0 && count_arg > 1) type1 = 'D'; /*register*/
                        else type2 = 'D'; 

                        if (two_reg_arg == 1){
                            two_reg_code(reg1, arg, ic + L-1, 'I'); /*convert register to code*/
                            break;
                        }
                        reg1=arg;
                        if(type2 == 'D'){
                            two_reg_code("r0",reg1, ic + L, 'I'); /*convert register to code*/
                            L += 1;
                            break;
                        }
                        two_reg_code(reg1, "r0", ic + L, 'I'); /*convert register to code*/
                        two_reg_arg++;
                        L += 1;
                        break;
                    case LABEL: 
                        if(!is_valid_argument(opcode_name, i, arg_type)){
                            print_error(ERROR9, current_filename, line_count);
                            break; /*continue to next line*/
                        }
                        if (i == 0 && count_arg > 1) type1 = 'B'; /*label*/
                        else type2 = 'B'; 
                        add_missing_line(ic + L, arg, &missing_lines, line_count); /*add missing line*/
                        L += 1;
                        break;
                    case ARG_MAT:
                        if(!is_valid_argument(opcode_name, i, arg_type)){
                            print_error(ERROR7, current_filename, line_count);
                            break; /*continue to next line*/
                        }
                        /* printf("ARG_MAT\t"); */
                        if (i == 0 && count_arg > 1) type1 = 'C'; /*matrix*/
                        else type2 = 'C'; 
                        add_missing_line(ic + L, get_matrix_name(arg), &missing_lines, line_count); /*add missing line*/
                        if (get_reg1_matrix_operand(arg) && get_reg2_matrix_operand(arg)) {

                            two_reg_code( get_reg1_matrix_operand(arg), get_reg2_matrix_operand(arg), ic + L + 1, 'I'); /*convert matrix to code*/
                            } else {
                            print_error(ERROR9, current_filename, line_count);
                        }
                        L += 2;
                        break;
                    default:
                        print_error(ERROR16, current_filename, line_count);
                        break;
                    }

                    i++;
                }
                end_of_line = strtok(NULL, " \t"); /*check if there is more text after the last argument*/
                if(end_of_line != NULL) {
                    print_error(ERROR19, current_filename, line_count); /*if there is more text after the last argument*/
                }
                if (i < count_arg) {
                    print_error(ERROR17, current_filename, line_count);
                    continue; /*continue to next line*/
                }
                op_to_code(opcode_name, type1, type2, ic, 'I'); /*convert opcode to code*/                /* printf(">>>>>----------L: %d\t", L); */
                ic += L; 
                L = 0; /*reset L for next line*/
                two_reg_arg = 0; /*reset two_reg_arg for next line*/


            break;
            }
            default:{
                print_error(ERROR21, current_filename, line_count); /*unknown word*/
                break;
            }

        
        }
        
        /*printf("\n");*/
    }

    ICF = ic;
    DCF = dc;
  
    update_symbol_address(symbols, count_labels, ICF);
    dcf_to_icf(ICF,DCF);
    /* dc_to_ic(ICF);*/
    add100(symbols, count_labels);
    /*print_missing_lines(missing_lines);*/
    
    fclose(input);
    fclose(f);
    return 1;
    }


