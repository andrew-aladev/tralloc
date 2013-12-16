// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_MAIN_H
#define TRALLOC_REFERENCE_MAIN_H

#include "chunk.h"

// Function works the same as "tralloc_add_reference".
// Function returns pointer to memory (with length size) or NULL if error occurred.
tralloc_context * tralloc_add_reference_with_data ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length );

// Function works the same as "tralloc_add_reference_with_data".
// It will use calloc instead of malloc to allocate new reference.
tralloc_context * tralloc_add_reference_with_zero_data ( const tralloc_context * child_context, const tralloc_context * parent_context, size_t length );

// If child_context is NULL or child_context equals parent_context function will return non-zero value.
// Function will obtain child chunk from child_context, parent chunk from parent_context.
// If child chunk's parent is parent chunk function will return non-zero value.
// Otherwise function will create reference to child_context and attach it to parent chunk.
// Function returns pointer to memory (with zero size) or NULL if error occurred.
// This memory should not be used for storing information.
inline
tralloc_context * tralloc_add_reference ( const tralloc_context * child_context, const tralloc_context * parent_context )
{
    return tralloc_add_reference_with_data ( child_context, parent_context, 0 );
}

// If chunk_context is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_context and delete all it's references.
// Function returns zero or non-zero value if error occurred.
uint8_t tralloc_clear_references ( const tralloc_context * chunk_context );

#endif
