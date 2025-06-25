typedef struct {
    char label[31];
    int address;
    int is_data;
} Symbol;

int add_symbol(Symbol **symbols, int *size, const char *label, int address, int is_data);