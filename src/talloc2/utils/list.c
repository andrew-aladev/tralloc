// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "list.h"

extern inline
talloc_list * talloc_list_new ( void * ctx );

extern inline
uint8_t talloc_list_append ( talloc_list * list, void * data );

extern inline
uint8_t talloc_list_free ( void * current_list );

extern inline
size_t talloc_list_get_length ( talloc_list * list );

extern inline
void talloc_list_pop ( talloc_list * list );