// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include "file.h"

uint8_t _tralloc_close ( tralloc_context * chunk_context, void * user_data )
{
    int * descriptor_ptr = chunk_context;
    if ( close ( * descriptor_ptr ) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}

extern inline int *   _tralloc_process_descriptor ( const tralloc_context * parent_context, int descriptor );
extern inline int *   tralloc_open                ( const tralloc_context * parent_context, const char * path_name, int flags );
extern inline int *   tralloc_open_mode           ( const tralloc_context * parent_context, const char * path_name, int flags, mode_t mode );