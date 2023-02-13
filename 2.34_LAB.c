/**
 * Description: User enters first name as the input and prints a welcome message to that name. 
 * Author names: Sofia Silva & Aden Mengistie 
 * Author emails: sofia.silva@sjsu.edu, aden.mengistie@sjsu.edu
 * Last modified date: 02/08/2023
 * Creation date: 02/08/2023
 **/
 
#include <stdio.h>

int main(void) {
   char userName[50];
   
   scanf("%s", userName);
   printf("%s %s%s\n", "Hello",userName,", and welcome to CS Online!");

   return 0;
}

