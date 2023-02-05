#include <stdio.h>

int main(void) {

   char hello[] = "Hello world!";
   char greeting[] = "How are you?";
   char reply[] = "(I'm fine).";
   
   printf("%s\n%s\n%14s\n", hello, greeting,reply);

   return 0;
}
