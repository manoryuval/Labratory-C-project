#include <stdio.h>
#include "../header_files/errors.h"

int error_count = 0;

Error errors[] = {
        {ERROR1,  "Could not open file"},
        {ERROR2,  "label is not valid"},
        {ERROR3,  "label already exists"},
        {ERROR4,  "invalid number"},
        {ERROR5,  "Invalid matrix definition"},
        {ERROR6,  "invalid opcode"},
        {ERROR7,  "invalid matrix operand"},
        {ERROR8,  "invalid register"},
        {ERROR9,  "Invalid label operand"},
        {ERROR10, "label dosent exists"},
        {ERROR11, "allocation memory error"},
        {ERROR12, "label is too long"},
        {ERROR13, "label is a macro name"},
        {ERROR14, "macro name is missing"},
        {ERROR15, "line is missing"},
        {ERROR16, "argument is unknown"},
        {ERROR17, "missing argument"},
        {ERROR18, "missing comma"}
};

void print_error(int error,char* filename, int line_number) {
    error_count++;
    if (line_number == 0){
        printf("~~ERROR: ID:%d~~ in file %s |error message: %s\n", error+1, filename, errors[error].error_msg);
        return;
    }else{
        printf("~~ERROR: ID:%d~~ in file %s |error message: %s |in line %d\n", error+1, filename, errors[error].error_msg, line_number);
    }
}
