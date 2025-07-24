#define MAX_LABEL_LENGTH 31
/* Label types */
typedef enum Type {
    LABEL_REGULAR,
    LABEL_ENTRY,
    LABEL_EXTERN,
    LABEL_OPERAND
} Type;
/* Address types */
typedef enum MODE {
    LABEL_TBD,  /* To Be Determined */
    LABEL_CODE,
    LABEL_DATA
} Mode;

/* Symbol table entry */
typedef struct {
    char label[MAX_LABEL_LENGTH];
    Type type;
    Mode mode; 
    int address;
} Symbol;


extern Symbol *symbols;


/* Function to add a symbol to the symbol table 
@param symbols Pointer to the symbol table
@param count Pointer to the number of symbols in the table
@param label The label to add
@param type The type of the label (REGULAR, ENTRY, EXTERN, OPERAND)
@param mode The mode of the label (CODE, DATA, TBD)
@param address The address of the label
@return 1 on success, 0 on failure
*/
int add_symbol(Symbol **symbols, int *count, const char *label, Type type, Mode mode, int address);

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
/*
Copy a label from source to destination.
@param dest The destination string
@param src The source string
*/
void copy_label(char *dest, char *src);
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

/*
Find the data symbols in the table and update the address
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
@param ICF final row in ic array
@return 1 when loop over
*/
int update_symbol_address(Symbol *symbols, int count, int ICF);

/*
Find the symbol in the table and update its type
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
@param label the name of label to update
@param type the type to update to
@return 1 if succssed (find label and update type), otherwise 0.
*/
int update_symbol_type(Symbol *symbols, int count,char *label,  Type type);


/*
count the label in entry type
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
@return sum of label with entry type
*/
int entry_count(Symbol *symbols, int count);

/*
count the label in extern type
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
@return sum of label with extern type
*/

int extern_count(Symbol *symbols, int count);

/*
add 100 to address of symbols tabel and missing lines
@param symbols Pointer to the symbol table
@param count Number of symbols in the table
*/
void add100(Symbol *symbols, int count_labels);



/* Free the symbol table.
@param symbols Pointer to the symbol table
*/
void free_symbols(Symbol *symbols, int count_labels);

