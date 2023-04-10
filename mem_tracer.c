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
 * reallocating.
 */
void* REALLOC(void* p, int t, char* file, int line) {
    FILE *fp;
    fp = fopen(TRACE_FILENAME,"a");
    if(fp == NULL){
        printf("Unable to open file: %s\n",TRACE_FILENAME);
        exit(1);
    }
    p = realloc(p, t);
    char* fn = strrchr(file,'/') ? strrchr(file,'/') : file;
    fprintf(fp,"File=\"%s\",Line=%d,Function=%s,Segment reallocation to address %p\n",fn,line,PRINT_TRACE(),p);
    fflush(fp);
    fclose(fp);
    return p;
}
/**
 * This function MALLOC calls malloc to allocate memory for command
 * line input (initial memory allocation). It also prints the sequence of function calls
 * on the stack using PRINT_TRACE and the address segment assigned when
 * reallocating.
 */
void* MALLOC(int t, char* file, int line) {
    FILE *fp;
    fp = fopen(TRACE_FILENAME,"a");
    if(fp == NULL){
        printf("Unable to open file: %s\n",TRACE_FILENAME);
        exit(1);
    }
    void* p = malloc(t);
    char* fn = strrchr(file,'/') ? strrchr(file,'/') : file;
    fprintf(fp,"File=\"%s\",Line=%d,Function=%s,Segment allocation to address %p\n",fn,line,PRINT_TRACE(),p);
    fflush(fp);
    fclose(fp);
    return p;
}
/**
 * This function FREE calls free to deallocate memory for the specified
 * command line input (void* p). It also prints the sequence of function calls
 * on the stack using PRINT_TRACE and the address segment assigned when
 * reallocating.
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
    char* fn = strrchr(file,'/') ? strrchr(file,'/') : file;
    fprintf(fp,"File=\"%s\",Line=%d,Function=%s,Segment at address %p deallocated\n",fn,line,PRINT_TRACE(),p);
    fflush(fp);
    fclose(fp);
}

#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)

void PUSH_COMMAND(char *p, int index){
    PUSH_TRACE("PUSH_COMMAND");
    COMMAND_NODE *currentNode;

    if(COMMAND_NODE_HEAD == NULL) {
        COMMAND_NODE_HEAD = (COMMAND_NODE *) malloc(sizeof(COMMAND_NODE));
        if (COMMAND_NODE_HEAD == NULL) {
            printf("PUSH_COMMAND: memory allocation error\n");
            exit(1);
        }
        COMMAND_NODE_HEAD->command = p;
        COMMAND_NODE_HEAD->index = index;
        COMMAND_NODE_HEAD->next = NULL;
    }
    else{
        currentNode = (COMMAND_NODE *) malloc(sizeof(COMMAND_NODE));
        if(currentNode == NULL) {
            printf("PUSH_COMMAND: memory allocation error\n");
            exit(1);
        }
        currentNode->command = p;
        currentNode->index = index;
        currentNode->next = COMMAND_NODE_HEAD;
        COMMAND_NODE_HEAD = currentNode;
    }
    POP_TRACE();
}
void PRINT_COMMANDS(COMMAND_NODE *current){
    PUSH_TRACE("PRINT_COMMANDS");
    if(current != NULL) {
        PRINT_COMMANDS(current->next);
        printf("Index: %d, Command: %s\n", current->index, current->command);
    }
    POP_TRACE();
}
void FREE_LIST(){
    PUSH_TRACE("FREE_LIST");
    while(COMMAND_NODE_HEAD != NULL){
        COMMAND_NODE *currentNode = COMMAND_NODE_HEAD;
        COMMAND_NODE_HEAD = currentNode->next;
        free(currentNode);
    }
    POP_TRACE();
}
void free_commands(char **commands, int count){
    PUSH_TRACE("free_commands");
    for(int i = 0; i <= count; i++)
        free(commands[i]);
    free(commands);
    POP_TRACE();
}
int main(int argc, char *argv[]) {
    PUSH_TRACE("main");
    //declare data
    int allocatedElements = 0;
    char **command;

    command = (char **) malloc(ALLOCATION_ELEMENT * sizeof (char *));
    allocatedElements = ALLOCATION_ELEMENT;
    //traverse trough each file
    int count = 0;
    command[count] = (char *) malloc(BUFFER_LENGTH * sizeof (char));
    while (fgets(command[count], BUFFER_LENGTH, stdin)) {
        char *temp = strchr(command[count],'\n');
        if(temp != NULL)
           temp = '\0';

        PUSH_COMMAND(command[count],count + 1);
        count++;
        if(count >= allocatedElements) {
            PRINT_COMMANDS(COMMAND_NODE_HEAD);
            allocatedElements = allocatedElements + ALLOCATION_ELEMENT;
            command = (char **) realloc(command, (allocatedElements) * sizeof(char *));
        }
        command[count] = (char *) malloc(BUFFER_LENGTH * sizeof (char));
    }
    free_commands(command, count);
    FREE_LIST();
    POP_TRACE();    //free main from stack
    POP_TRACE();    //free global from stack
    return 0;
}


