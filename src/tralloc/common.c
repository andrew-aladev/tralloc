// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_COMMON_INCLUDED_FROM_OBJECT
#include <tralloc/common.h>


// Extension's structures are situated in the memory in the following order : pool / pool_child, references, reference, destructors, length.

// Function returns size of extensions and chunk.
size_t tralloc_predict_chunk_length ( tralloc_extensions _TRALLOC_UNUSED ( extensions ) )
{
    size_t offset = 0;

#   if defined(TRALLOC_POOL)
    if ( extensions & TRALLOC_EXTENSION_POOL ) {
        offset += sizeof ( _tralloc_pool );
    } else if ( extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        offset += sizeof ( _tralloc_pool_child );
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    if ( extensions & TRALLOC_EXTENSION_REFERENCES ) {
        offset += sizeof ( _tralloc_references );
    }
    if ( extensions & TRALLOC_EXTENSION_REFERENCE ) {
        offset += sizeof ( _tralloc_reference );
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    if ( extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        offset += sizeof ( _tralloc_destructors );
    }
#   endif

#   if defined(TRALLOC_LENGTH)
    if ( extensions & TRALLOC_EXTENSION_LENGTH ) {
        offset += sizeof ( _tralloc_length );
    }
#   endif

#   if defined(TRALLOC_THREADS)
    if ( extensions & TRALLOC_EXTENSION_LOCK_SUBTREE ) {
        offset += sizeof ( _tralloc_mutex );
    }
    if ( extensions & TRALLOC_EXTENSION_LOCK_CHILDREN ) {
        offset += sizeof ( _tralloc_mutex );
    }
#   endif

    return offset + sizeof ( _tralloc_chunk );
}


#if defined(TRALLOC_EXTENSIONS)

// Extension is the one bit of the extensions, "extension & extensions" can be false.
// Function returns offset of the extension's structure from the bottom.
size_t _tralloc_get_offset_for_extension ( tralloc_extensions extensions, _tralloc_extension _TRALLOC_UNUSED ( extension ) )
{
    size_t offset = 0;

#   if defined(TRALLOC_THREADS)
    if ( extensions & TRALLOC_EXTENSION_LOCK_SUBTREE ) {
        offset += sizeof ( _tralloc_mutex );
    }
    if ( extension == TRALLOC_EXTENSION_LOCK_SUBTREE ) {
        return offset;
    }

    if ( extensions & TRALLOC_EXTENSION_LOCK_CHILDREN ) {
        offset += sizeof ( _tralloc_mutex );
    }
    if ( extension == TRALLOC_EXTENSION_LOCK_CHILDREN ) {
        return offset;
    }
#   endif

#   if defined(TRALLOC_LENGTH)
    if ( extensions & TRALLOC_EXTENSION_LENGTH ) {
        offset += sizeof ( _tralloc_length );
    }
    if ( extension == TRALLOC_EXTENSION_LENGTH ) {
        return offset;
    }
#   endif

#   if defined(TRALLOC_DESTRUCTOR)
    if ( extensions & TRALLOC_EXTENSION_DESTRUCTORS ) {
        offset += sizeof ( _tralloc_destructors );
    }
    if ( extension == TRALLOC_EXTENSION_DESTRUCTORS ) {
        return offset;
    }
#   endif

#   if defined(TRALLOC_REFERENCE)
    // First is reference, than references. It is important.

    if ( extensions & TRALLOC_EXTENSION_REFERENCE ) {
        offset += sizeof ( _tralloc_reference );
    }
    if ( extension == TRALLOC_EXTENSION_REFERENCE ) {
        return offset;
    }

    if ( extensions & TRALLOC_EXTENSION_REFERENCES ) {
        offset += sizeof ( _tralloc_references );
    }
    if ( extension == TRALLOC_EXTENSION_REFERENCES ) {
        return offset;
    }
#   endif

#   if defined(TRALLOC_POOL)
    if ( extensions & TRALLOC_EXTENSION_POOL ) {
        offset += sizeof ( _tralloc_pool );
    } else if ( extensions & TRALLOC_EXTENSION_POOL_CHILD ) {
        offset += sizeof ( _tralloc_pool_child );
    }
#   endif

    return offset;
}

#endif
