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
        {ERROR32, "label is a reserved word"}
};

void print_error(int error,char* filename, int line_number) {
    error_count++;
    if (line_number == 0){
        printf("~~ERROR: ID:%d~~ in file %s |error message: %s\n", error+1, filename, errors[error].error_msg);
        return;
    }else{
        printf("~~ERROR: in line %d | in file %s | error message: %s ~~ ID:%d \n", line_number ,filename, errors[error].error_msg, error+1);
    }
}
