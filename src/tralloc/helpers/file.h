// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_HELPERS_FILE_H )
#define TRALLOC_HELPERS_FILE_H

#include "../tree/alloc.h"

#include <fcntl.h>

#undef _TRALLOC_INLINE
#if defined ( _TRALLOC_INCLUDED_FROM_HELPERS_FILE_C )
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_OBJECT
#else
#    define _TRALLOC_INLINE _TRALLOC_INLINE_IN_HEADER
#endif


tralloc_error _tralloc_open ( _tralloc_alloc_options * options, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode );

#if defined ( TRALLOC_DEBUG_LOG )
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_open_mode_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_extensions extensions, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode )
{
    _tralloc_alloc_options options;
    options.file           = file;
    options.line           = line;
    options.parent_context = parent_context;
    options.extensions     = extensions;
    return _tralloc_open ( &options, descriptor_ptr, path_name, flags, mode );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_open_with_extensions ( const char * file, size_t line, tralloc_context * parent_context, tralloc_extensions extensions, int ** descriptor_ptr, const char * path_name, int flags )
{
    return _tralloc_debug_log_open_mode_with_extensions ( file, line, parent_context, extensions, descriptor_ptr, path_name, flags, 0 );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_open_mode ( const char * file, size_t line, tralloc_context * parent_context, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode )
{
    return _tralloc_debug_log_open_mode_with_extensions ( file, line, parent_context, 0, descriptor_ptr, path_name, flags, mode );
}
_TRALLOC_INLINE
tralloc_error _tralloc_debug_log_open ( const char * file, size_t line, tralloc_context * parent_context, int ** descriptor_ptr, const char * path_name, int flags )
{
    return _tralloc_debug_log_open_mode ( file, line, parent_context, descriptor_ptr, path_name, flags, 0 );
}

#define tralloc_open_mode_with_extensions(...) _tralloc_debug_log_open_mode_with_extensions (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_open_with_extensions(...)      _tralloc_debug_log_open_with_extensions      (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_open_mode(...)                 _tralloc_debug_log_open_mode                 (__FILE__, __LINE__, __VA_ARGS__)
#define tralloc_open(...)                      _tralloc_debug_log_open                      (__FILE__, __LINE__, __VA_ARGS__)

#else

_TRALLOC_INLINE
tralloc_error tralloc_open_mode_with_extensions ( tralloc_context * parent_context, tralloc_extensions extensions, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode )
{
    _tralloc_alloc_options options;
    options.parent_context = parent_context;
    options.extensions     = extensions;
    return _tralloc_open ( &options, descriptor_ptr, path_name, flags, mode );
}
_TRALLOC_INLINE
tralloc_error tralloc_open_with_extensions ( tralloc_context * parent_context, tralloc_extensions extensions, int ** descriptor_ptr, const char * path_name, int flags )
{
    return tralloc_open_mode_with_extensions ( parent_context, extensions, descriptor_ptr, path_name, flags, 0 );
}
_TRALLOC_INLINE
tralloc_error tralloc_open_mode ( tralloc_context * parent_context, int ** descriptor_ptr, const char * path_name, int flags, mode_t mode )
{
    return tralloc_open_mode_with_extensions ( parent_context, 0, descriptor_ptr, path_name, flags, mode );
}
_TRALLOC_INLINE
tralloc_error tralloc_open ( tralloc_context * parent_context, int ** descriptor_ptr, const char * path_name, int flags )
{
    return tralloc_open_mode ( parent_context, descriptor_ptr, path_name, flags, 0 );
}
#endif


#endif
