#include <pthread.h>

static pthread_rwlock_t static_rwlock = PTHREAD_RWLOCK_INITIALIZER;

int main ()
{
    pthread_rwlock_t rwlock;
    if ( pthread_rwlock_init ( &rwlock, NULL ) != 0 ) {
        return 1;
    }
    if (
        pthread_rwlock_wrlock ( &static_rwlock ) != 0 ||
        pthread_rwlock_unlock ( &static_rwlock ) != 0 ||
        pthread_rwlock_rdlock ( &static_rwlock ) != 0 ||
        pthread_rwlock_unlock ( &static_rwlock ) != 0 ||
        pthread_rwlock_wrlock ( &rwlock ) != 0 ||
        pthread_rwlock_unlock ( &rwlock ) != 0 ||
        pthread_rwlock_rdlock ( &rwlock ) != 0 ||
        pthread_rwlock_unlock ( &rwlock ) != 0
    ) {
        return 2;
    }
    if ( pthread_rwlock_destroy ( &rwlock ) != 0 ) {
        return 3;
    }
    return 0;
}
