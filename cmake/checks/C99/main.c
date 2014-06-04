#include <stdlib.h>

int main ()
{
    void * data_1 = malloc ( sizeof ( char ) * 10 );
    if ( data_1 == NULL ) {
        return 1;
    }
    void * data_2 = calloc ( 1, sizeof ( char ) * 10 );
    if ( data_2 == NULL ) {
        return 2;
    }
    free ( data_1 );
    free ( data_2 );
    return 0;
}
