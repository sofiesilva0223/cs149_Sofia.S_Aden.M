/**
 * Description:  Program gets a list of integers from input and the input begins with an integer indicating the number of integers that follow. Then the program get the last value from the input, which is the threshold. It outputs all integers less than or equal to that last threshold value. 
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/16/2023
 * Creation date: 02/16/2023
 **/
 

#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 20;
   int userValues[NUM_ELEMENTS];    // Set of data specified by the user
   int threshold;
   int actualNumElements;
   
   scanf("%d",&actualNumElements);
   fflush(stdin);
   
   for(int i = 0; i < actualNumElements; i++)
      scanf("%d",&userValues[i]);
      
   scanf("%d",&threshold);

   for(int i = 0; i < actualNumElements; i++){
      if(userValues[i] <= threshold)
         printf("%d,",userValues[i]);
   }
   
   printf("\n");

   return 0;
}
