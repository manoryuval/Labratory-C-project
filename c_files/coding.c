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

void num_to_code(int num, int line, char type) 
{
    char code[5];
    int i;
    int flag = 0;

    /* Check if the number is within the valid range */
    if( num < -512 || num > 511)
   {
      print_error(ERROR35, current_filename, line_count);
      return;
   }
   /* If the number is negative, we will convert it to positive and set a flag */
   if (num < 0)
   {
      num = -num;
      flag = 1;
   }
   /* Convert the number to a 5-character code */
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
   /* If the number was negative, invert the bits and add one */
   if (flag == 1) 
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
   /* Adjust the last character based on the flag */
   i = 4; 
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
   /* Add the code line to the appropriate linked list */
   add_code_line(type ,line, code);

}

void num_to_code8(int num, int line, char type) 
{
   char code[5];
    int i;
    int flag = 0;
    /* Check if the number is within the valid range */
    if( num < -128 || num > 127)
   {
      print_error(ERROR34, current_filename, line_count);
      return;
   }
   /* If the number is negative, we will convert it to positive and set a flag */
   if (num < 0)
   {
      num = -num;
      flag = 1;
   }
   /* Convert the number to a 4-character code */
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
   /* If the number was negative, invert the bits and add one */
   if (flag == 1) 
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
   /* Adjust the last character based on the flag */
   i = 3; 
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
   /* Add the code line to the appropriate linked list */
   add_code_line(type ,line, code);

}

void two_reg_code (char *reg1, char *reg2, int line, char type)
{
    char code[5];
    /* Get the register codes for both registers */
    strcpy(code, get_register_code(reg1));
    strcat(code, get_register_code(reg2));
    strcat(code, "a");/* a - ARE*/
   /* Add the code line to the appropriate linked list */
    add_code_line(type ,line, code);

}

void char_to_code(char c, int line, char type)
{
   /* Convert the character to its ASCII value */
   int i = (int)c;
   /* Check if the ASCII value is within the valid range */
   if( (i > 31 && i < 126) || i == 0) {
      /* If valid, convert to code */
      num_to_code(i, line, type);
      return;
   }
   print_error(ERROR28, current_filename, line_count);

}

void line_to_code(int num, int line, char type)
{
   char code[5];
   int i;
   /* Convert the line number to code */
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
   /* Add the code line to the appropriate linked list */
   add_code_line(type ,line, code);

}

void op_to_code(char* op, char type1, char type2, int line, char type)
{
   char code[5];
   /* Get the opcode code */
   strcpy(code, get_opcode_code(op));
   code[2] = lowercase(type1);
   code[3] = lowercase(type2);
   code[4] = 'a';
   /* Add the code line to the appropriate linked list */
   add_code_line(type ,line, code);
}

void extern_to_code(int line)
{
   char code[WORD_SIZE]= "aaaab";
   /* Convert extern label to code */
   add_code_line('I' ,line, code);

}

void fprint_ICF(char *file_name, int icf)
{
   char *ob_file = create_extension(file_name,".ob");
   FILE *f = fopen(ob_file, "w+");
   code_line *current = ic;
   int i;
   
   /* Print the instruction code final value */
   print_num(f, ICF);
   print_num(f, DCF);
   fprintf(f, "\n");
   /* Print the code lines */
   for (i = 0; i < icf; i++)
   {
      line_fprint(f,START_MEMORY_ADDRESS + i);
      fprintf(f, "%s", current->code);
      fprintf(f, "\n");
      current = current->next;
   }
   fclose(f);
   free(ob_file);
}

void line_fprint(FILE *ob, int num) {
   char line[WORD_LINE_SIZE];
   int i;
   if (!ob) {
      printf("Error opening file \n");
      return;
   }
   /* Print the line number */
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
   /* Print the line number */
   for (i = 0; i < WORD_LINE_SIZE; i++)
   {
      fprintf(ob, "%c", line[i]);
   }
   fprintf(ob, "\t");
} 

void clear_IC_DC()
{
   code_line *current, *next;
   /* Free the entire instruction+data code linked list (they may be connected) */
   current = ic;
   while (current != NULL) {
       next = current->next;
       free(current);
       current = next;
   }
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
         print_error(ERROR11, current_filename, line_count);
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
   /* If the instruction code linked list is empty */
   if (ic == NULL) 
   {
      ic = dc; /* Link the data code linked list to the instruction code linked list */
      return;
   }
   /* If the data code linked list is empty */
   if (dc == NULL) 
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

void print_num(FILE *ob, int num) 
{
   char code[4];
   int i = 0;
   
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

   i = 0;

   while (i < 4 && code[i] == 'a') /* Skip leading 'a's */
   {
      code[i] = '\0';
      i++;
   }

   fprintf(ob, " ");

   for (i = 0; i < 4; i++)
   {
      if (code[i] != '\0')
         fprintf(ob, "%c", code[i]);
   }
  
}


