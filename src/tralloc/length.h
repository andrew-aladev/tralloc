// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_LENGTH_H )
#define TRALLOC_LENGTH_H

#include "types.h"

#if !defined ( TRALLOC_LENGTH )
#   error TRALLOC_LENGTH is required
#endif


typedef struct _tralloc_length_type _tralloc_length;

tralloc_error tralloc_length_get ( tralloc_context * context, size_t * length_result_ptr );


#endif
