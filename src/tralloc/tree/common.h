// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_TREE_COMMON_H
#define TRALLOC_TREE_COMMON_H

#include "../common.h"


tralloc_error tralloc_realloc ( tralloc_context ** chunk_context, size_t length );
tralloc_error tralloc_move    ( tralloc_context * child_context, tralloc_context * parent_context );


tralloc_error _tralloc_add_chunk    ( tralloc_context * parent_context, _tralloc_chunk * child_chunk );
void          _tralloc_attach_chunk ( _tralloc_chunk * child, _tralloc_chunk * new_parent );
void          _tralloc_detach_chunk ( _tralloc_chunk * chunk );


tralloc_error _tralloc_free_single  ( _tralloc_chunk * chunk );
tralloc_error _tralloc_free_subtree ( _tralloc_chunk * root_chunk );

inline
tralloc_error tralloc_free ( tralloc_context * chunk_context )
{
    if ( chunk_context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    return _tralloc_free_subtree ( _tralloc_get_chunk_from_context ( chunk_context ) );
}


#endif
