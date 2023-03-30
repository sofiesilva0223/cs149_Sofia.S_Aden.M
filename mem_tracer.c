/**
 * Description:
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/24/2023
 * Creation date: 04/01/2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ALLOCATION_VALUE 10
#define COMMAND_LENGTH 30
#define TRACE_FILENAME "memtrace.out"

typedef struct COMMAND_NODE{
    char *command;
    int index;
    struct  COMMAND_NODE *next;
}COMMAND_NODE;

typedef struct TRACE_NODE{
    char *functionId;
    struct TRACE_NODE *next;
}TRACE_NODE;

static TRACE_NODE  *TRACE_NODE_TOP = NULL;
static COMMAND_NODE *COMMAND_NODE_HEAD = NULL;

void *allocate_memory(int size);
void *reallocate_memory(void *p, int size);
void free_memory(void *p);
void parse_argument(char *command, char **argument);

void PUSH_TRACE(char *p){
    TRACE_NODE *traceNode;

    if(TRACE_NODE_TOP == NULL){
        TRACE_NODE_TOP = (TRACE_NODE *) allocate_memory(sizeof(TRACE_NODE));

        if(TRACE_NODE_TOP == NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }
        TRACE_NODE_TOP->functionId = "global";
        TRACE_NODE_TOP->next = NULL;

        traceNode = (TRACE_NODE *) allocate_memory(sizeof(TRACE_NODE));
        if(traceNode == NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }
        traceNode->functionId = p;
        traceNode->next = TRACE_NODE_TOP;
        TRACE_NODE_TOP = traceNode;
    }
}
void POP_TRACE(){
    TRACE_NODE *traceNode;
    traceNode = TRACE_NODE_TOP;
    TRACE_NODE_TOP = traceNode->next;
    free_memory(traceNode);
}

int main(int argc, char *argv[]) {
    //declare data
    int file_desc = open(TRACE_FILENAME, O_RDWR | O_CREAT | O_APPEND, 0777);
    char **command;
    char **argument;
    pid_t child;

    command = (char **)allocate_memory(ALLOCATION_VALUE);
    argument = (char **)allocate_memory(ALLOCATION_VALUE);

    //traverse trough each file
    int count = 0;
    while (fgets(command[count], COMMAND_LENGTH, stdin)) {
        if(*command[strlen(command)-1] == '\n')
           *command[strlen(command)-1] = 0;
        count++;

        child = fork();     //fork a child
        if (child < 0) {    //if there is an error with fork exit
            printf("Error with fork for processing command %s.\n", command[count-1]);
            exit(1);
        } else if (child == 0) {    //child process
            parse_argument(command[count-1],argument);
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

            if(WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            else if(WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));
        }
    }
    return 0;
}

void *allocate_memory(int size){
    void *p;
    FILE *fp = fopen(TRACE_FILENAME,"a");

    p = malloc(size);
    fprintf("File = /""%s/"", line = %d, Function = %s, segment allocated to address %d\n", TRACE_FILENAME, count, PRINT_TRACE(),p);

    return p;
}
void *reallocate_memory(void *p, int size){
    FILE *fp = fopen(TRACE_FILENAME,"a");

    p = realloc(p,size);
    fprintf("File = /""%s/"", line = %d, Function = %s, segment reallocated to address %d\n", TRACE_FILENAME, count, PRINT_TRACE(),p);

    return p;
}
void free_memory(void *p){
    FILE *fp = fopen(TRACE_FILENAME,"a");

    free(p);
    fprintf("File = /""%s/"", line = %d, Function = %s, segment at address %d deallocated\n", TRACE_FILENAME, count, PRINT_TRACE(),p);
}
void parse_argument(char *command, char **argument){
    int i = 0;
    char delimit[]= " \t\n";

    argument[i] = strtok(command, delimit);
    while (argument[i] != NULL){
        i++;
        argument[i] = strtok(NULL, delimit);
    }
}
