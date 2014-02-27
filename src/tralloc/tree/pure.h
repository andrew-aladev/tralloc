// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TREE_PURE_H
#define TRALLOC_TREE_PURE_H

#include "../macro.h"
#include "../types.h"

#undef INLINE
#ifdef _TRALLOC_TREE_PURE_INCLUDED_FROM_OBJECT
#    define INLINE INLINE_IN_OBJECT
#else
#    define INLINE INLINE_IN_HEADER
#endif


tralloc_error tralloc      ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length );
tralloc_error tralloc_zero ( tralloc_context * parent_context, tralloc_context ** child_context, size_t length );

INLINE
tralloc_error tralloc_new ( tralloc_context * parent_context, tralloc_context ** child_context )
{
    return tralloc ( parent_context, child_context, 0 );
}

INLINE
tralloc_bool _tralloc_can_free_chunk ( _tralloc_chunk * UNUSED ( chunk ) )
{
    return TRALLOC_TRUE;
}

INLINE
tralloc_bool _tralloc_can_free_chunk_children ( _tralloc_chunk * UNUSED ( chunk ) )
{
    return TRALLOC_TRUE;
}

tralloc_error _tralloc_free_chunk ( _tralloc_chunk * chunk );


#endif
