#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define DIV_CEIL(X, Y)   ( ( (X) + ((Y) - 1) ) / (Y) )
#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


int resource_allocation(int N_xL_RB, int RB_start, int L_CRBs)
{
    int RIV;

    if ((L_CRBs - 1) <= DIV_FLOOR(N_xL_RB, 2))
    {
        RIV = ((N_xL_RB * (L_CRBs - 1)) + RB_start);
    }
    else
    {
        RIV = ((N_xL_RB * (N_xL_RB - L_CRBs + 1)) + (N_xL_RB - 1 - RB_start));
    }

    return RIV;
}

/*
 * R13 36.213 7.1.6	Resource allocation
 */
int main(int argc, char *argv[])
{
    int N_xL_RB = 6;
    int RB_start;
    int L_CRBs;
    int RIV;

    printf("[ Resource allocation type 0 ]\n");
    printf("\n");
    printf("=============================\n");
    printf(" RIV  | RB start | RB number\n");
    printf("=============================\n");

    for (RB_start = 0; RB_start < N_xL_RB; RB_start++)
    {
        for (L_CRBs = 1; L_CRBs < (N_xL_RB - RB_start + 1); L_CRBs++)
        {
            RIV = resource_allocation(N_xL_RB, RB_start, L_CRBs);
            printf("%3d   |     %d    |     %d\n", RIV, RB_start, L_CRBs);
        }
        if (RB_start < (N_xL_RB - 1))
        {
            printf("------+----------+----------\n");
        }
    }

    printf("============================\n");
    printf("\n");

    return 0;
}

