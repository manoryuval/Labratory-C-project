#define MAX_INPUT 100 /* !#$%^  צריך לוודא%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#define WORD_SIZE 5

char IC[MAX_INPUT][WORD_SIZE];
char DC[MAX_INPUT][WORD_SIZE];

void num_to_code(int num, int line, char type);
void two_reg_code (int reg1, int reg2, int line, char type);