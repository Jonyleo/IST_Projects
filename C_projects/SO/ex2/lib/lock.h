#ifndef LOCK_H
#define LOCK_H
#include "defines.h"


void lockInit(void * lock);
void lockDestroy(void * lock);
void lockRDlock(void * lock);
void lockWRlock(void * lock);
void lockUnlock(void * lock);
void lockTryWR(void * lock_1, void * lock_2);
void lockEndTryLock(void * lock_1, void * lock_2);

#endif