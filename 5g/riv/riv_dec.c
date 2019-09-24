#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


void decodeRiv(int N_size_BWP, int RIV, int *RB_start, int *L_RBs)
{
    int quotient, remainder;
    int rbStart[2];
    int lCRBs[2];
    int resAlloc[2];

    quotient  = DIV_FLOOR(RIV, N_size_BWP);
    remainder = (RIV % N_size_BWP);

    rbStart[0] = (remainder);
    rbStart[1] = (N_size_BWP - 1 - remainder);
    lCRBs[0]   = (quotient + 1);
    lCRBs[1]   = (N_size_BWP - quotient + 1);

    resAlloc[0] = (rbStart[0] + lCRBs[0] - 1);
    resAlloc[1] = (rbStart[1] + lCRBs[1] - 1);

    if ((resAlloc[0] < N_size_BWP) && (resAlloc[1] >= N_size_BWP))
    {
        *RB_start = rbStart[0];
        *L_RBs = lCRBs[0];
    }
    else if ((resAlloc[0] >= N_size_BWP) && (resAlloc[1] < N_size_BWP))
    {
        *RB_start = rbStart[1];
        *L_RBs = lCRBs[1];
    }
    else
    {
        printf("un-identify resource allocation type 2 (RIV=%d)\n\n", RIV);
        *RB_start = 0;
        *L_RBs = 0;
        exit(0);
    }
}

/* 38.214 Section 5.1.2.2.2 Downlink resource allocation type 1 */
int main(int argc, char *argv[])
{
    int N_size_BWP = 275;
    int RIV = 0;
    int RB_start;
    int L_RBs;


    if (argc > 1)
    {
        RIV = atoi( argv[1] );
        if ((RIV >= 0) && (RIV <= 37949))
        {
            decodeRiv(N_size_BWP, RIV, &RB_start, &L_RBs);

            printf("\n");
            printf("RB start  = %d\n", RB_start);
            printf("RB number = %d\n", L_RBs);
            printf("\n");
        }
        else
        {
            printf("ERR: wrong input RIV\n\n");
        }
    }
    else
    {
        printf("Usage: riv_dec RIV\n\n");
        return 0;
    }

    return 0;
}

