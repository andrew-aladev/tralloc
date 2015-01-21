// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_EXTENSIONS_H )
#define TRALLOC_EXTENSIONS_H

#include "macro.h"
#include "types.h"

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_EXTENSIONS_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


#if defined ( TRALLOC_LENGTH )
typedef struct _tralloc_length_type _tralloc_length;
#endif

#if defined ( TRALLOC_DESTRUCTORS )
typedef tralloc_error ( * tralloc_destructor_function ) ( tralloc_context * chunk_context, void * user_data );
typedef struct _tralloc_destructors_type _tralloc_destructors;
#endif

#if defined ( TRALLOC_REFERENCES )
typedef struct _tralloc_reference_type  _tralloc_reference;
typedef struct _tralloc_references_type _tralloc_references;
#endif

#if defined ( TRALLOC_POOL )
typedef struct _tralloc_pool_child_type _tralloc_pool_child;
typedef struct _tralloc_pool_type       _tralloc_pool;
#endif

_TRALLOC_INLINE
void _tralloc_extensions_enable_extension ( tralloc_extensions * extensions, _tralloc_extension extension )
{
    * extensions |= extension;
}

_TRALLOC_INLINE
void _tralloc_extensions_disable_extension ( tralloc_extensions * extensions, _tralloc_extension extension )
{
    * extensions &= ~extension;
}

_TRALLOC_INLINE
void _tralloc_extensions_toggle_extension ( tralloc_extensions * extensions, _tralloc_extension extension )
{
    * extensions ^= extension;
}

_TRALLOC_INLINE
tralloc_bool _tralloc_extensions_have_extension ( tralloc_extensions extensions, _tralloc_extension extension )
{
    return ( extensions & extension ) == extension;
}

_TRALLOC_INLINE
tralloc_extensions _tralloc_extensions_get_original_by_forced ( tralloc_extensions extensions, tralloc_extensions forced_extensions )
{
    return extensions ^ forced_extensions;
}

size_t _tralloc_extensions_get_length               ( tralloc_extensions extensions );
size_t _tralloc_extensions_get_offset_for_extension ( tralloc_extensions extensions, _tralloc_extension extension );

// Function returns size of extensions and chunk.
size_t tralloc_predict_chunk_length ( tralloc_extensions extensions );


#endif
