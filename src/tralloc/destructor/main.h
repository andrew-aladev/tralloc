// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#ifndef TRALLOC_DESTRUCTOR_H
#define TRALLOC_DESTRUCTOR_H

#include "../macro.h"
#include "../types.h"

#undef INLINE
#ifdef _TRALLOC_DESTRUCTOR_INCLUDED_FROM_OBJECT
#    define INLINE INLINE_IN_OBJECT
#else
#    define INLINE INLINE_IN_HEADER
#endif


tralloc_error tralloc_clear_destructors  ( tralloc_context * chunk_context );
tralloc_error tralloc_append_destructor  ( tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );
tralloc_error tralloc_prepend_destructor ( tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data );


INLINE
tralloc_bool _tralloc_destructor_comparator_by_function ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * _TRALLOC_UNUSED ( user_data ) )
{
    return destructor->function == function;
}

INLINE
tralloc_bool _tralloc_destructor_comparator_by_data ( _tralloc_destructor * destructor, tralloc_destructor_function _TRALLOC_UNUSED ( function ), void * user_data )
{
    return destructor->user_data == user_data;
}

INLINE
tralloc_bool _tralloc_destructor_comparator_strict ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data )
{
    return destructor->function == function && destructor->user_data == user_data;
}

typedef tralloc_bool ( * _tralloc_destructor_comparator ) ( _tralloc_destructor * destructor, tralloc_destructor_function function, void * user_data );

tralloc_error _tralloc_delete_destructors_by_comparator ( tralloc_context * chunk_context, _tralloc_destructor_comparator comparator, tralloc_destructor_function function, void * user_data );

INLINE
tralloc_error tralloc_delete_destructors ( tralloc_context * chunk_context, tralloc_destructor_function function, void * user_data )
{
    return _tralloc_delete_destructors_by_comparator ( chunk_context, _tralloc_destructor_comparator_strict, function, user_data );
}

INLINE
tralloc_error tralloc_delete_destructors_by_function ( tralloc_context * chunk_context, tralloc_destructor_function function )
{
    return _tralloc_delete_destructors_by_comparator ( chunk_context, _tralloc_destructor_comparator_by_function, function, NULL );
}

INLINE
tralloc_error tralloc_delete_destructors_by_data ( tralloc_context * chunk_context, void * user_data )
{
    return _tralloc_delete_destructors_by_comparator ( chunk_context, _tralloc_destructor_comparator_by_data, NULL, user_data );
}


#endif
