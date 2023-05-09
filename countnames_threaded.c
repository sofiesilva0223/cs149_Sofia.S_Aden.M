/**
 * Description: Program has two threads where thread one reads names from one file and
 * simultaneously thread two reads another file. Both threads are access the same hashtable
 * which stores the distinct names and number of occurrences within both file while ensuring
 * no data corruption (uses mutex locks). Additionally, each thread prints out a log message
 * to the stdout when it's created, their access control to THREADDATA and when it opens a new
 * file for reading. After both threads exit, the distinct names and number of occurrences are
 * printed to stdout.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 05/07/2023
 * Creation date: 05/07/2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//define declaration
#define HASHSIZE 101
#define NAME_LENGTH 30

//data declaration
//mutex lock for accessing the hashtable (count)
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
//mutex lock for accessing the THREADDATA
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
//mutex lock for accessing the log index
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER; 
pthread_t tid1, tid2;   //threads
int logindex = 0;       //initializing the logindex

//function declarations
void *thread_runner(void *);

//structure THREADATA typedef
typedef struct THREADDATA{
    pthread_t creator;
}THREADDATA;

//structure unique_name typedef
typedef struct unique_name{
    char *name;
    int count;
    struct unique_name *next;
}unique_name;

static THREADDATA *p = NULL;    //initialize THREADDATA
static struct unique_name *names[HASHSIZE]; //declare hashtable

//hashing function which take the name as a parameter
unsigned hash(char *s){
    unsigned hashval = 0;

    for(; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}
/*
 * Search the hashtable using the name parameter passed.
 * Then traverse through hash index's linked list until locating
 * the unique_name struct with provided name. If unable to locate
 * return null.
 * */
struct unique_name *search(char *s){
    struct unique_name *value;

    //locate the node with the string value
    pthread_mutex_lock(&lock);  //lock hashtable
    int hashval = hash(s);
    value = names[hashval];
    while(value != NULL){
        if(strcmp(s,value->name) == 0) {
            pthread_mutex_unlock(&lock); //unlock hashtable (found)
            return value;   //found
        }
        value = value->next;
    }
    pthread_mutex_unlock(&lock); //unlock hashtable (not found)

    return NULL;    //not found

}
/**
 * This function insert a name into the hashtable using the provided name;
 * if the name DNE. Otherwise, if the names exist increment the count of the
 * name that is being inserted.
 */
struct unique_name *insert(char *s){
    struct unique_name *value;

    //name is not in the hashtable
    if((value = search(s)) == NULL) {
        value = (struct unique_name *) malloc(sizeof(struct unique_name));
        if (value == NULL)
            return NULL;
        pthread_mutex_lock(&lock);  //lock hashtable
        int hashval = hash(s);
        if ((value->name = strdup(s)) == NULL) {
        pthread_mutex_unlock(&lock);    //unlock hashtable
        return NULL;
        }
        value->count = 1;
        value->next = names[hashval];
        names[hashval] = value;
        pthread_mutex_unlock(&lock);    //unlock hashtable
    }
    //name already in the hashtable
    else{
        pthread_mutex_lock(&lock);
        value->count++;
        pthread_mutex_unlock(&lock);
    }

    return value;
}
/*
 * delete name from has table, if the name passed in
 * parameter matches a name within the hashtable.
 **/
void delete(char *s){
    struct unique_name *value;
    struct unique_name *temp;

    pthread_mutex_lock(&lock);  //lock hashtable
    int hashval = hash(s);
    value = names[hashval];
    if(strcmp(value->name,s) == 0){
        temp = value;
        names[hashval] = value->next;
        free(temp->name);
        free(temp);
        pthread_mutex_unlock(&lock);    //unlock hashtable
        return;
    }

    while(value != NULL) {
        temp = value->next;
        if (temp != NULL && (strcmp(temp->name, s)) == 0) {
            value->next = temp->next;
            free(temp->name);
            free(temp);
            pthread_mutex_unlock(&lock);    //unlock hashtable
            return;
        }
        value = value->next;
    }
    pthread_mutex_unlock(&lock);    //unlock hashtable
}
//function free all the content of the hashtable and the hashtable
void clear(){
    struct unique_name *value;
    struct unique_name *temp;

    pthread_mutex_lock(&lock);  //lock hashtable
    for(int index = 0; index < HASHSIZE; index++){
        value = names[index];
        while(value != NULL) {
            temp = value;
            value = value->next;
            free(temp->name);
            free(temp);
        }
    }
    pthread_mutex_unlock(&lock);    //unlock hashtable

}
// This function displays each unique name with its occurrence
void display() {
    struct unique_name *value;

    pthread_mutex_lock(&lock);  //lock hashtable
    for(int index = 0; index < HASHSIZE; index++){
        value = names[index];
        while(value != NULL){
            printf("%s: %d\n", value->name, value->count);
            value = value->next;
        }
    }
    pthread_mutex_unlock(&lock);    //unlock hashtable
}

int main(int argc, char *argv[]) {

    //verify the number of argument passed through command line
    if (argc != 3) {
        printf("Unable to execute - check command line\n");
        exit(1);
    }

    printf("================================ Log Messages ================================\n");
    printf("Create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,argv[1]);   //create thread one w/ filename 1 as an argument

    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,argv[2]);   //create thread two w/ filename 2 as an argument

    //printf("Wait for first thread to exit\n");
    pthread_join(tid1,NULL);    //wait for thread one to exit
    printf("First thread exited\n");

    //printf("Wait for second thread to exit\n");
    pthread_join(tid2,NULL);    //wait for thread two to exit
    printf("Second thread exited\n");

    printf("\n================================ Name Counts ================================\n");

    display(); //call display
    clear();
    return 0;
}
/*
 * Runs the thread which opens respective file and calls insert with
 * name from the file to insert into hashtable.
 * */
void *thread_runner(void *x){
    //declare local variable
    pthread_t tid;
    tid = pthread_self();
    char *filename = (char *)x;

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    pthread_mutex_lock(&lock1); //lock THREADDATA
    if(p == NULL) {
        p = (THREADDATA *) malloc(sizeof(THREADDATA));
        p->creator = tid;
    }
    pthread_mutex_unlock(&lock1);   //unlock THREADDATA

    pthread_mutex_lock(&lock2); //lock logindex
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
    pthread_mutex_unlock(&lock2);   //unlock logindex

    FILE *fp = fopen(filename, "r"); //open file
    //verify file exist
    if (fp == NULL) {
        printf("Unable to open %s.\n", filename);
        exit(1);
    }

    pthread_mutex_lock(&lock2); //lock logindex
    logindex++;
    printf("Logindex %d, thread %ld, PID %d, %02d/%02d/%d ",
           logindex,tid,getpid(),local->tm_mday,(local->tm_mon + 1),(local->tm_year + 1900));
    if(local->tm_hour < 12)
        printf("%02d:%02d:%02d am: opened file %s\n",
               local->tm_hour,local->tm_min,local->tm_sec,filename);
    else
        printf("%02d:%02d:%02d pm: opened file %s\n",
               (local->tm_hour-12),local->tm_min,local->tm_sec,filename);
    pthread_mutex_unlock(&lock2);   //unlock logindex

    int line = 0;
    //allocate temo
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
    fclose(fp); //close file
    free(temp); //free temp
    temp = NULL;    //set temp to null

    pthread_mutex_lock(&lock2); //lock logindex
    logindex++;
    if(p != NULL && p->creator == tid){
        pthread_mutex_lock(&lock1); //lock THREADDATA
        free(p);    //free THREADDATA
        p = NULL;   //set THREADATA to null
        pthread_mutex_unlock(&lock1);   //unlock THREADDATA
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
    pthread_mutex_unlock(&lock2);   //unlock logindex

    pthread_exit(NULL); //exit thread
}