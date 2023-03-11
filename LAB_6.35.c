/**
 * Description: Function called SortArray takes an array of integers and the number of elements in the array as parameters. Function SortArray() modifies the array parameter by sorting the elements in descending order (highest to lowest). The main program reads a list of integers from input, stores the integers in an array, calls SortArray(), and outputs the sorted array. The first input integer indicates how many numbers are in the list.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 3/10/2023
 * Creation date: 3/10/2023
 **/
 
#include <stdio.h>

/* Define your function here */
void SortArray(int sortingList[], int numVals);
int main(void) {

   /* Type your code here. */
   int numVals;
   int sortingList[20];
   int i;
   
   //***User Input***
    // taking in number of integers from users
   scanf("%d",&numVals);
   // taking in the values of the array to be sorted
   for (i = 0; i < numVals; i++){
      scanf("%d", &sortingList[i]);
   }
   SortArray(sortingList,numVals);
   for(i=0;i<numVals;i++){
      printf("%d,",sortingList[i]);
   }
   printf("\n");
   return 0;
}

void SortArray(int sortingList[], int numVals){
   int j,k,temp;
   
   for (j=0; j<numVals-1;j++){
      for(k=0;k<numVals-j-1;k++){
         if (sortingList[k] < sortingList[k+1]){
            temp = sortingList[k];
            sortingList[k] = sortingList[k+1];
            sortingList[k+1]=temp;
         }
      }
   }
}
