// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/main.h>


bool test_specs ( tralloc_context * ctx )
{
    if (
        tralloc_reference_new                  ( NULL, NULL, NULL )       != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference                      ( NULL, NULL, NULL, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_zero                 ( NULL, NULL, NULL, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_with_extensions      ( NULL, NULL, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_zero_with_extensions ( NULL, NULL, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    tralloc_context * reference;
    if (
        tralloc_reference_new                  ( NULL, NULL, &reference )       != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference                      ( NULL, NULL, &reference, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_zero                 ( NULL, NULL, &reference, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_with_extensions      ( NULL, NULL, &reference, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_zero_with_extensions ( NULL, NULL, &reference, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    tralloc_context * empty;
    if ( tralloc_new ( ctx, &empty ) != 0 ) {
        return false;
    }

    if (
        tralloc_reference_new                  ( empty, NULL, NULL )       != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference                      ( empty, NULL, NULL, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_zero                 ( empty, NULL, NULL, 0 )    != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_with_extensions      ( empty, NULL, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_reference_zero_with_extensions ( empty, NULL, NULL, 0, 0 ) != TRALLOC_ERROR_CONTEXT_IS_NULL
    ) {
        return false;
    }

    if (
        tralloc_reference_new                  ( empty, empty, &reference )       != TRALLOC_ERROR_CHILD_EQUALS_PARENT ||
        tralloc_reference                      ( empty, empty, &reference, 0 )    != TRALLOC_ERROR_CHILD_EQUALS_PARENT ||
        tralloc_reference_zero                 ( empty, empty, &reference, 0 )    != TRALLOC_ERROR_CHILD_EQUALS_PARENT ||
        tralloc_reference_with_extensions      ( empty, empty, &reference, 0, 0 ) != TRALLOC_ERROR_CHILD_EQUALS_PARENT ||
        tralloc_reference_zero_with_extensions ( empty, empty, &reference, 0, 0 ) != TRALLOC_ERROR_CHILD_EQUALS_PARENT
    ) {
        return false;
    }

    if (
        tralloc_reference_new                  ( empty, NULL, &reference )       != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_reference                      ( empty, NULL, &reference, 0 )    != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_reference_zero                 ( empty, NULL, &reference, 0 )    != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_reference_with_extensions      ( empty, NULL, &reference, 0, 0 ) != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_reference_zero_with_extensions ( empty, NULL, &reference, 0, 0 ) != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return false;
    }

    tralloc_context * references;
    if ( tralloc_with_extensions_new ( empty, &references, TRALLOC_HAVE_REFERENCES ) != 0 ) {
        return false;
    }

    if (
        tralloc_reference_new                  ( references, empty, &reference )       != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_reference                      ( references, empty, &reference, 0 )    != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_reference_zero                 ( references, empty, &reference, 0 )    != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_reference_with_extensions      ( references, empty, &reference, 0, 0 ) != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_reference_zero_with_extensions ( references, empty, &reference, 0, 0 ) != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT
    ) {
        return false;
    }

    return true;
}
