/**
 * Description: Program creates an array of integers called data of size 100 using malloc then set data[100] to zero.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/23/2023
 * Creation date: 04/23/2023
 
Q: What happens when you run this program? 
A: No error is shown and the program ended successfully.

Q: What happens when you run this program using valgrind? 
A: Valgrind shows an error "Invalid write of size 4 at 0x108770: main (q5.c:23) at line 23 because data[100] was not allocated and there was a memory leak.

Q: Is the program correct? 
A: The program is not correct because data[100] was not allocated before being assigned to 0.
**/

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *data;
  data = (int*)malloc(sizeof(int)*100);
  data[100]=0;
}
