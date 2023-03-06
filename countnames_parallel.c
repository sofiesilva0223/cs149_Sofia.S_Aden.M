/**
 * Description: Determine each unique name and its quality within multiple file
 * using a multiple process to process the data and use pipe to merge the content.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/27/2023
 * Creation date: 03/04/2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_NAME 100
#define NAME_LENGTH 30

//structure of a unique_name type
typedef struct unique_name{
    char name[NAME_LENGTH];
    int count;
}unique_name;

//function declarations
void namesModify(struct unique_name names[NUM_NAME], struct unique_name tempName);
void display(struct unique_name names[NUM_NAME]);

int main(int argc, char *argv[]) {
    //declare data
    struct unique_name names[NUM_NAME] = {0};
    pid_t child;
    int fd[2];
    pipe(fd);

    //exit if no filename input
    if (argc < 2) {
        printf("Unable to execute - check command line\n");
        exit(1);
    }
    //traverse trough each file
    for (int i = 1; i < argc; i++) {
        child = fork();     //fork a child
        if (child < 0) {    //if there is an error with fork exit
            printf("Error with fork for processing file %s.\n", argv[i]);
            exit(1);
        } else if (child == 0) {    //child process
            FILE *fp = fopen(argv[i], "r");    //open file
            //verify file exist
            if (fp == NULL) {
                printf("Unable to open %s.\n", argv[i]);
                exit(1);
            }
            char temp[NAME_LENGTH];
            struct unique_name tempName;
            int line = 0;
            while (fgets(temp, NAME_LENGTH, fp)) {  //read each line in file
                line++;
                if (strcmp("\n", temp) == 0 || strcmp(" \n", temp) == 0) {  //check empty line
                    fprintf(stderr, "Warning - file %s line %d is empty.\n", argv[i], line);
                } else {
                    //change newline char into null
                    char *newlineCheck = strchr(temp, '\n');
                    if (newlineCheck)
                        *newlineCheck = '\0';
                    strcpy(tempName.name,temp);
                    tempName.count = 1;
                    namesModify(names,tempName);    //call namesModify
                }
            }
            //write to pipe the data read from file and assigned to names
            write(fd[1], names, NUM_NAME * sizeof(unique_name));
            fclose(fp); //close file
            exit(0);    //exit child process
        }
    }
    //parent process
    if(child > 0) {
        pid_t c;
        close(fd[1]);
        struct unique_name tempNames[NUM_NAME] = {0};
        //loop until all child process are exited
        while ((c = wait(NULL)) != -1) {
            int index = 0;
            //read from pipe data passed from chile process
            read(fd[0], tempNames, NUM_NAME * sizeof(unique_name));
            //printf("child %d display:\n", c);
            //display(tempNames);
            //loop until no names in array
            while (tempNames[index].count != 0) {
                namesModify(names, tempNames[index]); //call namesModify
                index++;
            }
        }
        display(names); //call display
    }
    return 0;
}
/**
 * This function insert a name into unique_name struct array is the name DNE
 * otherwise, if the names exist increment the count by the count amount of the
 * name that is being inserted.
 */
void namesModify(struct unique_name names[NUM_NAME], struct unique_name tempName){
    int index = 0;
    //loop until NUM_NAME (max number of unique names)
    while (index < NUM_NAME) {
        //name DNE in the array so insert at index
        if (names[index].count == 0) {
            names[index] = tempName;
            index = NUM_NAME;
        }
        //name exist so increment count by the name count value
        else if (strcmp(names[index].name, tempName.name) == 0) {
            names[index].count += tempName.count;
            index = NUM_NAME;
        }
        //otherwise increment index
        else
            index++;
    }
}
// This function displays each unique name with its occurrence
void display(struct unique_name names[NUM_NAME]) {
    int index = 0;
    while(names[index].count != 0) {
        printf("%s: %d\n", names[index].name, names[index].count);
        index++;
    }
}
