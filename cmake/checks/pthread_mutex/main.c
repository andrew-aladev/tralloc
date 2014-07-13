#include <pthread.h>

static pthread_mutex_t static_mutex = PTHREAD_MUTEX_INITIALIZER;

int main ()
{
    pthread_mutex_t mutex;
    if ( pthread_mutex_init ( &mutex, NULL ) != 0 ) {
        return 1;
    }
    if (
        pthread_mutex_lock   ( &static_mutex ) != 0 ||
        pthread_mutex_unlock ( &static_mutex ) != 0 ||
        pthread_mutex_lock   ( &mutex ) != 0 ||
        pthread_mutex_unlock ( &mutex ) != 0
    ) {
        return 2;
    }
    if ( pthread_mutex_destroy ( &mutex ) != 0 ) {
        return 3;
    }
    return 0;
}
