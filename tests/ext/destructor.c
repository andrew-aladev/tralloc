// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"
#include "../lib/malloc_dynarr.h"
#include <talloc2/helpers.h>
#include <talloc2/ext/destructor.h>

static
uint8_t destructor ( void * chunk_data, void * user_data )
{
    malloc_dynarr * talloc_history = user_data;
    if ( talloc_history == NULL ) {
        return 0;
    }
    if ( malloc_dynarr_append ( talloc_history, chunk_data ) != 0 ) {
        return 1;
    }
    return 0;
}

static
uint8_t destructor_empty_1 ( void * chunk_data, void * user_data )
{
    return 0;
}

static
uint8_t destructor_empty_2 ( void * chunk_data, void * user_data )
{
    return 0;
}

static
malloc_dynarr * malloc_history()
{
    malloc_dynarr * talloc_history = malloc_dynarr_new ( 8 );
    if ( talloc_history == NULL ) {
        return NULL;
    }
    return talloc_history;
}

static
void free_history ( malloc_dynarr * talloc_history )
{
    malloc_dynarr_free ( talloc_history );
}

bool test_destructor ( void * root )
{
    malloc_dynarr * talloc_history = malloc_history();
    if ( talloc_history == NULL ) {
        return false;
    }

    void * strings = talloc_new ( root );
    char * text_01 = talloc_strdup ( strings, "test text 01" );
    char * text_02 = talloc_strdup ( strings, "test text 02" );
    char * text_03 = talloc_strdup ( strings, "test text 03" );
    char * text_04 = talloc_strdup ( strings, "test text 04" );
    if ( strings == NULL || text_01 == NULL || text_02 == NULL || text_03 == NULL || text_04 == NULL ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_add_destructor ( text_01, destructor, talloc_history ) != 0 ||
        talloc_add_destructor ( text_02, destructor, NULL )           != 0 ||
        talloc_add_destructor ( text_03, destructor, talloc_history ) != 0 ||
        talloc_add_destructor ( text_03, destructor, talloc_history ) != 0 ||
        talloc_add_destructor ( text_04, destructor, NULL )           != 0 ||
        talloc_add_destructor ( text_04, destructor, talloc_history ) != 0 ||
        talloc_add_destructor ( text_04, destructor, NULL )           != 0 ||
        talloc_add_destructor ( text_04, destructor, talloc_history ) != 0
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if ( talloc_clear_destructors ( text_03 ) != 0 ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_add_destructor ( text_03, destructor_empty_2, talloc_history ) != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_1, NULL )           != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_2, NULL )           != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_2, talloc_history ) != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_1, talloc_history ) != 0 ||
        talloc_add_destructor ( text_03, destructor_empty_1, NULL )           != 0
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    talloc_ext * ext_03 = talloc_ext_from_chunk ( talloc_chunk_from_data ( text_03 ) );
    talloc_destructor * destructor;

    if (
        ( destructor = ext_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL           || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != talloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != talloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != NULL           || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL           || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != talloc_history ||
        destructor->next != NULL
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_del_destructor ( text_03, destructor_empty_2, NULL ) != 0 ||
        ( destructor = ext_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL           || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != talloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != talloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL           || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != talloc_history ||
        destructor->next != NULL
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_del_destructor_by_function ( text_03, destructor_empty_2 ) != 0 ||
        ( destructor = ext_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL           || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != talloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL           ||
        destructor->next != NULL
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_del_destructor_by_data ( text_03, NULL ) != 0 ||
        ( destructor = ext_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != talloc_history ||
        destructor->next != NULL
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_del_destructor_by_data ( text_03, talloc_history ) != 0 ||
        ext_03->first_destructor != NULL
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        talloc_free ( text_02 ) != 0 ||
        talloc_free ( text_01 ) != 0 ||
        talloc_free ( text_03 ) != 0 ||
        talloc_free ( text_04 ) != 0
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if (
        malloc_dynarr_get_length ( talloc_history ) != 3 ||
        strcmp ( malloc_dynarr_get ( talloc_history, 0 ), "test text 01" ) != 0 ||
        strcmp ( malloc_dynarr_get ( talloc_history, 1 ), "test text 04" ) != 0 ||
        strcmp ( malloc_dynarr_get ( talloc_history, 2 ), "test text 04" ) != 0
    ) {
        talloc_free ( strings );
        free_history ( talloc_history );
        return false;
    }

    if ( talloc_free ( strings ) != 0 ) {
        return false;
    }
    free_history ( talloc_history );
    return true;
}
