// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>


bool test_str_errors ( tralloc_context * ctx )
{
    if (
        tralloc_strdup   ( NULL, NULL, NULL )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_strndup  ( NULL, NULL, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_asprintf ( NULL, NULL, "\n" )    != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

#if defined(TRALLOC_EXTENSIONS)
    if (
        tralloc_strdup_with_extensions   ( NULL, NULL, 0, NULL )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_strndup_with_extensions  ( NULL, NULL, 0, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_asprintf_with_extensions ( NULL, NULL, 0, "\n" )    != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }
#endif

    char * empty;
    if (
        tralloc_strdup  ( ctx, &empty, NULL )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_strndup ( ctx, &empty, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

#if defined(TRALLOC_EXTENSIONS)
    if (
        tralloc_strdup_with_extensions  ( ctx, &empty, 0, NULL )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_strndup_with_extensions ( ctx, &empty, 0, NULL, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }
#endif

    if ( tralloc_asprintf ( ctx, &empty, NULL ) != TRALLOC_ERROR_PRINTF_FAILED ) {
        return false;
    }

#if defined(TRALLOC_EXTENSIONS)
    if ( tralloc_asprintf_with_extensions ( ctx, &empty, 0, NULL ) != TRALLOC_ERROR_PRINTF_FAILED ) {
        return false;
    }
#endif

    return true;
}

bool test_str ( tralloc_context * ctx )
{
    if ( !test_str_errors ( ctx ) ) {
        return false;
    }

    char * str = "Viktor Tsoi Star Called Sun";
    char * full;
    if (
        tralloc_strdup ( ctx, &full, str ) != 0 ||
        strcmp ( full, str ) != 0
    ) {
        return false;
    }

    char * walk = str;
    char * part_1;
    if (
        tralloc_strndup ( full, &part_1, walk, 6 ) != 0 ||
        strcmp ( part_1, "Viktor" ) != 0
    ) {
        return false;
    }

    walk += 7;
    char * part_2;
    if (
        tralloc_strndup ( full, &part_2, walk, 4 ) != 0 ||
        strcmp ( part_2, "Tsoi" ) != 0
    ) {
        return false;
    }

    walk += 5;
    char * part_3;
    if (
        tralloc_strndup ( full, &part_3, walk, 4 ) != 0 ||
        strcmp ( part_3, "Star" ) != 0
    ) {
        return false;
    }

    walk += 5;
    char * part_4;
    if (
        tralloc_strndup ( full, &part_4, walk, 6 ) != 0 ||
        strcmp ( part_4, "Called" ) != 0
    ) {
        return false;
    }

    walk += 7;
    char * part_5;
    if (
        tralloc_strndup ( full, &part_5, walk, 3 ) != 0 ||
        strcmp ( part_5, "Sun" ) != 0
    ) {
        return false;
    }

    char * formatted_string;
    if (
        tralloc_asprintf ( full, &formatted_string, "%s %s %s %s %s", part_1, part_2, part_3, part_4, part_5 ) != 0 ||
        strcmp ( formatted_string, "Viktor Tsoi Star Called Sun" ) != 0
    ) {
        return false;
    }

    if ( tralloc_free ( full ) != 0 ) {
        return false;
    }
    return true;
}
