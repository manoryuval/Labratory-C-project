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
   num_to_code(i, line, type); /* נשתמש בפונקציה num_to_code כדי להמיר את התו לקוד */ 

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

   switch (type)
   {
    case 'I': /* Instruction */
      for( i = 0; i < WORD_SIZE; i++) {
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
            line_fprint(f,FILE_LINE_VALUE + i);

      for (j = 0; j < WORD_SIZE; j++)
      {
            fprintf(f, "%c", IC[i][j]);
      }
      fprintf(f, "\n");
   }
   fclose(f);
}