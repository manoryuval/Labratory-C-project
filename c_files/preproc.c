#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/preproc.h"
#include "../header_files/utils.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"


Mcro *mcro_head = NULL; /* Initialize the global macro head pointer */

/*Function to add a new mcro to the linked list*/
void add_mcro(Mcro **head, char *name, char *body) {
    Mcro *new_node = (Mcro *)malloc(sizeof(Mcro));
    if (!new_node) {
        perror("malloc failed"); /*להוסיף שגיאה*/
        return;
    }
    /* Copy the macro name */
    strncpy(new_node->name, name, 80);
    new_node->name[80] = '\0';

    /* Allocate memory for the body and copy it */
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
    /* If the list is empty, set the new node as the head */
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
    /*run the macro list and serch macro name */
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
    /* Check if the first token is "mcro" */
    if (first && strcmp(first, "mcro") == 0) return 1;
    return 0;
}

int is_mcro_end(char *line) {
    /* Check if the line is "mcroend" */
    return strcmp(line, "mcroend") == 0;
}

int preproc(char *file_name) {
    char *output = create_extension(file_name,".am");
    FILE *f = fopen(output, "w+");/*create output file*/
    FILE *input = fopen(file_name,"r");/*open input file*/
    char line[MAX_LINE];
    char mcro_name[MAX_NAME] ;
    mcro_name[0] = '\0';
    char mcro_body[MAX_MCRO_BODY] ;
    mcro_body[0] = '\0';
    char *body;/*body of the macro*/
    int in_mcro = 0;/*in macro flag*/
    char *name;/*name of the macro*/
    /* Check if the file was opened successfully */
    if (!input || !f) {
        perror("File error");/*שגיאת קובץ להוסיף שגיאה*/
        return 1;
    }
    /* Read the input file line by line */
    while (fgets(line, sizeof(line), input)) {
        char trim_line[MAX_LINE];
        strcpy(trim_line, line);
        trim(trim_line);
        char line_copy[MAX_LINE];
        strcpy(line_copy, line);
        /* Tokenize the line */
        strtok(line_copy, " ");
        /* Check if the first token is "mcro" */
        if (in_mcro==0 && is_mcro_start(trim_line)) {
            in_mcro = 1;/* Set the in_mcro flag */
            /* set name */
            name = strtok(NULL, " ");
            if (name == NULL) {
                fprintf(stderr, "Error: Macro name is missing.\n");/*שגיאה*/
                continue; /* Skip to the next line */
            }
            /* Copy the macro name */
            strcpy(mcro_name, name);
            /*לבדוק שהשם לא אותו שם כמו הנחיה*/
            continue;
        }
        /* Check if the line is the end of a macro definition */
        if(is_mcro_end(trim_line)){
            /* Add the macro to the list */
            add_mcro(&mcro_head,mcro_name, mcro_body);
            mcro_body[0] = '\0';/* Clear the macro body */
            in_mcro = 0;/* Set the in_mcro flag */
            continue;
        }
        /* Check if we are inside a macro definition */
        if (in_mcro){
            /* Append the line to the macro body */
            strcat(mcro_body, line);
            continue;
        } else{
            /* Find the macro body */
            body  = find_mcro_body(mcro_head,trim_line);
            if (body != NULL){
                /* Replace the line with the macro body */
                fprintf(f, body, MAX_LINE);
            }
            else{
                /* If the macro body is not found, print the original line */
                fprintf(f, line, MAX_LINE);
            }
    }
    }
    print_mcro_list(mcro_head);
    fclose(f);/* Close the output file */
    fclose(input);/* Close the input file */
    free(output);/* Free the output file name */
    return 1;
}

/* Prints the list of macros */
void print_mcro_list(Mcro *head) {
    while (head != NULL) {
        printf(" NAME: %s\n", head->name);
        printf("BODY: %s\n", head->body);
        head = head->next;
    }
}

/* Frees all memory allocated for the Mcro linked list */
void free_mcro_list(Mcro *head) {
    Mcro *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        if (tmp->body) free(tmp->body);
        free(tmp);
    }
}