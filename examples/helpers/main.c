// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Lesser Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more details.
// You should have received a copy of the GNU General Lesser Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/tree.h>
#include <tralloc/helpers/string.h>

#if defined(TRALLOC_DEBUG)
#   include <tralloc/debug.h>
#endif

#if defined(TRALLOC_FILE)

#include <tralloc/helpers/file.h>
#include <tralloc/destructor.h>

#include <sys/stat.h>
#include <unistd.h>


tralloc_error destructor_unlink_file ( tralloc_context * _TRALLOC_UNUSED ( chunk_context ), void * user_data )
{
    char * file_name = user_data;
    if ( unlink ( file_name ) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}

#endif

int main ()
{
    tralloc_context * ctx;
    if ( tralloc_new ( NULL, &ctx ) != 0 ) {
        return 1;
    }
    char * string;
    if ( tralloc_strdup ( ctx, &string, "Some test text." ) != 0 ) {
        tralloc_free ( ctx );
        return 2;
    }
    char * text;
    if (
        tralloc_strndup ( string, &text, string + 10, 4 ) != 0 ||
        strcmp ( text, "text" ) != 0
    ) {
        tralloc_free ( ctx );
        return 3;
    }
    char * formatted_text;
    if (
        tralloc_asprintf ( ctx, &formatted_text, "%s %s %s.", "Some", "test", "text" ) != 0 ||
        strcmp ( formatted_text, "Some test text." ) != 0
    ) {
        tralloc_free ( ctx );
        return 4;
    }
    if ( tralloc_free ( ctx ) != 0 ) {
        return 5;
    }

#   if defined(TRALLOC_FILE)
    char * file_name;
    if ( tralloc_strdup ( NULL, &file_name, "/tmp/tralloc_test_file" ) != 0 ) {
        return 6;
    }
    int * test_file;
    if ( tralloc_open_mode ( NULL, &test_file, file_name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) != 0 ) { // 0644
        tralloc_free ( file_name );
        return 7;
    }
    if ( tralloc_move ( file_name, test_file ) != 0 ) {
        tralloc_free ( test_file );
        tralloc_free ( file_name );
        return 8;
    }
    if ( tralloc_append_destructor ( test_file, destructor_unlink_file, file_name ) != 0 ) {
        tralloc_free ( test_file );
        return 9;
    }
    if ( tralloc_free ( test_file ) != 0 ) {
        return 10;
    }
#   endif

#   if defined(TRALLOC_DEBUG)
    size_t length;
    if ( tralloc_debug_get_chunks_count ( &length ) != 0 || length != 0 ) {
        return 255;
    }
#   endif

    return 0;
}
