/**
 * Description: Program takes in input of a character and a string and whose output indicates the number of times the character appears in the string. The output includes the input character and use the plural form, n's, if the number of times the characters appears is not exactly 1.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/24/2023
 * Creation date: 03/24/2023
 **/

#include <stdio.h>
#include  <stdlib.h>
#include <string.h>

#define LENGTH 50

int CalcNumCharacters(char* userString, char userChar);

int main(void) {
   
   char userChar;
   char *userString = (char *)malloc(LENGTH * sizeof(char));
   
   scanf("%c", &userChar);
   scanf("%s", userString);
   printf("%d %c\n", CalcNumCharacters(userString,userChar), userChar);

   free(userString);
   
   return 0;
}

int CalcNumCharacters(char* userString, char userChar){
   int countOccurence = 0;
   
   for(int i = 0; i < strlen(userString); ++i){
      if(userString[i] == userChar)
         ++countOccurence;
   }
   return countOccurence;
}

