#include <linux/falloc.h>
#include <linux/fadvise.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/unistd_64.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <linux-ftools.h>

/** 

SYNTAX: filename mode [offset] [,length]
Where mode can be:

  POSIX_FADV_NORMAL       No further special treatment.  
  POSIX_FADV_RANDOM       Expect random page references.  
  POSIX_FADV_SEQUENTIAL   Expect sequential page references.  
  POSIX_FADV_WILLNEED     Will need these pages.  
  POSIX_FADV_DONTNEED     Don't need these pages.  
  POSIX_FADV_NOREUSE      Data will be accessed once.  

Allows an application to to tell the kernel how it expects to use a file handle,
so that the kernel can choose appropriate read-ahead and caching techniques for
access to the corresponding file. This is similar to the POSIX version of the
madvise system call, but for file access instead of memory access. The
sys_fadvise64() function is obsolete and corresponds to a broken glibc API,
sys_fadvise64_64() is the fixed version. The following are the values for the
advice parameter:

FADV_NORMAL

No special treatment.

FADV_RANDOM

Expect page references in random order.

FADV_SEQUENTIAL

Expect page references in sequential order.

FADV_WILLNEED

Expect access in the near future.

FADV_DONTNEED

Do not expect access in the near future. Subsequent access of pages in this
range will succeed, but will result either in reloading of the memory contents
from the underlying mapped file or zero-fill-in-demand pages for mappings
without an underlying file.

FADV_NOREUSE

Access data only once.

*/
int main(int argc, char *argv[]) {

    if ( argc < 3 ) {
        fprintf( stderr, "%s version %s\n", argv[0], LINUX_FTOOLS_VERSION );
        printf( "SYNTAX: fadvise filename mode [offset] [,length]\n" );

        printf( "Where mode can be:\n\n" );

        printf( "  POSIX_FADV_NORMAL       No further special treatment.  \n" );
        printf( "  POSIX_FADV_RANDOM       Expect random page references.  \n" );
        printf( "  POSIX_FADV_SEQUENTIAL   Expect sequential page references.  \n" );
        printf( "  POSIX_FADV_WILLNEED     Will need these pages.  \n" );
        printf( "  POSIX_FADV_DONTNEED     Don't need these pages.  \n" );
        printf( "  POSIX_FADV_NOREUSE      Data will be accessed once.  \n" );

        exit( 1 );
    }

    char* path = argv[1];
    char* param_mode = argv[2];

    printf( "Going to fadvise %s\n", path );

    int flags = O_RDWR;
    int fd = open( path, flags );

    if ( fd == -1 ) {
        perror( NULL );
        return 1;
    }

    struct stat fd_stat ;

    if ( fstat( fd, &fd_stat ) < 0 ) {
        perror( "Could not stat file: " );
        return 1;
    }

    loff_t offset = 0;
    loff_t length = fd_stat.st_size;

    if ( argc >= 4 ) {
        offset = strtol( argv[3], NULL, 10 );
    }

    if ( argc >= 5 ) {
        length = strtol( argv[4], NULL, 10 );
    }

    printf( "offset: %ld\n", offset );
    printf( "length: %ld\n", length );

    int mode = -1;

    if ( strcmp( param_mode , "POSIX_FADV_NORMAL" ) == 0 ) {
        mode = POSIX_FADV_NORMAL;
    } else if ( strcmp( param_mode , "POSIX_FADV_RANDOM" ) == 0 ) {
        mode = POSIX_FADV_RANDOM;
    } else if ( strcmp( param_mode , "POSIX_FADV_SEQUENTIAL" ) == 0 ) {
        mode = POSIX_FADV_SEQUENTIAL;
    } else if ( strcmp( param_mode , "POSIX_FADV_WILLNEED" ) == 0 ) {
        mode = POSIX_FADV_DONTNEED;
    } else if ( strcmp( param_mode , "POSIX_FADV_DONTNEED" ) == 0 ) {
        mode = POSIX_FADV_DONTNEED;
    } else if ( strcmp( param_mode , "POSIX_FADV_NOREUSE" ) == 0 ) {
        mode = POSIX_FADV_NOREUSE;
    } else {
        printf( "Invalid mode %s\n", param_mode );
        exit( 1 );
    }

    printf( "mode: %s\n", param_mode );

    long result = syscall( SYS_fadvise64, fd, offset, length , mode );

    if ( result != 0 ) {

        if ( result != -1 ) {
            errno=result;
            perror( "Unable to fadvise" );
        } else {
            printf( "Unable to fadvise: %ld\n" , result );
        }

        return 1;
    }

    printf( "WIN\n" );

    close( fd );
    return 0;

}
