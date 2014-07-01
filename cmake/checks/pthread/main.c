#include <pthread.h>

#ifdef __GNUC__
#    define _UNUSED(x) x __attribute__((__unused__))
#else
#    define _UNUSED(x) x
#endif

void * thread ( void * _UNUSED ( argument ) )
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
