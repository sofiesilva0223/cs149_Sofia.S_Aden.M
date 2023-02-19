/**
 * Description: Reads a list of integers, and outputs those integers in reverse. The input begins with an integer indicating the number of integers that follow.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/16/2023
 * Creation date: 02/16/2023
 **/
 
#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 20;         // Number of input integers
   int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers
   int actualNumElements;
   
   scanf("%d",&actualNumElements);
   fflush(stdin);
   
   for(int i = 0; i < actualNumElements; i++)
      scanf("%d",&userVals[i]);

   for(int i = actualNumElements-1; i >= 0; i--)
      printf("%d,",userVals[i]);
   
   printf("\n");
   
   return 0;
}

