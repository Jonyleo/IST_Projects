#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs.h"
#include "lib/bst.h"
#include "lib/hash.h"
#include "lib/lock.h"

int obtainNewInumber(tecnicofs* fs) {
	int newInumber = ++(fs->nextINumber);
	return newInumber;
}

tecnicofs* new_tecnicofs(int numBuckets){
	tecnicofs*fs = malloc(sizeof(tecnicofs));
	if (!fs) {
		perror("failed to allocate tecnicofs");
		exit(EXIT_FAILURE);
	}
	fs->bstHashTable = malloc(sizeof(node*) * numBuckets);

	if(!fs->bstHashTable) {
		perror("failed to allocate hashtable");
		exit(EXIT_FAILURE);
	}
	fs->nextINumber = 0;
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

void create(tecnicofs* fs, char *name, int inumber){
	int hashIndex = hash(name, fs->numBuckets);

	lockWRlock(BST_LOCK(hashIndex));
	fs->bstHashTable[hashIndex] = insert(fs->bstHashTable[hashIndex], name, inumber);
	lockUnlock(BST_LOCK(hashIndex));
}

void delete(tecnicofs* fs, char *name){
	int hashIndex = hash(name, fs->numBuckets);

	lockWRlock(BST_LOCK(hashIndex));
	fs->bstHashTable[hashIndex] = remove_item(fs->bstHashTable[hashIndex], name);
	lockUnlock(BST_LOCK(hashIndex));
}

int lookup(tecnicofs* fs, char *name){
	int hashIndex = hash(name, fs->numBuckets);

	lockRDlock(BST_LOCK(hashIndex));
	node* searchNode = search(fs->bstHashTable[hashIndex], name);
	lockUnlock(BST_LOCK(hashIndex));

	if ( searchNode ) return searchNode->inumber;
	return 0;
}

int renameFile(tecnicofs* fs, char *name, char *rename) {
	int hashOld = hash(name,fs->numBuckets);
	int hashNew = hash(rename, fs->numBuckets);

	lockTryWR(BST_LOCK(hashOld), BST_LOCK(hashNew));

	node* searchNode = search(fs->bstHashTable[hashOld], name);
	if(!searchNode || search(fs->bstHashTable[hashNew], rename)) {
		lockEndTryLock(BST_LOCK(hashOld),BST_LOCK(hashNew));
		return 0;
	}
	
	int inumber = searchNode->inumber;

	fs->bstHashTable[hashOld] = remove_item(fs->bstHashTable[hashOld], name);
	fs->bstHashTable[hashNew] = insert(fs->bstHashTable[hashNew], rename, inumber);
	lockEndTryLock(BST_LOCK(hashOld),BST_LOCK(hashNew));

	return 1;
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs){
	for( int x = 0; x < fs->numBuckets; x++ )
		print_tree(fp, fs->bstHashTable[x]);
}


void fsLockInit(tecnicofs * fs) {
	#if defined(MUTEX) || defined(RWLOCK)
	fs->lock = malloc(sizeof(LOCK_TYPE*) * fs->numBuckets);

	if( !fs->lock ) {
        fprintf(stderr,"Error: Unable to allocate locks\n");
        exit(EXIT_FAILURE);
    }

    for( int x = 0; x < fs->numBuckets; x++ ) {
    	BST_LOCK(x) = malloc(sizeof(LOCK_TYPE));

    	if(!BST_LOCK(x)) {
    		fprintf(stderr, "Error: Unable to allocate lock\n");
    		exit(EXIT_FAILURE);
    	}

        lockInit(BST_LOCK(x));
    }
    #endif

}

void fsLockDestroy(tecnicofs * fs) {
	#if defined(MUTEX) || defined(RWLOCK)
	for( int x = 0; x < fs->numBuckets; x++) {
        lockDestroy(BST_LOCK(x));
        free(BST_LOCK(x));
    }
    free(fs->lock);
    #endif
}
