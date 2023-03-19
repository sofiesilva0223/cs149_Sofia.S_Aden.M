/**
 * Description: Function named SwapValues takes four integers as parameters and swaps the first with the second, and the third with the fourth values. The main program reads four integers from input, calls function SwapValues() to swap the values, and prints the swapped values on a single line separated with spaces.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/16/2023 
 * Creation date: 03/16/2023
 **/

#include <stdio.h>
#include <stdlib.h>

void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4);

int main(void) {

   int input1; int input2; int input3; int input4;
   scanf("%d %d %d %d", &input1, &input2, &input3, &input4);
   
   SwapValues(&input1, &input2, &input3, &input4);
   printf("%d %d %d %d\n", input1, input2, input3, input4);

   return 0;
}
void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4){
   int* temp = (int*)malloc(sizeof(int));
   *temp = *userVal1;
   *userVal1 = *userVal2;

   *userVal2 = *temp;
   *temp = *userVal3;
   *userVal3 = *userVal4;
   *userVal4 = *temp;
}
