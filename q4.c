/**
 * Description: Program allocates memory using malloc() but forgets to free it before exiting.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/23/2023
 * Creation date: 04/23/2023
 
Q: What happens when this program runs?
A: Memory is allocated in the heap, but and takes up space causing a memory leak.

Q: Can you use gdb to find any problems with it? 
A: No, gdb only shows the output and where it could have crashed but it did not in this problem.

Q: How about valgrind (with the --leak-check=yes flag)?
A: Yes, the valgrind does show the problem by showing the memory leaks.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *var = "test";
  char *destination = (char*)malloc(strlen(var)+1);
  strcpy(destination,var);
  printf("%s\n", destination);
  printf("%s\n", var);

  return 0;
}
