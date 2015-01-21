// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/destructors/common.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/destructors.h>


tralloc_bool test_destructors_errors ( tralloc_context * ctx )
{
    if (
        tralloc_destructor_append              ( NULL, NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_destructor_prepend             ( NULL, NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_destructors_clear              ( NULL )             != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_destructors_delete             ( NULL, NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_destructors_delete_by_function ( NULL, NULL )       != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_destructors_delete_by_data     ( NULL, NULL )       != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * test;
    if ( tralloc_new_empty ( ctx, &test ) != 0 ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_destructor_append              ( test, NULL, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_destructor_prepend             ( test, NULL, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_destructors_clear              ( test )             != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_destructors_delete             ( test, NULL, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_destructors_delete_by_function ( test, NULL )       != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_destructors_delete_by_data     ( test, NULL )       != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( test ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
