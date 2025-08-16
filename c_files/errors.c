#include <stdio.h>
#include "../header_files/errors.h"

int error_count = 0;

Error errors[] = {
        {ERROR1,  "Could not open file"},
        {ERROR2,  "label is not valid"},
        {ERROR3,  "label already exists"},
        {ERROR4,  "invalid number"},
        {ERROR5,  "invalid matrix definition"},
        {ERROR6,  "invalid opcode"},
        {ERROR7,  "invalid matrix operand"},
        {ERROR8,  "invalid register"},
        {ERROR9,  "invalid label operand"},
        {ERROR10, "label dosent exists"},
        {ERROR11, "allocation memory error"},
        {ERROR12, "label is too long"},
        {ERROR13, "label is a macro name"},
        {ERROR14, "macro name is missing"},
        {ERROR15, "line is missing"},
        {ERROR16, "unknown argument"},
        {ERROR17, "missing argument"},
        {ERROR18, "missing comma"},
        {ERROR19, "Extraneous text after end of line"},
        {ERROR20, ".as files name is missing"},
        {ERROR21, "Unknown word"},
        {ERROR22, "Invalid addressing method"},
        {ERROR23, "Invalid matrix format"},
        {ERROR24, "Invalid matrix operand"},
        {ERROR25, "Invalid matrix name"},
        {ERROR26, "multiple consecutive commas"},
        {ERROR27, "illegal comma"},
        {ERROR28,"illegal character"},
        {ERROR29, "illegal declaration of string"},
        {ERROR30, "comma after last argument"},
        {ERROR31, "comma before first argument"},
        {ERROR32, "label is a reserved word"},
        {ERROR33, "try to entering a label that doesn't exist"},
        {ERROR34, "the number you enterd are not between (-128) - (127)"},
        {ERROR35, "the number you enterd are not between (-512) - (511)"},
        {ERROR36, "line address exceeded over 255. you need to reduce the number of lines in the file code"},
        {ERROR37, "the amount of numbers you enterd to the matrix is not equal to the matrix size"},
        {ERROR38, "the amount of data entered to the matrix is more than the matrix size declaration"},
        {ERROR39, "label is not start with a letter"},
        {ERROR40, "line is too long"} /* This is a placeholder for the last error */
};

void print_error(int error,char* filename, int line_number) {
    error_count++;
    /* Print the error message */
    if (line_number == 0){
        printf("%s: %s. (ID:%d)\n", filename, errors[error].error_msg, error+1);
        return;
    }else{
        printf("%s.%d: %s. (ID:%d)\n",filename, line_number , errors[error].error_msg, error+1);
    }
}
