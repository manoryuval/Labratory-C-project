
#define CODING_H

#include "../header_files/analyse.h"

#define MAX_INPUT 256 /* !#$%^  צריך לוודא%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#define WORD_SIZE 5
#define WORD_LINE_SIZE 4
#define FILE_LINE_VALUE 100

extern char IC[MAX_INPUT][WORD_SIZE]; /* Instruction Code */
extern char DC[MAX_INPUT][WORD_SIZE]; /* Data Code */

/*Function to convert a number into code
@param num the number to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void num_to_code(int num, int line, char type); 
/*Function to convert a number into code with 8 bits
@param num the number to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void num_to_code8(int num, int line, char type); 
/*Function to convert two registers into code
@param reg1 the first register
@param reg2 the second register
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void two_reg_code (char *reg1, char *reg2, int line, char type); 
/*Function to convert a character into code
@param c the character to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void char_to_code(char c, int line, char type); 
/*Function to convert a line number into code
@param num the line number to convert
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void line_to_code(int num, int line, char type); 
/*Function to convert an operator into code
@param op the operator to convert
@param type1 the type of the first dispatch 
@param type2 the type of the second dispatch 
@param line the line number in the code
@param type the type of the code ('I' for instruction, 'D' for data)
*/
void op_to_code(char *op, char type1, char type2, int line, char type);
/*Function to convert an extern label into code
@param line the line number in the code
*/
void extern_to_code(int line); 
/*Function to print the number of line to a file
@param file_name the name of the file to print to
@param num the number of the line to print
*/
void line_fprint(FILE *ob, int num); 
/*Function to copy DC to IC
@param icf the instruction code final value
@param dcf the data code final value
*/
void dcf_to_icf(int icf,int dcf); 
/*Function to print the instruction code and data code to a file
@param file_name the name of the file to print to
@param icf the instruction code final value
*/
void fprint_ICF(char *file_name, int icf); 

void print_DCF(char *file_name, int dcf); /* לבדיקה בלבד!*/
void print_ICF( int icf); /* לבדיקה בלבד!*/