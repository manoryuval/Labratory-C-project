#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../header_files/assembler.h"
#include "../header_files/utils.h"
#include "../header_files/preproc.h"
#include "../header_files/labels.h"
#include "../header_files/first_pass.h"
#include "../header_files/analyse.h"
#include "../header_files/coding.h"
#include "../header_files/errors.h"

code_line *ic = NULL; /* Pointer to the head of the instruction code linked list */
code_line *dc = NULL; /* Pointer to the head of the data code linked list */

void num_to_code(int num, int line, char type) /* מניח שהמספר שמתקבל בין -512 ל513 ושמתקבל מספר ולא משהו לא תקין*/
{
    char code[5];
    int i;
    int flag = 0;

    if (num < 0)
    {
         num = -num;
         flag = 1;
    }

    
   for (i = 4; i >= 0; i--)
   {
      switch (num%4) 
      {
         case 0:
            code[i] = 'a';
            break;
         case 1:
            code[i] = 'b';
            break;
         case 2:
            code[i] = 'c';
            break;
         case 3:
            code[i] = 'd';
            break;
      }
      num /= 4;
   }

   if (flag == 1) /* נהפוך את הסיביות אם המספר היה שלילי ונוסיף אחד */
   {
      for (i = 0; i < 5; i++)
      {
         switch (code[i])
         {
            case 'a': code[i] = 'd'; break;
            case 'b': code[i] = 'c'; break;
            case 'c': code[i] = 'b'; break;
            case 'd': code[i] = 'a'; break;
         }
      }
   }

   i = 4; /* נתחיל מהאחרון */
   while (flag && i >=0)
   {
      switch (code[i])
      {
         case 'a': code[i] = 'b'; flag = 0; break;
         case 'b': code[i] = 'c'; flag = 0; break;
         case 'c': code[i] = 'd'; flag = 0; break;
         case 'd': code[i] = 'a'; i--; break;
      }
   }

   add_code_line(type ,line, code);

}
void num_to_code8(int num, int line, char type) /* להכניס תקלה על מספר גדול מידי ל8 ביטים  */
{
   char code[5];
    int i;
    int flag = 0;

    if (num < 0)
    {
         num = -num;
         flag = 1;
    }

    
   for (i = 3; i >= 0; i--)
   {
      switch (num%4) 
      {
         case 0:
            code[i] = 'a';
            break;
         case 1:
            code[i] = 'b';
            break;
         case 2:
            code[i] = 'c';
            break;
         case 3:
            code[i] = 'd';
            break;
      }
      num /= 4;
   }

   if (flag == 1) /* נהפוך את הסיביות אם המספר היה שלילי ונוסיף אחד */
   {
      for (i = 0; i < 4; i++)
      {
         switch (code[i])
         {
            case 'a': code[i] = 'd'; break;
            case 'b': code[i] = 'c'; break;
            case 'c': code[i] = 'b'; break;
            case 'd': code[i] = 'a'; break;
         }
      }
   }

   i = 3; /* נתחיל מהאחרון */
   while (flag && i >=0)
   {
      switch (code[i])
      {
         case 'a': code[i] = 'b'; flag = 0; break;
         case 'b': code[i] = 'c'; flag = 0; break;
         case 'c': code[i] = 'd'; flag = 0; break;
         case 'd': code[i] = 'a'; i--; break;
      }
   }
   code[4] = 'a';

   add_code_line(type ,line, code);

}
void two_reg_code (char *reg1, char *reg2, int line, char type)
{
    char code[5];
    strcpy(code, get_register_code(reg1));
    strcat(code, get_register_code(reg2));
    strcat(code, "a");/* a - ARE*/

    add_code_line(type ,line, code);

}
void char_to_code(char c, int line, char type)
{

   int i = (int)c;
   if( (i > 32 && i < 126) || i == 0) {
      num_to_code(i, line, type); /* נשתמש בפונקציה num_to_code כדי להמיר את התו לקוד */ 
      return;
   }
   print_error(ERROR28, current_filename, line_count); /* Illegal character */

}
void op_to_code(char* op, char type1, char type2, int line, char type)
{
      char code[5];
   
      strcpy(code, get_opcode_code(op));
      code[2] = lowercase(type1);
      code[3] = lowercase(type2);
      code[4] = 'a';

      add_code_line(type ,line, code);
}
void line_to_code(int num, int line, char type) /* להכניס תקלה על מספר שורה גדול מידי ל8 ביטים*/
{
    char code[5];
    int i;
    
   for (i = 3; i >= 0; i--)
   {
      switch (num%4) 
      {
         case 0:
            code[i] = 'a';
            break;
         case 1:
            code[i] = 'b';
            break;
         case 2:
            code[i] = 'c';
            break;
         case 3:
            code[i] = 'd';
            break;
      }
      num /= 4;
   }
   code[4] = 'c'; /*ARE - always 10 (E) */
 
   add_code_line(type ,line, code);

}
void line_fprint(FILE *ob, int num) {
   char line[WORD_LINE_SIZE];
   int i;
   if (!ob) {
      printf("Error opening file \n");/*שגיאה*/
      return;
   }
   for (i = 3; i >= 0; i--)
   {
      switch (num%4) 
      {
         case 0:
            line[i] = 'a';
            break;
         case 1:
            line[i] = 'b';
            break;
         case 2:
            line[i] = 'c';
            break;
         case 3:
            line[i] = 'd';
            break;
      }
      num /= 4;
   }
   for (i = 0; i < WORD_LINE_SIZE; i++)
      {
            fprintf(ob, "%c", line[i]);
      }
   fprintf(ob, "\t");
} 

void extern_to_code(int line)
{
   char code[WORD_SIZE]= "aaaab"; 
   add_code_line('I' ,line, code);

}

void fprint_ICF(char *file_name, int icf)
{
   char *ob_file = create_extension(file_name,".ob");
   FILE *f = fopen(ob_file, "w+");
   code_line *current = ic;
   int i;

   for (i = 0; i < icf; i++)
   {
      line_fprint(f,START_MEMORY_ADDRESS + i);
      fprintf(f, "%s", current->code);
      fprintf(f, "\n");
      current = current->next;
   }
   fclose(f);
}

void clear_IC_DC()
{
   printf("free1 \n");
   ic = NULL;
   dc = NULL;
}

void add_code_line(char type, int line, char *code)
{
    code_line **head = (type == 'I') ? &ic : &dc;
    code_line *current;
    code_line *prev;
    code_line *newNode;

    /* If list is empty, create head node */
    if (*head == NULL) {
        *head = malloc(sizeof(code_line));
        if (!*head) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        (*head)->line = line;
        strcpy((*head)->code, code);
        (*head)->next = NULL;
        return;
    }

   current= *head;
   prev = NULL;

    /* Traverse to find insertion point (sorted by line) */
    while (current != NULL && current->line < line) {
        prev = current;
        current = current->next;
    }

    /* If the line already exists → overwrite code */
    if (current != NULL && current->line == line) {
        strcpy(current->code, code);
        return;
    }

    /* Create new node */
    newNode = malloc(sizeof(code_line));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->line = line;
    strcpy(newNode->code, code);

    /* Insert at correct place */
    newNode->next = current;
    if (prev == NULL) {
        /* Insert at head */
        *head = newNode;
    } else {
        prev->next = newNode;
    }
}

void dc_to_ic(int icf)
{
   code_line *current = ic;
   code_line *dc_current = dc;
   
   if (ic == NULL) /* If the instruction code linked list is empty */
   {
      ic = dc; /* Link the data code linked list to the instruction code linked list */
      return;
   }
   if (dc == NULL) /* If the data code linked list is empty */
      return; /* Nothing to do, return */

   /* Traverse to the end of the IC linked list */
   while (current->next != NULL)
   {
      current = current->next;
   }
   
   /* Link the end of IC to the start of DC */
   current->next = dc;
   
   /* Update line numbers in DC part by adding icf (instruction counter final) */
   while (dc_current != NULL)
   {
      dc_current->line += icf;
      dc_current = dc_current->next;
   }
}


void print_code_lines() /* למחוק לפני הגשה */
 {
   code_line *current = ic;
   printf("print code lines\n");
   while (current != NULL) {
      printf("Line %d: %s\n", current->line, current->code);
      current = current->next;
   }
}
