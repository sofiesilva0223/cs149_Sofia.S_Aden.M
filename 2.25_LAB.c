/**
 * Description: Prompts user to enter integers userNum and x as input and output userNum divided by x three times. 
 * Author names: Sofia Silva & Aden Mengistie 
 * Author emails: sofia.silva@sjsu.edu, aden.mengistie@sjsu.edu
 * Last modified date: 02/08/2023
 * Creation date: 02/08/2023
 **/
 
#include <stdio.h>

int main(void) {

   int userNum, x;
   
   scanf("%d", &userNum);
   scanf("%d", &x);
   
   if (x != 0)
      printf("%d %d %d\n",userNum/x, userNum/(x*x), userNum/(x*x*x));
   else
      printf("%s", "Invalid x");
      
   return 0;
}
