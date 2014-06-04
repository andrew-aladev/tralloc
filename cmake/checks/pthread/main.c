#include <pthread.h>

void * thread ( void * argument )
{
    return NULL;
}

int main ()
{
    pthread_t thread_1, thread_2;
    if (
        pthread_create ( &thread_1, NULL, &thread, NULL ) != 0 ||
        pthread_create ( &thread_2, NULL, &thread, NULL ) != 0
    ) {
        return 1;
    }
    if (
        pthread_join ( thread_1, NULL ) != 0 ||
        pthread_join ( thread_2, NULL ) != 0
    ) {
        return 2;
    }
    return 0;
}
