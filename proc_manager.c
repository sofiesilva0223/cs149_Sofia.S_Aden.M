/**
 * Description: Program reads command for each execution from stdin on one line of input and executes its parameters as specified on each line of stdin input. The proc_manager program will start up all the processes with the exec command and wait for them to complete, while each command will write its stdout and stderr to log files.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/23/2023
 * Creation date: 03/15/2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LENGTH 30
#define FILENAME_LENGTH 10

//defining functions
void parse_argument(char *command, char **argument);
void open_files(pid_t child, char *filename_out, char *filename_err);

int main(int argc, char *argv[]) {
    //declare data
    char *command = (char *)malloc(COMMAND_LENGTH * sizeof(char));
    char **argument = (char **) malloc(COMMAND_LENGTH * sizeof(char *));    //set argument for execvp
    char *filename_out = (char *) malloc(FILENAME_LENGTH * sizeof(char));
    char *filename_err = (char *) malloc(FILENAME_LENGTH * sizeof(char));
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
            parse_argument(command,argument);
            open_files(getpid(), filename_out, filename_err);

            printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
            fflush(stdout);
            execvp(*argument,argument);

            //only happens if execvp fails
            fprintf(stderr, "Invalid command: %s\n",command);
            exit(2);    //exit child process
        }
    }
    //parent process
    if(child > 0) {
        pid_t c;
        int status;
    
        //loop until all child process are exited
        while ((c = wait(&status)) != -1) {
            open_files(c, filename_out, filename_err);
            printf("Finished child %d pid of parent %d\n",c,getpid());
            fflush(stdout);
	    //exitcode
            if(WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            //kill signal
            else if(WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));
        }
        free(command);
        free(argument);
        free(filename_out);
        free(filename_err);
        
        close(1);
        close(2);
    }
    return 0;
}
//function to parse argument
void parse_argument(char *command, char **argument){
    int i = 0;
    char delimit[]= " \t\n";

    argument[i] = strtok(command, delimit);
    while (argument[i] != NULL){
        i++;
        argument[i] = strtok(NULL, delimit);
    }
}
//function for opening and writing stdout and stderr files for the child process
void open_files(pid_t child, char *filename_out, char *filename_err){
    sprintf(filename_out,"%d.out",child);
    int file_desc1 = open(filename_out, O_RDWR | O_CREAT | O_APPEND, 0777);
    close(1);
    dup2(file_desc1,1);

    sprintf(filename_err,"%d.err",child);
    int file_desc2 = open(filename_err, O_RDWR | O_CREAT | O_APPEND, 0777);
    close(2);
    dup2(file_desc2,2);
}
