#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_NAME 100
#define NAME_LENGTH 30

typedef struct unique_name unique_name;
struct unique_name{
    char name[NAME_LENGTH];
    int count;
};

int main(int argc, char *argv[]) {
    struct unique_name names[NUM_NAME];
    char temp[NAME_LENGTH];
    int numUniqueName = 0;
    int pipe(int pFds1[2]);
    int pipe(int pFds2[2]);

    if(argc < 2) {
        printf("Unable to execute - check command line\n");
        exit(1);
    }
    for(int i = 1; i < argc; i++){
        pid_t child = fork();

        if(child < 0)
            printf("Error with fork\n");
        else if(child == 0){
            FILE* fp = fopen(argv[i],"r")
            if(fp == NULL){
                printf("Unable to open %s.\n", argv[i]);
                exit(1);
            }
            while(fgets(temp,NAME_LENGTH,fp)) {
                if (strcmp("\n", temp) == 0 || strcmp(" \n", temp) == 0) {
                    fprintf(stderr, "Warning - file %s line %d is empty.", _FILE_, _LINE_);
                } else {
                    char *newlineCheck = strchr(temp, '\n');
                    if (newlineCheck)
                        *newlineCheck = '\0';

                    int index = 0;
                    while (index < NUM_NAME) {
                        if (index >= numUniqueName) {
                            strcpy(names[index].name, temp);
                            numUniqueName++;
                            names[index].count++;
                            index = NUM_NAME;
                        } else if (strcmp(names[index].name, temp) == 0) {
                            names[index].count++;
                            index = NUM_NAME;
                        } else
                            index++;
                    }
                }
            }
            write(pFds1[1], names, NUM_NAME*sizeof(unique_name));
            write(pFds2[1], numUniqueName, sizeof(numUniqueName));
        }
        else{
            close(pFds1[1]);
            close(pFds2[1]);

            wait(NULL);
            read(pFds1[0], names, NUM_NAME*sizeof(unique_name));
            read(pFds2[0], numUniqueName, sizeof(numUniqueName));

        }

    }
    return 0;
}
