// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/length/main.h>


tralloc_bool test_length ( tralloc_context * ctx )
{
    size_t length;
    tralloc_context * empty;
    if (
        tralloc_with_extensions_new ( ctx, &empty, TRALLOC_EXTENSION_LENGTH ) != 0 ||
        tralloc_get_length ( empty, &length ) != 0 ||
        length != 0
    ) {
        return TRALLOC_FALSE;
    }

    uint8_t * data;
    if (
        tralloc_with_extensions ( ctx, ( tralloc_context ** ) &data, TRALLOC_EXTENSION_LENGTH, sizeof ( uint8_t ) * 2 ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint8_t ) * 2
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &data, sizeof ( uint8_t ) * 20 ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint8_t ) * 20
    ) {
        return TRALLOC_FALSE;
    }

    return TRALLOC_TRUE;
}
