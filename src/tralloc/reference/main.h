// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_MAIN_H
#define TRALLOC_REFERENCE_MAIN_H

#include "../common.h"


_tralloc_chunk * _tralloc_reference_with_extensions ( _tralloc_chunk * child_chunk, tralloc_context * parent_context, uint8_t extensions, size_t length );

inline
tralloc_context * tralloc_reference_with_extensions ( tralloc_context * child_context, tralloc_context * parent_context, uint8_t extensions, size_t length )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_reference_with_extensions ( _tralloc_chunk_from_context ( child_context ), parent_context, extensions, length );
    if ( chunk == NULL ) {
        return NULL;
    }
    return _tralloc_context_from_chunk ( chunk );
}

inline
tralloc_context * tralloc_reference ( tralloc_context * child_context, tralloc_context * parent_context, size_t length )
{
    return tralloc_reference_with_extensions ( child_context, parent_context, 0, length );
}


_tralloc_chunk * _tralloc_reference_zero_with_extensions ( _tralloc_chunk * child_chunk, tralloc_context * parent_context, uint8_t extensions, size_t length );

inline
tralloc_context * tralloc_reference_zero_with_extensions ( tralloc_context * child_context, tralloc_context * parent_context, uint8_t extensions, size_t length )
{
    if ( child_context == NULL || parent_context == child_context ) {
        return NULL;
    }
    _tralloc_chunk * chunk = _tralloc_reference_zero_with_extensions ( _tralloc_chunk_from_context ( child_context ), parent_context, extensions, length );
    if ( chunk == NULL ) {
        return NULL;
    }
    return _tralloc_context_from_chunk ( chunk );
}

inline
tralloc_context * tralloc_reference_zero ( tralloc_context * child_context, tralloc_context * parent_context, size_t length )
{
    return tralloc_reference_zero_with_extensions ( child_context, parent_context, 0, length );
}


inline
tralloc_context * tralloc_reference_new ( tralloc_context * child_context, tralloc_context * parent_context )
{
    return tralloc_reference ( child_context, parent_context, 0 );
}


uint8_t _tralloc_clear_references ( _tralloc_chunk * chunk );

inline
uint8_t tralloc_clear_references ( tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return 1;
    }
    return _tralloc_clear_references ( _tralloc_chunk_from_context ( chunk_context ) );
}


#endif
