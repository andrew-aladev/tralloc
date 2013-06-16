// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <string.h>

#include <talloc2/helpers.h>

#ifdef TALLOC_EXT_DESTRUCTOR
#include <talloc2/ext/destructor.h>
#endif

#ifdef TALLOC_EXT_LENGTH
#include <talloc2/ext/length.h>
#endif

#include "utils/malloc_dynarr.h"

void * root;
static malloc_dynarr * history;

bool init_data ()
{
    root = talloc_new ( NULL );
    if ( root == NULL ) {
        return false;
    }
    history = malloc_dynarr_new ( 16 );
    if ( history == NULL ) {
        talloc_free ( root );
        return false;
    }
    return true;
}

uint8_t destructor ( void * data )
{
    if ( malloc_dynarr_append ( history, data ) != 0 ) {
        return 1;
    }
    return 0;
}

#ifdef TALLOC_EXT_DESTRUCTOR
bool test_destructor ()
{
    char * text_01 = talloc_strdup ( root, "test text 01" );
    char * text_02 = talloc_strdup ( root, "test text 02" );
    char * text_03 = talloc_strdup ( root, "test text 03" );
    if ( text_01 == NULL || text_02 == NULL || text_03 == NULL ) {
        return false;
    }

    if (
        talloc_set_destructor ( text_01, destructor ) != 0 ||
        talloc_set_destructor ( text_02, destructor ) != 0 ||
        talloc_set_destructor ( text_03, destructor ) != 0
    ) {
        return false;
    }

    if (
        talloc_free ( text_02 ) != 0 ||
        talloc_free ( text_01 ) != 0 ||
        talloc_free ( text_03 )
    ) {
        return false;
    }

    if (
        strcmp ( malloc_dynarr_get ( history, 0 ), "test text 02" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 1 ), "test text 01" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 2 ), "test text 03" ) != 0
    ) {
        return false;
    }

    return true;
}
#endif

#if defined (TALLOC_EXT_DESTRUCTOR) && defined (TALLOC_EXT_LENGTH)
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
        talloc_set_destructor ( str, destructor ) != 0 ||
        talloc_free ( str ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 3 ), "ab" ) != 0
    ) {
        return false;
    }
    return true;
}
#endif

void free_data ()
{
    talloc_free ( root );
    if ( history != NULL ) {
        malloc_dynarr_free ( history );
    }
}

int main ()
{
    if ( !init_data () ) {
        return 1;
    }

#ifdef TALLOC_EXT_DESTRUCTOR
    if ( !test_destructor () ) {
        free_data ();
        return 2;
    }
#endif
#if defined (TALLOC_EXT_DESTRUCTOR) && defined (TALLOC_EXT_LENGTH)
    if ( !test_length () ) {
        free_data ();
        return 3;
    }
#endif

    free_data ();
    return 0;
}

