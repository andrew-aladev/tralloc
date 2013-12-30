// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <stdlib.h>

#include "tree.h"
#include "chunk.h"

uint8_t tralloc_move ( const tralloc_context * child_context, const tralloc_context * parent_context )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return 1;
    }
    _tralloc_chunk * child = _tralloc_chunk_from_context ( child_context );

#if defined(TRALLOC_DEBUG)
    _tralloc_chunk * old_parent;
#endif

    if ( parent_context == NULL ) {
        if ( child->parent == NULL ) {
            return 0;
        }

#if defined(TRALLOC_DEBUG)
        old_parent = child->parent;
#endif

        _tralloc_detach_chunk ( child );
        child->parent = NULL;
    } else {
        _tralloc_chunk * new_parent = _tralloc_chunk_from_context ( parent_context );
        if ( child->parent == new_parent ) {
            return 0;
        }

#if defined(TRALLOC_DEBUG)
        old_parent = child->parent;
#endif

        _tralloc_detach_chunk ( child );
        _tralloc_set_child_chunk ( new_parent, child );
    }

#if defined(TRALLOC_DEBUG)
    return _tralloc_on_move ( child, old_parent );
#else
    return 0;
#endif

}
