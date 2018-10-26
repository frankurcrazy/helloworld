#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


#define CEIL(x, y)   (((x) + ((y) - 1)) / (y))
#define FLOOR(x, y)  ((x) / (y))


typedef enum
{
    PTW_NULL  = 0,
    PTW_START = 1,
    PTW_END   = 2
} tPtwState;


void show_PTW(int UE_ID_H, int T_eDRX_H, int i_eDRX, int L)
{
    int H_SFN;
    int SFN;
    int PH;  /* Paging Hyperframe */
    int PTW_start;
    int PTW_end;
    int state = PTW_NULL;
    int PH2 = 0;


    printf("--------------+--------------\n");
    printf(" PTW start    | PTW end\n");
    printf("-------+------+-------+------\n");
    printf(" H-SFN | SFN  | H-SFN | SFN \n");
    printf("-------+------+-------+------\n");
    for (H_SFN=0; H_SFN<1024; H_SFN++)
    {
        if ((H_SFN % T_eDRX_H) == (UE_ID_H % T_eDRX_H))
        {
            PH = H_SFN;
            for (SFN=0; SFN<1024; SFN++)
            {
                if (SFN == (256 * i_eDRX))
                {
                    PTW_start = SFN;
                    printf(" [1;36m%4d[0m  | [1;33m%4d[0m |", PH, PTW_start);
                    if (PTW_NULL == state)
                    {
                        PH2 = PH;
                    }
                    state = PTW_START;
                }
                if ((PTW_START == state) &&
                    (SFN == ((PTW_start + (L * 256) - 1) % 1024)))
                {
                    PTW_end = SFN;
                    printf(" [1;36m%4d[0m  | [1;33m%4d[0m\n", PH, PTW_end);
                    state = PTW_END;
                }
            }
        }
    }
    if (PTW_START == state)
    {
        printf(" [1;36m%4d[0m  | [1;33m%4d[0m\n", PH2, PTW_end);
    }
    printf("-------+------+-------+------\n");
    printf("\n");
}

void help(void)
{
    printf("Usage: paging [OPTION]...\n");
    printf("\n");
    printf("  -m M-TMSI  M-TMSI (32-bit LSB of S-TMSI).\n");
    printf("  -t T_eDRX  eDRX cycle (2, 4, 6, 8, 10, 12, 14, 16, 32,\n");
    printf("                         64, 128, 256, 512, 1024) hyper frames.\n");
    printf("  -w PTW     Paging time window (1, 2, 3, 4, 5, 6, 7, 8, 9,\n");
    printf("                                 10, 11, 12, 13, 14, 15, 16) 2.56s.\n");
    printf("\n");
    printf("  -h       Show the help message.\n");
    printf("\n");
}

/*
 * R13 36.304 Section 7.1 Discontinuous Reception for paging
 */
int main(int argc, char *argv[])
{
    unsigned long long M_TMSI = 0;
    unsigned long long Y2;
    unsigned int Hashed_ID;
    int UE_ID_H = 0;
    int L = 10;
    int T_eDRX_H = 8;
    int i_eDRX;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "m:t:w:b:h")) != -1)
    {
        switch ( ch )
        {
            case 'm':
                M_TMSI = atoi( optarg );
                break;
            case 't':
                T_eDRX_H = atoi( optarg );
                break;
            case 'w':
                L = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((T_eDRX_H !=    2) &&
        (T_eDRX_H !=    4) &&
        (T_eDRX_H !=    6) &&
        (T_eDRX_H !=    8) &&
        (T_eDRX_H !=   10) &&
        (T_eDRX_H !=   12) &&
        (T_eDRX_H !=   14) &&
        (T_eDRX_H !=   16) &&
        (T_eDRX_H !=   32) &&
        (T_eDRX_H !=   64) &&
        (T_eDRX_H !=  128) &&
        (T_eDRX_H !=  256) &&
        (T_eDRX_H !=  512) &&
        (T_eDRX_H != 1024))
    {
        printf("ERR: incorrect T_eDRX %d\n\n", T_eDRX_H);
        return -1;
    }

    if ((L !=  1) &&
        (L !=  2) &&
        (L !=  3) &&
        (L !=  4) &&
        (L !=  5) &&
        (L !=  6) &&
        (L !=  7) &&
        (L !=  8) &&
        (L !=  9) &&
        (L != 10) &&
        (L != 11) &&
        (L != 12) &&
        (L != 13) &&
        (L != 14) &&
        (L != 15) &&
        (L != 16))
    {
        printf("ERR: incorrect PTW %d\n\n", L);
        return -1;
    }


    bin_div64(M_TMSI, 0x104C11DB7LL, &Y2);
    Hashed_ID = calc_fcs(0xC704DD7BLL, Y2);
    UE_ID_H = (Hashed_ID >> 20);
    i_eDRX = (FLOOR(UE_ID_H, T_eDRX_H) % 4);


    printf("\n");
    printf("M_TMSI  = %llu\n", M_TMSI);
    printf("UE_ID_H = %d\n", UE_ID_H);
    printf("PTW     = %d\n", L);
    printf("T_eDRX  = %d\n", T_eDRX_H);
    printf("i_eDRX  = %d\n", i_eDRX);
    printf("\n");

    show_PTW(
        UE_ID_H,
        T_eDRX_H,
        i_eDRX,
        L
    );

    return 0;
}

