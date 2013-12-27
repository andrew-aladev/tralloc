// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"

extern inline tralloc_context * _tralloc_context_from_chunk ( _tralloc_chunk * chunk );
extern inline _tralloc_chunk *  _tralloc_chunk_from_context ( const tralloc_context * context );

#ifdef TRALLOC_REFERENCE
extern inline _tralloc_chunk *      _tralloc_chunk_from_references ( _tralloc_references * references );
extern inline _tralloc_references * _tralloc_references_from_chunk ( _tralloc_chunk * chunk );
extern inline _tralloc_chunk *      _tralloc_chunk_from_reference  ( _tralloc_reference * reference );
extern inline _tralloc_reference *  _tralloc_reference_from_chunk  ( _tralloc_chunk * chunk );
#endif