// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <talloc2/tree.h>
#include <talloc2/ext/chunk.h>
#include <talloc2/reference/main.h>

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

    // create common data
    char * common = talloc ( root, sizeof ( char ) * 2 );

    // test bad arguments of talloc_add_reference
    if (
        common == NULL ||
        talloc_add_reference ( NULL, common )   != NULL ||
        talloc_add_reference ( common, NULL )   != NULL ||
        talloc_add_reference ( NULL, NULL )     != NULL ||
        talloc_add_reference ( common, common ) != NULL ||
        talloc_add_reference ( root, common )   != NULL
    ) {
        talloc_free ( root );
        return 3;
    }

    // create 2 references to common data
    char ** b_common = ( char ** ) talloc_add_reference ( b, common );
    char ** c_common = ( char ** ) talloc_add_reference ( c, common );

    // test that creation was successful
    if (
        b_common == NULL ||
        c_common == NULL ||
        * b_common != common ||
        * c_common != common
    ) {
        talloc_free ( root );
        return 4;
    }

    // fill the common data
    common[0] = 'a';
    common[1] = 'b';

    // create reference to common data
    char ** a_common = ( char ** ) talloc_add_reference ( a, common );

    // test that all references point to common data and this data is acessible
    if (
        a_common == NULL ||
        * a_common != common ||
        * b_common != common ||
        * c_common != common ||
        ( * a_common ) [0] != 'a' ||
        ( * a_common ) [1] != 'b'
    ) {
        talloc_free ( root );
        return 5;
    }

    // test reallocation of common data
    // reallocation from 2 char to 256 chars has a big chance that realloc will change pointer
    // in this case references should be updated automaticaly
    common = talloc_realloc ( common, sizeof ( char ) * 256 );
    if ( common == NULL ) {
        talloc_free ( root );
        return 6;
    }

    // test that common data chunk is valid
    talloc_chunk * common_chunk = talloc_chunk_from_data ( common );
    if ( common_chunk->mode != TALLOC_MODE_EXT ) {
        talloc_free ( root );
        return 7;
    }

    // test that references list is valid
    talloc_ext * common_ext        = talloc_ext_from_chunk ( common_chunk );
    talloc_reference * a_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( a_common ) );
    talloc_reference * b_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( b_common ) );
    talloc_reference * c_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( c_common ) );
    talloc_reference * reference   = common_ext->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->chunk != common_ext ||
        ( reference = reference->next ) != c_reference ||
        reference->prev != a_reference || reference->chunk != common_ext ||
        ( reference = reference->next ) != b_reference ||
        reference->prev != c_reference || reference->chunk != common_ext ||
        ( reference = reference->next ) != NULL
    ) {
        talloc_free ( root );
        return 8;
    }

    // delete all references of common data
    if ( talloc_clear_references ( common ) != 0 ) {
        talloc_free ( root );
        return 9;
    }

    // create 1 reference again
    a_common = ( char ** ) talloc_add_reference ( a, common );

    // test that references list is valid
    a_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( a_common ) );
    reference   = common_ext->first_reference;
    if (
        reference != a_reference ||
        reference->prev != NULL || reference->chunk != common_ext || reference->next != NULL
    ) {
        talloc_free ( root );
        return 10;
    }

    // create other references again
    b_common = ( char ** ) talloc_add_reference ( b, common );
    c_common = ( char ** ) talloc_add_reference ( c, common );

    // delete 2 references
    if (
        talloc_free ( a_common ) != 0 ||
        talloc_free ( c_common ) != 0
    ) {
        talloc_free ( root );
        return 11;
    }

    // test that references list is valid
    b_reference = talloc_reference_from_chunk ( talloc_chunk_from_data ( b_common ) );
    reference   = common_ext->first_reference;
    if (
        reference != b_reference ||
        reference->prev != NULL || reference->chunk != common_ext || reference->next != NULL
    ) {
        talloc_free ( root );
        return 12;
    }

    if ( talloc_free ( root ) != 0 ) {
        return 13;
    }
    return 0;
}
