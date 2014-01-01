// This file is part of tralloc. Copyright (C) 2013 Andrew Aladjev aladjev.andrew@gmail.com
// tralloc is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// tralloc is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with tralloc. If not, see <http://www.gnu.org/licenses/>.

#include <tralloc/helpers/string.h>
#include <tralloc/helpers/file.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

static
uint8_t destructor_unlink_file ( tralloc_context * UNUSED ( chunk_context ), void * user_data )
{
    char * file_name = user_data;
    if ( unlink ( file_name ) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}

bool test_file ( tralloc_context * ctx )
{
    int * hosts = tralloc_open ( ctx, "/etc/hosts", O_RDONLY );
    if ( hosts == NULL ) {
        return false;
    }
    if ( tralloc_free ( hosts ) != 0 ) {
        return false;
    }
    hosts = tralloc_open ( ctx, "/etc/hosts", O_RDONLY | O_DIRECTORY );
    if ( hosts != NULL ) {
        tralloc_free ( hosts );
        return false;
    }

    char * file_name = tralloc_strdup ( ctx, "/tmp/tralloc_test_file" );
    int * test_file  = tralloc_open_mode ( ctx, file_name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ); // 0644
    if ( test_file == NULL ) {
        tralloc_free ( file_name );
        return false;
    }
    if ( tralloc_move ( file_name, test_file ) != 0 ) {
        tralloc_free ( test_file );
        tralloc_free ( file_name );
        return false;
    }
    if ( tralloc_append_destructor ( test_file, destructor_unlink_file, file_name ) != 0 ) {
        tralloc_free ( test_file );
        return false;
    }
    if ( tralloc_free ( test_file ) != 0 ) {
        return false;
    }
    return true;
}