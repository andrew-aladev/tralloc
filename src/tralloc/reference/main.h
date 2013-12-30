// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_MAIN_H
#define TRALLOC_REFERENCE_MAIN_H

#include "../common.h"


// If child_context is NULL or child_context equals parent_context function will return non-zero value.
// Function will obtain child chunk from child_context, parent chunk from parent_context.
// If child chunk's parent is parent chunk function will return non-zero value.
// Otherwise function will create reference to child_context and attach it to parent chunk.
// Function returns pointer to memory (with zero size) or NULL if error occurred.
// This memory should not be used for storing information.

_tralloc_chunk * _tralloc_reference_with_extensions ( _tralloc_chunk * child_chunk, _tralloc_chunk * parent_chunk, size_t length, uint8_t extensions );

inline
tralloc_context * tralloc_reference_with_extensions ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length, uint8_t extensions )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_reference_with_extensions ( _tralloc_chunk_from_context ( child_context ), _tralloc_chunk_from_context ( parent_context ), length, extensions );
    return _tralloc_context_from_chunk ( chunk );
}

inline
tralloc_context * tralloc_reference ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length )
{
    return tralloc_reference_with_extensions ( child_context, parent_context, length, 0 );
}

inline
tralloc_context * tralloc_reference_new ( const tralloc_context * child_context, const tralloc_context * parent_context )
{
    return tralloc_reference ( child_context, parent_context, 0 );
}


_tralloc_chunk * _tralloc_reference_zero_with_extensions ( _tralloc_chunk * child_chunk, _tralloc_chunk * parent_chunk, size_t length, uint8_t extensions );

inline
tralloc_context * tralloc_reference_zero_with_extensions ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length, uint8_t extensions )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_reference_zero_with_extensions ( _tralloc_chunk_from_context ( child_context ), _tralloc_chunk_from_context ( parent_context ), length, extensions );
    return _tralloc_context_from_chunk ( chunk );
}

inline
tralloc_context * tralloc_reference_zero ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length )
{
    return tralloc_reference_zero_with_extensions ( child_context, parent_context, length, 0 );
}


uint8_t _tralloc_clear_references ( _tralloc_chunk * chunk );

// Function will delete all references from chunk.
// Function returns zero or non-zero value if error occurred.
inline
uint8_t tralloc_clear_references ( const tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    return _tralloc_clear_references ( _tralloc_chunk_from_context ( chunk_context ) );
}


#endif
