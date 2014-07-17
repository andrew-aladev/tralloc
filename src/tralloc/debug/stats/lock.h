// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_DEBUG_STATS_LOCK_H )
#define TRALLOC_DEBUG_STATS_LOCK_H

#include "../../types.h"


tralloc_error _tralloc_debug_stats_rdlock_chunks_count ();
tralloc_error _tralloc_debug_stats_wrlock_chunks_count ();
tralloc_error _tralloc_debug_stats_unlock_chunks_count ();

tralloc_error _tralloc_debug_stats_rdlock_chunks_overhead_length ();
tralloc_error _tralloc_debug_stats_wrlock_chunks_overhead_length ();
tralloc_error _tralloc_debug_stats_unlock_chunks_overhead_length ();

tralloc_error _tralloc_debug_stats_rdlock_chunks_length ();
tralloc_error _tralloc_debug_stats_wrlock_chunks_length ();
tralloc_error _tralloc_debug_stats_unlock_chunks_length ();


#endif
