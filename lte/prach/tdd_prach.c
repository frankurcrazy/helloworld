#include <stdio.h>
#include <stdlib.h>  /* atoi */
#include "tdd_config_0.c"
#include "tdd_config_1.c"
#include "tdd_config_2.c"
#include "tdd_config_3.c"
#include "tdd_config_4.c"
#include "tdd_config_5.c"
#include "tdd_config_6.c"

typedef enum
{
    TDD_PRACH_EVEN_FRAMES = 0,
    TDD_PRACH_ODD_FRAMES  = 1,
    TDD_PRACH_ALL_FRAMES  = 2,
    TDD_PRACH_NO_FRAMES   = 3
} tTddPrachT0RA;


#define D  2
#define U  1
#define S  0
int TDD_FRAME[7][10] = {
   /* 0  1  2  3  4  5  6  7  8  9      CFG */
    { D, S, U, U, U, D, S, U, U, U }, /* 0  */
    { D, S, U, U, D, D, S, U, U, D }, /* 1  */
    { D, S, U, D, D, D, S, U, D, D }, /* 2  */
    { D, S, U, U, U, D, D, D, D, D }, /* 3  */
    { D, S, U, U, D, D, D, D, D, D }, /* 4  */
    { D, S, U, D, D, D, D, D, D, D }, /* 5  */
    { D, S, U, U, U, D, S, U, U, D }  /* 6  */
};

int CONFIG_SF[2][4] = {
    { 2, 3, 4, 1 },
    { 7, 8, 9, 6 }
};

int  (*CONFIG_MAP)[4] = { NULL };
int   *CONFIG_NUM = NULL;


/*
* 3GPP TS 36.211 Table 5.7.1-4
*   Frame structure type 2 random access preamble mapping in time and frequency
*/
int main(int argc, char *argv[])
{
    int sf[2][10];
    int frame;
    int i, j, k;
    int n, x, y;

    int cfg;
    int fra;
    int t0ra;
    int t1ra;
    int t2ra;


    if (argc != 2)
    {
        printf("Usage: tdd_prach UL_DL_configuration (0 ~ 6)\n\n");
        return -1;
    }

    cfg = atoi( argv[1] );
    switch ( cfg )
    {
        case 0:
            CONFIG_MAP = config0_map;
            CONFIG_NUM = config0_num;
            break;

        case 1:
            CONFIG_MAP = config1_map;
            CONFIG_NUM = config1_num;
            break;

        case 2:
            CONFIG_MAP = config2_map;
            CONFIG_NUM = config2_num;
            break;

        case 3:
            CONFIG_MAP = config3_map;
            CONFIG_NUM = config3_num;
            break;

        case 4:
            CONFIG_MAP = config4_map;
            CONFIG_NUM = config4_num;
            break;

        case 5:
            CONFIG_MAP = config5_map;
            CONFIG_NUM = config5_num;
            break;

        case 6:
            CONFIG_MAP = config6_map;
            CONFIG_NUM = config6_num;
            break;

        default:
            printf("UL_DL_configuration should between 0 and 6\n\n");
            return -1;
    }

    printf("\033[1;32mTDD\033[0m UL/DL configuration [\033[1;33m %d\033[m ]\n", cfg);
    printf("------+------------------------------------------\n");
    printf("      | Even frame           Odd frame\n");
    printf("      | 0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9\n");
    printf("Index |");
    for (j=0; j<2; j++)
    {
        for (i=0; i<10; i++)
        {
            printf(
                " %c",
                ((D == TDD_FRAME[cfg][i]) ? 'D' : ((U == TDD_FRAME[cfg][i]) ? 'U' : 'S'))
            );
        }
        printf(" ");
    }
    printf("\n");
    printf("------+------------------------------------------\n");

    for (i=0, x=0; i<64; i++)
    {
        frame = TDD_PRACH_NO_FRAMES;
        for (j=0; j<2; j++)
        {
            for (k=0; k<10; k++)
            {
                sf[j][k] = 0;
            }
        }

        for (j=0; j<CONFIG_NUM[i]; j++)
        {
            fra  = CONFIG_MAP[x][0];
	        t0ra = CONFIG_MAP[x][1];
	        t1ra = CONFIG_MAP[x][2];
            t2ra = CONFIG_MAP[x][3];

            n = CONFIG_SF[t1ra][t2ra];
            if (1 == t0ra)
            {
                /* even frames */
                sf[0][n]++;
                frame = TDD_PRACH_EVEN_FRAMES;
                y = 0;
            }
            else if (2 == t0ra)
            {
                /* odd frames */
                sf[1][n]++;
                frame = TDD_PRACH_ODD_FRAMES;
                y = 1;
            }
            else
            {
                /* all frames */
                sf[0][n]++;
                sf[1][n]++;
                frame = TDD_PRACH_ALL_FRAMES;
                y = 0;
            }

            x++;
        }

#if 0
        printf("    { %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d },\n",
            sf[y][0],
            sf[y][1],
            sf[y][2],
            sf[y][3],
            sf[y][4],
            sf[y][5],
            sf[y][6],
            sf[y][7],
            sf[y][8],
            sf[y][9],
            frame
        );
#else
        printf("  %2d  |", i);
        ((sf[0][0] > 0) ? printf("\033[1;31m %d\033[m", sf[0][0]) : ((D == TDD_FRAME[cfg][0]) ? printf(" x") : printf(" .")));
        ((sf[0][1] > 0) ? printf("\033[1;31m %d\033[m", sf[0][1]) : ((D == TDD_FRAME[cfg][1]) ? printf(" x") : printf(" .")));
        ((sf[0][2] > 0) ? printf("\033[1;31m %d\033[m", sf[0][2]) : ((D == TDD_FRAME[cfg][2]) ? printf(" x") : printf(" .")));
        ((sf[0][3] > 0) ? printf("\033[1;31m %d\033[m", sf[0][3]) : ((D == TDD_FRAME[cfg][3]) ? printf(" x") : printf(" .")));
        ((sf[0][4] > 0) ? printf("\033[1;31m %d\033[m", sf[0][4]) : ((D == TDD_FRAME[cfg][4]) ? printf(" x") : printf(" .")));
        ((sf[0][5] > 0) ? printf("\033[1;31m %d\033[m", sf[0][5]) : ((D == TDD_FRAME[cfg][5]) ? printf(" x") : printf(" .")));
        ((sf[0][6] > 0) ? printf("\033[1;31m %d\033[m", sf[0][6]) : ((D == TDD_FRAME[cfg][6]) ? printf(" x") : printf(" .")));
        ((sf[0][7] > 0) ? printf("\033[1;31m %d\033[m", sf[0][7]) : ((D == TDD_FRAME[cfg][7]) ? printf(" x") : printf(" .")));
        ((sf[0][8] > 0) ? printf("\033[1;31m %d\033[m", sf[0][8]) : ((D == TDD_FRAME[cfg][8]) ? printf(" x") : printf(" .")));
        ((sf[0][9] > 0) ? printf("\033[1;31m %d\033[m", sf[0][9]) : ((D == TDD_FRAME[cfg][9]) ? printf(" x") : printf(" .")));
        printf(" ");
        ((sf[1][0] > 0) ? printf("\033[1;31m %d\033[m", sf[1][0]) : ((D == TDD_FRAME[cfg][0]) ? printf(" x") : printf(" .")));
        ((sf[1][1] > 0) ? printf("\033[1;31m %d\033[m", sf[1][1]) : ((D == TDD_FRAME[cfg][1]) ? printf(" x") : printf(" .")));
        ((sf[1][2] > 0) ? printf("\033[1;31m %d\033[m", sf[1][2]) : ((D == TDD_FRAME[cfg][2]) ? printf(" x") : printf(" .")));
        ((sf[1][3] > 0) ? printf("\033[1;31m %d\033[m", sf[1][3]) : ((D == TDD_FRAME[cfg][3]) ? printf(" x") : printf(" .")));
        ((sf[1][4] > 0) ? printf("\033[1;31m %d\033[m", sf[1][4]) : ((D == TDD_FRAME[cfg][4]) ? printf(" x") : printf(" .")));
        ((sf[1][5] > 0) ? printf("\033[1;31m %d\033[m", sf[1][5]) : ((D == TDD_FRAME[cfg][5]) ? printf(" x") : printf(" .")));
        ((sf[1][6] > 0) ? printf("\033[1;31m %d\033[m", sf[1][6]) : ((D == TDD_FRAME[cfg][6]) ? printf(" x") : printf(" .")));
        ((sf[1][7] > 0) ? printf("\033[1;31m %d\033[m", sf[1][7]) : ((D == TDD_FRAME[cfg][7]) ? printf(" x") : printf(" .")));
        ((sf[1][8] > 0) ? printf("\033[1;31m %d\033[m", sf[1][8]) : ((D == TDD_FRAME[cfg][8]) ? printf(" x") : printf(" .")));
        ((sf[1][9] > 0) ? printf("\033[1;31m %d\033[m", sf[1][9]) : ((D == TDD_FRAME[cfg][9]) ? printf(" x") : printf(" .")));
        printf("\n");
#endif
    }

    printf("\n");

    return 0;
}
