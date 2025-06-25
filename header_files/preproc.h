#define MAX_LINE 256
#define MAX_MCROS 100
#define MAX_MCRO_BODY 2048
#define MAX_NAME 81


typedef struct Mcro {
    char name[81];
    char *body;
    struct Mcro *next;
} Mcro;


void add_mcro(Mcro **head, char *name, char *body) ;
char *find_mcro_body(Mcro *head, char *name);
int is_mcro_start(char *line);
int is_mcro_end(char *line);
int preproc(char *file_name);
void print_mcro_list(Mcro *head);