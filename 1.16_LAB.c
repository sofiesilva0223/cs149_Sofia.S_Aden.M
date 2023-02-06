/**
 * Description: Prints a formatted No parking sign with the time.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/05/2023
 * Creation date: 02/02/2023
 **/
 
#include <stdio.h>

int main(void) {

   char sign[] = "NO PARKING";
   char timeSpan[] = "2:00 - 6:00 a.m.";
   
   printf("%12s\n", sign);
   printf("%s\n", timeSpan);
   
   return 0;
}
