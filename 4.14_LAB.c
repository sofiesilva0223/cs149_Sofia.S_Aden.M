/**
 * Description: Takes in a positive integer as input, and outputs a string of 1's and 0's representing the integer in reverse binary.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/10/2023
 * Creation date: 02/10/2023
 **/
#include <stdio.h>

int main(void) {
   
   int x;
   
   scanf("%d",&x);
   
   while(x > 0){
      printf("%d",(x%2));
      x = x/2;
   }
   
   printf("\n");
   

   return 0;
}

