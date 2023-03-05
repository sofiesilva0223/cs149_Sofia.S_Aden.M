/**
 * Description: Program takes in a line of text as input, output the number of characters excluding spaces, periods, exclamation points, or commas.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 3/03/2023
 * Creation date: 3/03/2023
 **/
 
#include <stdio.h>
#include <string.h>

#define SPECIAL " .!,"

int main(void) {
   char input[50];
   int numchar = 0;
   
   gets(input); 
   char *find = strchr(input,'\n');
   if(find)
      *find = '\0';
   
   for (int i = 0; i < strlen(input); i++){
      if(strchr(SPECIAL,input[i]) == NULL)
         numchar++;
   }
   printf("%d\n",numchar);
      
   
   
   return 0;
}
