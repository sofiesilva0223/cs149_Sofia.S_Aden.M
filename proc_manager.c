/**
 * Description: Program reads command for each execution from stdin on one line of input and executes its parameters as specified on each line of stdin input. The proc_manager program will start up all the processes with the exec command and wait for them to complete, while each command will write its stdout and stderr to log files.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/15/2023
 * Creation date: 04/24/2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LENGTH 100
#define HASHSIZE 101
#define FILENAME_LENGTH 10

//defining functions
void parse_argument(char *, char **);
void open_files(pid_t, char *, char *);
/**
 * COMMAND_NODE struct is a hashtable of pointers to
 * command, child pid, and it's index.
 */
typedef struct COMMAND_NODE{
    pid_t pid;
    char *command;                  //ptr to the command
    int index;
    struct timespec start;
    struct timespec finish;//index of the command
    struct  COMMAND_NODE *next;    //ptr to next COMMAND_NODE
}COMMAND_NODE;

static struct COMMAND_NODE *hashCommand[HASHSIZE];

unsigned hash(pid_t pid){
    return pid % HASHSIZE;
}

struct COMMAND_NODE *search(pid_t pid){
    struct COMMAND_NODE *command;

    for(command = hashCommand[hash(pid)]; command != NULL; command = command->next){
        if(command->pid == pid)
            return command;
    }
    return NULL;
}
struct COMMAND_NODE *insert(char *cmd, pid_t pid, int index){
    struct COMMAND_NODE *command;
    unsigned hashval;

    if((command = search(pid)) == NULL){
        command = (struct COMMAND_NODE *) malloc(sizeof(struct COMMAND_NODE));

        if(command == NULL)
            return NULL;
        if((command->command = strdup(cmd)) == NULL)
            return NULL;
        command->pid = pid;
        command->index = index;
        hashval = hash(pid);

        command->next = hashCommand[hashval];
        hashCommand[hashval] = command;

    }
    return command;
}
struct COMMAND_NODE *delete(pid_t pid){
    struct COMMAND_NODE *command;

    for(command = hashCommand[hash(pid)]; command != NULL; command = command->next){
        struct COMMAND_NODE *temp = command->next;
        if(temp != NULL && temp->pid == pid) {
            command->next = command->next->next;
            return temp;
        }
    }
    return NULL;
}

void forkexec(pid_t, char *, int, char **, struct COMMAND_NODE *, char *, char *);
/**
 *
 */
int main(int argc, char *argv[]) {
    //declare data
    char *command = (char *)malloc(COMMAND_LENGTH * sizeof(char));
    char **argument = (char **) malloc(COMMAND_LENGTH * sizeof(char *));    //set argument for execvp
    char *filename_out = (char *) malloc(FILENAME_LENGTH * sizeof(char));
    char *filename_err = (char *) malloc(FILENAME_LENGTH * sizeof(char));
    clock_t t = clock();
    struct COMMAND_NODE *cmdnode;;
    pid_t child;

    if(command == NULL || argument == NULL || filename_out == NULL || filename_err == NULL)
        exit(1);

    //traverse trough each file
    int count = 0;
    while (fgets(command, COMMAND_LENGTH, stdin)) {
        if(command[strlen(command)-1] == '\n')
           command[strlen(command)-1] = 0;
        count++;

        child = fork();     //fork a child
        cmdnode = insert(command, child, count);
        parse_argument(command, argument);
        clock_gettime(CLOCK_MONOTONIC, &cmdnode->start);
        forkexec(child,command,count,argument,cmdnode,filename_out,filename_err);
    }
    //parent process
    if(child > 0) {
        pid_t c;
        int status;
        struct timespec finish;

        //loop until all child process are exited
        while ((c = wait(&status)) != -1) {
            clock_gettime(CLOCK_MONOTONIC, &finish);
            cmdnode = search(c);
            cmdnode->finish = finish;
            double duration = 0.0;
            duration += (cmdnode->finish.tv_sec - cmdnode->start.tv_sec);

            open_files(c, filename_out, filename_err);
            printf("Finished child %d pid of parent %d\n", c, getpid());
            printf("Started at %ld, Finished at %ld, runtime duration %f\n", cmdnode->start.tv_sec,cmdnode->finish.tv_sec, duration);
            fflush(stdout);
            //exitcode
            if (WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            //kill signal
            else if (WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));

            if(duration > 2){
                pid_t rechild = fork();
                cmdnode = insert(cmdnode->command, rechild, cmdnode->index);

                if(rechild > 0){
                    open_files(rechild, filename_out, filename_err);
                    printf("RESTARTING\n");
                    fflush(stdout);
                }
                parse_argument(cmdnode->command, argument);

                clock_gettime(CLOCK_MONOTONIC, &cmdnode->start);
                forkexec(rechild,cmdnode->command,cmdnode->index,argument,cmdnode,filename_out,filename_err);
            }
            else
                fprintf(stderr, "Spawning too fast\n");
        }
    }
    close(1);
    close(2);

    free(command);
    free(argument);
    free(filename_out);
    free(filename_err);

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
void forkexec(pid_t child, char *command, int count,
              char ** argument, struct COMMAND_NODE *cmdnode, char *filename_out, char *filename_err){
    if (child < 0) {    //if there is an error with fork exit
        printf("Error with fork for processing command %s.\n", command);
        exit(1);
    } else if (child == 0){   //child process
        open_files(getpid(), filename_out, filename_err);
        printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
        fflush(stdout);

        execvp(*argument, argument);

        //only happens if execvp fails
        fprintf(stderr, "Invalid command: %s\n", command);
        exit(2);    //exit child process
    }
}