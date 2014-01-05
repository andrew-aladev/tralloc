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
    void * empty   = tralloc_new ( ctx );
    uint8_t * data = tralloc_with_extensions ( ctx, TRALLOC_HAVE_REFERENCES | TRALLOC_HAVE_LENGTH, sizeof ( uint8_t ) * 4 );
    if (
        empty == NULL ||
        data  == NULL ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint8_t ) * 4
    ) {
        return false;
    }

    uint16_t * reference_1 = tralloc_reference_with_extensions ( data, empty, TRALLOC_HAVE_LENGTH, sizeof ( uint16_t ) * 25 );
    if (
        reference_1 == NULL ||
        tralloc_get_length ( reference_1, &length ) != 0 ||
        length != sizeof ( uint16_t ) * 25
    ) {
        return false;
    }

    reference_1 = tralloc_realloc ( reference_1, sizeof ( uint16_t ) * 2 );
    if (
        reference_1 == NULL ||
        tralloc_get_length ( reference_1, &length ) != 0 ||
        length != sizeof ( uint16_t ) * 2
    ) {
        return false;
    }

    data = tralloc_realloc ( data, sizeof ( uint8_t ) * 40 );
    if (
        data == NULL ||
        tralloc_get_length ( data, &length ) != 0 ||
        length != sizeof ( uint8_t ) * 40
    ) {
        return false;
    }

    return true;
}
