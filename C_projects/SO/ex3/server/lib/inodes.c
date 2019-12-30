#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "inodes.h"
#include "../tecnicofs-api-constants.h"

inode_t inode_table[INODE_TABLE_SIZE];
pthread_mutex_t inode_table_lock;

void lock_inode_table(){
    if(pthread_mutex_lock(&inode_table_lock) != 0){
        perror("Failed to acquire the i-node table lock.");
        exit(EXIT_FAILURE);
    }
}

void unlock_inode_table(){
    if(pthread_mutex_unlock(&inode_table_lock) != 0){
        perror("Failed to release the i-node table lock.");
        exit(EXIT_FAILURE);
    }
}

/*
 * Initializes the i-nodes table and the mutex.
 */
void inode_table_init(){
    if(pthread_mutex_init(&inode_table_lock, NULL) != 0){
        perror("Failed to initialize inode table mutex.\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        inode_table[i].owner = FREE_INODE;
        inode_table[i].fileContent = NULL;
    }
}

/*
 * Releases the allocated memory for the i-nodes tables
 * and destroys the mutex.
 */

void inode_table_destroy(){
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        if(inode_table[i].owner!=FREE_INODE && inode_table[i].fileContent)
            free(inode_table[i].fileContent);
    }
    
    if(pthread_mutex_destroy(&inode_table_lock) != 0){
        perror("Failed to destroy inode table mutex.\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Creates a new i-node in the table with the given information.
 * Input:
 *  - owner: uid of the user that created the file
 *  - ownerPerm: permissions of the owner
 *  - othersPerm: permissions of all other users
 * Returns:
 *  inumber: identifier of the new i-node, if successfully created
 *       -1: if an error occurs
 */
int inode_create(uid_t owner, permission ownerPerm, permission othersPerm){
    lock_inode_table();
    for(int inumber = 0; inumber < INODE_TABLE_SIZE; inumber++){
        if(inode_table[inumber].owner == FREE_INODE){
            inode_table[inumber].owner = owner;
            inode_table[inumber].ownerPermissions = ownerPerm;
            inode_table[inumber].othersPermissions = othersPerm;
            inode_table[inumber].fileContent = NULL;
            inode_table[inumber].openCounter = 0;
            unlock_inode_table();
            return inumber;
        }
    }
    unlock_inode_table();
    return -1;
}

/*
 * Deletes the i-node.
 * Input:
 *  - inumber: identifier of the i-node
 * Returns:
 *   0: if successful
 *  -1: if an error occurs
 */
int inode_delete(int inumber){
    lock_inode_table();
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_delete: invalid inumber");
        unlock_inode_table();
        return -1;
    }

    inode_table[inumber].owner = FREE_INODE;
    if(inode_table[inumber].fileContent){
        free(inode_table[inumber].fileContent);
    }
    unlock_inode_table();
    return 0;
}

/*
 * Copies the contents of the i-node into the arguments.
 * Only the fields referenced by non-null arguments are copied.
 * Input:
 *  - inumber: identifier of the i-node
 *  - owner: pointer to uid_t
 *  - ownerPerm: pointer to permission
 *  - othersPerm: pointer to permission
 *  - fileContent: pointer to a char array with size >= len
 * Returns:
 *    len of content read:if successful
 *   -1: if an error occurs
 */
int inode_get(int inumber,uid_t *owner, permission *ownerPerm, permission *othersPerm,
                     char* fileContents, int len){
    lock_inode_table();
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_getValues: invalid inumber %d\n", inumber);
        unlock_inode_table();
        return -1;
    }

    if(len < 0){
        printf("inode_getValues: invalid len %d\n", len);
        unlock_inode_table();
        return -1;
    }

    if(owner)
        *owner = inode_table[inumber].owner;

    if(ownerPerm)
        *ownerPerm = inode_table[inumber].ownerPermissions;

    if(othersPerm)
        *othersPerm = inode_table[inumber].othersPermissions;

    if(fileContents && len > 0 && inode_table[inumber].fileContent){
        strncpy(fileContents, inode_table[inumber].fileContent, len);
        fileContents[len-1] = '\0';
        unlock_inode_table();
        return strlen(fileContents);
    }

    unlock_inode_table();
    return 0;
}


/*
 * Updates the i-node file content.
 * Input:
 *  - inumber: identifier of the i-node
 *  - fileContent: pointer to the string with size >= len
 *  - len: length to copy
 * Returns:
 *    0:if successful
 *   -1: if an error occurs
 */
int inode_set(int inumber, char *fileContents, int len){
    lock_inode_table();
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_setFileContent: invalid inumber");
        unlock_inode_table();
        return -1;
    }

    if(!fileContents || len < 0 || strlen(fileContents) < len){
        printf("inode_setFileContent: \
               fileContents must be non-null && len > 0 && strlen(fileContents) > len");
        unlock_inode_table();
        return -1;
    }
    
    if(inode_table[inumber].fileContent)
        free(inode_table[inumber].fileContent);

    inode_table[inumber].fileContent = malloc(sizeof(char) * (len+1));
    strncpy(inode_table[inumber].fileContent, fileContents, len);
    inode_table[inumber].fileContent[len] = '\0';

    unlock_inode_table();
    return 0;
}

int inode_isOpen(int inumber) {
    lock_inode_table();
    if(inode_table[inumber].owner == FREE_INODE) {
        unlock_inode_table();
        return -1;
    }

    int isOpen = inode_table[inumber].openCounter > 0;
    unlock_inode_table();
    return isOpen;
}

int inode_open(int inumber) {
    lock_inode_table();
    if(inode_table[inumber].owner == FREE_INODE) {
        unlock_inode_table();
        return -1;
    }

    inode_table[inumber].openCounter++;
    unlock_inode_table();
    return 0;
}

int inode_close(int inumber) {
    lock_inode_table();
    if(inode_table[inumber].owner == FREE_INODE) {
        unlock_inode_table();
        return -1;
    }

    inode_table[inumber].openCounter--;
    unlock_inode_table();
    return 0;
}