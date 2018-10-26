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
    unsigned long long C;
    unsigned long long D;

    unsigned long long Y1;
    unsigned long long Y2;
    unsigned long long fcs;


    A = 0xFFFFFFFF00000000;
    B = 0x104C11DB7;
    C = bin_div64(A, B, &D);
    printf("step a)\n");
    printf("  numerator  :   0x%llX\n", A);
    printf("  denominator:   0x%llX\n", B);
    printf("  remainder Y1 = 0x%llX\n", D);
    printf("\n");
    Y1 = D;


    A = 0x1234123400000000;
    B = 0x104C11DB7;
    C = bin_div64(A, B, &D);
    printf("step b)\n");
    printf("  numerator  :   0x%llX\n", A);
    printf("  denominator:   0x%llX\n", B);
    printf("  remainder Y2 = 0x%llX\n", D);
    printf("\n");
    Y2 = D;


    fcs = calc_fcs(Y1, Y2);
    printf("Hashed_ID (FCS) = 0x%llX\n", fcs);
    printf("\n");


    return 0;
}

