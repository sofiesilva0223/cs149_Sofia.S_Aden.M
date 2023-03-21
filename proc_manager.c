/**
 * Description:
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/15/2023
 * Creation date: 03/22/2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define COMMAND_LENGTH 30

int main(int argc, char *argv[]) {
    //declare data
    char *command = (char *)malloc(COMMAND_LENGTH * sizeof(char));
    pid_t child;

    //traverse trough each file
    int count = 0;
    while (fgets(command, COMMAND_LENGTH, stdin)) {
        if(command[strlen(command)-1] == '\n')
           command[strlen(command)-1] = 0;
        count++;

        child = fork();     //fork a child
        if (child < 0) {    //if there is an error with fork exit
            printf("Error with fork for processing command %s.\n", command);
            exit(1);
        } else if (child == 0) {    //child process
            char *exec_command = {0};
            char *filename = (char *) malloc(10 * sizeof(char));
            sprintf(filename,"%d.out",getpid());
            int file_desc = open(filename, O_RDWR | O_CREAT | O_APPEND, 0777);
            dup2(file_desc,1);
            printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
            strtok(exec_command,command);
            execvp(strtok(exec_command," "),  command);
            fprintf(stderr, "Invalid command: %s\n",command);
            exit(2);    //exit child process
        }
    }
    //parent process
    if(child > 0) {
        pid_t c;
        //int status;

        //loop until all child process are exited
        while ((c = wait(NULL)) != -1) {
            char *filename = (char *) malloc(10 * sizeof(char));
            sprintf(filename,"%d.err",c);
            int file_desc = open(filename, O_RDWR | O_CREAT | O_APPEND, 0777);
            dup2(file_desc,2);
        }
    }
    return 0;
}
