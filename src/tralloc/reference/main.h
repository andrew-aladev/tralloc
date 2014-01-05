// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_REFERENCE_MAIN_H
#define TRALLOC_REFERENCE_MAIN_H

#include "../common.h"


tralloc_error tralloc_reference_with_extensions ( tralloc_context * child_context, tralloc_context * parent_context, tralloc_context ** chunk_context, uint8_t extensions, size_t length );

inline
tralloc_error tralloc_reference ( tralloc_context * child_context, tralloc_context * parent_context, tralloc_context ** chunk_context, size_t length )
{
    return tralloc_reference_with_extensions ( child_context, parent_context, chunk_context, 0, length );
}

tralloc_error tralloc_reference_zero_with_extensions ( tralloc_context * child_context, tralloc_context * parent_context, tralloc_context ** chunk_context, uint8_t extensions, size_t length );

inline
tralloc_error tralloc_reference_zero ( tralloc_context * child_context, tralloc_context * parent_context, tralloc_context ** chunk_context, size_t length )
{
    return tralloc_reference_zero_with_extensions ( child_context, parent_context, chunk_context, 0, length );
}


inline
tralloc_error tralloc_reference_new ( tralloc_context * child_context, tralloc_context * parent_context, tralloc_context ** chunk_context )
{
    return tralloc_reference ( child_context, parent_context, chunk_context, 0 );
}


tralloc_error tralloc_clear_references ( tralloc_context * chunk_context );


#endif
