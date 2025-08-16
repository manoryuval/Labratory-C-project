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
int ICF = 0;/* Instruction counter */
int DCF = 0;/* Data counter */
int count_labels = 0;/* Count of labels */
int first_pass (char *file_name) 
{
    int ic = 0,dc = 0, is_label = 0, is_data = 0, empty = 0;
    char line[MAX_LINE];
    char trimmed_line[MAX_LINE];
    char copy_trimmed_line[MAX_LINE];
    char label[MAX_LABEL_LENGTH];
    char matrix_def[MAX_LINE];  
    char *token1, *token2, *token3, *token4, *arg, *reg1, *opcode_name, type1, type2, *end_of_line = NULL;
    WordType arg_type;
    char *am_file;
    FILE *f;
    FILE *input;
    int L = 0;
    int count_arg = 0;
    int i , num, mat_arg, comma_index;
    int two_reg_arg = 0;
    line_count = 0;

    /* Open file */
    am_file = create_extension(file_name,".am");
    f = fopen(am_file, "r");
    input = fopen(file_name,"r");
    if (!input || !f ) {
        print_error(ERROR1, current_filename, 0);
        return 1;
    }

    /* Read file line by line */
    while (fgets(line, sizeof(line), f)) {
        is_label = 0; /* Reset label flag for each line */
        is_data = 0; /* Reset data flag for each line */
        empty = 0; /* Reset empty flag for each line */
        line_count++; /* Increment line count */
        reg1 = "r1"; /* Default register */
        type1 = 'A'; /* Default type */
        type2 = 'A'; /* Default type */
        /* Remove unnecessary whitespace */
        strcpy(trimmed_line, line);
        trim(trimmed_line);
        strcpy(copy_trimmed_line, trimmed_line);
        /* Check for line length */
        if (strlen(copy_trimmed_line) >= LENGTH_LINE) {
            print_error(ERROR40, current_filename, line_count);
            continue;
        }
        /* Skip empty lines */
        if(trimmed_line[0] == '\0' || line[0] == COMMENT) {
            continue;
        }
        /* Check for multiple consecutive commas */
        if(multiple_consecutive_commas(trimmed_line)) {
            print_error(ERROR26, current_filename, line_count);
        }
        /* Check for missing commas */
        comma_index= check_missing_commas(trimmed_line);
        if(comma_index == 2) {
            print_error(ERROR27, current_filename, line_count);
        }
        /* Tokenize the next word */
        token1 = strtok(trimmed_line, " \t"); 
        /* Check for label */
        if (is_label_start(token1)) {
            copy_label(label, token1);
            if (valid_label(label) !=1) {/* Check if label is valid */
                continue;
            }
            is_label = 1;
            /* Remove label from line */
            token1 =  strtok(NULL, " \t"); 
        }

        /* Scan word and find its type */
        switch(scan_word(token1)) 
        {
            case DATA: /* Data directive */
            { 
                /* If there is a label, add it to the symbol table */
                if(is_label) 
                {
                    if(!is_label_exists(symbols, count_labels, label)) 
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, LABEL_DATA, dc);
                    else {
                        print_error(ERROR3, current_filename, line_count);
                        continue;
                    }   
                }
                /* Check for data kind */
                switch (get_data_kind(token1))
                {
                    case DATA_:/*Numbers*/
                    {    
                        while((token2 = strtok(NULL, ",")))  /*next token should be the numbers*/
                        {
                            is_data ++;
                            trim(token2);
                            if (token2[0] == '\0') 
                            {
                                continue;
                            }
                            /* Check if the token is a number */
                            if (!is_number(token2))
                            {
                                print_error(ERROR4, current_filename, line_count);
                                continue;
                            }
                            num = atoi(token2); /*convert to integer*/
                            num_to_code(num, dc, 'D'); 
                            dc ++; /*increment DC for each number*/
                        }
                        /* Check for trailing comma */
                        if(copy_trimmed_line[strlen(copy_trimmed_line)-1] == ',') {
                            print_error(ERROR30, current_filename, line_count); 
                        }
                        /* Check for comma in the start of the data */
                        if(is_data) {
                            token4 = strtok(copy_trimmed_line, " \t"); /* check for comma in the start of the data*/
                            token4 = strtok(NULL, " \t");
                            if(is_label)
                                token4 = strtok(NULL, " \t");
                            if (token4[0] == ',') 
                                print_error(ERROR31, current_filename, line_count); 
                        }
                        break;

                    }    
                    case STRING_:/*String*/
                    {
                        token2 = strtok(NULL,"\n"); /*next token should be the string*/
                        trim(token2);
                        /* Check for empty string */
                        if (alpha_count(token2) < 0) 
                        {
                            print_error(ERROR29, current_filename, line_count);
                            continue; /*continue to next line*/
                        }
                        /* Code the characters */
                        for (i = 1; i < alpha_count(token2) + 1; i++)
                        {
                            char_to_code(token2[i], dc,'D');
                            dc ++;
                        }
                        char_to_code(0, dc, 'D');/*code the null terminator at the end*/
                        dc ++;
                        break;
                    }
                    case MAT_: /*Matrix*/
                    {
                        token2 = strtok(NULL, "]"); /*next token should be the matrix*/
                        strcpy(matrix_def, token2);
                        strcat(matrix_def,"]"); 
                        strcat(matrix_def,strtok(NULL, "]")); 
                        strcat(matrix_def,"]"); 
                        mat_arg = is_matrix_definition(matrix_def); /*check if matrix definition*/
                        if (!mat_arg) 
                        {
                            print_error(ERROR5, current_filename, line_count);
                            continue;
                        } 
                        /*Tokenize the matrix data*/
                        while((token3 = strtok(NULL, ","))){ /*next token should be the matrix data*/
                            trim(token3);
                            if (!is_number(token3)) 
                            {
                                print_error(ERROR4, current_filename, line_count);
                                continue;
                            }
                            num = atoi(token3); /*convert to integer*/
                            num_to_code(num, dc, 'D');
                            is_data ++; /*increment data count*/
                            dc ++; /*increment DC for each number*/
                            empty ++;
                        }
                        /*Fill empty matrix slots with zeros*/
                        while(is_data < mat_arg)
                        {
                                num_to_code(0, dc, 'D');
                                is_data++;
                                dc++;
                        }
                        /*Check for too much data*/
                        if(is_data > mat_arg)
                            print_error(ERROR38, current_filename, line_count);
                        if(copy_trimmed_line[strlen(copy_trimmed_line)-1] == ',')
                            print_error(ERROR30, current_filename, line_count);
                        /*Check for empty matrix slots*/
                        if(empty){                    
                            token4 = strtok(copy_trimmed_line, " \t");
                            token4 = strtok(NULL, " \t");
                            token4 = strtok(NULL, " \t");
                            if(is_label)
                                token4 = strtok(NULL, " \t");
                            if (token4[0] == ',') 
                                print_error(ERROR31, current_filename, line_count); 
                        }
                        break;
                    }
                    default:{
                        /* Invalid data directive */
                        print_error(ERROR21, current_filename, line_count);
                        break;
                    } 
                }
            }  
            case ENTRY:{ /* Entry directive */
                /*Handle in second pass*/
                break;
            }
            case EXTERN:/* External directive */
            {
                /* Get the argument */
                arg = strtok(NULL, " \t");
                /* Check if argument is valid */
                if (arg && valid_label(arg)) 
                {
                    /* Check if label already exists */
                    if(!is_label_exists(symbols, count_labels, arg)) {
                       add_symbol(&symbols, &count_labels, arg, LABEL_EXTERN, LABEL_TBD, 0);
                    }else {
                        print_error(ERROR3, current_filename, line_count);
                        continue; 
                    }  
                } 
                
            break;
            }    
            case CODE:/* Code directive */
            {
                /*If there is a label, add it to the symbol table*/
                if (is_label) {
                    if(!is_label_exists(symbols, count_labels, label))
                        add_symbol(&symbols, &count_labels, label, LABEL_REGULAR, LABEL_CODE,ic);
                    else {
                        print_error(ERROR3, current_filename, line_count);
                        continue; 
                    }
                }
                L += 1;/* Increment instruction counter */
                /* Get the opcode name */
                opcode_name = get_opcode_name(token1);
                if (!opcode_name) {
                    print_error(ERROR6, current_filename, line_count);
                    continue;
                }
                count_arg = get_opcode_arg(token1);/* Get the number of arguments */
                i = 0; /*reset argument index*/
                /*read the arguments*/
                while (i < count_arg && (arg = strtok(NULL, ",\t"))) {
                    trim(arg); 
                    switch ((arg_type = scan_word(arg))) {
                        case ARG_NUM:/* Immediate */
                            /*Check if immediate argument is valid*/
                            if(!is_valid_argument(opcode_name, i, arg_type)){
                                print_error(ERROR22, current_filename, line_count);
                                break;
                            }
                            /* Check if we are in the first argument or the second */
                            if (i == 0 && count_arg > 1) type1 = 'A';
                            else type2 = 'A';
                            /* Convert number to code */
                            num_to_code8(num_to_int(arg), ic + L, 'I');
                            L += 1;
                            break;
                        case ARG_REG: /* Register */
                            /* Check if register argument is valid */
                            if(!is_valid_argument(opcode_name, i, arg_type)){
                                print_error(ERROR8, current_filename, line_count);
                                break; 
                            }
                            /* Check if we are in the first argument or the second */            
                            if (i == 0 && count_arg > 1) type1 = 'D';
                            else type2 = 'D'; 
                            /* Check if we have two register arguments */
                            if (two_reg_arg == 1){
                                two_reg_code(reg1, arg, ic + L-1, 'I'); 
                                break;
                            }
                            reg1=arg;
                            /* convert registers to code */
                            if(type2 == 'D'){
                                two_reg_code("r0",reg1, ic + L, 'I');
                                L += 1;
                                break;
                            }
                            two_reg_code(reg1, "r0", ic + L, 'I'); 
                            two_reg_arg++;
                            L += 1;
                            break;
                        case LABEL: 
                            /* Check if label argument is valid */
                            if(!is_valid_argument(opcode_name, i, arg_type)){
                                print_error(ERROR9, current_filename, line_count);
                                break;
                            }
                            /* Check if we are in the first argument or the second */
                            if (i == 0 && count_arg > 1) type1 = 'B'; /*label*/
                            else type2 = 'B'; 
                            /* add to missing lines */
                            add_missing_line(ic + L, arg, &missing_lines, line_count); /*add missing line*/
                            L += 1;
                            break;
                        case ARG_MAT:
                            /* Check if matrix argument is valid */
                            if(!is_valid_argument(opcode_name, i, arg_type)){
                                print_error(ERROR7, current_filename, line_count);
                                break; 
                            }
                            /* Check if we are in the first argument or the second */
                            if (i == 0 && count_arg > 1) type1 = 'C'; /*matrix*/
                            else type2 = 'C'; 
                            {
                                char *mat_name = get_matrix_name(arg);
                                add_missing_line(ic + L, mat_name, &missing_lines, line_count); /*add missing line*/
                                free(mat_name);
                            }
                            /* Check if the matrix has two registers */
                            if (get_reg1_matrix_operand(arg) && get_reg2_matrix_operand(arg)) {

                                two_reg_code( get_reg1_matrix_operand(arg), get_reg2_matrix_operand(arg), ic + L + 1, 'I'); /*convert matrix to code*/
                                } else {
                                print_error(ERROR9, current_filename, line_count);
                            }
                            L += 2;
                            break;
                        default:/*Unknown argument type*/
                            print_error(ERROR16, current_filename, line_count);
                            break;
                    }
                    i++;
                }
                /*check if there is more text after the last argument*/
                end_of_line = strtok(NULL, " \t");
                if(end_of_line != NULL) {
                    print_error(ERROR19, current_filename, line_count); 
                }
                /* Check if we have enough arguments */
                if (i < count_arg) {
                    print_error(ERROR17, current_filename, line_count);
                    continue; 
                }
                /*convert opcode to code*/
                op_to_code(opcode_name, type1, type2, ic, 'I');
                ic += L; 
                L = 0; /*reset L for next line*/
                two_reg_arg = 0; /*reset two_reg_arg for next line*/
            break;
            }
            default:{/*Unknown argument type*/
                print_error(ERROR21, current_filename, line_count); 
                break;
            }
        }
    }
    /*Finish processing all lines*/
    ICF = ic;
    DCF = dc;
    update_symbol_address(symbols, count_labels, ICF);
    dc_to_ic(ICF);
    add100(symbols, count_labels);    
    
    /* Check if the instruction code final value is within the valid range */
    if(ic + dc + START_MEMORY_ADDRESS > 255)
    {
        print_error(ERROR36, current_filename, 0);
    }
    fclose(input);
    fclose(f);
    free(am_file);
    return 1;
    }


