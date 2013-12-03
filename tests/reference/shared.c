// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "shared.h"
#include <talloc2/tree.h>
#include <talloc2/ext/chunk.h>
#include <talloc2/reference/main.h>

#include <string.h>

bool test_shared ( void * root )
{
    int *    a = talloc ( root, sizeof ( int ) * 2 );
    char *   b = talloc ( root, sizeof ( char ) * 3 );
    size_t * c = talloc ( root, sizeof ( size_t ) * 4 );
    if ( a == NULL || b == NULL || c == NULL ) {
        talloc_free ( root );
        return false;
    }

    char * shared = talloc ( root, sizeof ( char ) * 7 );
    strcpy ( shared, "shared" );

    if (
        shared == NULL ||
        talloc_add_reference ( NULL, shared )   != NULL ||
        talloc_add_reference ( shared, NULL )   != NULL ||
        talloc_add_reference ( NULL, NULL )     != NULL ||
        talloc_add_reference ( shared, shared ) != NULL ||
        talloc_add_reference ( root, shared )   != NULL
    ) {
        talloc_free ( root );
        return false;
    }

    char ** b_shared = ( char ** ) talloc_add_reference ( b, shared );
    char ** c_shared = ( char ** ) talloc_add_reference ( c, shared );

    if (
        b_shared == NULL || * b_shared != shared ||
        c_shared == NULL || * c_shared != shared
    ) {
        talloc_free ( root );
        return false;
    }

    char ** a_shared = ( char ** ) talloc_add_reference ( a, shared );

    if (
        a_shared == NULL     || * a_shared != shared ||
        * b_shared != shared ||
        * c_shared != shared
    ) {
        talloc_free ( root );
        return false;
    }

    shared = talloc_realloc ( shared, sizeof ( char ) * 256 );
    if ( shared == NULL || strncmp ( shared, "shared", 6 ) != 0 ) {
        talloc_free ( root );
        return false;
    }

    talloc_chunk * shared_chunk = talloc_chunk_from_data ( shared );
    if ( shared_chunk->mode != TALLOC_MODE_EXT ) {
        talloc_free ( root );
        return false;
    }

    talloc_ext * shared_ext        = talloc_ext_from_chunk ( shared_chunk );
    talloc_reference * a_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( a_shared ) );
    talloc_reference * b_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( b_shared ) );
    talloc_reference * c_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( c_shared ) );
    talloc_reference * reference   = shared_ext->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->parent_ext != shared_ext ||
        ( reference = reference->next ) != c_reference ||
        reference->prev != a_reference || reference->parent_ext != shared_ext ||
        ( reference = reference->next ) != b_reference ||
        reference->prev != c_reference || reference->parent_ext != shared_ext ||
        ( reference = reference->next ) != NULL
    ) {
        talloc_free ( root );
        return false;
    }

    if ( talloc_clear_references ( shared ) != 0 ) {
        talloc_free ( root );
        return false;
    }

    a_shared = ( char ** ) talloc_add_reference ( a, shared );

    a_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( a_shared ) );
    reference   = shared_ext->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->parent_ext != shared_ext || reference->next != NULL
    ) {
        talloc_free ( root );
        return false;
    }

    b_shared = ( char ** ) talloc_add_reference ( b, shared );
    c_shared = ( char ** ) talloc_add_reference ( c, shared );

    if (
        talloc_free ( a_shared ) != 0 ||
        talloc_free ( c_shared ) != 0
    ) {
        talloc_free ( root );
        return false;
    }

    b_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( b_shared ) );
    reference   = shared_ext->first_reference;
    if (
        reference != b_reference ||
        reference->prev != NULL || reference->parent_ext != shared_ext || reference->next != NULL
    ) {
        talloc_free ( root );
        return false;
    }

    return true;
}