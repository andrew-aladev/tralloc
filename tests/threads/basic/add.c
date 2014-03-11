// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <pthread.h>


void * thread ( tralloc_context * ctx )
{
    tralloc_context * data;
    for ( uint8_t index = 0; index < 10; index ++ ) {
        if (
            tralloc_new  ( ctx, &data ) != 0 ||
            tralloc_free ( data ) != 0
        ) {
            return NULL + 1;
        }
    }
    return NULL;
}

tralloc_bool test_basic_add ( tralloc_context * ctx )
{
    void * result;
    pthread_t thread_1, thread_2, thread_3, thread_4, thread_5;
    if (
        pthread_create ( &thread_1, NULL, &thread, ctx ) != 0 ||
        pthread_create ( &thread_2, NULL, &thread, ctx ) != 0 ||
        pthread_create ( &thread_3, NULL, &thread, ctx ) != 0 ||
        pthread_create ( &thread_4, NULL, &thread, ctx ) != 0 ||
        pthread_create ( &thread_5, NULL, &thread, ctx ) != 0 ||
        pthread_join   ( thread_1, &result ) != 0 || result != NULL ||
        pthread_join   ( thread_2, &result ) != 0 || result != NULL ||
        pthread_join   ( thread_3, &result ) != 0 || result != NULL ||
        pthread_join   ( thread_4, &result ) != 0 || result != NULL ||
        pthread_join   ( thread_5, &result ) != 0 || result != NULL
    ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
