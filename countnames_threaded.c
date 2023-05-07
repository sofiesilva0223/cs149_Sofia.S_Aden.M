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
#include <pthread.h>
#include <time.h>

#define HASHSIZE 101
#define NAME_LENGTH 30

//data declaration
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid1, tid2;
int logindex = 0;

//function declarations
void *thread_runner(void *);

typedef struct THREADDATA{
    pthread_t creator;
}THREADDATA;

//structure of a unique_name type
typedef struct unique_name{
    char *name;
    int count;
    struct unique_name *next;
}unique_name;

static THREADDATA *p = NULL;
static struct unique_name *names[HASHSIZE];

unsigned hash(char *s){
    unsigned hashval = 0;

    for(; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}
struct unique_name *search(char *s){
    struct unique_name *value;

    //locate the node with the string value
    pthread_mutex_lock(&lock);
    int hashval = hash(s);
    value = names[hashval];
    while(value != NULL){
        if(strcmp(s,value->name) == 0) {
            pthread_mutex_unlock(&lock);
            return value;   //found
        }
        value = value->next;
    }
    pthread_mutex_unlock(&lock);

    return NULL;    //not found

}
/**
 * This function insert a name into unique_name struct array is the name DNE
 * otherwise, if the names exist increment the count by the count amount of the
 * name that is being inserted.
 */
struct unique_name *insert(char *s){
    struct unique_name *value;

    if((value = search(s)) == NULL) {
        value = (struct unique_name *) malloc(sizeof(struct unique_name));
        if (value == NULL)
            return NULL;
        pthread_mutex_lock(&lock);
        int hashval = hash(s);
        if ((value->name = strdup(s)) == NULL) {
        pthread_mutex_unlock(&lock);
        return NULL;
        }
        value->count = 1;
        value->next = names[hashval];
        names[hashval] = value;
        pthread_mutex_unlock(&lock);
    }
    else{
        pthread_mutex_lock(&lock);
        value->count++;
        pthread_mutex_unlock(&lock);
    }

    return value;
}
void delete(char *s){
    struct unique_name *value;
    struct unique_name *temp;

    pthread_mutex_lock(&lock);
    int hashval = hash(s);
    value = names[hashval];
    if(strcmp(value->name,s) == 0){
        temp = value;
        names[hashval] = value->next;
        free(temp->name);
        free(temp);
        pthread_mutex_unlock(&lock);
        return;
    }

    while(value != NULL) {
        temp = value->next;
        if (temp != NULL && (strcmp(temp->name, s)) == 0) {
            value->next = temp->next;
            free(temp->name);
            free(temp);
            pthread_mutex_unlock(&lock);
            return;
        }
        value = value->next;
    }
    pthread_mutex_unlock(&lock);
}
void clear(){
    struct unique_name *value;
    struct unique_name *temp;

    pthread_mutex_lock(&lock);
    for(int index = 0; index < HASHSIZE; index++){
        value = names[index];
        while(value != NULL) {
            temp = value;
            value = value->next;
            free(temp->name);
            free(temp);
        }
    }
    pthread_mutex_unlock(&lock);

}
// This function displays each unique name with its occurrence
void display() {
    struct unique_name *value;

    pthread_mutex_lock(&lock);
    for(int index = 0; index < HASHSIZE; index++){
        value = names[index];
        while(value != NULL){
            printf("%s: %d\n", value->name, value->count);
            value = value->next;
        }
    }
    pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[]) {

    //exit if no filename input
    if (argc != 3) {
        printf("Unable to execute - check command line\n");
        exit(1);
    }

    printf("================================ Log Messages ================================\n");
    printf("Create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,argv[1]);

    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,argv[2]);

    //printf("Wait for first thread to exit\n");
    pthread_join(tid1,NULL);
    printf("First thread exited\n");

    //printf("Wait for second thread to exit\n");
    pthread_join(tid2,NULL);
    printf("Second thread exited\n");

    printf("\n================================ Name Counts ================================\n");

    display(); //call display
    clear();
    return 0;
}
void *thread_runner(void *x){
    pthread_t tid;
    tid = pthread_self();
    char *filename = (char *)x;

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    pthread_mutex_lock(&lock1);
    if(p == NULL) {
        p = (THREADDATA *) malloc(sizeof(THREADDATA));
        p->creator = tid;
    }
    pthread_mutex_unlock(&lock1);

    pthread_mutex_lock(&lock2);
    logindex++;
    if(p != NULL && p->creator == tid){
        printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d ",
            logindex,tid,getpid(),local->tm_mday,(local->tm_mon + 1),(local->tm_year + 1900));
        if(local->tm_hour < 12)
            printf("%02d:%02d:%02d am: This is thread %ld and I created THREADDATA %p\n",
                   local->tm_hour,local->tm_min,local->tm_sec,tid,p);
        else
            printf("%02d:%02d:%02d pm: This is thread %ld and I created THREADDATA %p\n",
                   (local->tm_hour-12),local->tm_min,local->tm_sec,tid,p);
    }
    else{
        printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d ",
               logindex,tid,getpid(),local->tm_mday,(local->tm_mon + 1),(local->tm_year + 1900));
        if(local->tm_hour < 12)
            printf("%02d:%02d:%02d am: This is thread %ld and I can access the THREADDATA %p\n",
                   local->tm_hour,local->tm_min,local->tm_sec,tid,p);
        else
            printf("%02d:%02d:%02d pm: This is thread %ld and I can access THREADDATA %p\n",
                   (local->tm_hour-12),local->tm_min,local->tm_sec,tid,p);
    }
    pthread_mutex_unlock(&lock2);

    FILE *fp = fopen(filename, "r");    //open file
    //verify file exist
    if (fp == NULL) {
        printf("Unable to open %s.\n", filename);
        exit(1);
    }

    pthread_mutex_lock(&lock2);
    logindex++;
    printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d ",
           logindex,tid,getpid(),local->tm_mday,(local->tm_mon + 1),(local->tm_year + 1900));
    if(local->tm_hour < 12)
        printf("%02d:%02d:%02d am: opened file %s\n",
               local->tm_hour,local->tm_min,local->tm_sec,filename);
    else
        printf("%02d:%02d:%02d pm: opened file %s\n",
               (local->tm_hour-12),local->tm_min,local->tm_sec,filename);
    pthread_mutex_unlock(&lock2);

    int line = 0;
    char *temp = (char *)malloc(NAME_LENGTH * sizeof(char));
    if(temp == NULL)
        return NULL;

    while (fgets(temp, NAME_LENGTH, fp)) {  //read each line in file
        line++;
        if (strcmp("\n", temp) == 0 || strcmp(" \n", temp) == 0)  //check empty line
            fprintf(stderr, "Warning - file %s line %d is empty.\n", filename, line);
        else {
            //change newline char into null
            char *newlineCheck = strchr(temp, '\n');
            if (newlineCheck)
                *newlineCheck = '\0';
            insert(temp);   //call insert
        }
    }
    fclose(fp);
    free(temp);
    temp = NULL;

    pthread_mutex_lock(&lock2);
    logindex++;
    if(p != NULL && p->creator == tid){
        pthread_mutex_lock(&lock1);
        free(p);
        p = NULL;
        pthread_mutex_unlock(&lock1);
        printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d ",
               logindex,tid,getpid(),local->tm_mday,(local->tm_mon + 1),(local->tm_year + 1900));
        if(local->tm_hour < 12)
            printf("%02d:%02d:%02d am: This is thread %ld and I delete THREADDATA\n",
                   local->tm_hour,local->tm_min,local->tm_sec,tid);
        else
            printf("%02d:%02d:%02d pm: This is thread %ld and I delete THREADDATA\n",
                   (local->tm_hour-12),local->tm_min,local->tm_sec,tid);
    }
    else{
        printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d ",
               logindex,tid,getpid(),local->tm_mday,(local->tm_mon + 1),(local->tm_year + 1900));
        if(local->tm_hour < 12)
            printf("%02d:%02d:%02d am, This is thread %ld and I can access the THREADDATA\n",
                   local->tm_hour,local->tm_min,local->tm_sec,tid);
        else
            printf("%02d:%02d:%02d pm, This is thread %ld and I can access THREADDATA\n",
                   (local->tm_hour-12),local->tm_min,local->tm_sec,tid);
    }
    pthread_mutex_unlock(&lock2);

    pthread_exit(NULL);
}
