// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <string.h>

#include <talloc/helpers.h>
#include <talloc/ext/destructor.h>
#include <talloc/ext/length.h>

#include "utils/dynarr.h"

void * root;
static talloc_dynarr * history;

bool init_data ()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }
    history = talloc_dynarr_new ( 16 );
    if ( history == NULL ) {
        talloc_free ( root );
        return false;
    }
    return true;
}

uint8_t destructor ( void * data )
{
    talloc_dynarr_append ( history, data );
    return 0;
}

bool test_destructor ()
{
    char * text_01 = talloc_strdup ( root, "test text 01" );
    if ( text_01 == NULL ) {
        return false;
    }
    char * text_02 = talloc_strdup ( root, "test text 02" );
    if ( text_02 == NULL ) {
        return false;
    }
    char * text_03 = talloc_strdup ( root, "test text 03" );
    if ( text_03 == NULL ) {
        return false;
    }

    if (
        ! (
            !talloc_set_destructor ( text_01, destructor ) &&
            !talloc_set_destructor ( text_02, destructor ) &&
            !talloc_set_destructor ( text_03, destructor )
        )
    ) {
        return false;
    }

    talloc_free ( text_02 );
    talloc_free ( text_01 );
    talloc_free ( text_03 );

    if (
        ! (
            !strcmp ( talloc_dynarr_get ( history, 0 ), "test text 02" ) &&
            !strcmp ( talloc_dynarr_get ( history, 1 ), "test text 01" ) &&
            !strcmp ( talloc_dynarr_get ( history, 2 ), "test text 03" )
        )
    ) {
        return false;
    }

    return true;
}

bool test_length ()
{
    size_t length;
    char * str = talloc_zero_mode ( root, sizeof ( char ) * 200, TALLOC_MODE_LENGTH );
    if ( str == NULL || talloc_get_length ( str, &length ) != 0 || length != 200 ) {
        return false;
    }
    str = talloc_realloc ( str, sizeof ( char ) * 300 );
    if ( str == NULL || talloc_get_length ( str, &length ) != 0 || length != 300 ) {
        return false;
    }
    str = talloc_realloc ( str, sizeof ( char ) * 3 );
    if ( str == NULL || talloc_get_length ( str, &length ) != 0 || length != 3 ) {
        return false;
    }
    str[0] = 'a';
    str[1] = 'b';
    str[2] = '\0';
    if (
        ! (
            talloc_set_destructor ( str, destructor ) == 0 &&
            talloc_free ( str ) == 0 &&
            strcmp ( talloc_dynarr_get ( history, 3 ), "ab" ) == 0
        )
    ) {
        return false;
    }
    return true;
}

void free_data ()
{
    if ( root != NULL ) {
        talloc_free ( root );
    }
    if ( history != NULL ) {
        talloc_dynarr_free ( history );
    }
}

int main ()
{
    if ( !init_data () ) {
        return 1;
    }

#ifdef TALLOC_EXT
    if ( !test_destructor () ) {
        free_data ();
        return 2;
    }
    if ( !test_length () ) {
        free_data ();
        return 3;
    }
#endif

    free_data ();
    return 0;
}

