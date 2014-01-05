// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/destructor/main.h>
#include <tralloc/length/main.h>


static
uint8_t empty_destructor ( tralloc_context * UNUSED ( chunk_context ), void * UNUSED ( user_data ) )
{
    return 0;
}

bool test_destructor ( tralloc_context * ctx )
{
    size_t length;
    uint32_t * data;
    if (
        tralloc_with_extensions ( ctx, ( tralloc_context ** ) &data, TRALLOC_HAVE_DESTRUCTORS | TRALLOC_HAVE_LENGTH, sizeof ( uint32_t ) * 20 ) != 0 ||
        tralloc_append_destructor ( data, empty_destructor, NULL ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint32_t ) * 20
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &data, sizeof ( uint32_t ) ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint32_t )
    ) {
        return false;
    }

    return true;
}
