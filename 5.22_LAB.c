/**
 * Description: Program gets a list of integers from input and is followed by two more integers representing lower and upper bounds of a range. The program will output all integers from the list that are within that range and end with a newline.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/15/2023
 * Creation date: 02/15/2023
 **/
 
#include <stdio.h>

int main(void) {

   int inputNum, arr[20], i;
   int val1, val2;
  
  //scanner open
   scanf("%d", &inputNum);

  //reading the array
   for (i = 0; i < inputNum; i++){
      scanf("%d", &arr[i]);
   }
   //determining range 
   scanf("%d%d",&val1,&val2);

   for(i=0; i < inputNum;i++){
      if(arr[i] >= val1 && arr[i] <= val2){
         printf("%d,",arr[i]);
      }
   }
   printf("\n");
   return 0;
}

