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

char IC[MAX_INPUT][WORD_SIZE];
char DC[MAX_INPUT][WORD_SIZE];

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
   switch (type)
   {
    case 'I': /* Instruction */
      strcpy(IC[line], code); /* נעתיק את הקוד למערך ה-IC */
      break;
    case 'D': /* Data */
      strcpy(DC[line], code); /* נעתיק את הקוד למערך ה-DC */
    break;
   default: break;
   }
}
void num_to_code8(int num, int line, char type)
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
   switch (type)
   {
    case 'I': /* Instruction */
      strcpy(IC[line], code); /* נעתיק את הקוד למערך ה-IC */
         /* printf("\n\n%s\n\n", IC[line]); */
      break;
    case 'D': /* Data */
       strcpy(DC[line], code); /* נעתיק את הקוד למערך ה-DC */
    break;
   default: break;
   }
}
void two_reg_code (char *reg1, char *reg2, int line, char type)
{
    char code[5];
    strcpy(code, get_register_code(reg1));
    strcat(code, get_register_code(reg2));

    strcat(code, "a");/* a - ARE*/

    add_code_line(type ,line, code);
\
    switch (type)
    {
        case 'I': /* Instruction */
            strcpy(IC[line], code);
\
            break;
        case 'D': /* Data */
            strcpy(DC[line], code);
            break;
        default:
            fprintf(stderr, "Error: Invalid type for two_reg_code function.\n");
            exit(EXIT_FAILURE);
    }
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
      int i;
      /* נעתיק את הקוד של האופרטור */
      strcpy(code, get_opcode_code(op));

      /* נוסיף את סוגי מיון האופרנדים */
      code[2] = lowercase(type1);
      code[3] = lowercase(type2);
      /* נוסיף את ה-ARE */
      code[4] = 'a';
      add_code_line(type ,line, code);
      switch (type)
      {
         case 'I': /* Instruction */
               for( i = 0; i < WORD_SIZE; i++) {
                   IC[line][i] = code[i]; /* Copy the code to the IC array */
               }
               break;
         case 'D': /* Data */
               strcpy(DC[line], code);
               break;
      }
}
void line_to_code(int num, int line, char type)
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
   switch (type)
   {
    case 'I': /* Instruction */
      for(i = 0; i < WORD_SIZE; i++) {
                   IC[line][i] = code[i]; /* Copy the code to the IC array */
               }
      break;
    case 'D': /* Data */
       strcpy(DC[line], code); /* נעתיק את הקוד למערך ה-DC */
    break;
   default: break;
   }
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
   int i;
   add_code_line('I' ,line, code);
   for( i = 0; i < WORD_SIZE; i++) {
            IC[line][i] = code[i]; /* Copy the code to the IC array */
      }

}
void print_DCF(char *file_name, int dcf)
{
   char *ob_file = create_extension(file_name,".ob");
   FILE *f = fopen(ob_file, "w+");
   int i = 0;
   int j = 0;
   for (i = 0; i < dcf; i++)
   {
      fprintf(f, "DC[%d]: ", i);
      for (j = 0; j < WORD_SIZE; j++)
      {
            fprintf(f, "%c", DC[i][j]);
      }
      fprintf(f, "\n");
   }
}
void print_ICF( int icf)
{
   /*char *ob_file = create_extension(file_name,".ob");
   FILE *f = fopen(ob_file, "w");*/
   int i = 0;
   int j = 0;
   for (i = 0; i < icf; i++)
   {
      printf( "IC[%d]: ", i);
      for (j = 0; j < WORD_SIZE; j++)
      {
            printf( "%c", IC[i][j]);
      }
      printf( "\n");
   }
}
void dcf_to_icf(int icf,int dcf)
{
   int i;
   for (i = 0; i < dcf; i++)
   {
      strcpy(IC[icf + i], DC[i]); /* נעתיק את ה-DC ל-IC */
   }
}
void fprint_ICF(char *file_name, int icf)
{
   char *ob_file = create_extension(file_name,".ob");
   FILE *f = fopen(ob_file, "w+");
   int i = 0;
   int j = 0;
   for (i = 0; i < icf; i++)
   {
            line_fprint(f,START_MEMORY_ADDRESS + i);

      for (j = 0; j < WORD_SIZE; j++)
      {
            fprintf(f, "%c", IC[i][j]);
      }
      fprintf(f, "\n");
   }
   fclose(f);
}

void clear_IC_DC()
{
   int i;
   for (i = 0; i < MAX_INPUT; i++)
   {
      memset(IC[i], 0, WORD_SIZE);
      memset(DC[i], 0, WORD_SIZE);
   }
}

void add_code_line(char type, int line, char *code)
{
   int i;
   code_line *current = NULL;
   code_line *temp = (code_line *)malloc(sizeof(code_line));
   if (type == 'I') printf("in add code for IC line: %d \n",line);
   else printf("in add code for DC line: %d \n",line);
   if (ic == NULL && type == 'I') /* If the instruction code linked list is empty */
   {
      ic = (code_line *)malloc(sizeof(code_line)); /* Initialize the head of the linked list if it's NULL */
      ic->line = 0;
   }
   if (dc == NULL && type == 'D') /* If the data code linked list is empty */
   {
      dc = (code_line *)malloc(sizeof(code_line)); /* Initialize the head of the linked list if it's NULL */
      dc->line = 0;
   }

   if (type == 'I')
      current = ic;
   else
      current = dc;


   for (i = 0; i < line - 1 ; i++)
   {
      if(current->next == NULL) {
         current->next = (code_line *)malloc(sizeof(code_line));
         if (!current->next) {
            fprintf(stderr, "Memory allocation failed for code line.\n");
            exit(EXIT_FAILURE);
         }
         current->next->line = i+1;         
      }
      current = current->next;
   }
   if(line == 0)
   {
      current->line = line;
      strcpy(current->code, code);
      if (current->next != NULL)
      {
         temp->line = current->next->line;
         strcpy(temp->code,current->next->code);
         temp->next = current->next->next;
         current->next->next = temp;
      }
      
      
   }

   else if (current->next == NULL){
      current->next = (code_line *)malloc(sizeof(code_line));
      current->next->line = line;
      strcpy(current->next->code,code);
   }

   else
   {
      temp->line = current->next->line;
      strcpy(temp->code,current->next->code);
      temp->next = current->next->next;
      current->next->line = line;
      strcpy(current->next->code, code);
      current->next->next = temp;
   }

   print_code_lines();
  
}
void dc_to_ic(int icf)
{
   code_line *current = ic;
   int counter = 0;
   print_code_lines();
   if (ic == NULL) /* If the instruction code linked list is empty */
   {
      ic = dc; /* Link the data code linked list to the instruction code linked list */
      return;
   }
   if (dc == NULL) /* If the data code linked list is empty */
      return; /* Nothing to do, return */

   while (current->next != NULL)
   {
      printf("code line %d: %s\n", current->next->line, current->next->code);
      counter++;
      current = current->next; /* Move to the next code line */
   }
   while (counter < icf - 1 )
   {
      
      counter++;
   }
   
   current->next = dc; /* Link the end of IC to the start of DC */
   
   
   while (current->next != NULL) /* Traverse to the end of the IC linked list */
   {
      current->next->line += counter;
      current = current->next;
   }
   
   printf("count: %d\n", counter);
   
}
void print_code_lines() {
   code_line *current = ic;
   printf("print code lines\n");
   while (current != NULL) {
      printf("Line %d: %s\n", current->line, current->code);
      current = current->next;
   }
}

