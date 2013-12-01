// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <talloc2/tree.h>
#include <talloc2/ext/chunk.h>
#include <talloc2/reference/main.h>

#if defined(TALLOC_DEBUG)
#include <talloc2/events.h>
#endif

#include <string.h>

int main ()
{
    void * root = talloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }

    // create 3 test objects
    int * a = talloc ( root, sizeof ( int ) );
    int * b = talloc ( root, sizeof ( int ) );
    int * c = talloc ( root, sizeof ( int ) );
    if ( a == NULL || b == NULL || c == NULL ) {
        talloc_free ( root );
        return 2;
    }

    // create shared data
    char * shared = talloc ( root, sizeof ( char ) * 2 );

    // test bad arguments of talloc_add_reference
    if (
        shared == NULL ||
        talloc_add_reference ( NULL, shared )   != NULL ||
        talloc_add_reference ( shared, NULL )   != NULL ||
        talloc_add_reference ( NULL, NULL )     != NULL ||
        talloc_add_reference ( shared, shared ) != NULL ||
        talloc_add_reference ( root, shared )   != NULL
    ) {
        talloc_free ( root );
        return 3;
    }

    // create 2 references to shared data
    char ** b_shared = ( char ** ) talloc_add_reference ( b, shared );
    char ** c_shared = ( char ** ) talloc_add_reference ( c, shared );

    // test that creation was successful
    if (
        b_shared == NULL ||
        c_shared == NULL ||
        * b_shared != shared ||
        * c_shared != shared
    ) {
        talloc_free ( root );
        return 4;
    }

    // fill the shared data
    shared[0] = 'a';
    shared[1] = 'b';

    // create reference to shared data
    char ** a_shared = ( char ** ) talloc_add_reference ( a, shared );

    // test that all references point to shared data and this data is acessible
    if (
        a_shared == NULL ||
        * a_shared != shared ||
        * b_shared != shared ||
        * c_shared != shared ||
        ( * a_shared ) [0] != 'a' ||
        ( * a_shared ) [1] != 'b'
    ) {
        talloc_free ( root );
        return 5;
    }

    // test reallocation of shared data
    // reallocation from 2 char to 256 chars has a big chance that realloc will change pointer
    // in this case references should be updated automaticaly
    shared = talloc_realloc ( shared, sizeof ( char ) * 256 );
    if ( shared == NULL ) {
        talloc_free ( root );
        return 6;
    }

    // test that shared data chunk is valid
    talloc_chunk * shared_chunk = talloc_chunk_from_data ( shared );
    if ( shared_chunk->mode != TALLOC_MODE_EXT ) {
        talloc_free ( root );
        return 7;
    }

    // test that references list is valid
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
        return 8;
    }

    // delete all references of shared data
    if ( talloc_clear_references ( shared ) != 0 ) {
        talloc_free ( root );
        return 9;
    }

    // create 1 reference again
    a_shared = ( char ** ) talloc_add_reference ( a, shared );

    // test that references list is valid
    a_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( a_shared ) );
    reference   = shared_ext->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->parent_ext != shared_ext || reference->next != NULL
    ) {
        talloc_free ( root );
        return 10;
    }

    // create other references again
    b_shared = ( char ** ) talloc_add_reference ( b, shared );
    c_shared = ( char ** ) talloc_add_reference ( c, shared );

    // delete 2 references
    if (
        talloc_free ( a_shared ) != 0 ||
        talloc_free ( c_shared ) != 0
    ) {
        talloc_free ( root );
        return 11;
    }

    // test that references list is valid
    b_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( b_shared ) );
    reference   = shared_ext->first_reference;
    if (
        reference != b_reference ||
        reference->prev != NULL || reference->parent_ext != shared_ext || reference->next != NULL
    ) {
        talloc_free ( root );
        return 12;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 13;
    }

    // create 3 test objects
    a = talloc ( NULL, sizeof ( int ) );
    b = talloc ( NULL, sizeof ( int ) );
    c = talloc ( NULL, sizeof ( int ) );
    if ( a == NULL || b == NULL || c == NULL ) {
        talloc_free ( root );
        return 14;
    }

    // create common data
    char * common = talloc ( NULL, sizeof ( char ) * 7 );
    strcpy ( common, "common" );

    // create 2 references to common data
    char ** b_common = ( char ** ) talloc_add_reference ( b, common );
    char ** c_common = ( char ** ) talloc_add_reference ( c, common );

    if (
        b_common == NULL ||
        c_common == NULL
    ) {
        talloc_free ( a );
        talloc_free ( b );
        talloc_free ( c );
    }

    // common data should be deleted automaticaly
    if (
        talloc_free ( a ) != 0 ||
        talloc_free ( b ) != 0 ||
        talloc_free ( c ) != 0
    ) {
        return 15;
    }

#if defined(TALLOC_DEBUG)
    // no memory leaks should be here
    if ( talloc_get_objects_count() != 0 ) {
        return 16;
    }
#endif

    return 0;
}
