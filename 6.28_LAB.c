/**
 * Description: Program that removes all non-alphabetic characters from the given input. Assume the input string will not exceed 50 characters.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 3/03/2023
 * Creation date: 3/03/2023
 **/
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void RemoveNonAlpha(char userString[], char userStringAlphaOnly[]);

int main(void) {
   char input[50];
   char output[50] = {'\0'};
   
   gets(input); 
   char *find = strchr(input,'\n');
   if(find)
      *find = '\0';
   
   RemoveNonAlpha(input,output);
   printf("%s\n", output); 

   return 0;
}
void RemoveNonAlpha(char userString[], char userStringAlphaOnly[]){
   
   int index = 0;
   for(int i = 0; i < strlen(userString); i++){
      
      if(isalpha(userString[i])){
         userStringAlphaOnly[index] = userString[i];
         index++;
      }
   }
   
   //printf("%s\n", userStringAlphaOnly);      
}

