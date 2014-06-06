// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/length/main.h>
#include <tralloc/length/chunk.h>
#include <tralloc/common.h>


tralloc_error tralloc_get_length ( tralloc_context * context, size_t * length_result_ptr )
{
    if ( context == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_NULL;
    }
    _tralloc_chunk * chunk = _tralloc_get_chunk_from_context ( context );
    if ( ! ( chunk->extensions & TRALLOC_EXTENSION_LENGTH ) ) {
        return TRALLOC_ERROR_NO_SUCH_EXTENSION;
    }
    _tralloc_length * length_ptr = _tralloc_get_length_from_chunk ( chunk );
    * length_result_ptr          = * length_ptr;
    return 0;
}
