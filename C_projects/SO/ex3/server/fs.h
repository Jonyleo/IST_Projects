#ifndef FS_H
#define FS_H

#include <pthread.h>
#include "lib/bst.h"

typedef struct tecnicofs {
    node** bstHashTable;
    int numBuckets;
    pthread_rwlock_t** lock;
} tecnicofs;

tecnicofs* fs;

static void errorParse(int test, char * errMsg);
tecnicofs* new_tecnicofs();
void free_tecnicofs(tecnicofs* fs);
int lookup(tecnicofs* fs, char *name, int hashIndex);
int renameFile(tecnicofs* fs, char *name, char *rename);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);
void fsLockInit(tecnicofs * fs);
void fsLockDestroy(tecnicofs * fs);

#endif /* FS_H */
