// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#define _TRALLOC_INCLUDED_FROM_REFERENCES_REFERENCE_C
#include <tralloc/references/reference.h>
#include <tralloc/references/chunk.h>
#include <tralloc/tree/free.h>


void _tralloc_update_reference ( _tralloc_reference * reference )
{
    _tralloc_reference * prev = reference->prev;
    _tralloc_reference * next = reference->next;

    if ( prev != NULL ) {
        prev->next = reference;
    } else {
        _tralloc_references * references = reference->references;
        if ( references != NULL ) {
            // "reference" is the first reference in "reference->references".
            references->first_reference = reference;
        }
    }
    if ( next != NULL ) {
        next->prev = reference;
    }
}

tralloc_error _tralloc_free_reference ( _tralloc_reference * reference )
{
    _tralloc_reference * prev = reference->prev;
    _tralloc_reference * next = reference->next;

    if ( prev == NULL ) {
        _tralloc_references * references = reference->references;
        if ( references != NULL ) {
            references->first_reference = next;
            if ( next == NULL ) {
                // "reference" is the last reference of "reference->references".
                if ( references->autofree ) {
                    return _tralloc_free_subtree ( _tralloc_references_get_chunk ( references ) );
                }
                return 0;
            }
        }
    } else {
        prev->next = next;
    }
    if ( next != NULL ) {
        next->prev = prev;
    }

    return 0;
}
