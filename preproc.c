#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preproc.h"
#include "utils.h"

/*Function to add a new mcro to the linked list*/
void add_mcro(Mcro **head, char *name, char *body) {
   Mcro *new_node = malloc(sizeof(Mcro));
   printf("%s,%s\n", name, body);
   if (!new_node) {
       printf("eror: Memory allocation failed for new mcro.\n");/*שגיאה בהמשך*/
        return;
   }
   strncpy(new_node->name, name, sizeof(new_node->name));
   new_node->body = body;  
   new_node->next = NULL;
   if (*head == NULL) {
       *head = new_node;
   } else {
       Mcro *curr = *head;
       while (curr->next) curr = curr->next;
       curr->next = new_node;
   }
}

char *find_mcro_body(Mcro *head, char *name) {
    while (head) {
        if (strcmp(head->name, name) == 0) {
            return head->body;
        }
        head = head->next;
    }
    return NULL;
}

void trim(char *line) {
    char *end;
    while (isspace((unsigned char)*line)) line++; /* leading spaces*/
    end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) *end-- = '\0';
}

int is_mcro_start(char *line) {
    char line_copy[MAX_LINE];
    strcpy(line_copy, line);
    char *first = strtok(line_copy, " ");
    if (first && strcmp(first, "mcro") == 0) return 1;
    return 0;
}

int is_mcro_end(char *line) {
    return strcmp(line, "mcroend") == 0;
}

int preproc(char *file_name) {
    char *output = create_extension(file_name,".am");
    FILE *f = fopen(output, "w+");
    FILE *input = fopen(file_name,"r");
    char line[MAX_LINE];
    char mcro_name[MAX_NAME];
    char mcro_body[MAX_MCRO_BODY];
    int in_mcro = 0;
    Mcro *mcro_head = NULL;

    if (!input || !f) {
        perror("File error");/*שגיאת קובץ*/
        return 1;
    }
    while (fgets(line, sizeof(line), input)) {
        char trim_line[MAX_LINE];
        strcpy(trim_line, line);
        trim(trim_line);
        char line_copy[MAX_LINE];
        strcpy(line_copy, line);
        char *first = strtok(line_copy, " ");

        if (in_mcro==0 && is_mcro_start(trim_line)) {
            in_mcro = 1;
            char *name = strtok(NULL, " ");
            printf("%s\n", name);
            strcpy(mcro_name, name);
            continue;
        }

        if(is_mcro_end(trim_line)){
            add_mcro(&mcro_head,mcro_name, mcro_body);
            in_mcro = 0;
            continue;
        }
        if (in_mcro){
            strcat(mcro_body, line);
            continue;
        } else{
            if (find_mcro_body(mcro_head,line) != NULL){
                fprintf(f, "body", MAX_LINE);
                fprintf(f, find_mcro_body(mcro_head,line), MAX_LINE);
                mcro_name[0]='\0';
            }
            else{
                fprintf(f, line, MAX_LINE);
            }
    }
    }
    fclose(f);
    return 1;
}

int print_mcro_list(Mcro *head){
    
}