/**
 * Description: Displays each unique name and its quality within a given file.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 02/19/2023
 * Creation date: 02/15/2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define NUM_NAME 100    //defines max number of unique names
#define NAME_LENGTH 30  //defines max length for a name

void display(int actualNumName, char name[NUM_NAME][NAME_LENGTH], int num[NUM_NAME]);

int main(int argc, char *argv[]) {

    //declares variables
    char names[NUM_NAME][NAME_LENGTH] = {'\0'};
    int count[NUM_NAME] = {0};
    char temp[NAME_LENGTH];
    int numUniqueName = 0;

    //verify arguments passed in command line includes filename
    if(argc != 2) {
        printf("Unable to execute - check command line\n");
        exit(0);
    }

    FILE* fp = fopen(argv[1], "r"); //open file
    //if file does not exists print error message and exit program with code 1
    if(fp == NULL){
        printf("Error: cannot open file.\n");
        exit(1);
    }

    int lineNumber = 0; //file line counter

    //read each line in the file until the EOF
    while(fgets(temp,NAME_LENGTH,fp)){
        lineNumber++;
        //verify current file line contains a name
        if(strcmp("\n",temp) == 0 || strcmp(" \n",temp) == 0){
            //print empty line error to stderr
            fprintf(stderr,"Warning - Line %d is empty.\n", lineNumber);
        }
        else{
            //find if string contains newline
            char *newlineCheck = strchr(temp,'\n');
            //if newline exists replace with null
            if(newlineCheck)
                *newlineCheck = '\0';

            int i = 0;
            while(i < NUM_NAME) {   //loop until max number of unique names
                if (i >= numUniqueName) {   //name read from file is unique
                    strcpy(names[i],temp);  //add name to array
                    numUniqueName++;    //increment number of unique names
                    count[i]++; //increment count of occurrence
                    i = NUM_NAME;   //exit loop
                }
                else if (strcmp(names[i], temp) == 0) { //not unique name
                    count[i]++; //increment count of occurrence
                    i = NUM_NAME; //exit loop
                }
                else    //continue loop
                    i++;
            }
        }
    }
    fclose(fp); //close file
    display(numUniqueName, names, count);
    return 0;
}

// This function displays each unique name with its occurrence
void display(int actualNumName, char name[NUM_NAME][NAME_LENGTH], int num[NUM_NAME]) {

    for (int i = 0; i < actualNumName; i++)
        printf("%s: %d\n",name[i],num[i]);
}

