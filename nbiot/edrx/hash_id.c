#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

/*
 * R13 36.304 Annex B: Example of Hashed ID Calculation using 32-bit FCS
 */
int main(void)
{
    unsigned long long A;
    unsigned long long B;
    unsigned long long Q;
    unsigned long long R;

    unsigned long long Y1;
    unsigned long long Y2;
    unsigned long long fcs;


    A = 0xFFFFFFFF00000000LL;
    B = 0x104C11DB7LL;
    Q = bin_div64(A, B, &R);
    printf("step a)\n");
    printf("  numerator  :   0x%llX\n", A);
    printf("  denominator:   0x%llX\n", B);
    printf("  remainder Y1 = 0x%llX\n", R);
    printf("\n");
    Y1 = R;


    A = 0x1234123400000000LL;
    B = 0x104C11DB7LL;
    Q = bin_div64(A, B, &R);
    printf("step b)\n");
    printf("  numerator  :   0x%llX\n", A);
    printf("  denominator:   0x%llX\n", B);
    printf("  remainder Y2 = 0x%llX\n", R);
    printf("\n");
    Y2 = R;


    fcs = calc_fcs(Y1, Y2);
    printf("Hashed_ID (FCS) = 0x%llX\n", fcs);
    printf("\n");


    return 0;
}

