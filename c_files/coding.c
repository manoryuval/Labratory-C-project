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
            code[i] = 'A';
            break;
         case 1:
            code[i] = 'B';
            break;
         case 2:
            code[i] = 'C';
            break;
         case 3:
            code[i] = 'D';
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
            case 'A': code[i] = 'D'; break;
            case 'B': code[i] = 'C'; break;
            case 'C': code[i] = 'B'; break;
            case 'D': code[i] = 'A'; break;
         }
      }
   }

   i = 4; /* נתחיל מהאחרון */
   while (flag && i >=0)
   {
      switch (code[i])
      {
         case 'A': code[i] = 'B'; flag = 0; break;
         case 'B': code[i] = 'C'; flag = 0; break;
         case 'C': code[i] = 'D'; flag = 0; break;
         case 'D': code[i] = 'A'; i--; break;
      }
   }

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
void two_reg_code (int reg1, int reg2, int line, char type)
{
    char code[5];
    strcpy(code, REGS[reg1].code);
    strcat(code, REGS[reg2].code);
    strcat(code, "A");/* A - ARE*/
    switch (type)
    {
        case 'I': /* Instruction */
            strcpy(IC[line], code);
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
void op_to_code(op_code op,int type1, int type2, int line, char type)
{
      char code[5];
   
      /* נעתיק את הקוד של האופרטור */
      strcpy(code, op.code);
   
      /* נוסיף את סוגי מיון האופרנדים */
      switch (type1)
      {
      case 0:
         code[3] = 'A';
         break;
      case 1: 
         code[3] = 'B';
         break;
      case 2:
         code[3] = 'C';
         break;
      case 3:
         code[3] = 'D';
         break;
      }
      
       switch (type2)
      {
      case 0:
         code[4] = 'A';
         break;
      case 1: 
         code[4] = 'B';
         break;
      case 2:
         code[4] = 'C';
         break;
      case 3:
         code[4] = 'D';
         break;
      }

      /* נוסיף את ה-ARE */
      code[5] = 'A';
      switch (type)
      {
         case 'I': /* Instruction */
               strcpy(IC[line], code);
               break;
         case 'D': /* Data */
               strcpy(DC[line], code);
               break;
      }
}
int line_to_code(int num, int line, char type)
{
      
}