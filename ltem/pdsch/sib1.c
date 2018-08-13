#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int availableNB(int N_DL_RB, int N_DL_NB, int nNB)
{
    int centerRB[6][6] = {
            {  0,  1,  2,  3,  4,  5 },  // [0]   6 RBs
            {  4,  5,  6,  8,  9, 10 },  // [1]  15 RBs
            {  9, 10, 11, 13, 14, 15 },  // [2]  25 RBs
            { 22, 23, 24, 25, 26, 27 },  // [3]  50 RBs
            { 34, 35, 36, 38, 39, 40 },  // [4]  75 RBs
            { 47, 48, 49, 50, 51, 52 }   // [5] 100 RBs
        };
    int bw;
    int rb;
    int i0;
    int i;
    int j;

    if (6 == N_DL_RB)
    {
        bw = 0;
        return 1;
    }
    else if (15 == N_DL_RB)
    {
        bw = 1;
        return 1;
    }
    else if (25 == N_DL_RB)
    {
        bw = 2;
    }
    else if (50 == N_DL_RB)
    {
        bw = 3;
    }
    else if (75 == N_DL_RB)
    {
        bw = 4;
    }
    else if (100 == N_DL_RB)
    {
        bw = 5;
    }
    else
    {
        /* wrong bandwidth */
        return 0;
    }

    i0 = (DIV_FLOOR(N_DL_RB, 2) - ((6 * N_DL_NB) / 2));

    for (i=0; i<6; i++)
    {
        if ((N_DL_RB % 2) == 0)
        {
            rb = ((6 * nNB) + i0 + i);
        }
        else
        {
            if (nNB < (N_DL_NB / 2))
            {
                rb = ((6 * nNB) + i0 + i);
            }
            else
            {
                rb = ((6 * nNB) + i0 + i + 1);
            }
        }

        for (j=0; j<6; j++)
        {
            if (rb == centerRB[bw][j])
            {
                /* overlapped the center 6 RBs */
                return 0;
            }
        }
    }

    return 1;
}

void showSj(void)
{
    int N_DL_RB[6] = { 6, 15, 25, 50, 75, 100 };
    int s[6][16] = {
        { 0, },
        { 0, 1, },
        { 0, 3, },
        { 0, 1, 2, 5, 6, 7, },
        { 0, 1, 2, 3, 4, 7, 8, 9, 10, 11, },
        { 0, 1, 2, 3, 4, 5, 6, 9, 10, 11, 12, 13, 14, 15, }
    };
    int N_S_NB[6] = { 1, 2, 2, 6, 10, 14 };
    int i;
    int j;

    printf("N_DL_RB | N_S_NB | { Sj }\n");
    printf("--------+--------+---------------------------------------------\n");

    for (i=0; i<6; i++)
    {
        printf("  %3d   |   %2d   |", N_DL_RB[i], N_S_NB[i]);

        for (j=0; j<N_S_NB[i]; j++)
        {
            printf(" %2d", s[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

void showNB(int N_cell_ID, int N_DL_RB, int N_SIB1_BR_PDSCH)
{
    int SFN_SF[6][8][2] = {
            /*   0           1           2           3           4           5           6           7      */
            { {  4, -1 }, { -1, -1 }, {  4, -1 }, { -1, -1 }, {  4, -1 }, { -1, -1 }, {  4, -1 }, { -1, -1 } },
            { { -1, -1 }, {  4, -1 }, { -1, -1 }, {  4, -1 }, { -1, -1 }, {  4, -1 }, { -1, -1 }, {  4, -1 } },
            { {  4, -1 }, {  4, -1 }, {  4, -1 }, {  4, -1 }, {  4, -1 }, {  4, -1 }, {  4, -1 }, {  4, -1 } },
            { {  9, -1 }, {  9, -1 }, {  9, -1 }, {  9, -1 }, {  9, -1 }, {  9, -1 }, {  9, -1 }, {  9, -1 } },
            { {  4,  9 }, {  4,  9 }, {  4,  9 }, {  4,  9 }, {  4,  9 }, {  4,  9 }, {  4,  9 }, {  4,  9 } },
            { {  0,  9 }, {  0,  9 }, {  0,  9 }, {  0,  9 }, {  0,  9 }, {  0,  9 }, {  0,  9 }, {  0,  9 } }
        };
    int sj[16];
    int si[16];
    int N_DL_NB;
    int N_S_NB;
    int nNB;
    int m;
    int i;
    int j;
    int k;
    int n;

    N_DL_NB = DIV_FLOOR(N_DL_RB, 6);
    for (j=0, nNB=0; nNB<N_DL_NB; nNB++)
    {
        if ( availableNB(N_DL_RB, N_DL_NB, nNB) )
        {
            //printf("sj[%2d] = %2d\n", j, nNB);
            sj[j] = nNB;
            j++;
        }
    }

    N_S_NB = j;
    if (0 == N_S_NB)
    {
        printf("ERR: incorrect bandwidth (%d RBs)\n", N_DL_RB);
        return;
    }

    if (N_DL_RB < 12)
    {
        m = 1;
    }
    else if ((N_DL_RB >= 12) && (N_DL_RB <= 50))
    {
        m = 2;
    }
    else
    {
        m = 4;
    }

    printf("[ SIB1-BR ]\n");
    printf("\n");
    printf("frame   :      0      1      2      3      4      5      6      7\n");
    printf("subframe:");
    if (16 == N_SIB1_BR_PDSCH)
    {
        i = (4 + (N_cell_ID % 2));
        for (j=0; j<8; j++)
        {
            printf("   %d, %d", SFN_SF[i][j][0], SFN_SF[i][j][1]);
        }
    }
    else
    {
        if (8 == N_SIB1_BR_PDSCH)
        {
            i = (2 + (N_cell_ID % 2));
        }
        else
        {
            i = (0 + (N_cell_ID % 2));
        }
        for (j=0; j<8; j++)
        {
            if (SFN_SF[i][j][0] >= 0)
            {
                printf("      %d", SFN_SF[i][j][0]);
            }
            else
            {
                printf("       ");
            }
        }
    }
    printf("\n");
    printf("nNB     :[1;33m");
    n = 0;
    for (k=0; k<(N_SIB1_BR_PDSCH / m); k++)
    {
        for (i=0; i<m; i++)
        {
            j = (((N_cell_ID % N_S_NB) + (i * DIV_FLOOR(N_S_NB, m))) % N_S_NB);
            si[n++] = sj[j];
        }
    }
    if (16 == N_SIB1_BR_PDSCH)
    {
        i = 0;
        for (j=0; j<8; j++)
        {
            printf("  %2d,%2d", si[i], si[i+1]);
            i += 2;
        }
    }
    else
    {
        if (8 == N_SIB1_BR_PDSCH)
        {
            i = 0;
            for (j=0; j<8; j++)
            {
                printf("     %2d", si[i]);
                i++;
            }
        }
        else
        {

            i = 0;
            for (j=0; j<8; j++)
            {
                if (0 == ((j + N_cell_ID) % 2))
                {
                    printf("     %2d", si[i]);
                    i++;
                }
                else
                {
                    printf("       ");
                }
            }

        }
    }
    printf("[0m\n");
    printf("\n");
}

/*
 * R13 36.211 Section 6.4.1	Physical downlink shared channel for BL/CE UEs
 */
int main(int argc, char *argv[])
{
    int N_cell_ID = 0;
    int N_DL_RB = 100;  /* 6, 15, 25, 50, 75, 100 */
    int N_SIB1_BR_PDSCH = 4;


    if (2 == argc)
    {
        N_DL_RB = atoi( argv[1] );
    }
    else if (3 == argc)
    {
        N_DL_RB = atoi( argv[1] );
        N_SIB1_BR_PDSCH = atoi( argv[2] );
    }
    else
    {
        printf("Usage: sib1 <bandwidth>\n");
        printf("Usage: sib1 <bandwidth> <repetition>\n");
        printf("\n");

        showSj();

        return 0;
    }

    if ((N_DL_RB !=   6) &&
        (N_DL_RB !=  15) &&
        (N_DL_RB !=  25) &&
        (N_DL_RB !=  50) &&
        (N_DL_RB !=  75) &&
        (N_DL_RB != 100))
    {
        printf("ERR: incorrect bandwidth (%d RBs)\n", N_DL_RB);
        printf("\n");
        return -1;
    }

    if ((N_SIB1_BR_PDSCH !=  4) &&
        (N_SIB1_BR_PDSCH !=  8) &&
        (N_SIB1_BR_PDSCH != 16))
    {
        printf("ERR: incorrect repetition (%d)\n", N_SIB1_BR_PDSCH);
        printf("\n");
        return -1;
    }

    showNB(N_cell_ID, N_DL_RB, N_SIB1_BR_PDSCH);

    return 0;
}

