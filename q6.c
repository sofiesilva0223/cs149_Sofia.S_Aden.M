/**
 * Description: Program creates and allocates an array of integers called data of size 100 using malloc  frees them, and then tries to print the value of one of the elements of the array. 
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/23/2023
 * Creation date: 04/23/2023
 
Q: Does the program run? 
A: The program does run.

Q: What happens when you use valgrind on it?
A: Valgrind shows that all memory was freed (so no memory leaks) but that at line 22, shows an invalid read error.
**/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *data;
  data = (int*)malloc(sizeof(int)*100);
  free(data);
  printf("%d",data[2]);
  return 0;
}
