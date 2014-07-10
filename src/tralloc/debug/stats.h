// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#if !defined ( TRALLOC_DEBUG_STATS_H )
#define TRALLOC_DEBUG_STATS_H

#include "../types.h"


tralloc_error _tralloc_debug_stats_after_add_chunk    ( size_t chunk_length, size_t length );
tralloc_error _tralloc_debug_stats_after_resize_chunk ( size_t old_length, size_t length );
tralloc_error _tralloc_debug_stats_before_free_chunk  ( _tralloc_chunk * chunk );

tralloc_error _tralloc_debug_stats_add_chunks_overhead_length      ( size_t length );
tralloc_error _tralloc_debug_stats_subtract_chunks_overhead_length ( size_t length );

tralloc_error tralloc_debug_stats_get_chunks_count           ( size_t * length );
tralloc_error tralloc_debug_stats_get_chunks_overhead_length ( size_t * length );
tralloc_error tralloc_debug_stats_get_chunks_length          ( size_t * length );


#endif
