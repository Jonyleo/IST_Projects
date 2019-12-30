#ifndef SEM_H
#define SEM_H
#include <semaphore.h>

void semInit(sem_t * sem, int value);
void semDestroy(sem_t * sem);
void semEnter(sem_t * sem);
void semLeave(sem_t * sem);

#endif