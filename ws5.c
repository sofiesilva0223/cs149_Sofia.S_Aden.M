/**
 * Description: Program uses fork() to print the string "hello world from PID xxx!" 4 times. In place of "xxx" print the process ID of the current process.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 3/05/2023
 * Creation date: 3/05/2023
 **/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
	for (int i=0; i<=4; i++){
		if (fork() == 0){
			printf("hello world from PID %d!\n", getpid());
			exit(0);
		}
	}
	return 0;
}
	
