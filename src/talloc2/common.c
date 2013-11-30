// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#include "common.h"

extern inline void *         talloc_data_from_chunk ( talloc_chunk * chunk );
extern inline talloc_chunk * talloc_chunk_from_data ( const void * data );

#ifdef TALLOC_EXT
extern inline talloc_chunk * talloc_chunk_from_ext ( talloc_ext * ext );
extern inline talloc_ext *   talloc_ext_from_chunk ( talloc_chunk * chunk );
#endif

#ifdef TALLOC_REFERENCE
extern inline talloc_chunk *     talloc_chunk_from_reference ( talloc_reference * reference );
extern inline talloc_reference * talloc_reference_from_chunk ( talloc_chunk * chunk );
#endif