#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_NAME 100
#define NAME_LENGTH 30

typedef struct unique_name{
    char name[NAME_LENGTH];
    int count;
}unique_name;

void namesModify(struct unique_name names[NUM_NAME], struct unique_name tempName);
void display(struct unique_name names[NUM_NAME]);

int main(int argc, char *argv[]) {
    struct unique_name names[NUM_NAME] = {0};
    pid_t child;
    int fd[2];
    pipe(fd);

    if (argc < 2) {
        printf("Unable to execute - check command line\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        child = fork();
        if (child < 0) {
            printf("Error with fork for processing file %s.\n", argv[i]);
            exit(1);
        } else if (child == 0) {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("Unable to open %s.\n", argv[i]);
                exit(1);
            }
            char temp[NAME_LENGTH];
            struct unique_name tempName;
            int line = 0;
            while (fgets(temp, NAME_LENGTH, fp)) {
                line++;
                if (strcmp("\n", temp) == 0 || strcmp(" \n", temp) == 0) {
                    fprintf(stderr, "Warning - file %s line %d is empty.\n", argv[i], line);
                } else {
                    char *newlineCheck = strchr(temp, '\n');
                    if (newlineCheck)
                        *newlineCheck = '\0';
                    strcpy(tempName.name,temp);
                    tempName.count = 1;
                    namesModify(names,tempName);
                }
            }
            write(fd[1], names, NUM_NAME * sizeof(unique_name));
            fclose(fp);
            exit(0);
        }
    }
        pid_t c;
        close(fd[1]);
        struct unique_name tempNames[NUM_NAME] = {0};
        while ((c = wait(NULL)) != -1) {
            int index = 0;
            read(fd[0], tempNames, NUM_NAME * sizeof(unique_name));
            printf("child %d display:\n", c);
            display(tempNames);
            while(tempNames[index].count != 0){
                namesModify(names,tempNames[index]);
                index++;
            }

    }
    display(names);
    return 0;
}
void namesModify(struct unique_name names[NUM_NAME], struct unique_name tempName){
    int index = 0;
    while (index < NUM_NAME) {
        if (names[index].count == 0) {
            names[index] = tempName;
            index = NUM_NAME;
        } else if (strcmp(names[index].name, tempName.name) == 0) {
            names[index].count += tempName.count;
            index = NUM_NAME;
        } else
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
