// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "chunk.h"

extern inline _tralloc_chunk * _tralloc_reference_process_new_chunk ( _tralloc_chunk * chunk, const tralloc_context * parent_context );
extern inline _tralloc_chunk * _tralloc_reference_malloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline _tralloc_chunk * _tralloc_reference_calloc_chunk      ( const tralloc_context * parent_context, size_t length );
extern inline void             _tralloc_reference_update            ( _tralloc_reference * reference );
extern inline _tralloc_chunk * _tralloc_reference_realloc_chunk     ( _tralloc_chunk * reference_chunk, size_t length );
extern inline uint8_t          _tralloc_reference_free_chunk        ( _tralloc_chunk * chunk );