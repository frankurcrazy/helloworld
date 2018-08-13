#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int centerNB[6][6] = {
    {  0,  1,  2,  3,  4,  5 },  //   6RB
    {  4,  5,  6,  8,  9, 10 },  //  15RB
    {  9, 10, 11, 13, 14, 15 },  //  25RB
    { 22, 23, 24, 25, 26, 27 },  //  50RB
    { 34, 35, 36, 38, 39, 40 },  //  75RB
    { 47, 48, 49, 50, 51, 52 }   // 100RB
};

int bw2index(int bw)
{
    int index = -1;

    switch ( bw )
    {
        case 6:
            index = 0;
            break;
        case 15:
            index = 1;
            break;
        case 25:
            index = 2;
            break;
        case 50:
            index = 3;
            break;
        case 75:
            index = 4;
            break;
        case 100:
            index = 5;
            break;
        default:
            printf("ERR: illegal bandwidth (%d RBs)\n", bw);
    }

    return index;
}

/*
 * R13 36.211 Section 5.2.4	Narrowbands
 */
int main(int argc, char *argv[])
{
    int N_xL_RB = 100;
    int N_xL_NB;
    int index;
    int nNB;
    int rb;
    int i0;
    int i;
    int j;

    if (argc > 1)
    {
        N_xL_RB = atoi( argv[1] );
    }
    else
    {
        printf("Usage: narrowbands <BW_in_RBs>\n");
        printf("\n");
    }

    index = bw2index( N_xL_RB );
    if (index < 0)
    {
        printf("Exit ...\n");
        return 0;
    }

    N_xL_NB = DIV_FLOOR(N_xL_RB, 6);
    i0 = (DIV_FLOOR(N_xL_RB, 2) - ((6 * N_xL_NB) / 2));

    printf("nNB |  RB index\n");
    printf("----+--------------------------\n");

    for (nNB=0, j=0; j<N_xL_NB; j++)
    {
        printf("%3d |", nNB);
        for (i=0; i<6; i++)
        {
            if ((N_xL_RB % 2) == 0)
            {
                rb = ((6 * nNB) + i0 + i);
            }
            else
            {
                if (nNB < (N_xL_NB / 2))
                {
                    rb = ((6 * nNB) + i0 + i);
                }
                else
                {
                    rb = ((6 * nNB) + i0 + i + 1);
                }
            }

            if ((rb >= centerNB[index][0]) && (rb <= centerNB[index][5]))
            {
                printf(" [1;35m%3d[0m", rb);
            }
            else
            {
                printf(" %3d", rb);
            }
        }
        printf("\n");
        nNB++;
    }
        printf("----+--------------------------\n");
        printf("%3d | [1;33m%3d[0m\n", N_xL_NB, N_xL_RB);

    return 0;
}
