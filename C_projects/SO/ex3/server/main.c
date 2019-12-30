#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "lib/bst.h"
#include "lib/lock.h"
#include "lib/inodes.h"
#include "lib/hash.h"
#include "fs.h"

#define MAX_COMMANDS 10
#define MAX_COMMAND_SIZE 100
#define MAX_OPEN_FILES 5

#define USAGE "Usage: %s socketname outputfile numbuckets\n"
#define TIME_MSG "TecnicoFS completed in [%.4Lf] seconds.\n"

#define AC_BREAK_CONDITION(C, O, R) if(C) {O; R; break;}

int END_SIGINT = 0;
int serversockfd = -1;

static void errorParse(int test, char * errMsg) {
    if(!test) {
        return;
    }
    fputs(errMsg, stderr);
    exit(EXIT_FAILURE);
}

static void perrorParse(int test, char * errMsg) {
    if(!test) {
        return;
    }
    perror(errMsg);
    exit(EXIT_FAILURE);
}

static void pthread_perrorParse(int test, char * errMsg) {
    if(!test) {
        return;
    }
    perror(errMsg);
    pthread_exit(NULL);
}

static void displayUsage (const char* appName){
    printf(USAGE, appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (int argc, char* const argv[]) {
    if(argc != 4) {
        displayUsage(argv[0]);
    }
    errorParse(atoi(argv[3]) <= 0, "Error: invalid number of buckets");
}


int getCommand(int sockfd, char * line) {
    int i = read(sockfd, line, MAX_COMMAND_SIZE);
    pthread_perrorParse(i < 0, "Error: reading command from client");
    line[i] = '\0';
    return i;
}

void returnBuffer(int sockfd, char *buffer, int len) {
    pthread_perrorParse(write(sockfd, buffer, len) != len,"Error: returning buffer to client");
}

void returnStatus(int sockfd, int statusCode) {
    pthread_perrorParse(write(sockfd, &statusCode, sizeof(int)) != sizeof(int), "Error: returning status to client");
}

uid_t getClientId(int sockfd) {
    struct ucred creds;
    unsigned int ucredLen = sizeof(creds);
    if(getsockopt(sockfd, SOL_SOCKET, SO_PEERCRED, &creds, &ucredLen) < 0)
        return -1;
    return creds.uid;
}

int checkPerms(permission action, permission perms) {
    if(perms == RW)
        return 0;

    if(perms == action)
        return 0;

    return -1;
}

void *applyCommands(void *sockfd_addr) {
    sigset_t set;
    pthread_perrorParse(sigemptyset(&set), "Error: sigemptyset failed");
    pthread_perrorParse(sigaddset(&set, SIGINT), "Error: sigaddset failed");

    pthread_perrorParse(sigprocmask(SIG_BLOCK, &set, NULL), "Error: sigprocmask failed");
    
    int sockfd = *((int*) sockfd_addr), running = 1, fileds;
    free((int *) sockfd_addr);

    uid_t clientID;
    pthread_perrorParse((clientID = getClientId(sockfd)) < 0, "Error: failed to get client id");

    int openFiles[MAX_OPEN_FILES][2], currFile; /*2 => file inumber and mode*/
    for(currFile = 0; currFile < MAX_OPEN_FILES; currFile++)
        openFiles[currFile][0] = -1; /*Represents no file*/


    char command[MAX_COMMAND_SIZE];
    char token;
    char input1[MAX_COMMAND_SIZE];
    char input2[MAX_COMMAND_SIZE];

    while(running){

        getCommand(sockfd, command);
        int numTokens = sscanf(command, "%c %s %[^\n]", &token, input1, input2);

        int inumber, hashIndex;
        permission ownerPerm, otherPerm;
        uid_t owner;        

        switch (token) {
            case 'c':
                if(numTokens != 3) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }
                
                hashIndex = hash(input1, fs->numBuckets);

                lockWRlock(fs->lock[hashIndex]);

                ownerPerm = input2[0] - '0';
                otherPerm = input2[1] - '0';

                AC_BREAK_CONDITION(ownerPerm > 3 || ownerPerm < 0 || otherPerm > 3 || otherPerm < 0, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_OTHER))

                AC_BREAK_CONDITION(lookup(fs, input1, hashIndex) != -1, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_ALREADY_EXISTS))

                inumber = inode_create(clientID, ownerPerm, otherPerm);

                AC_BREAK_CONDITION(inumber == -1, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_OTHER))
                
                fs->bstHashTable[hashIndex] = insert(fs->bstHashTable[hashIndex], input1, inumber);

                lockUnlock(fs->lock[hashIndex]);
                
                returnStatus(sockfd, 0);
                break;
            
            case 'd':
                if(numTokens != 2) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }

                hashIndex = hash(input1, fs->numBuckets);

                lockWRlock(fs->lock[hashIndex]);

                inumber = lookup(fs, input1, hashIndex);
                
                AC_BREAK_CONDITION(inumber == -1, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_NOT_FOUND))

                inode_get(inumber, &owner, NULL, NULL, NULL, 0);

                AC_BREAK_CONDITION(owner != clientID, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_PERMISSION_DENIED))

                AC_BREAK_CONDITION(inode_isOpen(inumber), lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_IS_OPEN))

                inode_delete(inumber);
                fs->bstHashTable[hashIndex] = remove_item(fs->bstHashTable[hashIndex], input1); 

                lockUnlock(fs->lock[hashIndex]);

                returnStatus(sockfd, 0);
                break;

            case 'r':
                if(numTokens != 3) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }

                int hashOld = hash(input1, fs->numBuckets);
                int hashNew = hash(input2, fs->numBuckets);

                lockTryWR(fs->lock[hashNew], fs->lock[hashOld]);

                inumber = lookup(fs, input1, hashOld);
                
                AC_BREAK_CONDITION(inumber == -1, lockEndTryLock(fs->lock[hashNew], fs->lock[hashOld]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_NOT_FOUND))

                AC_BREAK_CONDITION(lookup(fs, input2, hashNew) != -1, lockEndTryLock(fs->lock[hashNew], fs->lock[hashOld]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_ALREADY_EXISTS))

                inode_get(inumber, &owner, NULL, NULL, NULL, 0);

                AC_BREAK_CONDITION(owner != clientID, lockEndTryLock(fs->lock[hashNew], fs->lock[hashOld]), returnStatus(sockfd, TECNICOFS_ERROR_PERMISSION_DENIED))

                AC_BREAK_CONDITION(inode_isOpen(inumber), lockEndTryLock(fs->lock[hashNew], fs->lock[hashOld]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_IS_OPEN))

                fs->bstHashTable[hashOld] = remove_item(fs->bstHashTable[hashOld], input1);
                fs->bstHashTable[hashNew] = insert(fs->bstHashTable[hashNew], input2, inumber);

                lockEndTryLock(fs->lock[hashNew], fs->lock[hashOld]);

                returnStatus(sockfd, 0);
                break;

            case 'o':
                if(numTokens != 3) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }
            
                hashIndex = hash(input1, fs->numBuckets);

                lockWRlock(fs->lock[hashIndex]);

                int mode = atoi(input2);

                AC_BREAK_CONDITION(mode > 3 || mode < 0, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_OTHER))

                inumber = lookup(fs, input1, hashIndex);

                AC_BREAK_CONDITION(inumber == -1, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_NOT_FOUND))
                
                inode_get(inumber, &owner, &ownerPerm, &otherPerm, NULL, 0);

                int permResult = (owner == clientID ? checkPerms(mode, ownerPerm) : checkPerms(mode, otherPerm));
                AC_BREAK_CONDITION(permResult == -1, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_PERMISSION_DENIED))

                for(currFile = 0; currFile < MAX_OPEN_FILES; currFile++) {
                    if(openFiles[currFile][0] == -1) {
                        openFiles[currFile][0] = inumber;
                        openFiles[currFile][1] = mode;
                        inode_open(inumber);
                        break;
                    }

                    if(openFiles[currFile][0] == inumber) {
                        currFile = -1;
                        break;
                    }             
                }


                AC_BREAK_CONDITION(currFile == -1, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_FILE_IS_OPEN))

                AC_BREAK_CONDITION(currFile == MAX_OPEN_FILES, lockUnlock(fs->lock[hashIndex]), returnStatus(sockfd, TECNICOFS_ERROR_MAXED_OPEN_FILES))

                lockUnlock(fs->lock[hashIndex]);
                returnStatus(sockfd, currFile);
                break;
            case 'x':
                if(numTokens != 2) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }

                fileds = atoi(input1);

                AC_BREAK_CONDITION(fileds < 0 || fileds > MAX_OPEN_FILES, NULL, returnStatus(sockfd, TECNICOFS_ERROR_OTHER))

                inumber = openFiles[fileds][0];

                AC_BREAK_CONDITION(inumber == -1, NULL, returnStatus(sockfd, TECNICOFS_ERROR_FILE_NOT_OPEN))

                inode_close(inumber);
                openFiles[fileds][0] = -1;
                
                returnStatus(sockfd, 0);
                break;
            case 'l':
                if(numTokens != 3) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }
                fileds = atoi(input1);
                int len = atoi(input2);

                AC_BREAK_CONDITION(fileds < 0 || fileds > MAX_OPEN_FILES, NULL, returnStatus(sockfd, TECNICOFS_ERROR_OTHER))

                inumber = openFiles[fileds][0];

                AC_BREAK_CONDITION(len < 0, NULL, returnStatus(sockfd, TECNICOFS_ERROR_OTHER))

                AC_BREAK_CONDITION(inumber == -1, NULL, returnStatus(sockfd, TECNICOFS_ERROR_FILE_NOT_OPEN))

                AC_BREAK_CONDITION(checkPerms(READ, openFiles[fileds][1]) == -1, NULL, returnStatus(sockfd, TECNICOFS_ERROR_INVALID_MODE))

                char fileContent[MAX_COMMAND_SIZE];
                int n = inode_get(inumber, NULL, NULL, NULL, fileContent, len);
                
                if(n < 0)
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                returnStatus(sockfd, n);

                if(n > 0) returnBuffer(sockfd, fileContent, n);
                break;
            case 'w':
                if(numTokens != 3 && numTokens != 2) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }
                fileds = atoi(input1);

                AC_BREAK_CONDITION(fileds < 0 || fileds > MAX_OPEN_FILES, NULL, returnStatus(sockfd, TECNICOFS_ERROR_OTHER))

                inumber = openFiles[fileds][0];

                AC_BREAK_CONDITION(inumber == -1, NULL, returnStatus(sockfd, TECNICOFS_ERROR_FILE_NOT_OPEN))

                AC_BREAK_CONDITION(checkPerms(WRITE, openFiles[fileds][1]) == -1, NULL, returnStatus(sockfd, TECNICOFS_ERROR_INVALID_MODE))


                if(numTokens == 3)
                    inode_set(inumber, input2, strlen(input2));
                else
                    inode_set(inumber, "", 0);  // 2 tokens => write vazio

                returnStatus(sockfd, 0);
                break;
            case 'q':
                if(numTokens != 1) {
                    returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                    break;
                }

                for(currFile = 0; currFile < MAX_OPEN_FILES; currFile++) {
                    if(openFiles[currFile][0] != -1) {
                        inode_close(openFiles[currFile][0]);
                        openFiles[currFile][0] = -1;
                    }
                }

                returnStatus(sockfd, 0);
                close(sockfd);
                running = 0;
                break;
            default: { /* error */
                returnStatus(sockfd, TECNICOFS_ERROR_OTHER);
                break;
            }
        }
    }
    return NULL;
}

void handleSIGINT(int sig) {
    END_SIGINT = 1;
    close(serversockfd);
}

void endProgram(char *socketPath, int currThread, pthread_t *tid, 
                struct timeval start, struct timeval end, FILE * outputFile) {

    if(serversockfd != -1)
        unlink(socketPath);
    
    for(int x = 0; x < currThread; x++)
        perrorParse(pthread_join(tid[x], NULL),"Error: Thread merge failed");

    print_tecnicofs_tree(outputFile, fs);

    //End time
    perrorParse(gettimeofday(&end,NULL),"Error: Checking time failed");

    printf(TIME_MSG,(long double)(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6);

    free(tid);

    free_tecnicofs(fs);
    inode_table_destroy();

    perrorParse(fflush(outputFile),"Error: Flushing file failed");
    perrorParse(fclose(outputFile),"Error: Closing file failed");
}

int serverMount(char *socketPath) {
    int servlen, sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un serv_addr;
    
    perrorParse(sockfd < 0, "Error: can't open stream socket");

    unlink(socketPath);

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, socketPath);

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    perrorParse(bind(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0, "Error: can't bind local address");

    perrorParse(listen(sockfd, 5) < 0, "Error: listen failed");

    return sockfd;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, &handleSIGINT);

    parseArgs(argc, argv);

    struct timeval start, end;
    int *newsockfd_addr, newsockfd;

    int currThread = 0;
    int sizeOfTid = 10;
    pthread_t *tid = malloc(sizeof(pthread_t) * sizeOfTid);
    perrorParse(tid == NULL, "Error: allocating tid array failed");

    FILE *outputFile = fopen(argv[2],"w");

    perrorParse(outputFile == NULL, "Error: opening output file");  

    fs = new_tecnicofs(atoi(argv[3]));
    inode_table_init(); 
    
    serversockfd = serverMount(argv[1]);

    struct sockaddr_un cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    
    //Start time
    perrorParse(gettimeofday(&start,NULL),"Error: Checking time failed");

    for(currThread = 0; !END_SIGINT ;currThread++) {
        if(currThread == sizeOfTid) {
            sizeOfTid *=2;
            tid = realloc(tid, sizeof(pthread_t) * sizeOfTid);
            perrorParse(tid == NULL, "Error: reallocing tid array failed");
        }

        newsockfd = accept(serversockfd, (struct sockaddr *) &cli_addr, &clilen);

        if((errno == EBADF || errno == EINTR) && END_SIGINT == 1)
            break;
        
        perrorParse(newsockfd < 0, "Error: accept error");

        newsockfd_addr = malloc(sizeof(int));
        perrorParse(newsockfd_addr == NULL, "Error: allocing sockfd for thread");
        *newsockfd_addr = newsockfd;

        perrorParse(pthread_create(tid + currThread, 0, applyCommands, (void *) newsockfd_addr),"Error: Thread creation failed");           
    }

    endProgram(argv[1], currThread, tid, start, end, outputFile);
    exit(EXIT_SUCCESS);
}
