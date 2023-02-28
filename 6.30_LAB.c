/**
 * Description: Program whose inputs are four integers, and whose outputs are the maximum and the minimum of the four values. 
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 2/22/2023
 * Creation date: 2/22/2023
 **/
 
 #include <stdio.h>

/* Define your function here */ 
int MaxNumber(int num1, int num2, int num3, int num4);
int MinNumber(int num1, int num2, int num3, int num4);

int main(void) {

   /* Type your code here. Your code must call the function.  */
   int num1, num2, num3, num4;
   
   scanf("%d %d %d %d",&num1, &num2, &num3, &num4);
   printf("Maximum is %d\n",MaxNumber(num1,num2,num3,num4));
   printf("Minimum is %d\n",MinNumber(num1,num2,num3,num4));

   return 0;
}
int MaxNumber(int num1, int num2, int num3, int num4){
   int max;
   
   if(num1 > num2 && num1 > num3 && num1 > num4)
      max = num1;
   else if(num2 > num3 && num2 > num4)
      max = num2;
   else if(num3 > num4)
      max = num3;
   else
      max = num4;
      
   return max;
}
   
int MinNumber(int num1, int num2, int num3, int num4){
   int min;
   
   if(num1 < num2 && num1 < num3 && num1 < num4)
      min = num1;
   else if(num2 < num3 && num2 < num4)
      min = num2;
   else if(num3 < num4)
      min = num3;
   else
      min = num4;

   return min;
}
