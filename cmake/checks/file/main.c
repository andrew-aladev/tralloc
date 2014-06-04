#include <fcntl.h>
#include <unistd.h>

int main()
{
    int descriptor = open ( "/tmp/test_file", O_RDONLY );
    if ( descriptor != -1 ) {
        close ( descriptor );
    }
    return 0;
}
