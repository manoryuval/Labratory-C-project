#define MAX_MCROS 100
#define MAX_MCRO_BODY 2048
#define MAX_NAME 81

/* Macro definition structure. */
typedef struct Mcro {
    char name[MAX_NAME];
    char *body;
    struct Mcro *next;
} Mcro;
/* Head of the macro list */
extern Mcro *mcro_head;
extern int line_count; /* Global variable to keep track of the line count */

/*add new macro to the list
@param head Pointer to the head of the macro list
@param name Name of the macro
@param body Body of the macro*/
void add_mcro(Mcro **head, char *name, char *body) ;

/*find macro from the list by name
@param head Pointer to the head of the macro list
@param name Name of the macro to search
@return Pointer to the macro body, or NULL if not found
*/
char *find_mcro_body(Mcro *head, char *name);

/*
Check if a line is the start of a macro definition.
@param line The line to check.
@return 1 if the line is a macro start, 0 otherwise.
 */
int is_mcro_start(char *line);


/*
Check if a line is the end of a macro definition.
@param line The line to check.
@return 1 if the line is a macro end, 0 otherwise.
*/
int is_mcro_end(char *line);

/*
Preprocess the input file and generate the output file.
@param file_name The name of the input file.
@return 0 on success, 1 on failure.
*/
int preproc(char *file_name);

/* Print the list of macros 
@param head Pointer to the head of the macro list
*/
void print_mcro_list(Mcro *head);

/*
Free the memory allocated for the macro list.
*/
void clear_macros();