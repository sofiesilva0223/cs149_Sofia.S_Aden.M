/**
 * Description: User inputs three integers and prints whose output is the smallest of the three values. 
 * Author names: Sofia Silva & Aden Mengistie 
 * Author emails: sofia.silva@sjsu.edu, aden.mengistie@sjsu.edu
 * Last modified date: 02/08/2023
 * Creation date: 02/08/2023
 **/
#include <stdio.h>

int main(void) {

   int userNum1, userNum2, userNum3; 
   
   scanf("%d",&userNum1);
   scanf("%d",&userNum2);
   scanf("%d",&userNum3);

   if(userNum1 <= userNum2 && userNum1 <= userNum3)
      printf("%d\n",userNum1);
   else if( userNum2 <= userNum3)
      printf("%d\n",userNum2);
   else
      printf("%d\n",userNum3);
      
   return 0;
}
