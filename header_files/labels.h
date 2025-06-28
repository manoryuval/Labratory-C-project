#define MAX_LABEL_LENGTH 31

typedef enum Type {
    REGULAR,
    ENTRY,
    EXTERN,
    OPERAND
} Type;

typedef enum Address {
    TBD,  /* To Be Determined */
    CODE,
    DATA
} Address;

typedef struct {
    char label[MAX_LABEL_LENGTH];
    Type type;
    Address address;
    int is_data;
} Symbol;

int add_symbol(Symbol **symbols, int *count, const char *label, Type type, Address address, int is_data);
int is_label_exists(Symbol *symbols, int count, const char *label);
int is_label_start(char *line);
int valid_label(char *label);
void free_symbols(Symbol *symbols);

