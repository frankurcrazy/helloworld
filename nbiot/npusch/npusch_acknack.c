#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/* 36.213, Table 16.4.2-1: ACK/NACK subcarrier and
   ko for subcarrier spacing 3.75KHz/15KHz */
int AckNackScAndKo[2][16][2] =
{
    {
        /* 15KHz */
        {0,   13},
        {1,   13},
        {2,   13},
        {3,   13},
        {0,   15},
        {1,   15},
        {2,   15},
        {3,   15},
        {0,   17},
        {1,   17},
        {2,   17},
        {3,   17},
        {0,   18},
        {1,   18},
        {2,   18},
        {3,   18},
    },
    {
        /* 3.75KHz */
        {38,  13},
        {39,  13},
        {40,  13},
        {41,  13},
        {42,  13},
        {43,  13},
        {44,  13},
        {45,  13},
        {38,  21},
        {39,  21},
        {40,  21},
        {41,  21},
        {42,  21},
        {43,  21},
        {44,  21},
        {45,  21},
    }
};



void help(void)
{
    printf("Usage: npusch_acknack [OPTION]...\n");
    printf("\n");
    printf("  -f FREQ   Subcarrier spacing (0: 15KHz, 1: 3.75KHz).\n");
    printf("  -n NUM    End subframe of NPDSCH (0 ~ 9).\n");
    printf("  -r RES    HARQ-ACK resource (0 ~ 15).\n");
    printf("  -h        Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.211 Section 16.4.2 UE procedure for reporting ACK/NACK
*/
int main(int argc, char *argv[])
{
    int N_AN_Rep = 1;
    int N_UL_slots = 4;
    unsigned int deltaF = 0;
    unsigned int npdsch = 0;
    unsigned int resource = 0;
    int npusch;
    int N;
    int sc;
    int k0;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "f:n:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'f':
                deltaF = atoi( optarg );
                if (deltaF > 1) deltaF = 0;
                break;
            case 'n':
                npdsch = atoi( optarg );
                if (npdsch > 9) npdsch = 0;
                break;
            case 'r':
                resource = atoi( optarg );
                if (resource > 15) resource = 0;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    N  = (N_AN_Rep * N_UL_slots);
    sc = AckNackScAndKo[deltaF][resource][0];
    k0 = AckNackScAndKo[deltaF][resource][1];
    npusch = (npdsch + k0 - 1);

    printf("\n");
    printf("  [%d]   +%d subframes\n", npdsch, npusch);
    printf("NPDSCH -----> NPUSCH (subcarrier %d, %d slots)\n", sc, N);
    printf("\n");

    return 0;
}

