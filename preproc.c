#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preproc.h"
#include "utils.h"

/*Function to add a new mcro to the linked list*/
void add_mcro(Mcro **head, char *name, char *body) {
    Mcro *new_node = (Mcro *)malloc(sizeof(Mcro));
    if (!new_node) {
        perror("malloc failed"); /*להוסיף שגיאה*/
        return;
    }

    strncpy(new_node->name, name, 80);
    new_node->name[80] = '\0';


    if (body != NULL) {
        size_t body_len = strlen(body); 
        new_node->body = (char *)malloc(body_len + 1);
        if (new_node->body == NULL) {
            perror("Failed to allocate memory for Mcro body");/*להוסיף שגיאה*/
            free(new_node); 
            return;
        }
        strcpy(new_node->body, body);
    } else {
        new_node->body = NULL;
    }
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Mcro *curr = *head;
        while (curr->next) {
            curr = curr->next;
        }
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
    char mcro_name[MAX_NAME] ;
    mcro_name[0] = '\0';
    char mcro_body[MAX_MCRO_BODY] ;
    mcro_body[0] = '\0';
    char *body;
    int in_mcro = 0;
    Mcro *mcro_head = NULL;

    if (!input || !f) {
        perror("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }
    while (fgets(line, sizeof(line), input)) {
        char trim_line[MAX_LINE];
        strcpy(trim_line, line);
        trim(trim_line);
        char line_copy[MAX_LINE];
        strcpy(line_copy, line);
        strtok(line_copy, " ");

        if (in_mcro==0 && is_mcro_start(trim_line)) {
            in_mcro = 1;
            char *name = strtok(NULL, " ");
            strcpy(mcro_name, name);
            /*לבדוק שהשם לא אותו שם כמו הנחיה*/
            continue;
        }

        if(is_mcro_end(trim_line)){
            add_mcro(&mcro_head,mcro_name, mcro_body);
            mcro_body[0] = '\0';
            in_mcro = 0;
            continue;
        }
        if (in_mcro){
            strcat(mcro_body, line);
            continue;
        } else{
            body  = find_mcro_body(mcro_head,trim_line);
            if (body != NULL){
                fprintf(f, body, MAX_LINE);
            }
            else{
                fprintf(f, line, MAX_LINE);
            }
    }
    }
    fclose(f);
    return 1;
}

void print_mcro_list(Mcro *head) {
    while (head != NULL) {
        printf(" NAME: %s\n", head->name);
        printf("BODY: %s\n", head->body);
        head = head->next;
    }
}