// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_REFERENCES_H )
#define TRALLOC_REFERENCES_H

#include "types.h"

#if !defined ( TRALLOC_REFERENCES )
#   error TRALLOC_REFERENCES is required
#endif


tralloc_error tralloc_reference_move ( tralloc_context * child_context, tralloc_context * parent_context );

tralloc_error tralloc_references_clear ( tralloc_context * context );


#endif
