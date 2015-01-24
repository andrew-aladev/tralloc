// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_REFERENCES_REFERENCES_DATA_H )
#define TRALLOC_REFERENCES_REFERENCES_DATA_H

#include "reference_data.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_REFERENCES_REFERENCES_DATA_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


// References are represented by double linked list.
// Order of references is given by user (move_reference function).

struct _tralloc_references_type {
    _tralloc_reference * first_reference;
    tralloc_extensions extensions;
    tralloc_bool autofree;
};

_TRALLOC_INLINE
void _tralloc_references_new ( _tralloc_references * references, tralloc_extensions extensions )
{
    references->first_reference = NULL;
    references->extensions      = extensions;
    references->autofree        = TRALLOC_FALSE;
}

_TRALLOC_INLINE
void _tralloc_references_update ( _tralloc_references * references )
{
    _tralloc_reference  * reference = references->first_reference;
    while ( reference != NULL ) {
        reference->references = references;
        reference = reference->next;
    }
}

_TRALLOC_INLINE
tralloc_bool _tralloc_references_can_be_freed ( _tralloc_references * references )
{
    if ( references->first_reference == NULL ) {
        return TRALLOC_TRUE;
    } else {
        if ( !references->autofree ) {
            references->autofree = TRALLOC_TRUE;
        }
        return TRALLOC_FALSE;
    }
}

_TRALLOC_INLINE
tralloc_bool _tralloc_references_can_free_children (  _tralloc_references * references )
{
    if ( references->first_reference == NULL ) {
        return TRALLOC_TRUE;
    } else {
        return TRALLOC_FALSE;
    }
}

_TRALLOC_INLINE
_tralloc_references * _tralloc_chunk_get_references ( _tralloc_chunk * chunk )
{
    return ( _tralloc_references * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_REFERENCES ) );
}

_TRALLOC_INLINE
_tralloc_chunk * _tralloc_references_get_chunk ( _tralloc_references * references )
{
    return ( _tralloc_chunk * ) ( ( uintptr_t ) references + _tralloc_extensions_get_offset_for_extension ( references->extensions, TRALLOC_EXTENSION_REFERENCES ) );
}


#endif
