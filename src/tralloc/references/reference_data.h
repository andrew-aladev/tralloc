// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_REFERENCES_REFERENCE_DATA_H )
#define TRALLOC_REFERENCES_REFERENCE_DATA_H

#include "../references.h"
#include "../extensions.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_REFERENCES_REFERENCE_DATA_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


struct _tralloc_reference_type {
    _tralloc_references * references;
    _tralloc_reference * prev;
    _tralloc_reference * next;
};

_TRALLOC_INLINE
void _tralloc_reference_new ( _tralloc_reference * reference )
{
    reference->references = NULL;
    reference->next       = NULL;
    reference->prev       = NULL;
}

void          _tralloc_reference_update ( _tralloc_reference * reference );
tralloc_error _tralloc_reference_free   ( _tralloc_reference * reference );

_TRALLOC_INLINE
_tralloc_reference * _tralloc_chunk_get_reference ( _tralloc_chunk * chunk )
{
    return ( _tralloc_reference * ) ( ( uintptr_t ) chunk - _tralloc_extensions_get_offset_for_extension ( chunk->extensions, TRALLOC_EXTENSION_REFERENCE ) );
}


#endif
