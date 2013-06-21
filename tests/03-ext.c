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

#ifdef TALLOC_EXT_DESTRUCTOR
uint32_t user_data = 123456789;
uint8_t destructor ( void * data, void * user_data_ptr )
{
    uint32_t * data_ptr = user_data_ptr;
    if ( data_ptr != &user_data || * data_ptr != user_data ) {
        return 1;
    }
    if ( malloc_dynarr_append ( history, data ) != 0 ) {
        return 2;
    }
    return 0;
}

uint8_t destructor_empty_1 ( void * data, void * user_data_ptr )
{
    return 0;
}

uint8_t destructor_empty_2 ( void * data, void * user_data_ptr )
{
    return 0;
}

bool test_destructor ()
{
    void * strings = talloc_new ( root );
    char * text_01 = talloc_strdup ( strings, "test text 01" );
    char * text_02 = talloc_strdup ( strings, "test text 02" );
    char * text_03 = talloc_strdup ( strings, "test text 03" );
    char * text_04 = talloc_strdup ( strings, "test text 04" );
    if ( strings == NULL || text_01 == NULL || text_02 == NULL || text_03 == NULL || text_04 == NULL ) {
        return false;
    }

    if (
        talloc_add_destructor ( text_01, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_02, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_03, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_03, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_04, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_04, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_04, destructor, &user_data ) != 0 ||
        talloc_add_destructor ( text_04, destructor, &user_data ) != 0
    ) {
        talloc_free ( strings );
        return false;
    }

    if ( talloc_clear_destructors ( text_03 ) != 0 ) {
        talloc_free ( strings );
        return false;
    }

    if (
        talloc_add_destructor ( text_03, destructor_empty_2, &user_data ) != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_1, NULL )       != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_2, NULL )       != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_2, &user_data ) != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_1, &user_data ) != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_1, NULL )       != 0
    ) {
        talloc_free ( strings );
        return false;
    }

    talloc_chunk * chunk_03 = talloc_chunk_from_data ( text_03 );
    talloc_destructor_items * items = talloc_ext_get ( chunk_03, TALLOC_EXT_INDEX_DESTRUCTOR );
    talloc_destructor_item * item;

    if (
        items->length != 6 ||
        ( item = items->data[0] ) == NULL || item->destructor != destructor_empty_2 || item->user_data != &user_data ||
        ( item = items->data[1] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != NULL ||
        ( item = items->data[2] ) == NULL || item->destructor != destructor_empty_2 || item->user_data != NULL ||
        ( item = items->data[3] ) == NULL || item->destructor != destructor_empty_2 || item->user_data != &user_data ||
        ( item = items->data[4] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != &user_data ||
        ( item = items->data[5] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != NULL
    ) {
        talloc_free ( strings );
        return false;
    }

    if (
        talloc_del_destructor ( text_03, destructor_empty_2, NULL ) != 0 ||
        items->length != 5 ||
        ( item = items->data[0] ) == NULL || item->destructor != destructor_empty_2 || item->user_data != &user_data ||
        ( item = items->data[1] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != NULL ||
        ( item = items->data[2] ) == NULL || item->destructor != destructor_empty_2 || item->user_data != &user_data ||
        ( item = items->data[3] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != &user_data ||
        ( item = items->data[4] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != NULL
    ) {
        talloc_free ( strings );
        return false;
    }

    if (
        talloc_del_destructor_by_function ( text_03, destructor_empty_2 ) != 0 ||
        items->length != 3 ||
        ( item = items->data[0] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != NULL ||
        ( item = items->data[1] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != &user_data ||
        ( item = items->data[2] ) == NULL || item->destructor != destructor_empty_1 || item->user_data != NULL
    ) {
        talloc_free ( strings );
        return false;
    }

    if (
        talloc_del_destructor_by_data ( text_03, NULL ) != 0 ||
        items->length != 1 ||
        ( item = ( talloc_destructor_item * ) items->data ) == NULL || item->destructor != destructor_empty_1 || item->user_data != &user_data
    ) {
        talloc_free ( strings );
        return false;
    }

    if (
        talloc_del_destructor_by_data ( text_03, &user_data ) != 0 ||
        talloc_ext_get ( chunk_03, TALLOC_EXT_INDEX_DESTRUCTOR ) != NULL
    ) {
        talloc_free ( strings );
        return false;
    }

    if (
        talloc_free ( text_02 ) != 0 ||
        talloc_free ( text_01 ) != 0 ||
        talloc_free ( text_03 ) != 0 ||
        talloc_free ( text_04 ) != 0
    ) {
        talloc_free ( strings );
        return false;
    }

    if (
        malloc_dynarr_get_length ( history ) != 6 ||
        strcmp ( malloc_dynarr_get ( history, 0 ), "test text 02" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 1 ), "test text 01" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 2 ), "test text 04" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 3 ), "test text 04" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 4 ), "test text 04" ) != 0 ||
        strcmp ( malloc_dynarr_get ( history, 5 ), "test text 04" ) != 0
    ) {
        talloc_free ( strings );
        return false;
    }

    if ( talloc_free ( strings ) != 0 ) {
        return false;
    }
    return true;
}
#endif

#ifdef TALLOC_EXT_LENGTH
bool test_length ()
{
    size_t length;
    char * str = talloc_zero_mode ( root, sizeof ( char ) * 200, TALLOC_MODE_LENGTH );
    if ( str == NULL ) {
        return false;
    }
    if ( talloc_get_length ( str, &length ) != 0 || length != 200 ) {
        return false;
    }
    str = talloc_realloc ( str, sizeof ( char ) * 300 );
    if ( str == NULL ) {
        return false;
    }
    if ( talloc_get_length ( str, &length ) != 0 || length != 300 ) {
        return false;
    }
    str = talloc_realloc ( str, sizeof ( char ) * 3 );
    if ( str == NULL ) {
        return false;
    }
    if ( talloc_get_length ( str, &length ) != 0 || length != 3 ) {
        return false;
    }

    str[0] = 'a';
    str[1] = 'b';
    str[2] = '\0';

    if ( talloc_free ( str ) != 0 ) {
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

#ifdef TALLOC_EXT_LENGTH
    if ( !test_length () ) {
        free_data ();
        return 3;
    }
#endif

    free_data ();
    return 0;
}

