// This file is part of talloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// talloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// talloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with talloc. If not, see <http://www.gnu.org/licenses/>.

#include "helpers.h"
#include "ext.h"

extern inline
void * talloc_data_from_chunk ( talloc_chunk * chunk );

extern inline
talloc_chunk * talloc_chunk_from_data ( const void * data );

extern inline
void * talloc_new ( const void * parent_data );

extern inline
char * talloc_strndup ( const void * parent_data, const char * str, size_t length );

extern inline
char * talloc_strdup ( const void * parent_data, const char * str );

#ifdef TALLOC_EXT
extern inline
void talloc_ext_on_del ( talloc_chunk * parent );

extern inline
talloc_ext * get_ext ( talloc_chunk * parent );
#endif

#ifdef TALLOC_EXT_DESTRUCTOR
extern inline
uint8_t talloc_set_destructor ( const void * parent_data, talloc_destructor destructor );

extern inline
void talloc_destructor_on_del ( talloc_chunk * parent );
#endif
