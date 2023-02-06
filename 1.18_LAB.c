/**
 * Description: Prints a formatted greeting.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/05/2023
 * Creation date: 02/02/2023
 **/

#include <stdio.h>

int main(void) {

   char hello[] = "Hello world!";
   char greeting[] = "How are you?";
   char reply[] = "(I'm fine).";
   
   printf("%s\n%s\n%14s\n", hello, greeting,reply);

   return 0;
}
