// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"
#include <tralloc/tree.h>


int test_utils ( tralloc_context * ctx )
{
    tralloc_context * root;
    if ( tralloc_new ( ctx, &root ) != 0 ) {
        return 1;
    }

#   if defined(TRALLOC_UTILS_BUFFER)
    if ( !test_utils_buffer ( root ) ) {
        tralloc_free ( root );
        return 2;
    }
#   endif

    if ( tralloc_free ( root ) != 0 ) {
        return 3;
    }
    return 0;
}
