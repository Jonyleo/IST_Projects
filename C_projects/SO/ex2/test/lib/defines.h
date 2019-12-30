#ifndef DEFINES_H
#define DEFINES_H

#ifdef MUTEX
#define LOCK_INIT(M) pthread_mutex_init(M, NULL)
#define LOCK_DESTROY(M) pthread_mutex_destroy(M)
#define LOCK_WRLOCK(M) pthread_mutex_lock(M)
#define LOCK_RDLOCK(M) LOCK_WRLOCK(M)
#define LOCK_UNLOCK(M) pthread_mutex_unlock(M)
#define LOCK_TRYWR(M) pthread_mutex_trylock(M)
#define LOCK_CONSUMIDOR &lockConsumidor
#define BST_LOCK(I) fs->lock[I]
#define THREAD_CONDITION(C) C < 1
#define LOCK_VARIABLE pthread_mutex_t** lock
#define LOCK_TYPE pthread_mutex_t

#elif RWLOCK
#define LOCK_INIT(M) pthread_rwlock_init(M, NULL)
#define LOCK_DESTROY(M) pthread_rwlock_destroy(M)
#define LOCK_WRLOCK(M) pthread_rwlock_wrlock(M)
#define LOCK_RDLOCK(M) pthread_rwlock_rdlock(M)
#define LOCK_UNLOCK(M) pthread_rwlock_unlock(M)
#define LOCK_TRYWR(M) pthread_rwlock_trywrlock(M)
#define LOCK_CONSUMIDOR &lockConsumidor
#define BST_LOCK(I) fs->lock[I]
#define THREAD_CONDITION(C) C < 1
#define LOCK_VARIABLE pthread_rwlock_t** lock
#define LOCK_TYPE pthread_rwlock_t

#else
#define LOCK_PRODUTOR NULL
#define LOCK_CONSUMIDOR NULL
#define LOCK_PROCON NULL
#define BST_LOCK(I) NULL
#define THREAD_CONDITION(C) C != 1
#define LOCK_VARIABLE
#endif
#endif

