// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_DESTRUCTORS_H )
#define TRALLOC_DESTRUCTORS_H

#include "types.h"

#if !defined ( TRALLOC_DESTRUCTORS )
#   error TRALLOC_DESTRUCTORS is required
#endif


typedef tralloc_error ( * tralloc_destructor_function ) ( tralloc_context * chunk_context, void * user_data );

tralloc_error tralloc_destructor_append  ( tralloc_context * context, tralloc_destructor_function function, void * user_data );
tralloc_error tralloc_destructor_prepend ( tralloc_context * context, tralloc_destructor_function function, void * user_data );

tralloc_error tralloc_destructors_clear ( tralloc_context * context );

tralloc_error tralloc_destructors_delete             ( tralloc_context * context, tralloc_destructor_function function, void * user_data );
tralloc_error tralloc_destructors_delete_by_function ( tralloc_context * context, tralloc_destructor_function function );
tralloc_error tralloc_destructors_delete_by_data     ( tralloc_context * context, void * user_data );


#endif
