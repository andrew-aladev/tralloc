#include <pthread.h>

int main ()
{
    pthread_spinlock_t spinlock;
    if (
        pthread_spin_init    ( &spinlock, NULL ) != 0 ||
        pthread_spin_lock    ( &spinlock ) != 0 ||
        pthread_spin_unlock  ( &spinlock ) != 0 ||
        pthread_spin_destroy ( &spinlock ) != 0
    ) {
        return 1;
    }
    return 0;
}
