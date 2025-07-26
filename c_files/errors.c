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
        {ERROR11, "stop"},
        {ERROR12, "invalid argument"},
        {ERROR13, "invalid matrix operand"},
        {ERROR14, "invalid matrix operand"},
        {ERROR15, "invalid matrix operand"}
};

void print_error(int error,char* filename, int line_number) {
    error_count++;
    printf("~~ERROR: ID:%d~~ in file %s |error message %s |in line %d\n", error, filename, errors[error].error_msg, line_number);
}
