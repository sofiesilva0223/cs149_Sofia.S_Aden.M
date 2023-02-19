/**
 * Description: Takes any number of non-negative integers as input, and outputs the max and average. A negative integer ends the input and is not included.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/10/2023
 * Creation date: 02/10/2023
 **/
 
#include <stdio.h>

int main(void) {

   int input = 0;
   int count = 0; 
   int total = 0;
   int max = 0;
 
   
   while (input >= 0){
      scanf("%d",&input);
      
      if(input >= 0){
         total = total + input;
         if(input > max)
            max = input;
         count++;
      }      
   }
   printf("%d %0.2lf\n", max,(float)total/count);
   
   return 0;
}

