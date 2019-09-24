#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


int encodeRiv(int N_size_BWP, int RB_start, int L_RBs)
{
    int RIV = -1;

    if (L_RBs < 1) return -1;
    if (L_RBs > (N_size_BWP - RB_start)) return -1;

    if ((L_RBs - 1) < DIV_FLOOR(N_size_BWP, 2))
    {
        RIV = ((N_size_BWP * (L_RBs - 1)) + RB_start);
    }
    else
    {
        RIV = ((N_size_BWP * (N_size_BWP - L_RBs + 1)) + (N_size_BWP - 1 - RB_start));
    }

    return RIV;
}

/* 38.214 Section 5.1.2.2.2 Downlink resource allocation type 1 */
int main(int argc, char *argv[])
{
    int N_size_BWP = 275;
    int RB_start;
    int L_RBs;
    int RIV;


    if (argc > 2)
    {
        RB_start = atoi( argv[1] );
        L_RBs = atoi( argv[2] );

        RIV = encodeRiv(N_size_BWP, RB_start, L_RBs);
        if (RIV < 0)
        {
            printf("ERR: wrong input values\n\n");
        }
        else
        {
            printf("\n");
            printf("RIV = %d\n", RIV);
            printf("\n");
        }
    }
    else
    {
        printf("Usage: riv_enc RB_start L_RBs\n\n");
        return 0;
    }

    return 0;
}

