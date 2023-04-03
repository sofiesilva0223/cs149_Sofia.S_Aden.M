/**
 * Description: Program prompts user for a string that contains two strings separated by a comma. Error outputs if the input string does not contain a comma. Correct output displays two words from the input string with spaces removed. The program is able to handle multiple lines of input until the user enters q to quit.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/02/2023
 * Creation date: 03/31/2023
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  
  char *usrInput = (char *) malloc(50*sizeof(char));
  
  printf("Enter input string:\n");
  scanf("%[^\n]%*c", usrInput);
  
  while(strstr(usrInput, "q") == NULL){
    char *first = strtok(usrInput ,",");
    char *last = strtok(NULL," ,");
    if(first!= NULL && last!=NULL){
       if(first[strlen(first)-1] == ' ')
         first[strlen(first)-1] = '\0';
      printf("First word: %s\n",first);
      printf("Second word: %s\n",last);
    }
    else{
      printf("Error: No comma in string.\n");
    }
    printf("\n");
    printf("Enter input string:\n");
    scanf("%[^\n]%*c", usrInput);
   }
   free(usrInput);
   
  return 0;
}
