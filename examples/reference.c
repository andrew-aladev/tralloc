// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/helpers.h>
#include <tralloc/reference/main.h>

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif

#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static inline
void random_sleep ()
{
    uint miliseconds = ( ( double ) rand() ) / RAND_MAX * 300 + 100; // from 100 to 300 miliseconds
    usleep ( miliseconds * 1000 );
}

void * thread_1 ( void * data )
{
    char * common           = data;
    void * common_reference = tralloc_add_reference ( common, NULL );
    if ( common_reference == NULL ) {
        return ( void * ) 1;
    }
    random_sleep();
    tralloc_free ( common_reference ); // may free common
    return ( void * ) 0;
}

void * thread_2 ( void * data )
{
    char * common           = data;
    void * common_reference = tralloc_add_reference ( common, NULL );
    if ( common_reference == NULL ) {
        return ( void * ) 1;
    }
    random_sleep();
    tralloc_free ( common_reference ); // may free common
    return ( void * ) 0;
}

int main ()
{
    tralloc_context * root = tralloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    char * common = tralloc_strdup (
        NULL,
        "This is big data, that you dont want to duplicate."
        "Different code needs it."
        "You want it to be fried automaticaly when all code have finished it's processing."
    );
    if ( common == NULL ) {
        tralloc_free ( root );
        return 2;
    }

    time_t seed = time ( NULL );
    srand ( seed );

    pthread_t thread_1_id, thread_2_id;
    if (
        pthread_create ( &thread_1_id, NULL, thread_1, common ) != 0 ||
        pthread_create ( &thread_2_id, NULL, thread_2, common ) != 0
    ) {
        tralloc_free ( root );
        return 2;
    }

    void * result;
    if (
        pthread_join ( thread_1_id, &result ) != 0 || ( uint8_t ) result != 0 ||
        pthread_join ( thread_2_id, &result ) != 0 || ( uint8_t ) result != 0
    ) {
        tralloc_free ( root );
        return 3;
    }

    if ( tralloc_free ( root ) != 0 ) {
        return 4;
    }

#if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 5;
    }
#endif

    return 0;
}
