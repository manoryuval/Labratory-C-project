#ifndef ANALYSE_H
#define ANALYSE_H

/*type of known words*/
typedef enum
{
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

typedef enum
{
    DATA_,
    STRING_,
    MAT_,
    UNKNOWN_
} DataType;

/* Structure representing an opcode and its associated information */
typedef struct op_code
{
    char *opcode; /* The opcode corresponding to the operation */
    int arg_num;  /* The number of arguments for the operation */
    char *code;   /* The machine code representation of the operation */
} op_code;

typedef struct regs_code
{
    char *reg;  /* The register name */
    char *code; /* The machine code representation of the register */
} regs_code;

typedef struct missing_line
{
    int line;                  /* The line number where the missing label is found */
    char *label;               /* The label that is missing */
    int am_line;            /* The line number in the .am file where the label is missing */
    struct missing_line *next; /* Pointer to the next missing line */
    struct missing_line *prev; /* Pointer to the previous missing line */
} missing_line;

extern missing_line *missing_lines; /* Pointer to the head of the linked list of missing lines */

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
/*
search token from the opcodes array and return the opcode code.
@param token a word to search.
@return the opcode code if found, NULL otherwise.
*/
char *get_opcode_code(char *token);

/*
serch token from the register array and return the register name.
@param token a word to search.
@return the register name if found, NULL otherwise.
*/
char *get_register_name(char *token);
/*
search token from the register array and return the register code.
@param token a word to search.
@return the register code if found, NULL otherwise.
*/
char *get_register_code(char *token);
/*
check if string is in matrix operand format.
@param str a string to check if in matrix format.
@return 1 if format matching. 0 otherwise.
*/
int is_matrix_operand(char *str);
/*
search token from the matrix array and return the matrix name.
@param token a word to search.
@return the matrix name if found, NULL otherwise.
*/
char *get_matrix_name(char *str);
/*
search token from the matrix array and return the first register in the matrix operand.
@param str a string to search.
@return the first register name if found, NULL otherwise.
*/
char *get_reg1_matrix_operand(char *str);
/*
search token from the matrix array and return the second register in the matrix operand.
@param str a string to search.
@return the second register name if found, NULL otherwise.
*/
char *get_reg2_matrix_operand(char *str);
/*
check if string is in matrix definition format.
@param str a string to check if in matrix definition format.
@return the amount of lines the matrix occupies in the source code if format matching. 0 otherwise.
*/
int is_matrix_definition(const char *str);
/*
get the kind of argument from the token.
@param token a word to check.
@return the number of the kind of argument if found, -1 otherwise.
*/
int get_opcode_arg(char *token);
/*
get the kind of data from the token.
@param token a word to check.
@return the kind of data if found, UNKNOWN_ otherwise.
*/
DataType get_data_kind(char *token);
/*
Add a missing line to the linked list.
@param line the line number where the missing label is found.
@param label the label that is missing.
@param head a pointer to the head of the linked list.
*/
void add_missing_line(int line, char *label, missing_line **head, int am_line);
/*
print the missing lines in the linked list.
@param head a pointer to the head of the linked list.
*/
void print_missing_lines(missing_line *head);
/*
remove a node from the linked list.
@param head_ref a pointer to the head of the linked list.
@param current the node to remove.
*/
void remove_node(missing_line **head_ref, missing_line *current);
/*
update the missing lines in the code from the addresses of the labels and remove the missing lines that updated.
@param head a pointer to the head of the linked list.
@param symbols a pointer to the array of symbols.
@param count the number of symbols in the array.
@return the number of missing lines updated.
*/
int update_missing_lines(missing_line *head, Symbol *symbols, int count);
/*
clear the missing lines in the linked list.
*/
void clear_missing_lines();
/*
check if the argument is valid for the command.
@param cmd the command to check.
@param arg_num the number of the arguments to check.
@param type the type of the argument to check.
@return 1 if the argument is valid, 0 otherwise.
*/
int is_valid_argument(char *cmd, int arg_num, WordType type);

#endif
