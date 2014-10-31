// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_HELPERS_FILE_C
#include <tralloc/helpers/file.h>
#include <tralloc/tree/free.h>
#include <tralloc/destructors/append.h>

#include <unistd.h>


static
tralloc_error _tralloc_close ( tralloc_context * chunk_context, void * _TRALLOC_UNUSED ( user_data ) )
{
    int * descriptor_ptr = chunk_context;
    if ( close ( * descriptor_ptr ) != 0 ) {
        return TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED;
    }
    return 0;
}

tralloc_error _tralloc_open_mode ( _tralloc_alloc_options * options, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode )
{
    if ( descriptor_ptr == NULL ) {
        return TRALLOC_ERROR_REQUIRED_ARGUMENT_IS_UNDEFINED;
    }
    int descriptor = open ( path_name, flags, mode );
    if ( descriptor == -1 ) {
        return TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED;
    }

    options->zero       = 0;
    options->length     = sizeof ( int );
    options->extensions = options->extensions | TRALLOC_EXTENSION_DESTRUCTORS;

    tralloc_error result = _tralloc_alloc ( options, ( tralloc_context ** ) descriptor_ptr );
    if ( result != 0 ) {
        close ( descriptor );
        return result;
    }

    ** descriptor_ptr = descriptor;
    result = tralloc_append_destructor ( * descriptor_ptr, _tralloc_close, NULL );
    if ( result != 0 ) {
        tralloc_free ( * descriptor_ptr );
        close ( descriptor );
        return result;
    }
    return 0;
}
