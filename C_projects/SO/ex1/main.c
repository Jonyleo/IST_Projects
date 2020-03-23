#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "fs.h"

#define MAX_COMMANDS 150000
#define MAX_INPUT_SIZE 100

#define USAGE "Usage: %s inputfile outputfile numthreads\n"
#define TIME_MSG "TecnicoFS completed in [%.4Lf] seconds.\n"

#ifdef MUTEX
#define LOCK_INIT(M) pthread_mutex_init(M, NULL)
#define LOCK_DESTROY(M) pthread_mutex_destroy(M)
#define LOCK_WRLOCK(M) pthread_mutex_lock(M)
#define LOCK_RDLOCK(M) LOCK_WRLOCK(M)
#define LOCK_UNLOCK(M) pthread_mutex_unlock(M)
#define COMMAND_LOCK &commandLock
#define BST_LOCK &bstLock
#define THREAD_CONDITION numberThreads < 1
#elif RWLOCK
#define LOCK_INIT(M) pthread_rwlock_init(M, NULL)
#define LOCK_DESTROY(M) pthread_rwlock_destroy(M)
#define LOCK_WRLOCK(M) pthread_rwlock_wrlock(M)
#define LOCK_RDLOCK(M) pthread_rwlock_rdlock(M)
#define LOCK_UNLOCK(M) pthread_rwlock_unlock(M)
#define COMMAND_LOCK &commandLock
#define BST_LOCK &bstLock
#define THREAD_CONDITION numberThreads < 1
#else
#define LOCK_INIT(M) NULL
#define LOCK_DESTROY(M) NULL
#define LOCK_WRLOCK(M) NULL
#define LOCK_RDLOCK(M) NULL
#define LOCK_UNLOCK(M) NULL
#define COMMAND_LOCK NULL
#define BST_LOCK NULL
#define THREAD_CONDITION numberThreads != 1
#endif


int numberThreads = 0;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

#ifdef MUTEX
pthread_mutex_t commandLock;
pthread_mutex_t bstLock;
#elif RWLOCK
pthread_rwlock_t commandLock;
pthread_rwlock_t bstLock;
#endif

static void displayUsage (const char* appName){
    printf(USAGE, appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (long argc, char* const argv[]) {
    if(argc != 4) {
        displayUsage(argv[0]);
        return;
    }
    numberThreads = atoi(argv[3]);
    if( THREAD_CONDITION ) {
        fprintf(stderr,"Error: invalid number of threads\n");
        exit(EXIT_FAILURE);
    }
}

int insertCommand(char* data) {
    if(numberCommands != MAX_COMMANDS) {
        strcpy(inputCommands[numberCommands++], data);
        return 1;
    }
    return 0;
}

char* removeCommand() {
    if(numberCommands > 0){
        numberCommands--;
        return inputCommands[headQueue++];  
    }
    return NULL;
}

void errorParse(){
    fprintf(stderr, "Error: command invalid\n");
    exit(EXIT_FAILURE);
}

/* Lock functions with error messages */

void lockInit(void * lock) {
    if(LOCK_INIT(lock)) {
        fprintf(stderr,"Error: Lock initialization failed\n");
        exit(EXIT_FAILURE);
    }
}

void lockDestroy(void * lock) {
    if(LOCK_DESTROY(lock)) {
        fprintf(stderr, "Error: Lock destruction failed\n");
        exit(EXIT_FAILURE);
    }
}

void lockUnlock(void * lock) {
    if(LOCK_UNLOCK(lock)) {
        fprintf(stderr, "Error: Unable to unlock\n");
        exit(EXIT_FAILURE);
    }
}

void lockRDlock(void * lock) {
    if(LOCK_RDLOCK(lock)) {
        fprintf(stderr, "Error: Unable to lock\n");
        exit(EXIT_FAILURE);
    }
}

void lockWRlock(void * lock) {
    if(LOCK_WRLOCK(lock)) {
        fprintf(stderr, "Error: Unable to lock\n");
    }
}


void processInput(FILE *file){
    char line[MAX_INPUT_SIZE];

    while (fgets(line, MAX_INPUT_SIZE, file)) {
        char token;
        char name[MAX_INPUT_SIZE];

        int numTokens = sscanf(line, "%c %s", &token, name);

        /* perform minimal validation */
        if (numTokens < 1) {
            continue;
        }
        switch (token) {
            case 'c':
            case 'l':
            case 'd':
                if(numTokens != 2)
                    errorParse();
                if(insertCommand(line))
                    break;
                return;
            case '#':
                break;
            default: { /* error */
                errorParse();
            }
        }
    }
}

void * applyCommands(void * arg){
    while(1){

        lockRDlock(COMMAND_LOCK);
        if( numberCommands == 0 ) { 
            lockUnlock(COMMAND_LOCK);
            break; 
        }

        lockUnlock(COMMAND_LOCK);


        lockWRlock(COMMAND_LOCK);

        int iNumber;

        const char* command = removeCommand();

        if( !command )
            return NULL;

        if( *command == 'c' )
            iNumber = obtainNewInumber(fs);
        
        lockUnlock(COMMAND_LOCK);


        char token;
        char name[MAX_INPUT_SIZE];
        int numTokens = sscanf(command, "%c %s", &token, name);
        if (numTokens != 2) {
            fprintf(stderr, "Error: invalid command in Queue\n");
            return NULL;
        }

        int searchResult;
        

        switch (token) {
            case 'c':
                lockWRlock(BST_LOCK);
                create(fs, name, iNumber);
                lockUnlock(BST_LOCK);
                break;
            case 'l':
                lockRDlock(BST_LOCK);
                searchResult = lookup(fs, name);
                lockUnlock(BST_LOCK);

                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);
                
                break;
            case 'd':
                lockWRlock(BST_LOCK);
                delete(fs, name);
                lockUnlock(BST_LOCK);
                break;
            default: { /* error */
                fprintf(stderr, "Error: command to apply\n");
                return NULL;
            }
            
        }
    }
    return NULL;
}




int main(int argc, char* argv[]) {
    struct timeval start, end;
    parseArgs(argc, argv);

    FILE * inputFile = fopen(argv[1],"r");
    FILE * outputFile = fopen(argv[2],"w");

    /*Perform minimal file validation*/
    if( !inputFile ) {
        fprintf(stderr,"Error: Input file non-existant\n");
        exit(EXIT_FAILURE);
    }

    fs = new_tecnicofs();

    processInput(inputFile);
    fclose(inputFile);

    lockInit(COMMAND_LOCK);
    lockInit(BST_LOCK);

    //Start time
    gettimeofday(&start,NULL);
  
    #if defined(MUTEX) || defined(RWLOCK)
    pthread_t tid[numberThreads];

    // Create threads
    for(int x = 0; x < numberThreads; x++) {
        if(pthread_create(tid + x, 0, applyCommands, NULL)) {
            fprintf(stderr, "Error: Thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // Join threads
    for(int x = 0; x < numberThreads; x++){
        if(pthread_join(tid[x],NULL)) {
            fprintf(stderr, "Error: Thread merge failed\n");
            exit(EXIT_FAILURE);
        }
    }
    #else
    applyCommands(NULL);
    #endif

    print_tecnicofs_tree(outputFile, fs);

    //End time
    gettimeofday(&end,NULL);

    lockDestroy(COMMAND_LOCK);
    lockDestroy(BST_LOCK);

    free_tecnicofs(fs);

    fclose(outputFile);

    printf(TIME_MSG,(long double)(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
    
    exit(EXIT_SUCCESS);
}
