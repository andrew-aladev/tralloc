// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "stack.h"

extern inline
talloc_stack * talloc_stack_new ( void * ctx );

extern inline
uint8_t talloc_stack_push ( talloc_stack * stack, void * data );

extern inline
uint8_t talloc_stack_free ( void * current_stack, void * user_data );

extern inline
size_t talloc_stack_get_length ( talloc_stack * stack );

extern inline
void talloc_stack_pop ( talloc_stack * stack );