// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/references/common.h>
#include <tralloc/tree/alloc.h>
#include <tralloc/tree/free.h>
#include <tralloc/references/move.h>
#include <tralloc/references/clear.h>


tralloc_bool test_references_errors ( tralloc_context * ctx )
{
    if (
        tralloc_reference_move   ( NULL, NULL ) != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_references_clear ( NULL )       != TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED ||
        tralloc_reference_move   ( ctx, ctx )   != TRALLOC_ERROR_CHILD_EQUALS_PARENT
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * empty;
    if (
        tralloc_new_empty        ( ctx, &empty ) != 0 ||
        tralloc_references_clear ( empty )       != TRALLOC_ERROR_NO_SUCH_EXTENSION ||
        tralloc_reference_move   ( empty, NULL ) != TRALLOC_ERROR_NO_SUCH_EXTENSION
    ) {
        return TRALLOC_FALSE;
    }

    tralloc_context * references, * reference;
    if (
        tralloc_new_empty_with_extensions ( ctx, &references, TRALLOC_EXTENSION_REFERENCES ) != 0 ||
        tralloc_new_empty_with_extensions ( ctx, &reference,  TRALLOC_EXTENSION_REFERENCE )  != 0 ||
        tralloc_reference_move ( reference, NULL )       != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_reference_move ( reference, empty )      != TRALLOC_ERROR_NO_SUCH_EXTENSION     ||
        tralloc_reference_move ( reference, references ) != 0 ||
        tralloc_reference_move ( reference, references ) != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT ||
        tralloc_reference_move ( reference, NULL )       != 0 ||
        tralloc_reference_move ( reference, NULL )       != TRALLOC_ERROR_CHILD_HAS_SAME_PARENT
    ) {
        return TRALLOC_FALSE;
    }

    if (
        tralloc_free ( empty )      != 0 ||
        tralloc_free ( references ) != 0 ||
        tralloc_free ( reference )  != 0
    ) {
        return TRALLOC_FALSE;
    }

    return TRALLOC_TRUE;
}
