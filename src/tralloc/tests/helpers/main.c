// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tests/helpers/common.h>
#include <tralloc/debug.h>


int main ()
{
    int result = test_helpers ( NULL );
    if ( result != 0 ) {
        return result;
    }

#   if defined ( TRALLOC_DEBUG_STATS )
    size_t chunks_count, overhead_length, data_length;
    if (
        tralloc_debug_stats_get_chunks_count           ( &chunks_count )    != 0 || chunks_count    != 0 ||
        tralloc_debug_stats_get_chunks_overhead_length ( &overhead_length ) != 0 || overhead_length != 0 ||
        tralloc_debug_stats_get_chunks_length          ( &data_length )     != 0 || data_length     != 0
    ) {
        return 255;
    }
#   endif

    return 0;
}
