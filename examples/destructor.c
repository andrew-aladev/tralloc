// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/helpers.h>
#include <tralloc/extensions/destructor.h>

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

uint8_t empty_destructor ( tralloc_context * context, void * user_data )
{
    return 0;
}

uint8_t bad_destructor ( tralloc_context * context, void * user_data )
{
    return 1;
}

uint8_t file_destructor ( tralloc_context * context, void * user_data )
{
    int file_descriptor = * ( ( int * ) context );
    char * filename     = user_data;
    printf ( "closing file %s\n", filename );
    close ( file_descriptor );
    return 0;
}

int main ()
{
    tralloc_context * root = tralloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    int * file_descriptor = tralloc ( root, sizeof ( int ) );
    if ( file_descriptor == NULL ) {
        tralloc_free ( root );
        return 2;
    }
    char * filename = tralloc_strdup ( file_descriptor, "/etc/fstab" );
    * file_descriptor = open ( filename, O_RDONLY );
    if ( * file_descriptor == -1 ) {
        tralloc_free ( root );
        return 3;
    }
    if (
        tralloc_add_destructor ( file_descriptor, file_destructor,  filename ) != 0 ||
        tralloc_add_destructor ( file_descriptor, empty_destructor, filename ) != 0 ||
        tralloc_add_destructor ( file_descriptor, bad_destructor,   filename ) != 0 ||
        tralloc_add_destructor ( file_descriptor, empty_destructor, NULL )     != 0
    ) {
        tralloc_free ( root );
        return 4;
    }
    if (
        tralloc_del_destructor             ( file_descriptor, empty_destructor, filename ) != 0 ||
        tralloc_del_destructor_by_function ( file_descriptor, bad_destructor )             != 0 ||
        tralloc_del_destructor_by_data     ( file_descriptor, NULL )                       != 0
    ) {
        tralloc_free ( root );
        return 5;
    }
    if ( tralloc_free ( root ) != 0 ) {
        return 6;
    }

#if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 7;
    }
#endif

    return 0;
}