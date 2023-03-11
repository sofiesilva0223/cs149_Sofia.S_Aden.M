/**
 * Description: Program takes in an input that is a word or phrase, and that outputs whether the input is a palindrome.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 3/10/2023
 * Creation date: 3/10/2023
 **/
 
#include <stdio.h>
#include <string.h>

int main(void) {
   
   char input[50];
   fgets(input,50,stdin);

   int i = 0;
   int j = strlen(input)-1;

   for(i=0; i<j; i++,j--){ 
      while(input[i] == ' ' && i<j)
         i++;
      while(input[j] == ' ' && i<j)
         j--;
      if(input[i] != input[j]){
         printf("not a palindrome: %s\n", input);
         return 0;
         }
      }
   printf("palindrome: %s\n", input);
   return 0;
}
