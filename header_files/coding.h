#define MAX_INPUT 100 /* !#$%^  צריך לוודא%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#define WORD_SIZE 5

void num_to_code(int num, int line, char type);
void two_reg_code (int reg1, int reg2, int line, char type);
void char_to_code(char c, int line, char type);
int line_to_code(int num, int line, char type);
void op_to_code(int op, char type1, char type2, int line, char type);