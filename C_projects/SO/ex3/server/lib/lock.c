#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void lockInit(void * lock) {
    if(pthread_rwlock_init(lock, NULL)) {
        fprintf(stderr,"Error: Lock initialization failed\n");
        exit(EXIT_FAILURE);
    }
}

void lockDestroy(void * lock) {
    if(pthread_rwlock_destroy(lock)) {
        fprintf(stderr, "Error: Lock destruction failed\n");
    	exit(EXIT_FAILURE);
    }
}

void lockRDlock(void * lock) {
	if(pthread_rwlock_rdlock(lock)) {
        fprintf(stderr, "Error: Unable to lock\n");
        exit(EXIT_FAILURE);
    }
}

void lockWRlock(void * lock) {
	if(pthread_rwlock_wrlock(lock)) {
        fprintf(stderr, "Error: Unable to lock\n");
    	exit(EXIT_FAILURE);
    }
}

void lockUnlock(void * lock) {
	if(pthread_rwlock_unlock(lock)) {
        fprintf(stderr, "Error: Unable to unlock\n");
        exit(EXIT_FAILURE);
    }
}

void lockTryWR(void * lock_1, void * lock_2) {
    int didLock;

    if(lock_1 == lock_2) {
        lockWRlock(lock_1);
        return;
    }
    
    while(1) {
        lockWRlock(lock_1);

        didLock = pthread_rwlock_trywrlock(lock_2);
        
        switch(didLock) {
            case EBUSY:
                lockUnlock(lock_1);
                usleep(rand() % 1000);
                break;
            case 0:
                return;
            default:
                fprintf(stderr, "Error: Unable to try_lock\n");
                exit(EXIT_FAILURE);
        }
    }
}

void lockEndTryLock(void * lock_1, void * lock_2) {
    if(lock_1 != lock_2) {
        lockUnlock(lock_2);
    }
    lockUnlock(lock_1);
}