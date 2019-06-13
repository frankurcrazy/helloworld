#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int N_frame_slot[5] = {
    10, 20, 40, 80, 160
};

int Table_13_1[16][4] = {
    { 1, 24, 2,  0 },
    { 1, 24, 2,  2 },
    { 1, 24, 2,  4 },
    { 1, 24, 3,  0 },
    { 1, 24, 3,  2 },
    { 1, 24, 3,  4 },
    { 1, 48, 1, 12 },
    { 1, 48, 1, 16 },
    { 1, 48, 2, 12 },
    { 1, 48, 2, 16 },
    { 1, 48, 3, 12 },
    { 1, 48, 3, 16 },
    { 1, 96, 1, 38 },
    { 1, 96, 2, 38 },
    { 1, 96, 3, 38 },
    { 0,  0, 0,  0 }
};

int Table_13_2[16][4] = {
    { 1, 24, 2,  5 },
    { 1, 24, 2,  6 },
    { 1, 24, 2,  7 },
    { 1, 24, 2,  8 },
    { 1, 24, 3,  5 },
    { 1, 24, 3,  6 },
    { 1, 24, 3,  7 },
    { 1, 24, 3,  8 },
    { 1, 48, 1, 18 },
    { 1, 48, 1, 20 },
    { 1, 48, 2, 18 },
    { 1, 48, 2, 20 },
    { 1, 48, 3, 18 },
    { 1, 48, 3, 20 },
    { 0,  0, 0,  0 },
    { 0,  0, 0,  0 }
};


int Table_13_11[16][5] = {
    {  0, 2, 2, 0,  0 },
    {  0, 4, 1, 0, -1 },
    {  4, 2, 2, 0,  0 },
    {  4, 4, 1, 0, -1 },
    { 10, 2, 2, 0,  0 },
    { 10, 4, 1, 0, -1 },
    { 14, 2, 2, 0,  0 },
    { 14, 4, 1, 0, -1 },
    {  0, 2, 4, 0,  0 },
    { 10, 2, 4, 0,  0 },
    {  0, 2, 2, 1,  1 },
    {  0, 2, 2, 2,  2 },
    {  4, 2, 2, 1,  1 },
    {  4, 2, 2, 2,  2 },
    { 10, 2, 2, 1,  1 },
    { 10, 2, 2, 2,  2 }
};



int is_case_A_symbol(int slot, int symb, int ssbIndex)
{
    int index = ((slot * 14) + symb);
    int retval = 0;

    if ((0 == slot) || (1 == slot))
    {
        if ((ssbIndex >> 1) == slot)
        {
            if (0 == (ssbIndex % 2))
            {
                retval = (((2 + (14 * slot)) <= index) && (index <= (5 + (14 * slot))));
            }
            else
            {
                retval = (((8 + (14 * slot)) <= index) && (index <= (11 + (14 * slot))));
            }
        }
    }

    return retval;
}

int is_ccs0_symbol(
    int slot,
    int symb,
    int scs,
    int coresetId,
    int cssId,
    int ssbIndex,
    int u
)
{
    int pattern = 0;
    int N_CORESET_symb;
    int O;
    int M;
    int symb1st;
    int n0;
    int retval = 0;


    if (15 == scs)
    {
        pattern = Table_13_1[coresetId][0];
        N_CORESET_symb = Table_13_1[coresetId][2];
    }
    else if (30 == scs)
    {
        pattern = Table_13_2[coresetId][0];
        N_CORESET_symb = Table_13_2[coresetId][2];
    }

    if (pattern != 1)
    {
        printf("\nERR: wrong pattern %d\n", pattern);
        return 0;
    }

    O = Table_13_11[cssId][0];
    M = Table_13_11[cssId][2];
    if (0 == (ssbIndex % 2))
    {
        symb1st = Table_13_11[cssId][3];
    }
    else
    {
        symb1st = Table_13_11[cssId][4];
        if (symb1st < 0)
        {
            symb1st = N_CORESET_symb;
        }
    }

    n0 = ((((O << u) >> 1) + ((ssbIndex * M) >> 1)) % N_frame_slot[u]);

    if ((slot == n0) || (slot == (n0 + 1)))
    {
        retval = ((symb1st <= symb) && (symb < (symb1st + N_CORESET_symb)));
    }

    return retval;
}

void help(void)
{
    printf("Usage: CCS0_pattern1_FR1 [OPTION]...\n");
    printf("\n");
    printf("  -c   CORESET with ID #0 (0 ~ 15).\n");
    printf("  -d   Common search space with ID #0 (0 ~ 15).\n");
    printf("  -s   PDCCH subcarrier spacing (15, 30).\n");
    printf("  -i   SS/PBCH block index (0 ~ 3).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.213 Section 13 UE procedure for monitoring Type0-PDCCH CSS sets */
int main(int argc, char *argv[])
{
    int coresetId = 0;
    int cssId = 0;
    int scs = 15;
    int ssbIndex = 0;

    int slot;
    int symb;
    int u;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:d:s:i:h")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                coresetId = atoi( optarg );
                break;
            case 'd':
                cssId = atoi( optarg );
                break;
            case 's':
                scs = atoi( optarg );
                break;
            case 'i':
                ssbIndex = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((coresetId < 0) || (coresetId > 15))
    {
        printf("ERR: wrong coresetId %d\n", coresetId);
        return 0;
    }

    if ((cssId < 0) || (cssId > 15))
    {
        printf("ERR: wrong cssId %d\n", cssId);
        return 0;
    }

    if (scs == 15)
    {
        u = 0;
    }
    else if (scs == 30)
    {
        u = 1;
    }
    else
    {
        printf("ERR: wrong SCS %d\n", scs);
        return 0;
    }

    if ((ssbIndex < 0) || (ssbIndex > 3))
    {
        printf("ERR: wrong ssbIndex %d\n", ssbIndex);
        return 0;
    }


    printf("SCS: SSB 15KHz, PDCCH %dKHz\n", scs);
    for (slot=0; slot<N_frame_slot[u]; slot++)
    {
        printf("+\n");
        /* 15KHz: 1ms has 1 slot */
        printf("[slot %2d]", slot);
        for (symb=0; symb<14; symb++)
        {
            if ( is_case_A_symbol(slot, symb, ssbIndex) )
            {
                printf(" [1;31m%2d[0m", symb);
            }
            else
            {
                printf(" %2d", symb);
            }
        }
        printf("\n");

        printf("         ");
        for (symb=0; symb<14; symb++)
        {
            if ( is_ccs0_symbol(slot, symb, scs, coresetId, cssId, ssbIndex, u) )
            {
                printf(" [1;36m%2d[0m", symb);
            }
            else
            {
                printf(" %2d", symb);
            }
        }
        printf("\n");
    }
    printf("+\n");
    printf("\n");


    return 0;
}

