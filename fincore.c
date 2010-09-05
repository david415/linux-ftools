#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux-ftools.h>
#include <math.h>

struct fincore_result 
{
    long cached_size;
};

// wrapper function for perror
f_err(char* path, char* msg) {

    char* errstr;

    if( (asprintf(&errstr, "%s: %s", msg, path)) == -1)
    {
        fprintf (stderr,"asprintf: out of memory\n");
        exit(1);
    }
    perror(errstr);

}

void fincore(char* path, int pages, int summarize, int only_cached, struct fincore_result *result ) {

    int fd;
    struct stat file_stat;
    void *file_mmap;
    unsigned char *mincore_vec;
    size_t page_size = getpagesize();
    size_t page_index;
    char *errstr;

    int flags = O_RDWR;
    
    //TODO:
    //
    // pretty print integers with commas... 

    fd = open(path,flags);

    if ( fd == -1 ) {
        f_err(path,"Cannot open file");
        return;
    }

    if ( fstat( fd, &file_stat ) < 0 ) {
        f_err(path, "Could not stat file");
        return;
    }

    // cannot mmap a zero size file
    if ( file_stat.st_size == 0 ) {
        return;
    }

    file_mmap = mmap((void *)0, file_stat.st_size, PROT_NONE, MAP_SHARED, fd, 0);
    
    if ( file_mmap == MAP_FAILED ) {
        f_err(path, "Could not mmap file");
        return;
    }

    mincore_vec = calloc(1, (file_stat.st_size+page_size-1)/page_size);

    if ( mincore_vec == NULL ) {
        // if we cannot calloc then someting is seriously wrong
        perror( "Could not calloc" );
        exit( 1 );
    }

    if ( mincore(file_mmap, file_stat.st_size, mincore_vec) != 0 ) {
        f_err(path, "Could not call mincore for file");
        exit( 1 );
    }

    int cached = 0;
    int printed = 0;
    for (page_index = 0; page_index <= file_stat.st_size/page_size; page_index++) {
        if (mincore_vec[page_index]&1) {
            ++cached;
            if ( pages ) {
                printf("%lu ", (unsigned long)page_index);
                ++printed;
            }
        }
    }

    if ( printed ) printf("\n");

    // TODO: make all these variables long and print them as ld

    int total_pages = (int)ceil( (double)file_stat.st_size / (double)page_size );
    double cached_perc = 100 * (cached / (double)total_pages); 

    long cached_size = (double)cached * (double)page_size;

    if ( only_cached == 0 || cached > 0 ) {
        printf( "%s %ld %d %d %ld %f\n", 
                path, file_stat.st_size, total_pages, cached, cached_size, cached_perc );
    }

    free(mincore_vec);
    munmap(file_mmap, file_stat.st_size);
    close(fd);

    result->cached_size = cached_size;

    return;

}

// print help / usage
void help() {

    fprintf( stderr, "%s version %s\n", "fincore", LINUX_FTOOLS_VERSION );
    fprintf( stderr, "fincore [options] files...\n" );
    fprintf( stderr, "\n" );

    fprintf( stderr, "  --pages=false      Don't print pages\n" );
    fprintf( stderr, "  --summarize        When comparing multiple files, print a summary report\n" );
    fprintf( stderr, "  --only-cached      Only print stats for files that are actually in cache.\n" );

}

/**
 * see README
 */
int main(int argc, char *argv[]) {

    if ( argc == 1 ) {
        help();
        exit(1);
    }

    //keep track of the file index.
    int fidx = 1;

    // parse command line options.

    //TODO options:
    //
    // --pages=true|false print/don't print pages
    // --summarize when comparing multiple files, print a summary report
    // --only-cached only print stats for files that are actually in cache.

    int i = 1; 

    int pages         = 1;
    int summarize     = 0;
    int only_cached   = 0;

    for( ; i < argc; ++i ) {

        if ( strcmp( "--pages=false" , argv[i] ) == 0 ) {
            pages = 0;
            ++fidx;
        }

        if ( strcmp( "--summarize" , argv[i] ) == 0 ) {
            summarize = 1;
            ++fidx;
        }

        if ( strcmp( "--only-cached" , argv[i] ) == 0 ) {
            only_cached = 1;
            ++fidx;
        }

        if ( strcmp( "--help" , argv[i] ) == 0 ) {
            help();
            exit(1);
        }

        //TODO what if this starts -- but we don't know what option it is?

    }

    long total_cached_size = 0;

    printf("filename size\ttotal pages\tcached pages\tcached size\tcached percentage\n");

    for( ; fidx < argc; ++fidx ) {

        char* path = argv[fidx];

        struct fincore_result result;

        fincore( path, pages, summarize, only_cached , &result );

        total_cached_size += result.cached_size;

    }
    
    if ( summarize ) {
        
        printf( "---\n" );

        //TODO: add more metrics including total size... 
        printf( "total cached size: %ld\n", total_cached_size );

    }

}
