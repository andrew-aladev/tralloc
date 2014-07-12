#include <pthread.h>

int main ()
{
    pthread_rwlock_t rwlock;
    if (
        pthread_rwlock_init    ( &rwlock, NULL ) != 0 ||
        pthread_rwlock_wrlock  ( &rwlock ) != 0 ||
        pthread_rwlock_unlock  ( &rwlock ) != 0 ||
        pthread_rwlock_rdlock  ( &rwlock ) != 0 ||
        pthread_rwlock_unlock  ( &rwlock ) != 0 ||
        pthread_rwlock_destroy ( &rwlock ) != 0
    ) {
        return 1;
    }
    return 0;
}
