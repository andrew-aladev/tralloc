// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/destructor/main.h>


tralloc_bool test_destructor_errors ( tralloc_context * ctx )
{
    if (
        tralloc_append_destructor              ( NULL, NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_prepend_destructor             ( NULL, NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_clear_destructors              ( NULL )             != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_delete_destructors             ( NULL, NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_delete_destructors_by_function ( NULL, NULL )       != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL ||
        tralloc_delete_destructors_by_data     ( NULL, NULL )       != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * test;
    if ( tralloc_new ( ctx, &test ) != 0 ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_append_destructor              ( test, NULL, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_prepend_destructor             ( test, NULL, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_clear_destructors              ( test )             != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_delete_destructors             ( test, NULL, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_delete_destructors_by_function ( test, NULL )       != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_delete_destructors_by_data     ( test, NULL )       != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return TRALLOC_FALSE;
    }

    if ( tralloc_free ( test ) != 0 ) {
        return TRALLOC_FALSE;
    }
    return TRALLOC_TRUE;
}
