// This file is part of talloc2. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc2 is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc2. If not, see <http://www.gnu.org/licenses/>.

#ifndef TALLOC_EXT_MAIN_H
#define TALLOC_EXT_MAIN_H

#include "chunk.h"
#include "destructor.h"

inline
void talloc_ext_on_add ( talloc_chunk * chunk )
{
    talloc_destructor_on_add ( chunk, talloc_memory_from_ext_chunk ( chunk ) );
}

inline
uint8_t talloc_ext_on_del ( talloc_chunk * chunk )
{
    return talloc_destructor_on_del ( chunk, talloc_memory_from_ext_chunk ( chunk ) );
}

#endif
