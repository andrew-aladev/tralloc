// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>
#include <tralloc/destructor/main.h>

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif


tralloc_error empty_destructor ( tralloc_context * _TRALLOC_UNUSED ( context ), void * _TRALLOC_UNUSED ( user_data ) )
{
    return 0;
}

tralloc_error bad_destructor ( tralloc_context * _TRALLOC_UNUSED ( context ), void * _TRALLOC_UNUSED ( user_data ) )
{
    return 1;
}

tralloc_error file_destructor ( tralloc_context * context, void * _TRALLOC_UNUSED ( user_data ) )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    int * _TRALLOC_UNUSED ( number ) = context;
    return 0;
}

int main ()
{
    tralloc_context * ctx;
    if ( tralloc_new ( NULL, &ctx ) != 0 ) {
        return 1;
    }
    int * number;
    if ( tralloc_with_extensions ( ctx, ( tralloc_context ** ) &number, TRALLOC_EXTENSION_DESTRUCTORS, sizeof ( int ) ) != 0 ) {
        tralloc_free ( ctx );
        return 2;
    }
    * number = 12345;
    if (
        tralloc_prepend_destructor ( number, empty_destructor, NULL ) != 0 ||
        tralloc_append_destructor  ( number, file_destructor,  NULL ) != 0 ||
        tralloc_append_destructor  ( number, bad_destructor,   NULL ) != 0 ||
        tralloc_prepend_destructor ( number, empty_destructor, NULL ) != 0
    ) {
        tralloc_free ( ctx );
        return 3;
    }
    if (
        tralloc_delete_destructors             ( number, empty_destructor, NULL ) != 0 ||
        tralloc_delete_destructors_by_function ( number, bad_destructor )         != 0 ||
        tralloc_delete_destructors_by_data     ( number, NULL )                   != 0
    ) {
        tralloc_free ( ctx );
        return 4;
    }
    if ( tralloc_free ( ctx ) != 0 ) {
        return 5;
    }

#if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 6;
    }
#endif

    return 0;
}
