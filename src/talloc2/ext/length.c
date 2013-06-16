// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "length.h"

extern inline
uint8_t talloc_add_length ( talloc_chunk * child, size_t user_length, uint8_t mode );

extern inline
uint8_t talloc_set_length ( talloc_chunk * child, size_t user_length );

extern inline
uint8_t talloc_get_length ( const void * child_data, size_t * result_length );
