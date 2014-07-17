// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_DEBUG_CALLBACKS_LOCK_H )
#define TRALLOC_DEBUG_CALLBACKS_LOCK_H

#include "../../types.h"


tralloc_error _tralloc_debug_callback_rdlock_add ();
tralloc_error _tralloc_debug_callback_wrlock_add ();
tralloc_error _tralloc_debug_callback_unlock_add ();

tralloc_error _tralloc_debug_callback_rdlock_move ();
tralloc_error _tralloc_debug_callback_wrlock_move ();
tralloc_error _tralloc_debug_callback_unlock_move ();

tralloc_error _tralloc_debug_callback_rdlock_resize ();
tralloc_error _tralloc_debug_callback_wrlock_resize ();
tralloc_error _tralloc_debug_callback_unlock_resize ();

tralloc_error _tralloc_debug_callback_rdlock_free ();
tralloc_error _tralloc_debug_callback_wrlock_free ();
tralloc_error _tralloc_debug_callback_unlock_free ();


#endif
