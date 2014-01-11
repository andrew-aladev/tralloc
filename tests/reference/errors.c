// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>
#include <tralloc/reference/main.h>


bool test_errors ( tralloc_context * ctx )
{
    if (
        tralloc_move_reference ( NULL, NULL ) != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_clear_references ( NULL )     != TRALLOC_ERROR_CONTEXT_IS_NULL ||
        tralloc_move_reference ( ctx, ctx )   != TRALLOC_ERROR_CHILD_EQUALS_PARENT
    ) {
        return false;
    }

    tralloc_context * empty;
    if (
        tralloc_new              ( ctx, &empty ) != 0 ||
        tralloc_clear_references ( empty )       != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_move_reference   ( empty, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return false;
    }

    tralloc_context * references, * reference;
    if (
        tralloc_with_extensions_new ( ctx, &references, TRALLOC_HAVE_REFERENCES ) != 0 ||
        tralloc_with_extensions_new ( ctx, &reference,  TRALLOC_HAVE_REFERENCE )  != 0 ||
        tralloc_move_reference      ( reference, NULL )       != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_move_reference      ( reference, empty )      != TRALLOC_ERROR_NO_SUCH_EXTENSION     ||
        tralloc_move_reference      ( reference, references ) != 0 ||
        tralloc_move_reference      ( reference, references ) != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_move_reference      ( reference, NULL )       != 0 ||
        tralloc_move_reference      ( reference, NULL )       != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT
    ) {
        return false;
    }

    if (
        tralloc_free ( empty )      != 0 ||
        tralloc_free ( references ) != 0 ||
        tralloc_free ( reference )  != 0
    ) {
        return false;
    }

    return true;
}
