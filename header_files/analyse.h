
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
    char *opcode;    /* The opcode corresponding to the operation */
    char *code;    /* The machine code representation of the operation */
} regs_code;

extern regs_code REGS[];



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
serch token from the register array and return the register name.
@param token a word to search.
@return the register name if found, NULL otherwise.
*/
char *get_register_name(char *token);

/*
check if string is in matrix operand format.
@param str a string to check if in matrix format.
@return 1 if format matching. 0 otherwise.
*/
int is_matrix_operand(const char *str);

/*
check if string is in matrix definition format.
@param str a string to check if in matrix definition format.
@return 1 if format matching. 0 otherwise.
*/
int is_matrix_definition(const char *str);


int get_opcode_arg(char *token);



DataType get_data_kind(char *token);