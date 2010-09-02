#include <linux/falloc.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/unistd_64.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux-ftools.h>

/** 

SYNTAX: fallocate file length

fallocate() allows the caller to directly manipulate the allocated disk space
for the file referred to by fd for the byte range starting at offset and
continuing for len bytes.

The mode argument determines the operation to be performed on the given
range. Currently only one flag is supported for mode:

FALLOC_FL_KEEP_SIZE

This flag allocates and initializes to zero the disk space within the range
specified by offset and len. After a successful call, subsequent writes into
this range are guaranteed not to fail because of lack of disk
space. Preallocating zeroed blocks beyond the end of the file is useful for
optimizing append workloads. Preallocating blocks does not change the file size
(as reported by stat(2)) even if it is less than offset+len.

If FALLOC_FL_KEEP_SIZE flag is not specified in mode, the default behavior is
almost same as when this flag is specified. The only difference is that on
success, the file size will be changed if offset + len is greater than the file
size. This default behavior closely resembles the behavior of the
posix_fallocate(3) library function, and is intended as a method of optimally
implementing that function.

Because allocation is done in block size chunks, fallocate() may allocate a
larger range than that which was specified.

*/

int main(int argc, char *argv[]) {

    if ( argc != 3 ) {
        fprintf( stderr, "%s version %s\n", argv[0], LINUX_FTOOLS_VERSION );
        fprintf( stderr, "SYNTAX: fallocate file length\n" );
        return 1;
    }

    char* path = argv[1];

    printf( "Going to fallocate %s\n", path );

    int flags = O_RDWR;
    int fd = open( path, flags );

    if ( fd == -1 ) {
        perror( NULL );
        return 1;
    }

    struct stat fd_stat ;

    if ( fstat( fd, &fd_stat )  < 0 ) {
        perror( "Could not stat file: " );
        return 1;
    }

    printf( "Current length: %ld\n", fd_stat.st_size );
    printf( "Current block size: %ld\n", fd_stat.st_blksize );
    printf( "Current blocks allocated: %ld\n" , fd_stat.st_blocks );

    long increase = strtol( argv[2], NULL, 10 );

    printf( "Increasing file to: %ld\n", increase );

    loff_t offset = 0;
    loff_t len = increase;

    //int mode = 0;
    //TODO: make this a command line option.
    int mode = FALLOC_FL_KEEP_SIZE;

    //long result = fallocate( fd, mode, offset, len );
    //long result = posix_fallocate( fd, offset, len );

    long result = syscall( SYS_fallocate, fd, mode, offset, len );

    if ( result != 0 ) {

        if ( result != -1 ) {
            errno=result;
            perror( "Unable to fallocate" );
        } else {
            printf( "Unable to fallocate: %ld\n" , result );
        }

        return 1;
    }

    close( fd );
    return 0;

}
