#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void help(void)
{
    printf("Usage: sync_raster [OPTION]...\n");
    printf("\n");
    printf("  -x   N value for (0 ~ 3000) MHz.\n");
    printf("  -y   N value for (3000 ~ 24250) MHz.\n");
    printf("  -z   N value for (24250 ~ 100000) MHz.\n");
    printf("  -m   M value (1, 3, 5).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.3 Synchronization raster */
int main(int argc, char *argv[])
{
    int N1 = 1;
    int N2 = 0;
    int N3 = 0;
    int M  = 3;
    int GSCN;
    double SS_REF;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "x:y:z:m:h")) != -1)
    {
        switch ( ch )
        {
            case 'x':
                N1 = atoi( optarg );
                break;
            case 'y':
                N2 = atoi( optarg );
                break;
            case 'z':
                N3 = atoi( optarg );
                break;
            case 'm':
                M = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    printf("-------------------+-------+-----------------\n");
    printf("   Frequency range |  GSCN | SS_REF\n");
    printf("-------------------+-------+-----------------\n");

    /* 0 ~ 3000MHz */
    if (N1 < 1) N1 = 1;
    if (N1 > 2499) N1 = 2499;
    if ((M != 1) && (M != 3) && (M != 5)) M = 3;
    GSCN = ((3 * N1) + ((M - 3) / 2));
    SS_REF = ((N1 * 1200) + (M * 50));
    printf("     0 ~ 3000MHz   | %5d | %.0f KHz\n", GSCN, SS_REF);

    /* 3000 ~ 24250MHz */
    if (N2 < 0) N2 = 0;
    if (N2 > 14756) N2 = 14756;
    GSCN = (7499 + N2);
    SS_REF = (3000.0 + (N2 * 1.44));
    printf("  3000 ~ 24250MHz  | %5d | %.2f MHz\n", GSCN, SS_REF);

    /* 24250 ~ 100000MHz */
    if (N3 < 0) N3 = 0;
    if (N3 > 4383) N3 = 4383;
    GSCN = (22256 + N3);
    SS_REF = (24250.08 + (N3 * 17.28));
    printf(" 24250 ~ 100000MHz | %5d | %.2f MHz\n", GSCN, SS_REF);

    printf("-------------------+-------+-----------------\n");
    printf("N = %d, %d, %d\n", N1, N2, N3);
    printf("M = %d\n", M);
    printf("\n");


    return 0;
}

