// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "move.h"
#include "chunk.h"
#include "../common.h"

#if defined(TRALLOC_DEBUG)
#   include "../debug/chunk.h"
#endif


tralloc_error tralloc_move ( tralloc_context * child_context, tralloc_context * parent_context )
{
    if ( child_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    if ( child_context == parent_context ) {
        return TRALLOC_ERROR_CHILD_EQUALS_PARENT;
    }
    _tralloc_chunk * child_chunk = _tralloc_get_chunk_from_context ( child_context );

#   if defined(TRALLOC_DEBUG)
    _tralloc_chunk * old_parent_chunk;
    tralloc_error result = _tralloc_debug_before_move_chunk ( child_chunk );
    if ( result != 0 ) {
        return result;
    }
#   endif

    if ( parent_context == NULL ) {
        if ( child_chunk->parent == NULL ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }

#       if defined(TRALLOC_DEBUG)
        old_parent_chunk = child_chunk->parent;
#       endif

        _tralloc_detach_chunk ( child_chunk );
    } else {
        _tralloc_chunk * new_parent_chunk = _tralloc_get_chunk_from_context ( parent_context );
        if ( child_chunk->parent == new_parent_chunk ) {
            return TRALLOC_ERROR_CHILD_HAS_SAME_PARENT;
        }

#       if defined(TRALLOC_DEBUG)
        old_parent_chunk = child_chunk->parent;
#       endif

        _tralloc_attach_chunk ( child_chunk, new_parent_chunk );
    }

#   if defined(TRALLOC_DEBUG)
    return _tralloc_debug_after_move_chunk ( child_chunk, old_parent_chunk );
#   else
    return 0;
#   endif

}
