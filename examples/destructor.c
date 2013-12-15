// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/helpers.h>
#include <talloc2/extensions/destructor.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

uint8_t empty_destructor ( talloc_context * context, void * user_data )
{
    return 0;
}

uint8_t bad_destructor ( talloc_context * context, void * user_data )
{
    return 1;
}

uint8_t file_destructor ( talloc_context * context, void * user_data )
{
    int file_descriptor = * ( ( int * ) context );
    char * filename     = user_data;
    printf ( "closing file %s\n", filename );
    close ( file_descriptor );
    return 0;
}

int main ()
{
    talloc_context * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    int * file_descriptor = talloc ( root, sizeof ( int ) );
    if ( file_descriptor == NULL ) {
        talloc_free ( root );
        return 2;
    }
    char * filename = talloc_strdup ( file_descriptor, "/etc/fstab" );
    * file_descriptor = open ( filename, O_RDONLY );
    if ( * file_descriptor == -1 ) {
        talloc_free ( root );
        return 3;
    }
    if (
        talloc_add_destructor ( file_descriptor, file_destructor,  filename ) != 0 ||
        talloc_add_destructor ( file_descriptor, empty_destructor, filename ) != 0 ||
        talloc_add_destructor ( file_descriptor, bad_destructor,   filename ) != 0 ||
        talloc_add_destructor ( file_descriptor, empty_destructor, NULL )     != 0
    ) {
        talloc_free ( root );
        return 4;
    }
    if (
        talloc_del_destructor             ( file_descriptor, empty_destructor, filename ) != 0 ||
        talloc_del_destructor_by_function ( file_descriptor, bad_destructor )             != 0 ||
        talloc_del_destructor_by_data     ( file_descriptor, NULL )                       != 0
    ) {
        talloc_free ( root );
        return 5;
    }
    if ( talloc_free ( root ) != 0 ) {
        return 6;
    }

#if defined(TALLOC_DEBUG)
    if ( talloc_get_chunks_count() != 0 ) {
        return 7;
    }
#endif

    return 0;
}