#define MAX_LABEL_LENGTH 31
/* Label types */
typedef enum Type {
    REGULAR,
    ENTRY,
    EXTERN,
    OPERAND
} Type;
/* Address types */
typedef enum Address {
    TBD,  /* To Be Determined */
    CODE,
    DATA
} Address;

/* Symbol table entry */
typedef struct {
    char label[MAX_LABEL_LENGTH];
    Type type;
    Address address;
    int is_data;
} Symbol;

/* Function to add a symbol to the symbol table 
@param symbols Pointer to the symbol table
@param count Pointer to the number of symbols in the table
@param label The label to add
@param type The type of the label (REGULAR, ENTRY, EXTERN, OPERAND)
@param address The address of the label (CODE, DATA, TBD)
@param is_data Flag indicating if the label is data (1) or not (0)
@return 1 on success, 0 on failure
*/
int add_symbol(Symbol **symbols, int *count, const char *label, Type type, Address address, int is_data);

/*
Check if a label exists in the symbol table.
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
@param label The label to check
@return 1 if the label exists, 0 otherwise
 */
int is_label_exists(Symbol *symbols, int count, char *label);
/*
Check if a line starts with a label.
@param line The line to check
@return 1 if the line starts with a label, 0 otherwise
 */
int is_label_start(char *line);

/* Check if a label is valid.
@param label The label to check
@return 1 if the label is valid, 0 otherwise
*/
int valid_label(char *label);

/* Print the symbol table. 
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
*/
void print_symbols(Symbol *symbols, int count);

/* Free the symbol table.
@param symbols Pointer to the symbol table
*/
void free_symbols(Symbol *symbols);

