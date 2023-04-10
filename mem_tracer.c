/**
 * Description: Traces the functions that allocates, reallocates, and frees memory.
 * This is used to ensure that there is no memory leak within the program.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/24/2023
 * Creation date: 04/09/2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOCATION_ELEMENT 10
#define BUFFER_LENGTH 100
#define TRACE_FILENAME "memtrace.out"

void free_commands(char **commands, int count);

/**
 * COMMAND_NODE struct is a linked list of pointers to
 * command and it's index. COMMAND_NODE_HEAD is the head
 * of the linked list.
 */
typedef struct COMMAND_NODE{
    char *command;                  //ptr to the command
    int index;                      //index of the command
    struct  COMMAND_NODE *next;    //ptr to next COMMAND_NODE
}COMMAND_NODE;
/**
 * TRACE_NODE struct is a linked list of pointers to function
 * identifiers. TRACE_NODE_TOP is the head of the list
 * (top of the stack).
 */
typedef struct TRACE_NODE{
    char *functionId;           //ptr to function identifier (function name)
    struct TRACE_NODE *next;    // ptr to the next TRACE_NODE
}TRACE_NODE;

static TRACE_NODE  *TRACE_NODE_TOP = NULL;      //ptr to the top of the stack
static COMMAND_NODE *COMMAND_NODE_HEAD = NULL;  //ptr to the head od the linked list

/**
 * This function push the function identifier passed through the
 * char *p. It trace the sequence of the function calls. The "global"
 * string is the start of the function call trace.
 */
void PUSH_TRACE(char *p){
    TRACE_NODE *traceNode;
    static char glob[] = "global";

    //checks if TRACE_NODE_TOP is NULL
    if(TRACE_NODE_TOP == NULL) {
        //allocate memory for TRACE_NODE_TOP
        TRACE_NODE_TOP = (TRACE_NODE *) malloc(sizeof(TRACE_NODE));
        //failed during memory allocation
        if (TRACE_NODE_TOP == NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }
        TRACE_NODE_TOP->functionId = glob;  //assign TRACE_NODE_TOP function identifier to glob ("global")
        TRACE_NODE_TOP->next = NULL;        //assign TRACE_NODE_TOP next to NULL
    }
    //allocate memory for traceNode
    traceNode = (TRACE_NODE *) malloc(sizeof(TRACE_NODE));
    //failed during memory allocation
    if(traceNode == NULL) {
        printf("PUSH_TRACE: memory allocation error\n");
        exit(1);
    }
    //push to the top of the stack
    traceNode->functionId = p;          //assign traceNode function identifier to p (passed through parameter)
    traceNode->next = TRACE_NODE_TOP;   //assign traceNode next to TRACE_NODE_TOP
    TRACE_NODE_TOP = traceNode;         //assign TRACE_NODE_TOP to traceNode
}
/**
 * This function pops the TRACE_NODE_TOP (top of the  stacks) and
 * frees the memory. Then assign TRACE_NODE_TOP next to TRACE_NODE_TOP
 */
void POP_TRACE(){
    TRACE_NODE *traceNode;

    //checks if TRACE_NODE_TOP is NULL
    if(TRACE_NODE_TOP != NULL) {
        traceNode = TRACE_NODE_TOP;
        TRACE_NODE_TOP = traceNode->next;
        free(traceNode);                //free traceNode ptr
    }
}
/**
 * This function print (max 50) the sequence of function calls that
 * are on the stack at this instance. Then return the sequence of function
 * calls as string (char *).
 */
char* PRINT_TRACE(){
    int depth = 50; //max of 50 levels in the stack will be combined in a string
    int i, j, length;
    TRACE_NODE *traceNode;
    static char buf[BUFFER_LENGTH];

    if (TRACE_NODE_TOP == NULL){        //stack not initialized ("global" area)
        strcpy(buf,"global");   //copy "global" to buf
        return buf;
    }
    /**
     * Peek at the depth top entries on the stack without exceeding
     * 100 chars and going past the bottom of the stack.
     */
    sprintf(buf,"%s",TRACE_NODE_TOP->functionId);
    length = strlen(buf);   //length of the string thus far

    for(i = 1, traceNode = TRACE_NODE_TOP->next; traceNode != NULL && i < depth; i++, traceNode = traceNode->next){
        j = strlen(traceNode->functionId);  //length to be added
        if((length + j + 1) < 100){     //total length less than 100
            sprintf((buf + length),":%s",traceNode->functionId);
            length += j + 1;
        }
        else        //length exceeds 100
            break;
    }
    return buf;
}
/**
 * This function REALLOC calls realloc to expand the allocated memory
 * for command line input. It also prints the sequence of function calls
 * on the stack using PRINT_TRACE and the address segment assigned when
 * reallocating memory.
 */
void* REALLOC(void* p, int t, char* file, int line) {
    FILE *fp;
    //open file
    fp = fopen(TRACE_FILENAME,"a");
    if(fp == NULL){     //verify if unable to open file
        printf("Unable to open file: %s\n",TRACE_FILENAME);
        exit(1);
    }
    p = realloc(p, t);  //realloc
    //program file (not the full file path)
    char* fn = strrchr(file,'/') ? strrchr(file,'/') : file;
    //print to memtrace.out file - call PRINT_TRACE for sequence of function calls
    fprintf(fp,"File=\"%s\",Line=%d,Function=%s,Segment reallocation to address %p\n",fn,line,PRINT_TRACE(),p);
    fflush(fp);
    fclose(fp);     //close file
    return p;
}
/**
 * This function MALLOC calls malloc to allocate memory for command
 * line input (initial memory allocation). It also prints the sequence of function calls
 * on the stack using PRINT_TRACE and the address segment assigned when
 * allocating memory.
 */
void* MALLOC(int t, char* file, int line) {
    FILE *fp;
    //open file
    fp = fopen(TRACE_FILENAME,"a");
    if(fp == NULL){     //verify if unable to open file
        printf("Unable to open file: %s\n",TRACE_FILENAME);
        exit(1);
    }
    void* p = malloc(t);    //allocate memory
    //program file (not the full file path)
    char* fn = strrchr(file,'/') ? strrchr(file,'/') : file;
    //print to memtrace.out file - call PRINT_TRACE for sequence of function calls
    fprintf(fp,"File=\"%s\",Line=%d,Function=%s,Segment allocation to address %p\n",fn,line,PRINT_TRACE(),p);
    fflush(fp);
    fclose(fp);     //close file
    return p;
}
/**
 * This function FREE calls free to deallocate memory for the specified
 * command line input (void* p). It also prints the sequence of function calls
 * on the stack using PRINT_TRACE and the address segment assigned when
 * freeing memory.
 */
void FREE(void* p, char* file, int line) {
    FILE *fp;
    //open file
    fp = fopen(TRACE_FILENAME,"a");
    if(fp == NULL){ //verify file open properly 
        printf("Unable to open file: %s\n",TRACE_FILENAME);
        exit(1);
    }
    free(p);
    //program file (not the full file path)
    char* fn = strrchr(file,'/') ? strrchr(file,'/') : file;
    //print to memtrace.out file - call PRINT_TRACE for sequence of function calls
    fprintf(fp,"File=\"%s\",Line=%d,Function=%s,Segment at address %p deallocated\n",fn,line,PRINT_TRACE(),p);
    fflush(fp);
    fclose(fp);
}

//define memory realloc, malloc, free
#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)

/**
 * This function push the command line input and index passed through the
 * char *p and int index. It stores the command lines and their index.
 */
void PUSH_COMMAND(char *p, int index){
    PUSH_TRACE("PUSH_COMMAND");
    COMMAND_NODE *currentNode;

    //check if COMMAND_NODE_HEAD is NULL
    if(COMMAND_NODE_HEAD == NULL) {
        //allocate COMMAND_NODE_HEAD memory
        COMMAND_NODE_HEAD = (COMMAND_NODE *) malloc(sizeof(COMMAND_NODE));
        if (COMMAND_NODE_HEAD == NULL) {    //verify if unable to allocate COMMAND_NODE_HEAD
            printf("PUSH_COMMAND: memory allocation error\n");
            exit(1);
        }
        COMMAND_NODE_HEAD->command = p;     //assign ptr p to COMMAND_NODE_HEAD command
        COMMAND_NODE_HEAD->index = index;   //assign index to COMMAND_NODE_HEAD index
        COMMAND_NODE_HEAD->next = NULL;     //assign NULL to COMMAND_NODE_HEAD next
    }
    else{       //COMMAND_NODE_HEAD already allocated
        //allocate currentNode memory
        currentNode = (COMMAND_NODE *) malloc(sizeof(COMMAND_NODE));
        if(currentNode == NULL) {   //verify if unable to allocate currentNode
            printf("PUSH_COMMAND: memory allocation error\n");
            exit(1);
        }
        currentNode->command = p;       //assign ptr p to currentNode command
        currentNode->index = index;     //assign index to currentNode index
        currentNode->next = COMMAND_NODE_HEAD;  //assign ptr COMMAND_NODE_HEAD to currentNode next
        COMMAND_NODE_HEAD = currentNode;        //assign ptr currentNode to COMMAND_NODE_HEAD
    }
    POP_TRACE();
}
/**
 * This function prints the content (command and index) of the
 * LinkedList recursively.
 */
void PRINT_COMMANDS(COMMAND_NODE *current){
    PUSH_TRACE("PRINT_COMMANDS");
    //calls PRINT_COMMANDS until NULL
    if(current != NULL) {
        PRINT_COMMANDS(current->next);
        printf("Index: %d, Command: %s\n", current->index, current->command);
    }
    POP_TRACE();
}
/**
 * This function free the LinkedList.
 */
void FREE_LIST(){
    PUSH_TRACE("FREE_LIST");
    //loops until COMMAND_NODE_HEAD is NULL
    while(COMMAND_NODE_HEAD != NULL){
        COMMAND_NODE *currentNode = COMMAND_NODE_HEAD;  //assign COMMAND_NODE_HEAD to currentNode
        COMMAND_NODE_HEAD = currentNode->next;          //assign currentNode next to COMMAND_NODE_HEAD
        free(currentNode);      //free currentNode
    }
    POP_TRACE();
}
/**
 * This function frees the command line inputs and
 * command line array.
 */
void free_commands(char **commands, int count){
    PUSH_TRACE("free_commands");
    //loop through command array to free each command
    for(int i = 0; i <= count; i++)
        free(commands[i]);
    free(commands);     //free command array
    POP_TRACE();
}
int main(int argc, char *argv[]) {
    PUSH_TRACE("main");
    //declare data
    int allocatedElements = 0;      //track for reallocation
    char **command;                 //command array

    //allocate command array
    command = (char **) malloc(ALLOCATION_ELEMENT * sizeof (char *));
    allocatedElements = ALLOCATION_ELEMENT;     //number of elements allocated thus far
    //traverse trough each file
    int count = 0;      //index tracking
    //allocate for command input
    command[count] = (char *) malloc(BUFFER_LENGTH * sizeof (char));
    //gets user input from stdin
    while (fgets(command[count], BUFFER_LENGTH, stdin)) {
        //replace newline with null 
        char *temp = strchr(command[count],'\n');
        if(temp != NULL)
           temp = '\0';

        PUSH_COMMAND(command[count],count + 1);    //push command and index into LinkedList
        count++;
        /**
         *Verify if allocated memory for command array (number of elments)
         *exceeds number of commands read, if so reallocate.
         */
        if(count >= allocatedElements) {
            PRINT_COMMANDS(COMMAND_NODE_HEAD);  //print content in LinkedList
            allocatedElements = allocatedElements + ALLOCATION_ELEMENT;     //number of elements allocated thus far
            command = (char **) realloc(command, (allocatedElements) * sizeof(char *));
        }
        //allocate for command input
        command[count] = (char *) malloc(BUFFER_LENGTH * sizeof (char));
    }
    //call free_commands to free memory command input and command array
    free_commands(command, count);
    FREE_LIST();    //free LinkedList
    POP_TRACE();    //free main from stack
    POP_TRACE();    //free global from stack
    return 0;
}


