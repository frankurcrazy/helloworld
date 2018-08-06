#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
*  R10 36.211 Section 6.6.4 Mapping to resource elements
*/
int main(int argc, char *argv[])
{
    char  bwStr[16];
    int N_RB_sc = 12;
    int N_DL_RB = 100;
    int centerB = 47;
    int centerT = 52;
    int rb;
    int kp;
    int k[72];
    int i;


    if (argc > 1)
    {
        if (0 == strcmp("-h", argv[1]))
        {
            printf("Usage: pbch BW   (Bandwidth in RBs)\n\n");
            return 0;
        }

        N_DL_RB = atoi( argv[1] );
    }

    switch ( N_DL_RB )
    {
        case 6:
            sprintf(bwStr, "1.4MHz");
            centerB = 0;
            centerT = 5;
            break;
        case 15:
            sprintf(bwStr, "3MHz");
            centerB = 4;
            centerT = 10;
            break;
        case 25:
            sprintf(bwStr, "5MHz");
            centerB = 9;
            centerT = 15;
            break;
        case 50:
            sprintf(bwStr, "10MHz");
            centerB = 22;
            centerT = 27;
            break;
        case 75:
            sprintf(bwStr, "15MHz");
            centerB = 34;
            centerT = 40;
            break;
        case 100:
            sprintf(bwStr, "20MHz");
            centerB = 47;
            centerT = 52;
            break;
        default:
            printf("ERR: wrong N_DL_RB %d\n", N_DL_RB);
            return -1;
    }

    for (i=0; i<72; i++)
    {
        kp = i;
        k[i] = ((((N_DL_RB * N_RB_sc) / 2) - 36) + kp);
    }


    printf("PBCH subcarrier at RB ([1;33m%s[0m):\n", bwStr);
    printf("\n");
    for (rb=0; rb<N_DL_RB; rb++)
    {
        int sc;

        if ((rb >= centerB) && (rb <= centerT))
        {
            printf("RB[[1;35m%2d[0m]", rb);
            for (i=0; i<N_RB_sc; i++)
            {
                sc = ((rb * N_RB_sc) + i);
                if ((sc >= k[0]) && (sc <= k[71]))
                {
                    printf(" [1;32m%4d[0m", sc);
                }
                else
                {
                    printf(" %4d", sc);
                }
            }
        }
        else
        {
            printf("RB[%2d]", rb);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

