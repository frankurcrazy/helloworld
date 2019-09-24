#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int N_frame_slot[5] = {
    10, 20, 40, 80, 160
};

int Table_13_7[16][4] = {
    { 1, 48, 1,   0 },
    { 1, 48, 1,   8 },
    { 1, 48, 2,   0 },
    { 1, 48, 2,   8 },
    { 1, 48, 3,   0 },
    { 1, 48, 3,   8 },
    { 1, 96, 1,  28 },
    { 1, 96, 2,  28 },
    { 2, 48, 1, -41 },
    { 2, 48, 1,  49 },
    { 2, 96, 1, -41 },
    { 2, 96, 1,  97 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 }
};

int Table_13_8[16][4] = {
    { 1, 24, 2,   0 },
    { 1, 24, 2,   4 },
    { 1, 48, 1,  14 },
    { 1, 48, 2,  14 },
    { 3, 24, 2, -20 },
    { 3, 24, 2,  24 },
    { 3, 48, 2, -20 },
    { 3, 48, 2,  48 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 },
    { 0,  0, 0,   0 }
};


int Table_13_12[16][5] = {
    {  0, 1, 2, 0,  0 },
    {  0, 2, 1, 0, -1 },
    {  5, 1, 2, 0,  0 },
    {  5, 2, 1, 0, -1 },
    { 10, 1, 2, 0,  0 },
    { 10, 2, 1, 0, -1 },
    {  0, 2, 1, 0, -1 },
    {  5, 2, 1, 0, -1 },
    { 10, 2, 1, 0, -1 },
    { 15, 1, 2, 0,  0 },
    { 15, 2, 1, 0, -1 },
    { 15, 2, 1, 0, -1 },
    {  0, 1, 4, 0,  0 },
    { 10, 1, 4, 0,  0 },
    {  0, 0, 0, 0,  0 },
    {  0, 0, 0, 0,  0 }
};



int is_case_D_symbol(int slot, int symb, int ssbIndex)
{
    int index = ((slot * 14) + symb);
    int slot2 = (slot >> 1);
    int retval = 0;

    if ((0  == slot2) || (1  == slot2) || (2  == slot2) || (3  == slot2) ||
        (5  == slot2) || (6  == slot2) || (7  == slot2) || (8  == slot2) ||
        (10 == slot2) || (11 == slot2) || (12 == slot2) || (13 == slot2) ||
        (15 == slot2) || (16 == slot2) || (17 == slot2) || (18 == slot2))
    {
        if ((ssbIndex >> 1) == slot)
        {
            if (0 == (ssbIndex % 4))
            {
                retval = (((4 + (28 * slot2)) <= index) && (index <= (7 + (28 * slot2))));
            }
            else if (1 == (ssbIndex % 4))
            {
                retval = (((8 + (28 * slot2)) <= index) && (index <= (11 + (28 * slot2))));
            }
            else if (2 == (ssbIndex % 4))
            {
                retval = (((16 + (28 * slot2)) <= index) && (index <= (19 + (28 * slot2))));
            }
            else
            {
                retval = (((20 + (28 * slot2)) <= index) && (index <= (23 + (28 * slot2))));
            }
        }
    }

    return retval;
}

int is_ccs0_symbol(
    int slot,
    int symb,
    int pdcchSCS,
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


    if (60 == pdcchSCS)
    {
        pattern = Table_13_7[coresetId][0];
        N_CORESET_symb = Table_13_7[coresetId][2];
    }
    else if (120 == pdcchSCS)
    {
        pattern = Table_13_8[coresetId][0];
        N_CORESET_symb = Table_13_8[coresetId][2];
    }

    if (pattern != 1)
    {
        printf("\nERR: wrong pattern %d\n", pattern);
        return 0;
    }

    O = Table_13_12[cssId][0];
    M = Table_13_12[cssId][2];
    if (0 == (ssbIndex % 2))
    {
        symb1st = Table_13_12[cssId][3];
    }
    else
    {
        symb1st = Table_13_12[cssId][4];
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
    printf("Usage: CCS0_pattern1_FR2 [OPTION]...\n");
    printf("\n");
    printf("  -c coresetId   CORESET with ID #0 (0 ~ 15).\n");
    printf("  -d cssId       Common search space with ID #0 (0 ~ 15).\n");
    printf("  -s pdcchSCS    PDCCH subcarrier spacing (60, 120 KHz).\n");
    printf("  -i ssbIndex    SS/PBCH block index (0 ~ 63).\n");
    printf("  -h             Show the help message.\n");
    printf("\n");
}

/* 38.213 Section 13 UE procedure for monitoring Type0-PDCCH CSS sets */
int main(int argc, char *argv[])
{
    int coresetId = 0;
    int cssId = 0;
    int pdcchSCS = 60;
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
                pdcchSCS = atoi( optarg );
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
        printf("ERR: wrong coresetId %d\n\n", coresetId);
        return 0;
    }

    if ((cssId < 0) || (cssId > 15))
    {
        printf("ERR: wrong cssId %d\n\n", cssId);
        return 0;
    }

    if (pdcchSCS == 60)
    {
        u = 2;
    }
    else if (pdcchSCS == 120)
    {
        u = 3;
    }
    else
    {
        printf("ERR: wrong SCS %d\n\n", pdcchSCS);
        return 0;
    }

    if ((ssbIndex < 0) || (ssbIndex > 63))
    {
        printf("ERR: wrong ssbIndex %d\n\n", ssbIndex);
        return 0;
    }


    printf("SCS: SSB 120KHz, PDCCH %dKHz\n", pdcchSCS);
    for (slot=0; slot<N_frame_slot[u]; slot++)
    {
        printf("+\n");
        /* 120KHz: 1ms has 8 slots */
        printf("[slot %2d]", slot);
        for (symb=0; symb<14; symb++)
        {
            if ( is_case_D_symbol(slot, symb, ssbIndex) )
            {
                printf(" [1;32m%2d[0m", symb);
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
            if ( is_ccs0_symbol(slot, symb, pdcchSCS, coresetId, cssId, ssbIndex, u) )
            {
                printf(" [1;31m%2d[0m", symb);
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

