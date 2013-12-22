// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "destructor.h"
#include "../lib/malloc_dynarr.h"
#include <tralloc/helpers/string.h>
#include <tralloc/destructor.h>

static
uint8_t normal_destructor ( tralloc_context * chunk_context, void * user_data )
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
uint8_t empty_destructor_1 ( tralloc_context * chunk_context, void * user_data )
{
    return 0;
}

static
uint8_t empty_destructor_2 ( tralloc_context * chunk_context, void * user_data )
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
        tralloc_append_destructor  ( text_01, normal_destructor, tralloc_history ) != 0 ||
        tralloc_prepend_destructor ( text_02, normal_destructor, NULL )            != 0 ||
        tralloc_prepend_destructor ( text_03, normal_destructor, tralloc_history ) != 0 ||
        tralloc_append_destructor  ( text_03, normal_destructor, tralloc_history ) != 0 ||
        tralloc_append_destructor  ( text_04, normal_destructor, NULL )            != 0 ||
        tralloc_prepend_destructor ( text_04, normal_destructor, NULL )            != 0 ||
        tralloc_append_destructor  ( text_04, normal_destructor, tralloc_history ) != 0 ||
        tralloc_prepend_destructor ( text_04, normal_destructor, tralloc_history ) != 0
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    tralloc_chunk * chunk_01 = _tralloc_chunk_from_context ( text_01 );
    tralloc_chunk * chunk_02 = _tralloc_chunk_from_context ( text_02 );
    tralloc_chunk * chunk_03 = _tralloc_chunk_from_context ( text_03 );
    tralloc_chunk * chunk_04 = _tralloc_chunk_from_context ( text_04 );
    tralloc_destructors_list * destructors;
    tralloc_destructor * destructor;

    if (
        ( destructors = chunk_01->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != tralloc_history || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = chunk_02->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != NULL || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = chunk_03->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = chunk_04->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_clear_destructors ( text_03 )   != 0 ||
        ( destructors = chunk_03->destructors ) != NULL
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }


    if (
        tralloc_append_destructor  ( text_03, empty_destructor_2, tralloc_history ) != 0 ||
        tralloc_append_destructor  ( text_03, empty_destructor_1, NULL )            != 0 ||
        tralloc_prepend_destructor ( text_03, empty_destructor_2, NULL )            != 0 ||
        tralloc_append_destructor  ( text_03, empty_destructor_2, tralloc_history ) != 0 ||
        tralloc_prepend_destructor ( text_03, empty_destructor_1, tralloc_history ) != 0 ||
        tralloc_prepend_destructor ( text_03, empty_destructor_1, NULL )            != 0
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        ( destructors = chunk_03->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor ( text_03, empty_destructor_1, NULL ) != 0 ||

        ( destructors = chunk_03->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor_by_function ( text_03, empty_destructor_2 ) != 0 ||

        ( destructors = chunk_03->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || destructor->next != NULL ||
        destructors->last_destructor != destructor
    ) {
        tralloc_free ( strings );
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_del_destructor_by_data ( text_03, NULL ) != 0 ||

        ( destructors = chunk_03->destructors ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        tralloc_del_destructor_by_data ( text_03, tralloc_history ) != 0 ||
        chunk_03->destructors != NULL
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
