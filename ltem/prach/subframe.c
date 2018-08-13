#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int prachSubframe(int prachConfigIndex, int frame, int subframe)
{
    int match = 0;

    switch ( prachConfigIndex )
    {
        case 0:
        case 32:
        case 48:
            match = (((frame % 2) == 0) && (subframe == 1));
            break;
        case 1:
        case 33:
        case 49:
            match = (((frame % 2) == 0) && (subframe == 4));
            break;
        case 2:
        case 34:
        case 50:
            match = (((frame % 2) == 0) && (subframe == 7));
            break;
        case 3:
        case 19:
        case 35:
        case 51:
            match = (subframe == 1);
            break;
        case 4:
        case 20:
        case 36:
        case 52:
            match = (subframe == 4);
            break;
        case 5:
        case 21:
        case 37:
        case 53:
            match = (subframe == 7);
            break;
        case 6:
        case 22:
        case 38:
        case 54:
            match = ((subframe == 1) || (subframe == 6));
            break;
        case 7:
        case 23:
        case 39:
        case 55:
            match = ((subframe == 2) || (subframe == 7));
            break;
        case 8:
        case 24:
        case 40:
        case 56:
            match = ((subframe == 3) || (subframe == 8));
            break;
        case 9:
        case 25:
        case 41:
        case 57:
            match = ((subframe == 1) || (subframe == 4) || (subframe == 7));
            break;
        case 10:
        case 26:
        case 42:
        case 58:
            match = ((subframe == 2) || (subframe == 5) || (subframe == 8));
            break;
        case 11:
        case 27:
        case 43:
        case 59:
            match = ((subframe == 3) || (subframe == 6) || (subframe == 9));
            break;
        case 12:
        case 28:
        case 44:
            match = ((subframe % 2) == 0);
            break;
        case 13:
        case 29:
        case 45:
            match = ((subframe % 2) == 1);
            break;
        case 14:
            match = 1;
            break;
        case 31:
        case 47:
        case 63:
            match = (((frame % 2) == 0) && (subframe == 9));
            break;
        case 30:
        case 46:
        case 60:
        case 61:
        case 62:
        default:
            match = 0;
    }

    return match;
}

/*
 * R13 36.211 Section 5.7.1	Time and frequency structure
 */
int main(int argc, char *argv[])
{
    int prachConfigIndex = 1;
    int N_PRACH_start = 8;
    int N_PRACH_rep = 4;
    int n_abs_sf[10240];
    int n_RA_sf;
    int N_RA_sf;
    int sfn;
    int sf;
    int i;
    int j;

    if (2 == argc)
    {
        prachConfigIndex = atoi( argv[1] );
    }
    else if (3 == argc)
    {
        prachConfigIndex = atoi( argv[1] );
        N_PRACH_rep = atoi( argv[2] );
    }
    else if (4 == argc)
    {
        prachConfigIndex = atoi( argv[1] );
        N_PRACH_rep = atoi( argv[2] );
        N_PRACH_start = atoi( argv[3] );
    }
    else
    {
        printf("Usage: subframe <cfgIndex>\n");
        printf("     : subframe <cfgIndex> <repetition>\n");
        printf("     : subframe <cfgIndex> <repetition> <start_subframe>\n");
        printf("\n");
    }

    /* find all preamble allowed subframe */
    N_RA_sf = 0;
    for (sfn=0; sfn<1024; sfn++)
    {
        for (sf=0; sf<10; sf++)
        {
            if ( prachSubframe(prachConfigIndex, sfn, sf) )
            {
                n_abs_sf[N_RA_sf] = (sfn * 10) + sf;
                //printf("n_RA_sf(%d)  n_abs_sf(%d)\n", N_RA_sf, n_abs_sf[N_RA_sf]);
                N_RA_sf++;
            }
        }
    }

    printf("n_RA_sf | Absolute subframe number\n");
    printf("--------+-----------------------------\n");
    for (j=0; j<N_RA_sf; j++)
    {
        if (0 == N_PRACH_start)
        {
            n_RA_sf = (j * N_PRACH_rep);
        }
        else
        {
            n_RA_sf = (j * N_PRACH_start) + N_PRACH_rep;
        }

        if (n_RA_sf >= N_RA_sf) break;

        printf("  %3d   | ", n_RA_sf);
        for (i=0; i<N_PRACH_rep; i++)
        {
            printf(" %5d", n_abs_sf[n_RA_sf + i]);
        }
        printf("\n");
    }

    printf("\n");

    printf("prachConfigIndex = %d\n", prachConfigIndex);
    printf("N_PRACH_rep = %d\n", N_PRACH_rep);
    printf("N_PRACH_start = %d\n", N_PRACH_start);
    printf("\n");

    return 0;
}

