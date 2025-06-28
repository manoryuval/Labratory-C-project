#define MAX_LINE 256
#define DOT '.'
#define HASH '#'
#define PLUS '+'
#define MINUS '-'
#define COLON ':'
#define COMMA ','
#define COMMENT ';'
#define ASTERISK '*'
#define DOUBLE_QUOTE '\"'
#define NULL_TERMINATOR '\0'

typedef struct instruction {
    char opcode[10];
    char operands[3][30];
} Instruction;
