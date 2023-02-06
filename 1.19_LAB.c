/**
 * Description: Prompt input from user and print a formatted arithmetic values.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/05/2023
 * Creation date: 02/02/2023
 **/

#include <stdio.h>

int main(void) {
   int userNum, userNum2;
   
   printf("Enter integer:\n");
   scanf("%d", &userNum);
   
   printf("You entered: %d\n",userNum);
   printf("%d squared is %d\n",userNum, userNum*userNum);
   printf("And %d cubed is %d!!\n",userNum, userNum*userNum*userNum);

   printf("Enter another integer:\n");
   scanf("%d", &userNum2);
   
   printf("%d + %d is %d\n",userNum, userNum2, userNum+userNum2);
   printf("%d * %d is %d\n",userNum, userNum2, userNum*userNum2);

   return 0;
}
