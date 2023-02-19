/**
 * Description: Program reads a list of integers and outputs the two smallest integers in the list in ascending order.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/13/2023
 * Creation date: 02/13/2023
 **/
 
#include <stdio.h>
#include <limits.h>

int main(void) {
   
   //initializing smallest and second smallest integers   
   int minNum = INT_MAX;
   int secNum= INT_MAX;
   //declaring other variables
   int inputNum, arr[20], i;
   
   //user input 
   scanf("%d", &inputNum);

   //reading the input, storing in the array
   for (i=0;i<inputNum;i++){
      scanf("%d", &arr[i] );
   }

   for(i = 0;i < inputNum; i++){
      if(arr[i] < minNum){
         secNum = minNum;
         minNum = arr[i];
      }
      else if(arr[i]< secNum && arr[i]!= i){
         secNum = arr[i];
      }
   }
   printf("%d and %d\n",minNum,secNum);
   return 0;
}

