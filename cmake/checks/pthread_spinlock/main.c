#include <pthread.h>
#include <stdlib.h>

static pthread_spinlock_t spinlock;

__attribute__((constructor))
void lock_constructor () {
    if ( pthread_spin_init ( &spinlock, 0 ) != 0 ) {
        exit ( 1 );
    }
}

int main () {
    if (
        pthread_spin_lock   ( &spinlock ) != 0 ||
        pthread_spin_unlock ( &spinlock ) != 0
    ) {
        return 2;
    }
    return 0;
}

__attribute__((destructor))
void lock_destructor () {
    if ( pthread_spin_destroy ( &spinlock ) != 0 ) {
        exit ( 3 );
    }
}
