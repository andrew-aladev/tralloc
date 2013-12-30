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

uint8_t _tralloc_close ( tralloc_context * chunk_context, void * user_data );

inline
int * _tralloc_process_descriptor ( const tralloc_context * parent_context, int descriptor )
{
    int * descriptor_ptr = tralloc_with_extensions ( parent_context, sizeof ( int ), TRALLOC_HAVE_DESTRUCTORS );
    if ( descriptor_ptr == NULL ) {
        close ( descriptor );
        return NULL;
    }
    * descriptor_ptr = descriptor;
    if ( tralloc_append_destructor ( descriptor_ptr, _tralloc_close, NULL ) != 0 ) {
        tralloc_free ( descriptor_ptr );
        close ( descriptor );
        return NULL;
    }
    return descriptor_ptr;
}

// Function opens descriptor and appends destructor, that will close it.
// Function returns pointer to descriptor or NULL if error occurred.
inline
int * tralloc_open ( const tralloc_context * parent_context, const char * path_name, int flags )
{
    int descriptor = open ( path_name, flags );
    if ( descriptor == -1 ) {
        return NULL;
    }
    return _tralloc_process_descriptor ( parent_context, descriptor );
}

// Function works the same as "tralloc_open". You can pass mode to it.
inline
int * tralloc_open_mode ( const tralloc_context * parent_context, const char * path_name, int flags, mode_t mode )
{
    int descriptor = open ( path_name, flags, mode );
    if ( descriptor == -1 ) {
        return NULL;
    }
    return _tralloc_process_descriptor ( parent_context, descriptor );
}

#endif
