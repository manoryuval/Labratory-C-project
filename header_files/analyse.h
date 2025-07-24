#ifndef ANALYSE_H
#define ANALYSE_H

/*type of known words*/
typedef enum {
    DATA,
    CODE,
    ENTRY,
    EXTERN,
    ARG_NUM,
    ARG_REG,
    ARG_MAT,
    LABEL,
    STRING,
    NUMBER,
    MAT,
    UNKNOWN
} WordType;

typedef enum {
    DATA_,
    STRING_,
    MAT_,
    UNKNOWN_
} DataType;

/* Structure representing an opcode and its associated information */
typedef struct op_code {
    char *opcode;    /* The opcode corresponding to the operation */
    int arg_num;  /* The number of arguments for the operation */
    char *code;    /* The machine code representation of the operation */
} op_code;

typedef struct regs_code {
    char *reg;    /* The register name */
    char *code;    /* The machine code representation of the register */
} regs_code;

typedef struct missing_line 
{
    int line; /* The line number where the missing label is found */
    char *label; /* The label that is missing */
    struct missing_line *next; /* Pointer to the next missing line */
    struct missing_line *prev; /* Pointer to the previous missing line */
} missing_line;

typedef struct extern_line
{
    int line; /* The line number where the missing label is found */
    char *label; /* The label that is missing */
    struct extern_line *next; /* Pointer to the next missing line */
    struct extern_line *prev; /* Pointer to the previous missing line */
} extern_line;

extern regs_code REGS[];
extern missing_line *missing_lines; /* Pointer to the head of the linked list of missing lines */
extern extern_line *extern_lines; /* Pointer to the head of the linked list of extern lines */
/*
 Scans a token and determines its type.
 @param token a word to scan.
 @return the type of the word. if the word is not recognized, returns UNKNOWN.
 */
WordType scan_word(char *token);

/*
serch token from the opcodes array and return the opcode name.
@param token a word to search.
@return the opcode name if found, NULL otherwise.
*/
char *get_opcode_name(char *token);

char *get_opcode_code(char *token);

/*
serch token from the register array and return the register name.
@param token a word to search.
@return the register name if found, NULL otherwise.
*/
char *get_register_name(char *token);

char *get_register_code(char *token);



/*
check if string is in matrix operand format.
@param str a string to check if in matrix format.
@return 1 if format matching. 0 otherwise.
*/
int is_matrix_operand( char *str);

char *get_matrix_name( char *str);

char *get_reg1_matrix_operand( char *str);

char *get_reg2_matrix_operand( char *str);

/*
check if string is in matrix definition format.
@param str a string to check if in matrix definition format.
@return Multiplication of place in the matrix if format matching. 0 otherwise.
*/
int is_matrix_definition(const char *str);


int get_opcode_arg(char *token);



DataType get_data_kind(char *token);

int num_to_int(char *token);

int is_number(char *token);

void add_missing_line(int line, char *label, missing_line **head);

void print_missing_lines(missing_line *head);

void remove_node(missing_line **head_ref, missing_line *current);

int update_missing_lines(missing_line *head/*,extern_line **extern_lines*/ , Symbol *symbols, int count);

#endif 
