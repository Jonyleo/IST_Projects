#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs.h"
#include "lib/bst.h"
#include "lib/hash.h"
#include "lib/lock.h"
#include "lib/inodes.h"

static void errorParse(int test, char * errMsg) {
    if(!test) {
        return;
    }
    perror(errMsg);
    exit(EXIT_FAILURE);
}

tecnicofs* new_tecnicofs(int numBuckets){
	tecnicofs*fs = malloc(sizeof(tecnicofs));
	errorParse(!fs, "failed to allocate tecnicofs");
	fs->bstHashTable = malloc(sizeof(node*) * numBuckets);
	errorParse(!fs->bstHashTable, "failed to allocate hashtable");

	fs->numBuckets = numBuckets;
	fsLockInit(fs);

	for( int x = 0; x < numBuckets; x++)
		fs->bstHashTable[x] = NULL;
	return fs;
}

void free_tecnicofs(tecnicofs* fs) {
	for(int x = 0; x < fs->numBuckets; x++)
		free_tree(fs->bstHashTable[x]);
	free(fs->bstHashTable);
	fsLockDestroy(fs);
	free(fs);
}

int lookup(tecnicofs* fs, char *name, int hashIndex){
	node* searchNode = search(fs->bstHashTable[hashIndex], name);

	if ( searchNode ) return searchNode->inumber;
	return -1;
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	for( int x = 0; x < fs->numBuckets; x++ )
		print_tree(fp, fs->bstHashTable[x]);
}


void fsLockInit(tecnicofs * fs) {
	fs->lock = malloc(sizeof(pthread_rwlock_t*) * fs->numBuckets);

	errorParse(!fs->lock, "Error: Unable to allocate locks");

    for( int x = 0; x < fs->numBuckets; x++ ) {
    	fs->lock[x] = malloc(sizeof(pthread_rwlock_t));
    	errorParse(!fs->lock[x], "Error: Unable to allocate lock");
        lockInit(fs->lock[x]);
    }
}

void fsLockDestroy(tecnicofs * fs) {
	for( int x = 0; x < fs->numBuckets; x++) {
        lockDestroy(fs->lock[x]);
        free(fs->lock[x]);
    }
    free(fs->lock);
}
