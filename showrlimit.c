#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

void padd( int val ) {

    char p[20];

    int len = sprintf( p, "%i", val );

    printf( "%i", val );

    if ( val > 1000000000 )
        printf( " " );
    else if ( val > 100000000 )
        printf( "  " );
    else if ( val > 10000000 )
        printf( "   " );
    else if ( val > 1000000 )
        printf( "    " );
    else if ( val > 100000 )
        printf( "     " );
    else if ( val > 10000 )
        printf( "      " );
    else if ( val > 1000 )
        printf( "       " );
    else if ( val > 100 )
        printf( "        " );
    else if ( val > 10 )
        printf( "         " );
    else if ( val >= 0 )
        printf( "          " );
    else if ( val == -1 )
        printf( "         " );

}

void dump( char *header, int resource ) {

    struct rlimit limit;

    getrlimit( resource, &limit );    

    printf( header );
    padd( limit.rlim_cur );
    padd( limit.rlim_max );
    printf( "\n" );

}

/*
 * print rlimit variables
 *
 * http://linux.die.net/man/2/getrlimit
 */
int main() {

    dump( "RLIMIT_AS:          ", RLIMIT_AS );
    dump( "RLIMIT_CORE:        ", RLIMIT_CORE );
    dump( "RLIMIT_CPU:         ", RLIMIT_CPU );
    dump( "RLIMIT_DATA:        ", RLIMIT_DATA );
    dump( "RLIMIT_FSIZE:       ", RLIMIT_FSIZE );
    dump( "RLIMIT_LOCKS:       ", RLIMIT_LOCKS );
    dump( "RLIMIT_MEMLOCK:     ", RLIMIT_MEMLOCK );
    dump( "RLIMIT_MSGQUEUE:    ", RLIMIT_MSGQUEUE );
    dump( "RLIMIT_NICE:        ", RLIMIT_NICE );
    dump( "RLIMIT_NOFILE:      ", RLIMIT_NOFILE );
    dump( "RLIMIT_NPROC:       ", RLIMIT_NPROC );
    dump( "RLIMIT_RSS:         ", RLIMIT_RSS );
    dump( "RLIMIT_RTPRIO:      ", RLIMIT_RTPRIO );
    dump( "RLIMIT_SIGPENDING:  ", RLIMIT_SIGPENDING );
    dump( "RLIMIT_STACK:       ", RLIMIT_STACK );
    dump( "RLIMIT_OFILE:       ", RLIMIT_OFILE );

}

