// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_REFERENCE_MAIN_H
#define TALLOC_REFERENCE_MAIN_H

#include "chunk.h"

// If child_data is NULL or parent_data is NULL or child_data equals parent_data function will return non-zero value.
// Function will obtain child chunk from child_data, parent chunk from parent_data.
// If child chunk's parent is parent chunk function will return non-zero value.
// Otherwise function will create reference to child_data and attach it to parent chunk.
// Function returns zero or non-zero value if error occurred.
void * talloc_add_reference_with_data      ( const void * child_data, const void * parent_data, size_t length );
void * talloc_add_reference_with_zero_data ( const void * child_data, const void * parent_data, size_t length );

inline
void * talloc_add_reference ( const void * child_data, const void * parent_data )
{
    return talloc_add_reference_with_data ( child_data, parent_data, 0 );
}

// If chunk_data is NULL function will return non-zero value.
// Otherwise function will obtain chunk from chunk_data and delete all it's references.
// Function returns zero or non-zero value if error occurred.
uint8_t talloc_clear_references ( const void * chunk_data );

#endif
