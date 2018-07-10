#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"


void help(void)
{
    printf("Usage: freq_dl [OPTION]...\n");
    printf("  -e EARFCN   N_DL (0 ~ 262143).\n");
    printf("  -o Offset   M_DL (-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, -0.5,\n");
    printf("                      0,  1,  2,  3,  4,  5,  6,  7,  8,  9).\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.104 Section 5.7.3 Carrier frequency and EARFCN
*/
int main(int argc, char *argv[])
{
    double  earfcn = 0;
    double  offset = 0;
    double  freq;
    double  bw;
    int  band;
    int  F_DL_low;
    int  N_Offs_DL;
    int  ch;

    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "e:o:h")) != -1)
    {
        switch ( ch )
        {
            case 'e':
                /* N_DL */
                earfcn = atof( optarg );
                break;
            case 'o':
                /* N_Offs-DL */
                offset = atof( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    /* F_DL = F_DL_low + 0.1 x (N_DL - N_Offs-DL) + 0.0025 x (2 x M_DL + 1) */
    if ((earfcn >= 0) && (earfcn <= 599))
    {
        band = 1;
        F_DL_low  = 2110;
        N_Offs_DL = 0;
        bw = 60;
    }
    else if ((earfcn >= 600) && (earfcn <= 1199))
    {
        band = 2;
        F_DL_low  = 1930;
        N_Offs_DL = 600;
        bw = 60;
    }
    else if ((earfcn >= 1200) && (earfcn <= 1949))
    {
        band = 3;
        F_DL_low  = 1805;
        N_Offs_DL = 1200;
        bw = 75;
    }
    else if ((earfcn >= 1950) && (earfcn <= 2399))
    {
        band = 4;
        F_DL_low  = 2110;
        N_Offs_DL = 1950;
        bw = 45;
    }
    else if ((earfcn >= 2400) && (earfcn <= 2649))
    {
        band = 5;
        F_DL_low  = 859;
        N_Offs_DL = 2400;
        bw = 25;
    }
    else if ((earfcn >= 2650) && (earfcn <= 2749))
    {
        band = 6;
        F_DL_low  = 875;
        N_Offs_DL = 2650;
        bw = 10;
    }
    else if ((earfcn >= 2750) && (earfcn <= 3449))
    {
        band = 7;
        F_DL_low  = 2620;
        N_Offs_DL = 2750;
        bw = 70;
    }
    else if ((earfcn >= 3450) && (earfcn <= 3799))
    {
        band = 8;
        F_DL_low  = 925;
        N_Offs_DL = 3450;
        bw = 35;
    }
    else if ((earfcn >= 3800) && (earfcn <= 4149))
    {
        band = 9;
        F_DL_low  = 1844.9;
        N_Offs_DL = 3800;
        bw = 35;
    }
    else if ((earfcn >= 4150) && (earfcn <= 4749))
    {
        band = 10;
        F_DL_low  = 2110;
        N_Offs_DL = 4150;
        bw = 60;
    }
    else if ((earfcn >= 4750) && (earfcn <= 4949))
    {
        band = 11;
        F_DL_low  = 1475.9;
        N_Offs_DL = 4750;
        bw = 20;
    }
    else if ((earfcn >= 5010) && (earfcn <= 5179))
    {
        band = 12;
        F_DL_low  = 729;
        N_Offs_DL = 5010;
        bw = 17;
    }
    else if ((earfcn >= 5180) && (earfcn <= 5279))
    {
        band = 13;
        F_DL_low  = 746;
        N_Offs_DL = 5180;
        bw = 10;
    }
    else if ((earfcn >= 5280) && (earfcn <= 5379))
    {
        band = 14;
        F_DL_low  = 758;
        N_Offs_DL = 5280;
        bw = 10;
    }
    else if ((earfcn >= 5730) && (earfcn <= 5849))
    {
        band = 17;
        F_DL_low  = 734;
        N_Offs_DL = 5730;
        bw = 12;
    }
    else if ((earfcn >= 5850) && (earfcn <= 5999))
    {
        band = 18;
        F_DL_low  = 860;
        N_Offs_DL = 5850;
        bw = 15;
    }
    else if ((earfcn >= 6000) && (earfcn <= 6149))
    {
        band = 19;
        F_DL_low  = 875;
        N_Offs_DL = 6000;
        bw = 15;
    }
    else if ((earfcn >= 6150) && (earfcn <= 6449))
    {
        band = 20;
        F_DL_low  = 791;
        N_Offs_DL = 6150;
        bw = 30;
    }
    else if ((earfcn >= 6450) && (earfcn <= 6599))
    {
        band = 21;
        F_DL_low  = 1495.9;
        N_Offs_DL = 6450;
        bw = 15;
    }
    else if ((earfcn >= 6600) && (earfcn <= 7399))
    {
        band = 22;
        F_DL_low  = 3510;
        N_Offs_DL = 6600;
        bw = 80;
    }
    else if ((earfcn >= 7500) && (earfcn <= 7699))
    {
        band = 23;
        F_DL_low  = 2180;
        N_Offs_DL = 7500;
        bw = 20;
    }
    else if ((earfcn >= 7700) && (earfcn <= 8039))
    {
        band = 24;
        F_DL_low  = 1525;
        N_Offs_DL = 7700;
        bw = 34;
    }
    else if ((earfcn >= 8040) && (earfcn <= 8689))
    {
        band = 25;
        F_DL_low  = 1930;
        N_Offs_DL = 8040;
        bw = 65;
    }
    else if ((earfcn >= 8690) && (earfcn <= 9039))
    {
        band = 26;
        F_DL_low  = 859;
        N_Offs_DL = 8690;
        bw = 35;
    }
    else if ((earfcn >= 9040) && (earfcn <= 9209))
    {
        band = 27;
        F_DL_low  = 852;
        N_Offs_DL = 9040;
        bw = 17;
    }
    else if ((earfcn >= 9210) && (earfcn <= 9659))
    {
        band = 28;
        F_DL_low  = 758;
        N_Offs_DL = 9210;
        bw = 45;
    }
    else if ((earfcn >= 9660) && (earfcn <= 9769))
    {
        band = 29;
        F_DL_low  = 717;
        N_Offs_DL = 9660;
        bw = 12;
    }
    else if ((earfcn >= 9770) && (earfcn <= 9869))
    {
        band = 30;
        F_DL_low  = 2350;
        N_Offs_DL = 9770;
        bw = 10;
    }
    else if ((earfcn >= 9870) && (earfcn <= 9919))
    {
        band = 31;
        F_DL_low  = 462.5;
        N_Offs_DL = 9870;
        bw = 5;
    }
    else if ((earfcn >= 9920) && (earfcn <= 10359))
    {
        band = 32;
        F_DL_low  = 1452;
        N_Offs_DL = 9920;
        bw = 44;
    }
    else if ((earfcn >= 36000) && (earfcn <= 36199))
    {
        band = 33;
        F_DL_low  = 1900;
        N_Offs_DL = 36000;
        bw = 20;
    }
    else if ((earfcn >= 36200) && (earfcn <= 36349))
    {
        band = 34;
        F_DL_low  = 2010;
        N_Offs_DL = 36200;
        bw = 15;
    }
    else if ((earfcn >= 36350) && (earfcn <= 36949))
    {
        band = 35;
        F_DL_low  = 1850;
        N_Offs_DL = 36350;
        bw = 60;
    }
    else if ((earfcn >= 36950) && (earfcn <= 37549))
    {
        band = 36;
        F_DL_low  = 1930;
        N_Offs_DL = 36950;
        bw = 60;
    }
    else if ((earfcn >= 37550) && (earfcn <= 37749))
    {
        band = 37;
        F_DL_low  = 1910;
        N_Offs_DL = 37550;
        bw = 20;
    }
    else if ((earfcn >= 37750) && (earfcn <= 38249))
    {
        band = 38;
        F_DL_low  = 2570;
        N_Offs_DL = 37750;
        bw = 50;
    }
    else if ((earfcn >= 38250) && (earfcn <= 38649))
    {
        band = 39;
        F_DL_low  = 1880;
        N_Offs_DL = 38250;
        bw = 40;
    }
    else if ((earfcn >= 38650) && (earfcn <= 39649))
    {
        band = 40;
        F_DL_low  = 2300;
        N_Offs_DL = 38650;
        bw = 100;
    }
    else if ((earfcn >= 39650) && (earfcn <= 41589))
    {
        band = 41;
        F_DL_low  = 2496;
        N_Offs_DL = 39650;
        bw = 194;
    }
    else if ((earfcn >= 41590) && (earfcn <= 43589))
    {
        band = 42;
        F_DL_low  = 3400;
        N_Offs_DL = 41590;
        bw = 200;
    }
    else if ((earfcn >= 43590) && (earfcn <= 45589))
    {
        band = 43;
        F_DL_low  = 3600;
        N_Offs_DL = 43590;
        bw = 200;
    }
    else if ((earfcn >= 45590) && (earfcn <= 46589))
    {
        band = 44;
        F_DL_low  = 703;
        N_Offs_DL = 45590;
        bw = 100;
    }
    else if ((earfcn >= 46590) && (earfcn <= 46789))
    {
        band = 45;
        F_DL_low  = 1447;
        N_Offs_DL = 46590;
        bw = 20;
    }
    else if ((earfcn >= 46790) && (earfcn <= 54539))
    {
        band = 46;
        F_DL_low  = 5150;
        N_Offs_DL = 46790;
        bw = 775;
    }
    else if ((earfcn >= 65536) && (earfcn <= 66435))
    {
        band = 65;
        F_DL_low  = 2110;
        N_Offs_DL = 65536;
        bw = 90;
    }
    else if ((earfcn >= 66436) && (earfcn <= 67335))
    {
        band = 66;
        F_DL_low  = 2110;
        N_Offs_DL = 66436;
        bw = 90;
    }
    else if ((earfcn >= 67336) && (earfcn <= 67535))
    {
        band = 67;
        F_DL_low  = 738;
        N_Offs_DL = 67336;
        bw = 20;
    }
    else if ((earfcn >= 67536) && (earfcn <= 67835))
    {
        band = 68;
        F_DL_low  = 753;
        N_Offs_DL = 67536;
        bw = 30;
    }
    else
    {
        band = 0;
        F_DL_low  = 0;
        N_Offs_DL = 0;
        bw = 0;
    }

    freq = (F_DL_low + (0.1 * (earfcn - N_Offs_DL)) + (0.0025 * ((2 * offset) + 1)));

    printf("Band %d\n", band);
    printf("Frequency %.4f MHz\n", freq);
    printf("Bandwidth %.0f MHz\n", bw);

    return 0;
}

