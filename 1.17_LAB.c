/**
 * Description: Prompts user for two integers and prints a right-facing arrow using the integers.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/05/2023
 * Creation date: 02/02/2023
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
   int baseInt;
   int headInt;
   

   scanf("%d", &baseInt);
   scanf("%d", &headInt);
   
   int nDigits = floor(log10(abs(headInt))) + 1;
   
   if(baseInt != 0){
      int nDigitsBase = floor(log10(abs(baseInt))) + 1;
      nDigits = 4*nDigitsBase+nDigits;
      
      if(baseInt < 0)
         nDigits += 4;
   }
   else
      nDigits += 4; 
      
   if(headInt < 0)
      nDigits += 1;
   
   printf("%*d\n",nDigits,headInt);
   printf("%*d%d\n",nDigits,headInt,headInt);
   printf("%d%d%d%d%d%d%d\n",baseInt,baseInt,baseInt,baseInt,headInt,headInt,headInt);
   printf("%d%d%d%d%d%d%d%d\n",baseInt,baseInt,baseInt,baseInt,headInt,headInt,headInt,headInt);
   printf("%d%d%d%d%d%d%d\n",baseInt,baseInt,baseInt,baseInt,headInt,headInt,headInt);
   printf("%*d%d\n",nDigits,headInt,headInt);
   printf("%*d\n",nDigits,headInt);
   

   return 0;
}

