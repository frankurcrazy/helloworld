#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


void help(void)
{
    printf("Usage: scrambling [OPTION]...\n");
    printf("\n");
    printf("  -f FRAME     Frame number (0 ~ 1023).\n");
    printf("  -s SUBFRAME  Subframe number (0 ~ 9).\n");
    printf("  -h           Show the help message.\n");
    printf("\n");
}

/*
 * R13 36.211 Section 5.3.1	Scrambling
 */
int main(int argc, char *argv[])
{
    int N_PUSCH_abs = 32;
    int N_acc = 4;
    int frmNo = 100;
    int subfrmNo = 1;
    int n_abs_sf;
    int i0;
    int j0;
    int J;
    int j;
    int i;
    int n;

    int ch;

    opterr = 0;
    while ((ch=getopt(argc, argv, "f:s:h")) != -1)
    {
        switch ( ch )
        {
            case 'f':
                frmNo = atoi( optarg );
                break;
            case 's':
                subfrmNo = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    printf("PUSCH 1st subframe: %d.%d\n", frmNo, subfrmNo);
    printf("\n");

    i0 = ((frmNo * 10) + subfrmNo);
    j0 = DIV_FLOOR(i0, N_acc);
    J = DIV_FLOOR((i0 + N_PUSCH_abs - 1), N_acc) - j0;

    printf("===================================\n");
    printf("jth block | n_abs_sf | mod 10 param\n");
    printf("===================================\n");
    n_abs_sf = i0;
    for (j=0; j<(J + 1); j++)
    {
        for (i=0; i<N_acc; i++)
        {
            n = ((j0 + j) * N_acc) % 10;
            printf("    %3d   |  %5d   |   %2d\n", j, n_abs_sf, n);

            n_abs_sf++;
            if (n_abs_sf > (i0 + N_PUSCH_abs - 1)) goto _DONE;
            if (n_abs_sf > 10239) n_abs_sf = 0;
        }
        if (j < J)
        {
            printf("----------+----------+-------------\n");
        }
    }
_DONE:
    printf("===================================\n");
    printf("\n");

    printf("N_acc = %d (block size in subframes)\n", N_acc);
    printf("N_PUSCH_abs = %d\n", N_PUSCH_abs);
    printf("i0 = %d\n", i0);
    printf("\n");

    return 0;
}

