#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "defines.h"

void lockInit(void * lock) {
	#if defined(MUTEX) || defined(RWLOCK)
    if(LOCK_INIT(lock)) {
        fprintf(stderr,"Error: Lock initialization failed\n");
        exit(EXIT_FAILURE);
    }
    #endif
}

void lockDestroy(void * lock) {
	#if defined(MUTEX) || defined(RWLOCK)
    int e;
    if((e = LOCK_DESTROY(lock))) {
        fprintf(stderr, "Error: Lock destruction failed\n");
    	exit(EXIT_FAILURE);
    }
    #endif
}

void lockRDlock(void * lock) {
	#if defined(MUTEX) || defined(RWLOCK)
	if(LOCK_RDLOCK(lock)) {
        fprintf(stderr, "Error: Unable to lock\n");
        exit(EXIT_FAILURE);
    }
    #endif
}

void lockWRlock(void * lock) {
	#if defined(MUTEX) || defined(RWLOCK)
	if(LOCK_WRLOCK(lock)) {
        fprintf(stderr, "Error: Unable to lock\n");
    	exit(EXIT_FAILURE);
    }
    #endif
}

void lockUnlock(void * lock) {
	#if defined(MUTEX) || defined(RWLOCK)
	if(LOCK_UNLOCK(lock)) {
        fprintf(stderr, "Error: Unable to unlock\n");
        exit(EXIT_FAILURE);
    }
    #endif
}

void lockTryWR(void * lock_1, void * lock_2) {
    #if defined(MUTEX) || defined(RWLOCK)
    int didLock, r;

    if(lock_1 == lock_2) {
        lockWRlock(lock_1);
        return;
    }
    
    while(1) {
        lockWRlock(lock_1);

        didLock = LOCK_TRYWR(lock_2);
        switch(didLock) {
            case EBUSY:
                lockUnlock(lock_1);                
                r = rand() % 10000;
                usleep(r);
                break;
            case 0:
                return;
            default:
                fprintf(stderr, "Error: Unable to try_lock\n");
                exit(EXIT_FAILURE);
        }
    }
    #endif
}

void lockEndTryLock(void * lock_1, void * lock_2) {
    #if defined(MUTEX) || defined(RWLOCK)
    if(lock_1 != lock_2) {
        lockUnlock(lock_2);
    }
    lockUnlock(lock_1);
    #endif
}