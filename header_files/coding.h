
#define CODING_H

#include "../header_files/analyse.h"

#define MAX_INPUT 256 /* !#$%^  צריך לוודא%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#define WORD_SIZE 5

extern char IC[MAX_INPUT][WORD_SIZE]; /* Instruction Code */
extern char DC[MAX_INPUT][WORD_SIZE]; /* Data Code */

void num_to_code(int num, int line, char type); /* convert a number into code*/
void two_reg_code (char *reg1, char *reg2, int line, char type); /* convert two/one registers into code */
void char_to_code(char c, int line, char type); /* convert a char into code */
void line_to_code(int num, int line, char type); /* convert a line number into code */
void op_to_code(char *op, char type1, char type2, int line, char type); /* convert an operator into code */
void extern_to_code(int line); /* convert an extern label into code */
void line_print(int num); /* print a line number into the output file */
void print_DCF(int icf); /* לבדיקה בלבד!*/
void print_ICF(int icf);
void dcf_to_icf(int icf,int dcf);