// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_HELPERS_FILE_H
#define TRALLOC_HELPERS_FILE_H

#include "../tree.h"
#include "../destructor/main.h"
#include <fcntl.h>
#include <unistd.h>


inline
tralloc_error _tralloc_close ( tralloc_context * chunk_context, void * UNUSED ( user_data ) )
{
    int * descriptor_ptr = chunk_context;
    if ( close ( * descriptor_ptr ) != 0 ) {
        return TRALLOC_ERROR_CLOSE_DESCRIPTOR_FAILED;
    } else {
        return 0;
    }
}

inline
tralloc_error _tralloc_process_descriptor ( tralloc_context * parent_context, int ** descriptor_ptr, int descriptor, uint8_t extensions )
{
    tralloc_error result = tralloc_with_extensions ( parent_context, ( tralloc_context ** ) descriptor_ptr, extensions | TRALLOC_HAVE_DESTRUCTORS, sizeof ( int ) );
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

inline
tralloc_error tralloc_open_with_extensions ( tralloc_context * parent_context, int ** descriptor_ptr, uint8_t extensions, const char * path_name, int flags )
{
    int descriptor = open ( path_name, flags );
    if ( descriptor == -1 ) {
        return TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED;
    }
    return _tralloc_process_descriptor ( parent_context, descriptor_ptr, descriptor, extensions );
}

inline
tralloc_error tralloc_open_mode_with_extensions ( tralloc_context * parent_context, int ** descriptor_ptr, uint8_t extensions, const char * path_name, int flags, mode_t mode )
{
    int descriptor = open ( path_name, flags, mode );
    if ( descriptor == -1 ) {
        return TRALLOC_ERROR_OPEN_DESCRIPTOR_FAILED;
    }
    return _tralloc_process_descriptor ( parent_context, descriptor_ptr, descriptor, extensions );
}

inline
tralloc_error tralloc_open ( tralloc_context * parent_context, int ** descriptor_ptr, const char * path_name, int flags )
{
    return tralloc_open_with_extensions ( parent_context, descriptor_ptr, 0, path_name, flags );
}

inline
tralloc_error tralloc_open_mode ( tralloc_context * parent_context, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode )
{
    return tralloc_open_mode_with_extensions ( parent_context, descriptor_ptr, 0, path_name, flags, mode );
}


#endif
