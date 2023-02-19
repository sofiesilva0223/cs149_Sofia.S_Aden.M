/**
 * Description: Program reads an integer, a list of words, and a character. The integer signifies how many words are in the list. The output of the program is every word in the list that contains the character at least once and a newline to the end of the last output.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/16/2023
 * Creation date: 02/16/2023
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

   const int NUM_ELEMENTS = 20;
   char *userValues[NUM_ELEMENTS];    // Set of data specified by the user
   char searchChar;
   int actualNumElements;
   
   scanf("%d",&actualNumElements);
   
   for(int i = 0; i < actualNumElements; i++){
      userValues[i] = (char*)malloc(9);
      scanf("%9s",userValues[i]);
   }
   
   scanf(" %c",&searchChar);

   for(int i = 0; i < actualNumElements; i++){
      if(strchr(userValues[i], searchChar) != NULL)
         printf("%s,",userValues[i]);
   }
   
   printf("\n");


   return 0;
}

