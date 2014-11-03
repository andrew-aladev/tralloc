// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/destructors/common.h>
#include <tralloc/tests/common/dynarr.h>
#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>
#include <tralloc/destructors.h>
#include <tralloc/destructors/chunk.h>


static
tralloc_error test_destructors_common_normal_destructor ( tralloc_context * chunk_context, void * user_data )
{
    dynarr * history = user_data;
    if ( history == NULL ) {
        return 0;
    }
    if ( dynarr_append ( history, chunk_context ) != 0 ) {
        return 1;
    }
    return 0;
}

static const tralloc_error BAD_DESTRUCTOR_ERROR_1 = 1;
static const tralloc_error BAD_DESTRUCTOR_ERROR_2 = 2;

static
tralloc_error test_destructors_common_bad_destructor_1 ( tralloc_context * _TRALLOC_UNUSED ( chunk_context ), void * _TRALLOC_UNUSED ( user_data ) )
{
    return BAD_DESTRUCTOR_ERROR_1;
}

static
tralloc_error test_destructors_common_bad_destructor_2 ( tralloc_context * _TRALLOC_UNUSED ( chunk_context ), void * _TRALLOC_UNUSED ( user_data ) )
{
    return BAD_DESTRUCTOR_ERROR_2;
}

static
tralloc_error test_destructors_common_empty_destructor_1 ( tralloc_context * _TRALLOC_UNUSED ( chunk_context ), void * _TRALLOC_UNUSED ( user_data ) )
{
    return 0;
}

static
tralloc_error test_destructors_common_empty_destructor_2 ( tralloc_context * _TRALLOC_UNUSED ( chunk_context ), void * _TRALLOC_UNUSED ( user_data ) )
{
    return 0;
}

tralloc_bool test_destructors_common ( tralloc_context * ctx )
{
    dynarr * history = dynarr_new ( 8 );
    if ( history == NULL ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * strings;
    char * text_01;
    char * text_02;
    char * text_03;
    char * text_04;
    if (
        tralloc_new_empty_with_extensions ( ctx,     &strings, TRALLOC_EXTENSION_DESTRUCTORS )              != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_01, TRALLOC_EXTENSION_DESTRUCTORS, "test text 01" ) != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_02, TRALLOC_EXTENSION_DESTRUCTORS, "test text 02" ) != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_03, TRALLOC_EXTENSION_DESTRUCTORS, "test text 03" ) != 0 ||
        tralloc_strdup_with_extensions ( strings, &text_04, TRALLOC_EXTENSION_DESTRUCTORS, "test text 04" ) != 0
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_append_destructor  ( text_01, test_destructors_common_normal_destructor, history ) != 0 ||
        tralloc_prepend_destructor ( text_02, test_destructors_common_normal_destructor, NULL )    != 0 ||
        tralloc_prepend_destructor ( text_03, test_destructors_common_normal_destructor, history ) != 0 ||
        tralloc_append_destructor  ( text_03, test_destructors_common_normal_destructor, history ) != 0 ||
        tralloc_append_destructor  ( text_04, test_destructors_common_normal_destructor, NULL )    != 0 ||
        tralloc_prepend_destructor ( text_04, test_destructors_common_normal_destructor, NULL )    != 0 ||
        tralloc_append_destructor  ( text_04, test_destructors_common_normal_destructor, history ) != 0 ||
        tralloc_prepend_destructor ( text_04, test_destructors_common_normal_destructor, history ) != 0
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    _tralloc_chunk * chunk_01 = _tralloc_context_get_chunk ( text_01 );
    _tralloc_chunk * chunk_02 = _tralloc_context_get_chunk ( text_02 );
    _tralloc_chunk * chunk_03 = _tralloc_context_get_chunk ( text_03 );
    _tralloc_chunk * chunk_04 = _tralloc_context_get_chunk ( text_04 );
    _tralloc_destructors * destructors;
    _tralloc_destructor * destructor;

    if (
        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_01 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor  || destructor->user_data != history || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_02 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor  || destructor->user_data != NULL || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor  || destructor->user_data != history || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor  || destructor->user_data != history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor ||

        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_04 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor || destructor->user_data != history || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor || destructor->user_data != NULL    || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor || destructor->user_data != NULL    || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_normal_destructor || destructor->user_data != history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_clear_destructors ( text_03 ) != 0 ||
        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL ||
        destructors->first_destructor != NULL || destructors->last_destructor != NULL
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }


    if (
        tralloc_append_destructor  ( text_03, test_destructors_common_empty_destructor_2, history ) != 0 ||
        tralloc_append_destructor  ( text_03, test_destructors_common_empty_destructor_1, NULL )    != 0 ||
        tralloc_prepend_destructor ( text_03, test_destructors_common_empty_destructor_2, NULL )    != 0 ||
        tralloc_append_destructor  ( text_03, test_destructors_common_empty_destructor_2, history ) != 0 ||
        tralloc_prepend_destructor ( text_03, test_destructors_common_empty_destructor_1, history ) != 0 ||
        tralloc_prepend_destructor ( text_03, test_destructors_common_empty_destructor_1, NULL )    != 0
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_1 || destructor->user_data != NULL    || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_1 || destructor->user_data != history || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_2 || destructor->user_data != NULL    || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_2 || destructor->user_data != history || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_1 || destructor->user_data != NULL    || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_2 || destructor->user_data != history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_delete_destructors ( text_03, test_destructors_common_empty_destructor_1, NULL ) != 0 ||

        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_1 || destructor->user_data != history || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_2 || destructor->user_data != NULL    || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_2 || destructor->user_data != history || ( destructor = destructor->next ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_2 || destructor->user_data != history || destructor->next                  != NULL ||
        destructors->last_destructor != destructor
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_delete_destructors_by_function ( text_03, test_destructors_common_empty_destructor_2 ) != 0 ||

        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_1 || destructor->user_data != history || destructor->next != NULL ||
        destructors->last_destructor != destructor
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_delete_destructors_by_data ( text_03, NULL ) != 0 ||

        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL || ( destructor = destructors->first_destructor ) == NULL ||
        destructor->function != test_destructors_common_empty_destructor_1 || destructor->user_data != history || destructor->next != NULL ||
        destructors->last_destructor != destructor ||

        tralloc_delete_destructors_by_data ( text_03, history ) != 0 ||
        ( destructors = _tralloc_get_destructors_from_chunk ( chunk_03 ) ) == NULL ||
        destructors->first_destructor != NULL || destructors->last_destructor != NULL
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( text_02 ) != 0 ||
        tralloc_free ( text_01 ) != 0 ||
        tralloc_free ( text_03 ) != 0 ||
        tralloc_free ( text_04 ) != 0
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        dynarr_get_length ( history ) != 3 ||
        dynarr_get ( history, 0 ) != text_01 ||
        dynarr_get ( history, 1 ) != text_04 ||
        dynarr_get ( history, 2 ) != text_04
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }

    if (
        tralloc_append_destructor  ( strings, test_destructors_common_bad_destructor_2, NULL ) != 0 ||
        tralloc_prepend_destructor ( strings, test_destructors_common_bad_destructor_1, NULL ) != 0
    ) {
        dynarr_free ( history );
        return TRALLOC_FALSE;
    }
    if ( tralloc_free ( strings ) != BAD_DESTRUCTOR_ERROR_2 ) {
        return TRALLOC_FALSE;
    }

    dynarr_free ( history );
    return TRALLOC_TRUE;
}
