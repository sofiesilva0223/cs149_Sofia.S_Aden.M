/**
 * Description: Program reads command for each execution from stdin and executes its parameters
 * as specified in the input. The proc_manager program will process each command using a forked
 * child process and run each command using execvp. It waits for each forked process to complete
 * with process duration time of <= 2 sec, else it will restart the command in a new forked child
 * process until the duration is 2 sec or less. Each command will be stored in a hash table with
 * its index within the input, start time and finish time. Additionally, the command processing
 * information will be logged into a file named with its respective child PID responsible for process
 * it (PID.out or PID.err).
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/15/2023
 * Creation date: 04/25/2023
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

/**
 * COMMAND_NODE struct is a hashtable of pointers to
 * command, child pid, and it's index.
 */
typedef struct COMMAND_NODE{
    pid_t pid;
    char *command;  //ptr to the command
    int index;  //index of the command
    struct timespec start;  //start time
    struct timespec finish; //finish time
    struct  COMMAND_NODE *next;    //ptr to next COMMAND_NODE
}COMMAND_NODE;

//defining functions
void parse_argument(char *, char **);
void open_files(pid_t, char *, char *);
void forkexec(pid_t, char *, int, char **, struct COMMAND_NODE *, char *, char *);

//hashtable declaration
static struct COMMAND_NODE *hashCommand[HASHSIZE];

//hashing function using child pid % hashsize
unsigned hash(pid_t pid){
    return pid % HASHSIZE;
}

//returns the respective command for the given pid
struct COMMAND_NODE *search(pid_t pid){
    struct COMMAND_NODE *command;

    /*
     * Find the hash index for the given pid and get the hash element.
     * Verify if the element's pid matches the given pid, else traverse
     * through the linked list until find the corresponding struct for the
     * given pid or until the end of the linked list in which return NULL.
     * */
    for(command = hashCommand[hash(pid)]; command != NULL; command = command->next){
        if(command->pid == pid)
            return command;
    }
    return NULL;
}

/*
 * Insert the given command to its corresponding hash index for the given pid
 * within the hashtable
 * */
struct COMMAND_NODE *insert(char *cmd, pid_t pid, int index){
    struct COMMAND_NODE *command;
    unsigned hashval;

    //there is no struct in the hash table with the given pid
    if((command = search(pid)) == NULL){
        command = (struct COMMAND_NODE *) malloc(sizeof(struct COMMAND_NODE));

        if(command == NULL) //command memory not allocated properly
            return NULL;
        if((command->command = strdup(cmd)) == NULL) //strdup the cmd
            return NULL;
        command->pid = pid;
        command->index = index;
        hashval = hash(pid);

        command->next = hashCommand[hashval];
        hashCommand[hashval] = command;

    }
    return command;
}

//Delete and free struct with the given pid from the hashtable
void delete(pid_t pid){
    struct COMMAND_NODE *command;
    struct COMMAND_NODE *temp;
    command = hashCommand[hash(pid)];

    //struct with the given pid is the head of the hash index linked list
    if(command->pid == pid){
        temp = command;
        hashCommand[hash(pid)] = command->next;
        free(temp->command);
        free(temp);
        return;
    }

    //struct with the given pid is not the head of the hash index linked list
    for(; command != NULL; command = command->next){
        temp = command->next;
        if(temp != NULL && temp->pid == pid) {
            command->next = temp->next;
            free(temp->command);
            free(temp);
            return;
        }
    }
}

/**
 *
 */
int main(int argc, char *argv[]) {
    //declare data
    char *command = (char *)malloc(COMMAND_LENGTH * sizeof(char));
    char **argument = (char **) malloc(COMMAND_LENGTH * sizeof(char *));    //set argument for execvp
    char *filename_out = (char *) malloc(FILENAME_LENGTH * sizeof(char));
    char *filename_err = (char *) malloc(FILENAME_LENGTH * sizeof(char));
    struct COMMAND_NODE *cmdnode;;
    pid_t child;

    //malloc error
    if(command == NULL || argument == NULL || filename_out == NULL || filename_err == NULL)
        exit(1);

    //traverse trough each command input
    int count = 0;
    while (fgets(command, COMMAND_LENGTH, stdin)) {
        //replace newline with null character
        if(command[strlen(command)-1] == '\n')
           command[strlen(command)-1] = 0;
        count++;

        child = fork();     //fork a child
        cmdnode = insert(command, child, count);    //insert into hashtable
        clock_gettime(CLOCK_MONOTONIC, &cmdnode->start);    //set start time
        forkexec(child,command,count,argument,cmdnode,filename_out,filename_err); //call forkexec function
    }

    //parent process
    if(child > 0) {
        pid_t c;
        int status;
        struct timespec finish;

        //loop until all child process are exited
        while ((c = wait(&status)) != -1) {
            clock_gettime(CLOCK_MONOTONIC, &finish); //set finish time
            cmdnode = search(c);    //retrieve struct of the completed process
            cmdnode->finish = finish;   //set finish time for the given struct
            double duration = 0.0;
            duration += (cmdnode->finish.tv_sec - cmdnode->start.tv_sec); //calculate runtime duration

            open_files(c, filename_out, filename_err);
            printf("Finished child %d pid of parent %d\n",c,getpid());
            printf("Finished at %ld, runtime duration %f\n",cmdnode->finish.tv_sec,duration);
            fflush(stdout);
            //exitcode
            if (WIFEXITED(status))
                fprintf(stderr, "Exited with exitcode = %d\n", WEXITSTATUS(status));
            //kill signal
            else if (WIFSIGNALED(status))
                fprintf(stderr, "Killed with signal %d\n", WTERMSIG(status));

            //restart process if duration is greater than 2 sec
            if (duration > 2) {
                pid_t rechild = fork(); //fork to restart command
                if (rechild > 0) {
                    cmdnode = insert(cmdnode->command, rechild, cmdnode->index);
                    open_files(rechild, filename_out, filename_err);
                    printf("RESTARTING\n");
                    fflush(stdout);
                }
                clock_gettime(CLOCK_MONOTONIC, &cmdnode->start);    //set start time
                //call forkexec function
                forkexec(rechild, cmdnode->command, cmdnode->index, argument, cmdnode, filename_out, filename_err);
            } else
                fprintf(stderr, "Spawning too fast\n");

            delete(c);  //delete and free
        }
    }
    //close and free
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
//Process fork child for executing the command using execvp or verify if error in forking
void forkexec(pid_t child, char *command, int count,
              char ** argument, struct COMMAND_NODE *cmdnode, char *filename_out, char *filename_err){
    if (child < 0) {    //if there is an error with fork exit
        printf("Error with fork for processing command %s.\n", command);
        exit(1);
    } else if (child == 0){   //child process
        open_files(getpid(), filename_out, filename_err);
        printf("Starting command %d: child %d pid of parent %d\n", count, getpid(), getppid());
        fflush(stdout);
        parse_argument(command, argument);  //parse argument
        execvp(*argument, argument);    //call execvp

        //only happens if execvp fails
        fprintf(stderr, "Invalid command: %s\n", command);
        exit(2);    //exit child process
    }
}