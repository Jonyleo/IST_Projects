#ifndef FS_H
#define FS_H

#include <pthread.h>
#include "lib/bst.h"
#include "lib/defines.h"

typedef struct tecnicofs {
    node** bstHashTable;
    int nextINumber;
    int numBuckets;
    LOCK_VARIABLE;
} tecnicofs;



tecnicofs* fs;

int obtainNewInumber(tecnicofs* fs);
tecnicofs* new_tecnicofs();
void free_tecnicofs(tecnicofs* fs);
void create(tecnicofs* fs, char *name, int inumber);
void delete(tecnicofs* fs, char *name);
int lookup(tecnicofs* fs, char *name);
int renameFile(tecnicofs* fs, char *name, char *rename);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs);
void fsLockInit(tecnicofs * fs);
void fsLockDestroy(tecnicofs * fs);

#endif /* FS_H */
