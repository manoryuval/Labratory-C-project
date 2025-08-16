#ifndef ERRORS_H
#define ERRORS_H

/* Structure to represent an error */
typedef struct Error {
    int error_id;     /* Unique identifier for the error */
    char *error_msg;  /* Error message associated with the error */
} Error;

/* Error codes */
typedef enum Error_code {
    ERROR1,
    ERROR2,
    ERROR3,
    ERROR4,
    ERROR5,
    ERROR6,
    ERROR7,
    ERROR8,
    ERROR9,
    ERROR10,
    ERROR11,
    ERROR12,
    ERROR13,
    ERROR14,
    ERROR15,
    ERROR16,
    ERROR17,
    ERROR18,
    ERROR19,
    ERROR20,
    ERROR21,
    ERROR22,
    ERROR23,
    ERROR24,
    ERROR25,
    ERROR26,
    ERROR27,
    ERROR28,
    ERROR29,
    ERROR30,
    ERROR31,
    ERROR32,
    ERROR33,
    ERROR34,
    ERROR35,
    ERROR36,
    ERROR37,
    ERROR38,
    ERROR39,
    ERROR40
} Error_code;

extern int error_count;
extern char *current_filename;

/*
 Function to print error messages
@param error The error code
@param filename The name of the file where the error occurred
@param line_number The line number where the error occurred
*/
void print_error(int error,char* filename, int line_number);

#endif