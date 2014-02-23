// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"


extern inline size_t tralloc_predict_chunk_length ( tralloc_extensions extensions );

extern inline tralloc_context * _tralloc_get_context_from_chunk ( _tralloc_chunk * chunk );
extern inline _tralloc_chunk *  _tralloc_get_chunk_from_context ( tralloc_context * context );

#if defined(TRALLOC_LENGTH)
extern inline _tralloc_length * _tralloc_get_length_from_chunk ( _tralloc_chunk * chunk );
#endif

#if defined(TRALLOC_DESTRUCTOR)
extern inline _tralloc_destructors * _tralloc_get_destructors_from_chunk ( _tralloc_chunk * chunk );
#endif

#if defined(TRALLOC_REFERENCE)
extern inline size_t                _tralloc_get_reference_offset      ( _tralloc_chunk * chunk );
extern inline _tralloc_references * _tralloc_get_references_from_chunk ( _tralloc_chunk * chunk );
extern inline _tralloc_chunk *      _tralloc_get_chunk_from_references ( _tralloc_references * references );
extern inline _tralloc_reference *  _tralloc_get_reference_from_chunk  ( _tralloc_chunk * chunk );
#endif

#if defined(TRALLOC_POOL)
extern inline size_t                _tralloc_get_pool_offset           ( _tralloc_chunk * chunk );
extern inline _tralloc_pool_child * _tralloc_get_pool_child_from_chunk ( _tralloc_chunk * chunk );
extern inline _tralloc_pool *       _tralloc_get_pool_from_chunk       ( _tralloc_chunk * chunk );
extern inline _tralloc_chunk *      _tralloc_get_chunk_from_pool       ( _tralloc_pool  * pool );
#endif
