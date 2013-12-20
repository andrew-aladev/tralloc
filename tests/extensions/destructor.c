// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"
#include "../lib/malloc_dynarr.h"
#include <tralloc/helpers.h>
#include <tralloc/destructor.h>

static
uint8_t destructor ( tralloc_context * chunk_context, void * user_data )
{
    malloc_dynarr * tralloc_history = user_data;
    if ( tralloc_history == NULL ) {
        return 0;
    }
    if ( malloc_dynarr_append ( tralloc_history, chunk_context ) != 0 ) {
        return 1;
    }
    return 0;
}

static
uint8_t destructor_empty_1 ( tralloc_context * chunk_context, void * user_data )
{
    return 0;
}

static
uint8_t destructor_empty_2 ( tralloc_context * chunk_context, void * user_data )
{
    return 0;
}

static
malloc_dynarr * malloc_history()
{
    malloc_dynarr * tralloc_history = malloc_dynarr_new ( 8 );
    if ( tralloc_history == NULL ) {
        return NULL;
    }
    return tralloc_history;
}

static
void free_history ( malloc_dynarr * tralloc_history )
{
    malloc_dynarr_free ( tralloc_history );
}

bool test_destructor ( const tralloc_context * root )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if ( tralloc_history == NULL ) {
        return false;
    }

    void * strings = tralloc_new ( root );
    char * text_01 = tralloc_strdup ( strings, "test text 01" );
    char * text_02 = tralloc_strdup ( strings, "test text 02" );
    char * text_03 = tralloc_strdup ( strings, "test text 03" );
    char * text_04 = tralloc_strdup ( strings, "test text 04" );
    if ( strings == NULL || text_01 == NULL || text_02 == NULL || text_03 == NULL || text_04 == NULL ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_add_destructor ( text_01, destructor, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_02, destructor, NULL )            != 0 ||
        tralloc_add_destructor ( text_03, destructor, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_03, destructor, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_04, destructor, NULL )            != 0 ||
        tralloc_add_destructor ( text_04, destructor, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_04, destructor, NULL )            != 0 ||
        tralloc_add_destructor ( text_04, destructor, tralloc_history ) != 0
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if ( tralloc_clear_destructors ( text_03 ) != 0 ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_add_destructor ( text_03, destructor_empty_2, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_03, destructor_empty_1, NULL )            != 0 ||
        tralloc_add_destructor ( text_03, destructor_empty_2, NULL )            != 0 ||
        tralloc_add_destructor ( text_03, destructor_empty_2, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_03, destructor_empty_1, tralloc_history ) != 0 ||
        tralloc_add_destructor ( text_03, destructor_empty_1, NULL )            != 0
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    tralloc_chunk * chunk_03 = tralloc_chunk_from_context ( text_03 );
    tralloc_destructor * destructor;

    if (
        ( destructor = chunk_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != tralloc_history ||
        destructor->next != NULL
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor ( text_03, destructor_empty_2, NULL ) != 0 ||
        ( destructor = chunk_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_2 || destructor->user_data != tralloc_history ||
        destructor->next != NULL
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor_by_function ( text_03, destructor_empty_2 ) != 0 ||
        ( destructor = chunk_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != NULL            ||
        destructor->next != NULL
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor_by_data ( text_03, NULL ) != 0 ||
        ( destructor = chunk_03->first_destructor ) == NULL ||
        destructor->function != destructor_empty_1 || destructor->user_data != tralloc_history ||
        destructor->next != NULL
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor_by_data ( text_03, tralloc_history ) != 0 ||
        chunk_03->first_destructor != NULL
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_free ( text_02 ) != 0 ||
        tralloc_free ( text_01 ) != 0 ||
        tralloc_free ( text_03 ) != 0 ||
        tralloc_free ( text_04 ) != 0
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        malloc_dynarr_get_length ( tralloc_history ) != 3 ||
        strcmp ( malloc_dynarr_get ( tralloc_history, 0 ), "test text 01" ) != 0 ||
        strcmp ( malloc_dynarr_get ( tralloc_history, 1 ), "test text 04" ) != 0 ||
        strcmp ( malloc_dynarr_get ( tralloc_history, 2 ), "test text 04" ) != 0
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if ( tralloc_free ( strings ) != 0 ) {
        return false;
    }
    free_history ( tralloc_history );
    return true;
}
