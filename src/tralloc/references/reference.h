// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_REFERENCES_REFERENCE_H )
#define TRALLOC_REFERENCES_REFERENCE_H

#include "../types.h"
#include "../macro.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_REFERENCES_REFERENCE_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


_TRALLOC_INLINE
void _tralloc_new_reference ( _tralloc_reference * reference )
{
    reference->references = NULL;
    reference->next       = NULL;
    reference->prev       = NULL;
}

void          _tralloc_update_reference ( _tralloc_reference * reference );
tralloc_error _tralloc_free_reference   ( _tralloc_reference * reference );


#endif