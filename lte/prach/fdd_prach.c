#include <stdio.h>
#include <stdlib.h>  /* atoi */
#include "fdd_config.c"


/*
* 3GPP TS 36.211 Table 5.7.1-2
*   Frame structure type 1 random access configuration for preamble format 0-3
*/
int main(int argc, char *argv[])
{
    int i;

    printf("\033[1;32mFDD\033[0m\n");
    printf("------+------------------------------------------\n");
    printf("      | Even frame           Odd frame\n");
    printf("Index | 0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9\n");
    printf("------+------------------------------------------\n");

    for (i=0; i<64; i++)
    {
        printf("  %2d  |", i);
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][0]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][1]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][2]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][3]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][4]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][5]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][6]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][7]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][8]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & E) && (table_5_7_1_2[i][9]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        printf(" ");
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][0]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][1]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][2]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][3]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][4]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][5]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][6]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][7]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][8]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        ((table_5_7_1_2[i][10] & O) && (table_5_7_1_2[i][9]) ? printf("\033[1;31m P\033[m") : printf(" x"));
        printf("\n");
    }

    printf("\n");

    return 0;
}
