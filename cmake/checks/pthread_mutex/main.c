#include <pthread.h>

int main ()
{
    pthread_mutex_t mutex;
    if (
        pthread_mutex_init    ( &mutex, NULL ) != 0 ||
        pthread_mutex_lock    ( &mutex ) != 0 ||
        pthread_mutex_unlock  ( &mutex ) != 0 ||
        pthread_mutex_destroy ( &mutex ) != 0
    ) {
        return 1;
    }
    return 0;
}
