#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define NUM_NAME 100
#define NAME_LENGTH 30

int main(int argc, char *argv[]) {

    char names[NUM_NAME][NAME_LENGTH] = {'\0'};
    int count[NUM_NAME] = {0};
    char temp[NAME_LENGTH];
    int numUniqueName = 0;

    if(argc != 2) {
        printf("Unable to execute - check command line\n");
        exit(1);
    }

    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("Unable to open file.\n");
        exit(1);
    }

    int lineNumber = 0;
    while(fgets(temp,NAME_LENGTH,fp)){
        lineNumber++;
        if(strcmp("\n",temp) == 0 || strcmp(" \n",temp) == 0){
            fprintf(stderr,"Warning - Line %d is empty.", lineNumber);
        }
        else{
            char *newlineCheck = strchr(temp,'\n');
            if(newlineCheck)
                *newlineCheck = '\0';

            int i = 0;
            while(i < NUM_NAME) {
                if (i >= numUniqueName) {
                    strcpy(names[i],temp);
                    numUniqueName++;
                    count[i]++;
                    i = NUM_NAME;
                }
                else if (strcmp(names[i], temp) == 0) {
                    count[i]++;
                    i = NUM_NAME;
                }
                else
                    i++;
            }
        }
    }
    fclose(fp);
    return 0;
}
