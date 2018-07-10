#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"


void guard_band(int bw)
{
    printf("RB   Raster   Frequency offset   500Hz unit\n");
    if (25 == bw)
    {
        dl_5MHz( -1 );
        dl_5MHz( 25 );
    }
    else if (50 == bw)
    {
        dl_10MHz( -2, 0 );
        dl_10MHz( -1, 1 );
        dl_10MHz( 50, 1 );
        dl_10MHz( 51, 0 );
    }
    else if (75 == bw)
    {
        dl_15MHz( -4, 0 );
        dl_15MHz( -3, 1 );
        dl_15MHz( -2, 0 );
        dl_15MHz( -1, 0 );
        dl_15MHz( 75, 0 );
        dl_15MHz( 76, 0 );
        dl_15MHz( 77, 1 );
        dl_15MHz( 78, 0 );
    }
    else if (100 == bw)
    {
        dl_20MHz(  -5, 0 );
        dl_20MHz(  -4, 0 );
        dl_20MHz(  -3, 0 );
        dl_20MHz(  -2, 0 );
        dl_20MHz(  -1, 1 );
        dl_20MHz( 100, 1 );
        dl_20MHz( 101, 0 );
        dl_20MHz( 102, 0 );
        dl_20MHz( 103, 0 );
        dl_20MHz( 104, 0 );
    }
    else
    {
        printf("ERR: wrong bandwidth %d\n", bw);
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: guardband_dl [OPTION]...\n");
    printf("  -b BW   Bandwidth in RBs (25, 50, 75, 100).\n");
    printf("  -h      Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.104 Section 5.6 Channel bandwidth
*/
int main(int argc, char *argv[])
{
    int bw = 0;
    int ch;

    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                /* Bandwidth */
                bw = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    guard_band( bw );

    return 0;
}

