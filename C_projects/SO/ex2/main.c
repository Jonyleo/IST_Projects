#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#include "lib/defines.h"
#include "lib/lock.h"
#include "lib/sem.h"
#include "fs.h"


#define MAX_COMMANDS 10
#define MAX_INPUT_SIZE 100

#define USAGE "Usage: %s inputfile outputfile numthreads numbuckets\n"
#define TIME_MSG "TecnicoFS completed in [%.4Lf] seconds.\n"

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int conptr = 0;
int proptr = 0;

#ifdef MUTEX
pthread_mutex_t lockConsumidor;
#elif RWLOCK
pthread_rwlock_t lockConsumidor;
#endif

sem_t semProdutor;
sem_t semConsumidor;

void errorParse(int test, char * errMsg) {
    if(!test) {
        return;
    }
    fputs(errMsg, stderr);
    exit(EXIT_FAILURE);
}

static void displayUsage (const char* appName){
    printf(USAGE, appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (int argc, char* const argv[]) {
    if(argc != 5) {
        displayUsage(argv[0]);
    }
    
    errorParse(THREAD_CONDITION((atoi(argv[3]))), "Error: invalid number of threads\n");
    errorParse(atoi(argv[4]) <= 0, "Error: invalid number of buckets\n");
}

int insertCommand(char* data) {
    semEnter(&semProdutor);

    strncpy(inputCommands[proptr], data, MAX_INPUT_SIZE);
    proptr = (proptr + 1) % MAX_COMMANDS;

    semLeave(&semConsumidor);

    return 1;
}

int removeCommand(char command[]) {
    if(!inputCommands[conptr])
        return -1;

    strncpy(command, inputCommands[conptr], MAX_INPUT_SIZE);

	if(*command != 'q')
		conptr = (conptr + 1) % MAX_COMMANDS;
	else
		semLeave(&semConsumidor);
    
    return 0;  
}


void * processInput(void * file){
    char line[MAX_INPUT_SIZE];
    file = (FILE *) file;
    while (fgets(line, MAX_INPUT_SIZE, file)) {
        char token;
        char name[MAX_INPUT_SIZE];

        int numTokens = sscanf(line, "%c %s %s", &token, name, name);

        /* perform minimal validation */
        if (numTokens < 1) {
            continue;
        }
        switch (token) {
            case 'c':
            case 'l':
            case 'd':
                errorParse(numTokens != 2, "Error: command invalid\n");
                if(insertCommand(line))
                    break;
                return NULL;
            case 'r':
                errorParse(numTokens != 3, "Error: command invalid\n");                
                if(insertCommand(line))
                    break;
                return NULL;
            case '#':
                break;
            default: { /* error */
                errorParse(1,"Error: command invalid\n");
            }
        }
    }
    insertCommand("q\n");

    return NULL;
}

void * applyCommands(void * arg){
    while(1){
        int iNumber;

        semEnter(&semConsumidor);
        lockWRlock(LOCK_CONSUMIDOR);

        char command[MAX_INPUT_SIZE];

        if( removeCommand(command) == -1 ) {
        	lockUnlock(LOCK_CONSUMIDOR);
            continue;
        }

        if( *command == 'c' )
            iNumber = obtainNewInumber(fs);
        
        lockUnlock(LOCK_CONSUMIDOR);
        semLeave(&semProdutor);

        char token;
        char name[MAX_INPUT_SIZE];
        char rename[MAX_INPUT_SIZE];
        int numTokens = sscanf(command, "%c %s %s", &token, name, rename);

        int searchResult;
        switch (token) {
            case 'c':
                errorParse(numTokens != 2, "Error: invalid command in Queue\n");
                create(fs, name, iNumber);
                break;
            case 'l':
                errorParse(numTokens != 2, "Error: invalid command in Queue\n");
                searchResult = lookup(fs, name);

                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);
                
                break;
            case 'd':
                errorParse(numTokens != 2, "Error: invalid command in Queue\n");
                delete(fs, name);
                break;
            case 'r':
                errorParse(numTokens != 3, "Error: invalid command in Queue\n");
            	renameFile(fs, name, rename);
            	break;
            case 'q':
                errorParse(numTokens != 1, "Error: invalid command in Queue\n");
            	return NULL;
            default: { /* error */
                errorParse(1, "Error: command to apply\n");
                return NULL;
            }
            
        }
    }
    return NULL;
}

static void runthreads(FILE * inputFile, int numberThreads) {
    pthread_t tid[numberThreads+1];

    // Create threads
    for(int x = 0; x < numberThreads; x++) {
    	errorParse(pthread_create(tid + x + 1, 0, applyCommands, NULL),"Error: Thread creation failed\n");
    }

    errorParse(pthread_create(tid, 0, processInput, inputFile),"Error: Thread creation failed\n");
    
    // Join threads
    for(int x = 0; x < numberThreads + 1; x++) {
        errorParse(pthread_join(tid[x],NULL),"Error: Thread merge failed\n");
    }

}

int main(int argc, char* argv[]) {
    struct timeval start, end;
    parseArgs(argc, argv);

    FILE * inputFile = fopen(argv[1],"r");
    FILE * outputFile = fopen(argv[2],"w");

    /*Perform minimal file validation*/
    errorParse(inputFile == NULL,"Error: Input file non-existant\n");

    fs = new_tecnicofs(atoi(argv[4]));  

    lockInit(LOCK_CONSUMIDOR);
    semInit(&semProdutor, MAX_COMMANDS);
    semInit(&semConsumidor, 0);
    
   	//Start time
   	errorParse(gettimeofday(&start,NULL),"Error: Checking time failed\n");
      
    runthreads(inputFile, atoi(argv[3]));

    print_tecnicofs_tree(outputFile, fs);

    //End time
    errorParse(gettimeofday(&end,NULL),"Error: Checking time failed\n");

    errorParse(fclose(inputFile),"Error: Closing file failed\n");

    lockDestroy(LOCK_CONSUMIDOR);
    semDestroy(&semProdutor);
    semDestroy(&semConsumidor);

    free_tecnicofs(fs);

    errorParse(fflush(outputFile),"Error: Flushing file failed\n");
    errorParse(fclose(outputFile),"Error: Closing file failed\n");

    printf(TIME_MSG,(long double)(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);
    
    exit(EXIT_SUCCESS);
}
