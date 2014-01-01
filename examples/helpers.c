// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/helpers/string.h>

#if defined(TRALLOC_DESTRUCTOR)
#include <tralloc/helpers/file.h>


uint8_t destructor_unlink_file ( tralloc_context * UNUSED ( chunk_context ), void * user_data )
{
    char * file_name = user_data;
    if ( unlink ( file_name ) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}

#endif

#if defined(TRALLOC_DEBUG)
#include <tralloc/events.h>
#endif

int main ()
{
    tralloc_context * root = tralloc_new ( NULL );
    if ( root == NULL ) {
        return 1;
    }
    char * string = tralloc_strdup ( root, "Some test text." );
    if ( string == NULL ) {
        tralloc_free ( root );
        return 2;
    }
    char * text = tralloc_strndup ( string, string + 10, 4 );
    if ( text == NULL || strcmp ( text, "text" ) != 0 ) {
        tralloc_free ( root );
        return 3;
    }
    char * formatted_text = tralloc_asprintf ( root, "%s %s %s.", "Some", "test", "text" );
    if ( formatted_text == NULL || strcmp ( formatted_text, "Some test text." ) != 0 ) {
        tralloc_free ( root );
        return 4;
    }
    if ( tralloc_free ( root ) != 0 ) {
        return 5;
    }

#if defined(TRALLOC_DESTRUCTOR)
    char * file_name = tralloc_strdup ( NULL, "/tmp/tralloc_test_file" );
    int * test_file  = tralloc_open_mode ( NULL, file_name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ); // 0644
    if ( test_file == NULL ) {
        tralloc_free ( file_name );
        return 6;
    }
    if ( tralloc_move ( file_name, test_file ) != 0 ) {
        tralloc_free ( test_file );
        tralloc_free ( file_name );
        return 7;
    }
    if ( tralloc_append_destructor ( test_file, destructor_unlink_file, file_name ) != 0 ) {
        tralloc_free ( test_file );
        return 8;
    }
    if ( tralloc_free ( test_file ) != 0 ) {
        return 9;
    }
#endif

#if defined(TRALLOC_DEBUG)
    if ( tralloc_get_chunks_count() != 0 ) {
        return 10;
    }
#endif

    return 0;
}
