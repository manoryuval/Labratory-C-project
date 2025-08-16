
#ifndef CODING_H
#define CODING_H

#include "../header_files/analyse.h"

#define START_MEMORY_ADDRESS 100 /* starting memory address at code destination */
#define MAX_INPUT 255 - START_MEMORY_ADDRESS /* maximum input size */
#define WORD_SIZE 5 /* word size in bits */
#define WORD_LINE_SIZE 4 /* word line size in bits */

/* Struct to represent a single line of code */
typedef struct code_line
{
    int line; /* The line number in the code */
    char code[WORD_SIZE]; /* The machine code representation of the line */ 
    struct code_line *next; /* Pointer to the next code in the linked list */
} code_line;
/*
Function to convert a number into code
@param num the number to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void num_to_code(int num, int line, char type); 
/*
Function to convert a number into code with 8 bits
@param num the number to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void num_to_code8(int num, int line, char type); 
/*
Function to convert two registers into code
@param reg1 the first register
@param reg2 the second register
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void two_reg_code (char *reg1, char *reg2, int line, char type); 
/*
Function to convert a character into code
@param c the character to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void char_to_code(char c, int line, char type); 
/*
Function to convert a line number into code
@param num the line number to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void line_to_code(int num, int line, char type); 
/*
Function to convert an operator into code
@param op the operator to convert
@param type1 the type of the first dispatch 
@param type2 the type of the second dispatch 
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void op_to_code(char *op, char type1, char type2, int line, char type);
/*
Function to convert an extern label into code
@param line the line number in the code
*/
void extern_to_code(int line); 
/*
Function to print the instruction code and data code to a file
@param file_name the name of the file to print to
@param icf the instruction code final value
*/
void fprint_ICF(char *file_name, int icf); 
/*
Function to print the number of line to a file
@param file_name the name of the file to print to
@param num the number of the line to print
*/
void line_fprint(FILE *ob, int num); 
/*
Function to clear the instruction code and data code
*/
void clear_IC_DC(); 
/*
Function to add a code line to the appropriate linked list (IC or DC)
@param type the type of the code ('I' for instruction, 'D' for data)
@param line the line number in the code
@param code the machine code string to add
*/
void add_code_line(char type, int line, char *code);
/*
Function to link the data code (DC) linked list to the end of the instruction code (IC) linked list
@param icf the instruction code final value (used to adjust DC line numbers)
*/
void dc_to_ic(int icf);
/*
Function to print a number to a file
@param ob the file to print to
@param num the number to print
*/
void print_num(FILE *ob, int num);
#endif