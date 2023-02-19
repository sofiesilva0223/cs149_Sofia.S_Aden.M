/**
 * Description: Program takes in a sorted list of odd integer and outputs the middle integer. A negative number indicates the end of the input and a negative number is not a part of the sorted list.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/10/2023
 * Creation date: 02/10/2023
 **/
 
#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 9;
   int userValues[NUM_ELEMENTS];    // Set of data specified by the user

   int input = 0;
   int count = 0; 
 
 
   
   while (input >= 0){
      scanf("%d",&input);

      if(input >= 0){
         if(count < NUM_ELEMENTS)
            userValues[count] = input;   
         count++;
      }
   }
   
   if(count > NUM_ELEMENTS)
      printf("%s\n", "Too many numbers");
   else
      printf("Middle item: %d\n",userValues[count/2]);

   return 0;
}

