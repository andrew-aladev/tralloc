// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/main.h>
#include <tralloc/length/main.h>


bool test_reference ( tralloc_context * ctx )
{
    size_t length;
    void * empty;
    uint8_t * data;
    if (
        tralloc_new ( ctx, &empty ) != 0 ||
        tralloc_with_extensions ( ctx, ( tralloc_context ** ) &data, TRALLOC_HAVE_REFERENCES | TRALLOC_HAVE_LENGTH, sizeof ( uint8_t ) * 4 ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint8_t ) * 4
    ) {
        return false;
    }

    uint16_t * reference_1;
    if (
        tralloc_reference_with_extensions ( data, empty, ( tralloc_context ** ) &reference_1, TRALLOC_HAVE_LENGTH, sizeof ( uint16_t ) * 25 ) != 0 ||
        tralloc_get_length ( reference_1, &length ) != 0 ||
        length != sizeof ( uint16_t ) * 25
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &reference_1, sizeof ( uint16_t ) * 2 ) != 0 ||
        tralloc_get_length ( reference_1, &length ) != 0 ||
        length != sizeof ( uint16_t ) * 2
    ) {
        return false;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &data, sizeof ( uint8_t ) * 40 ) != 0 ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint8_t ) * 40
    ) {
        return false;
    }

    return true;
}
