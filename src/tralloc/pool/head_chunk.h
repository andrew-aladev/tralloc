// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_POOL_HEAD_CHUNK_H
#define TRALLOC_POOL_HEAD_CHUNK_H

#include "../tree/common.h"
#include <stdlib.h>
#include <stdbool.h>


inline
tralloc_error _tralloc_pool_new_chunk ( _tralloc_chunk * chunk, size_t length )
{
    _tralloc_pool *          pool     = _tralloc_get_pool_from_chunk ( chunk );
    _tralloc_pool_fragment * fragment = malloc ( sizeof ( _tralloc_pool_fragment ) );
    if ( fragment == NULL ) {
        return TRALLOC_ERROR_MALLOC_FAILED;
    }
    fragment->offset     = 0;
    fragment->next       = NULL;
    fragment->length     = length;
    pool->first_fragment = fragment;
    pool->autofree       = false;
    return 0;
}

inline
bool _tralloc_pool_try_free_chunk ( _tralloc_chunk * chunk )
{
    _tralloc_pool * pool = _tralloc_get_pool_from_chunk ( chunk );
    if ( pool->first_fragment->next != NULL ) {
        _tralloc_detach_chunk ( chunk );
        pool->autofree = true;
        return false;
    }
    return true;
}


#endif
