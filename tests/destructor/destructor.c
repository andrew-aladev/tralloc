// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include "../lib/malloc_dynarr.h"
#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>
#include <tralloc/destructor/main.h>


static
tralloc_error normal_destructor ( tralloc_context * chunk_context, void * user_data )
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

static const tralloc_error BAD_DESTRUCTOR_ERROR_1 = 1;
static const tralloc_error BAD_DESTRUCTOR_ERROR_2 = 2;

static
tralloc_error bad_destructor_1 ( tralloc_context * UNUSED ( chunk_context ), void * UNUSED ( user_data ) )
{
    return BAD_DESTRUCTOR_ERROR_1;
}

static
tralloc_error bad_destructor_2 ( tralloc_context * UNUSED ( chunk_context ), void * UNUSED ( user_data ) )
{
    return BAD_DESTRUCTOR_ERROR_2;
}

static
tralloc_error empty_destructor_1 ( tralloc_context * UNUSED ( chunk_context ), void * UNUSED ( user_data ) )
{
    return 0;
}

static
tralloc_error empty_destructor_2 ( tralloc_context * UNUSED ( chunk_context ), void * UNUSED ( user_data ) )
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

bool test_destructor ( tralloc_context * ctx )
{
    malloc_dynarr * tralloc_history = malloc_history();
    if ( tralloc_history == NULL ) {
        return false;
    }

    tralloc_context * strings;
    char * text_01;
    char * text_02;
    char * text_03;
    char * text_04;
    if (
        tralloc_with_extensions_new    ( ctx,     &strings, TRALLOC_HAVE_DESTRUCTORS )                 != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_01, TRALLOC_HAVE_DESTRUCTORS, "test text 01" ) != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_02, TRALLOC_HAVE_DESTRUCTORS, "test text 02" ) != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_03, TRALLOC_HAVE_DESTRUCTORS, "test text 03" ) != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_04, TRALLOC_HAVE_DESTRUCTORS, "test text 04" ) != 0
    ) {
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
        free_history ( tralloc_history );
        return false;
    }

    _tralloc_chunk * chunk_01 = _tralloc_chunk_from_context ( text_01 );
    _tralloc_chunk * chunk_02 = _tralloc_chunk_from_context ( text_02 );
    _tralloc_chunk * chunk_03 = _tralloc_chunk_from_context ( text_03 );
    _tralloc_chunk * chunk_04 = _tralloc_chunk_from_context ( text_04 );
    _tralloc_destructors * destructors;
    _tralloc_destructor * destructor;

    if (
        ( destructors = _tralloc_destructors_from_chunk ( chunk_01 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != tralloc_history || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = _tralloc_destructors_from_chunk ( chunk_02 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != NULL || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor  || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = _tralloc_destructors_from_chunk ( chunk_04 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != normal_destructor || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_clear_destructors ( text_03 ) != 0 ||
        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL ||
        destructors->first_destructor != NULL || destructors->last_destructor != NULL
    ) {
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
        free_history ( tralloc_history );
        return false;
    }

    if (
        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_delete_destructors ( text_03, empty_destructor_1, NULL ) != 0 ||

        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != NULL            || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || ( destructor = destructor->next ) == NULL ||
        destructor->function != empty_destructor_2 || destructor->user_data != tralloc_history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_delete_destructors_by_function ( text_03, empty_destructor_2 ) != 0 ||

        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || destructor->next != NULL ||
        destructors->last_destructor != destructor
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_delete_destructors_by_data ( text_03, NULL ) != 0 ||

        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != empty_destructor_1 || destructor->user_data != tralloc_history || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        tralloc_delete_destructors_by_data ( text_03, tralloc_history ) != 0 ||
        ( destructors = _tralloc_destructors_from_chunk ( chunk_03 ) ) == NULL ||
        destructors->first_destructor != NULL || destructors->last_destructor != NULL
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_free ( text_02 ) != 0 ||
        tralloc_free ( text_01 ) != 0 ||
        tralloc_free ( text_03 ) != 0 ||
        tralloc_free ( text_04 ) != 0
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        malloc_dynarr_get_length ( tralloc_history ) != 3 ||
        strcmp ( malloc_dynarr_get ( tralloc_history, 0 ), "test text 01" ) != 0 ||
        strcmp ( malloc_dynarr_get ( tralloc_history, 1 ), "test text 04" ) != 0 ||
        strcmp ( malloc_dynarr_get ( tralloc_history, 2 ), "test text 04" ) != 0
    ) {
        free_history ( tralloc_history );
        return false;
    }

    if (
        tralloc_append_destructor  ( strings, bad_destructor_2, NULL ) != 0 ||
        tralloc_prepend_destructor ( strings, bad_destructor_1, NULL ) != 0
    ) {
        free_history ( tralloc_history );
        return false;
    }
    if ( tralloc_free ( strings ) != BAD_DESTRUCTOR_ERROR_2 ) {
        return false;
    }

    free_history ( tralloc_history );
    return true;
}
