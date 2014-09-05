// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/length/common.h>
#include <tralloc/tree.h>
#include <tralloc/length.h>


tralloc_bool test_length_common ( tralloc_context * ctx )
{
    size_t data_length;
    if (
        tralloc_get_length ( NULL, &data_length ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_get_length ( ctx,  &data_length ) != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * empty;
    if (
        tralloc_new_empty_with_extensions ( ctx, &empty, TRALLOC_EXTENSION_LENGTH ) != 0 ||
        tralloc_get_length ( empty, &data_length ) != 0 ||
        data_length != 0
    ) {
        return TRALLOC_FALSE;
    }

    uint8_t * data;
    if (
        tralloc_new_with_extensions ( ctx, ( tralloc_context ** ) &data, sizeof ( uint8_t ) * 2, TRALLOC_EXTENSION_LENGTH ) != 0 ||
        tralloc_get_length ( data, &data_length ) != 0 ||
        data_length != sizeof ( uint8_t ) * 2
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_realloc ( ( tralloc_context ** ) &data, sizeof ( uint8_t ) * 20 ) != 0 ||
        tralloc_get_length ( data, &data_length ) != 0 ||
        data_length != sizeof ( uint8_t ) * 20
    ) {
        return TRALLOC_FALSE;
    }

    return TRALLOC_TRUE;
}
