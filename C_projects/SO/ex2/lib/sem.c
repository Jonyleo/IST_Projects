#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>


void semInit(sem_t * sem, int value) {
	if( sem_init(sem, 1, value) ){
		fprintf(stderr, "Unable to initialize semaphore\n");
		exit(EXIT_FAILURE);
	}
}

void semDestroy(sem_t * sem) {
	if( sem_destroy(sem) ){
		fprintf(stderr, "Unable to destroy semaphore\n");
		exit(EXIT_FAILURE);
	}
}

void semEnter(sem_t * sem) {
	if( sem_wait(sem) ) {
		fprintf(stderr, "Unable to enter semaphore\n");
		exit(EXIT_FAILURE);
	}
}

void semLeave(sem_t * sem) {
	if( sem_post(sem) ) {
		fprintf(stderr, "Unable to enter semaphore\n");
		exit(EXIT_FAILURE);
	}
}