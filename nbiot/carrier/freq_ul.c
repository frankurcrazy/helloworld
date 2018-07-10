#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "common.h"


void help(void)
{
    printf("Usage: freq_ul [OPTION]...\n");
    printf("  -e EARFCN   N_UL (0 ~ 262143).\n");
    printf("  -o Offset   M_UL (-10, -9, -8, -7, -6, -5, -4, -3, -2, -1,\n");
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
    int  F_UL_low;
    int  N_Offs_UL;
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
                /* N_UL */
                earfcn = atof( optarg );
                break;
            case 'o':
                /* N_Offs-UL */
                offset = atof( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    /* F_UL = F_UL_low + 0.1 x (N_UL - N_Offs-UL) + 0.0025 x (2 x M_UL) */
    if ((earfcn >= 18000) && (earfcn <= 18599))
    {
        band = 1;
        F_UL_low  = 1920;
        N_Offs_UL = 18000;
        bw = 60;
    }
    else if ((earfcn >= 18600) && (earfcn <= 19199))
    {
        band = 2;
        F_UL_low  = 1850;
        N_Offs_UL = 18600;
        bw = 60;
    }
    else if ((earfcn >= 19200) && (earfcn <= 19949))
    {
        band = 3;
        F_UL_low  = 1710;
        N_Offs_UL = 19200;
        bw = 75;
    }
    else if ((earfcn >= 19950) && (earfcn <= 20399))
    {
        band = 4;
        F_UL_low  = 1710;
        N_Offs_UL = 19950;
        bw = 45;
    }
    else if ((earfcn >= 20400) && (earfcn <= 20649))
    {
        band = 5;
        F_UL_low  = 824;
        N_Offs_UL = 20400;
        bw = 25;
    }
    else if ((earfcn >= 20650) && (earfcn <= 20749))
    {
        band = 6;
        F_UL_low  = 830;
        N_Offs_UL = 20650;
        bw = 10;
    }
    else if ((earfcn >= 20750) && (earfcn <= 21449))
    {
        band = 7;
        F_UL_low  = 2500;
        N_Offs_UL = 20750;
        bw = 70;
    }
    else if ((earfcn >= 21450) && (earfcn <= 21799))
    {
        band = 8;
        F_UL_low  = 880;
        N_Offs_UL = 21450;
        bw = 35;
    }
    else if ((earfcn >= 21800) && (earfcn <= 22149))
    {
        band = 9;
        F_UL_low  = 1749.9;
        N_Offs_UL = 21800;
        bw = 35;
    }
    else if ((earfcn >= 22150) && (earfcn <= 22749))
    {
        band = 10;
        F_UL_low  = 1710;
        N_Offs_UL = 22150;
        bw = 60;
    }
    else if ((earfcn >= 22750) && (earfcn <= 22949))
    {
        band = 11;
        F_UL_low  = 1427.9;
        N_Offs_UL = 22750;
        bw = 20;
    }
    else if ((earfcn >= 23010) && (earfcn <= 23179))
    {
        band = 12;
        F_UL_low  = 699;
        N_Offs_UL = 23010;
        bw = 17;
    }
    else if ((earfcn >= 23180) && (earfcn <= 23279))
    {
        band = 13;
        F_UL_low  = 777;
        N_Offs_UL = 23180;
        bw = 10;
    }
    else if ((earfcn >= 23280) && (earfcn <= 23379))
    {
        band = 14;
        F_UL_low  = 788;
        N_Offs_UL = 23280;
        bw = 10;
    }
    else if ((earfcn >= 23730) && (earfcn <= 23849))
    {
        band = 17;
        F_UL_low  = 704;
        N_Offs_UL = 23730;
        bw = 12;
    }
    else if ((earfcn >= 23850) && (earfcn <= 23999))
    {
        band = 18;
        F_UL_low  = 815;
        N_Offs_UL = 23850;
        bw = 15;
    }
    else if ((earfcn >= 24000) && (earfcn <= 24149))
    {
        band = 19;
        F_UL_low  = 830;
        N_Offs_UL = 24000;
        bw = 15;
    }
    else if ((earfcn >= 24150) && (earfcn <= 24449))
    {
        band = 20;
        F_UL_low  = 832;
        N_Offs_UL = 24150;
        bw = 30;
    }
    else if ((earfcn >= 24450) && (earfcn <= 24599))
    {
        band = 21;
        F_UL_low  = 1447.9;
        N_Offs_UL = 24450;
        bw = 15;
    }
    else if ((earfcn >= 24600) && (earfcn <= 25399))
    {
        band = 22;
        F_UL_low  = 3410;
        N_Offs_UL = 24600;
        bw = 80;
    }
    else if ((earfcn >= 25500) && (earfcn <= 25699))
    {
        band = 23;
        F_UL_low  = 2000;
        N_Offs_UL = 25500;
        bw = 20;
    }
    else if ((earfcn >= 25700) && (earfcn <= 26039))
    {
        band = 24;
        F_UL_low  = 1626.5;
        N_Offs_UL = 25700;
        bw = 34;
    }
    else if ((earfcn >= 26040) && (earfcn <= 26689))
    {
        band = 25;
        F_UL_low  = 1850;
        N_Offs_UL = 26040;
        bw = 65;
    }
    else if ((earfcn >= 26690) && (earfcn <= 27039))
    {
        band = 26;
        F_UL_low  = 814;
        N_Offs_UL = 26690;
        bw = 35;
    }
    else if ((earfcn >= 27040) && (earfcn <= 27209))
    {
        band = 27;
        F_UL_low  = 807;
        N_Offs_UL = 27040;
        bw = 17;
    }
    else if ((earfcn >= 27210) && (earfcn <= 27659))
    {
        band = 28;
        F_UL_low  = 703;
        N_Offs_UL = 27210;
        bw = 45;
    }
    else if ((earfcn >= 27660) && (earfcn <= 27759))
    {
        band = 30;
        F_UL_low  = 2305;
        N_Offs_UL = 27660;
        bw = 10;
    }
    else if ((earfcn >= 27760) && (earfcn <= 27809))
    {
        band = 31;
        F_UL_low  = 452.5;
        N_Offs_UL = 27760;
        bw = 5;
    }
    else if ((earfcn >= 36000) && (earfcn <= 36199))
    {
        band = 33;
        F_UL_low  = 1900;
        N_Offs_UL = 36000;
        bw = 20;
    }
    else if ((earfcn >= 36200) && (earfcn <= 36349))
    {
        band = 34;
        F_UL_low  = 2010;
        N_Offs_UL = 36200;
        bw = 15;
    }
    else if ((earfcn >= 36350) && (earfcn <= 36949))
    {
        band = 35;
        F_UL_low  = 1850;
        N_Offs_UL = 36350;
        bw = 60;
    }
    else if ((earfcn >= 36950) && (earfcn <= 37549))
    {
        band = 36;
        F_UL_low  = 1930;
        N_Offs_UL = 36950;
        bw = 60;
    }
    else if ((earfcn >= 37550) && (earfcn <= 37749))
    {
        band = 37;
        F_UL_low  = 1910;
        N_Offs_UL = 37550;
        bw = 20;
    }
    else if ((earfcn >= 37750) && (earfcn <= 38249))
    {
        band = 38;
        F_UL_low  = 2570;
        N_Offs_UL = 37750;
        bw = 50;
    }
    else if ((earfcn >= 38250) && (earfcn <= 38649))
    {
        band = 39;
        F_UL_low  = 1880;
        N_Offs_UL = 38250;
        bw = 40;
    }
    else if ((earfcn >= 38650) && (earfcn <= 39649))
    {
        band = 40;
        F_UL_low  = 2300;
        N_Offs_UL = 38650;
        bw = 100;
    }
    else if ((earfcn >= 39650) && (earfcn <= 41589))
    {
        band = 41;
        F_UL_low  = 2496;
        N_Offs_UL = 39650;
        bw = 194;
    }
    else if ((earfcn >= 41590) && (earfcn <= 43589))
    {
        band = 42;
        F_UL_low  = 3400;
        N_Offs_UL = 41590;
        bw = 200;
    }
    else if ((earfcn >= 43590) && (earfcn <= 45589))
    {
        band = 43;
        F_UL_low  = 3600;
        N_Offs_UL = 43590;
        bw = 200;
    }
    else if ((earfcn >= 45590) && (earfcn <= 46589))
    {
        band = 44;
        F_UL_low  = 703;
        N_Offs_UL = 45590;
        bw = 100;
    }
    else if ((earfcn >= 46590) && (earfcn <= 46789))
    {
        band = 45;
        F_UL_low  = 1447;
        N_Offs_UL = 46590;
        bw = 20;
    }
    else if ((earfcn >= 46790) && (earfcn <= 54539))
    {
        band = 46;
        F_UL_low  = 5150;
        N_Offs_UL = 46790;
        bw = 775;
    }
    else if ((earfcn >= 131072) && (earfcn <= 131971))
    {
        band = 65;
        F_UL_low  = 1920;
        N_Offs_UL = 131072;
        bw = 90;
    }
    else if ((earfcn >= 131972) && (earfcn <= 132671))
    {
        band = 66;
        F_UL_low  = 1710;
        N_Offs_UL = 131972;
        bw = 70;
    }
    else if ((earfcn >= 132672) && (earfcn <= 132971))
    {
        band = 68;
        F_UL_low  = 698;
        N_Offs_UL = 132672;
        bw = 30;
    }
    else
    {
        band = 0;
        F_UL_low  = 0;
        N_Offs_UL = 0;
        bw = 0;
    }

    freq = (F_UL_low + (0.1 * (earfcn - N_Offs_UL)) + (0.0025 * (2 * offset)));

    printf("Band %d\n", band);
    printf("Frequency %.4f MHz\n", freq);
    printf("Bandwidth %.0f MHz\n", bw);

    return 0;
}

