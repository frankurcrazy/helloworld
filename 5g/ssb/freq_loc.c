#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct _tBand
{
    /* Operating frequency in MHz */
    int  F_UL_low;
    int  F_UL_high;
    int  F_DL_low;
    int  F_DL_high;

    /* Subcarrier spacing in KHz */
    int  SCS[2];

    /* GSCN range */
    int  first[2];
    int  last[2];
    int  step[2];
} tBand;

tBand g_band[280];


void SS_raster_1(int SCS, int first, int last, int step, int M)
{
    double SS_REF;
    int GSCN;

    for (GSCN=first; GSCN<=last; GSCN+=step)
    {
        int N;

        if ((GSCN < 2) || (GSCN > 7498))
        {
            printf("Wrong GSCN: %d\n", GSCN);
            continue;
        }

        if (((GSCN - ((M - 3) / 2)) % 3) == 0)
        {
            N = ((GSCN - ((M - 3) / 2)) / 3);
            SS_REF = ((N * 1200) + (M * 50));
            printf(" %4d    %5d    %.2f      %3d\n", N, GSCN, SS_REF, SCS);
        }
    }
}

void SS_raster_2(int SCS, int first, int last, int step)
{
    double SS_REF;
    int GSCN;

    for (GSCN=first; GSCN<=last; GSCN+=step)
    {
        int N;

        if ((GSCN < 7499) || (GSCN > 22255))
        {
            printf("Wrong GSCN: %d\n", GSCN);
            continue;
        }

        N = (GSCN - 7499);
        SS_REF = (3000 + (N * 1.44));
        printf(" %4d    %5d    %.2f      %3d\n", N, GSCN, SS_REF, SCS);
    }
}

void SS_raster_3(int SCS, int first, int last, int step)
{
    double SS_REF;
    int GSCN;

    for (GSCN=first; GSCN<=last; GSCN+=step)
    {
        int N;

        if ((GSCN < 22256) || (GSCN > 26639))
        {
            printf("Wrong GSCN: %d\n", GSCN);
            continue;
        }

        N = (GSCN - 22256);
        SS_REF = (24250.08 + (N * 17.28));
        printf(" %4d    %5d    %.2f      %3d\n", N, GSCN, SS_REF, SCS);
    }
}

void help(void)
{
    printf("Usage: freq_loc [OPTION]...\n");
    printf("\n");
    printf("  -b   NR operating band.\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.3 Synchronization raster */
int main(int argc, char *argv[])
{
    int band = 1;
    int n;
    int M;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "b:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                band = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    memset(&(g_band[0]), 0, (sizeof( tBand ) * 100));
    /* n1 */
    g_band[1].F_UL_low  = 1920;
    g_band[1].F_UL_high = 1980;
    g_band[1].F_DL_low  = 2110;
    g_band[1].F_DL_high = 2170;
    g_band[1].SCS[0] = 15;
    g_band[1].first[0] = 5279;
    g_band[1].last[0]  = 5419;
    g_band[1].step[0]  = 1;
    /* n2 */
    g_band[2].F_UL_low  = 1850;
    g_band[2].F_UL_high = 1910;
    g_band[2].F_DL_low  = 1930;
    g_band[2].F_DL_high = 1990;
    g_band[2].SCS[0] = 15;
    g_band[2].first[0] = 4829;
    g_band[2].last[0]  = 4969;
    g_band[2].step[0]  = 1;
    /* n3 */
    g_band[3].F_UL_low  = 1710;
    g_band[3].F_UL_high = 1785;
    g_band[3].F_DL_low  = 1805;
    g_band[3].F_DL_high = 1880;
    g_band[3].SCS[0] = 15;
    g_band[3].first[0] = 4517;
    g_band[3].last[0]  = 4693;
    g_band[3].step[0]  = 1;
    /* n5 */
    g_band[5].F_UL_low  = 824;
    g_band[5].F_UL_high = 849;
    g_band[5].F_DL_low  = 869;
    g_band[5].F_DL_high = 894;
    g_band[5].SCS[0] = 15;
    g_band[5].first[0] = 2177;
    g_band[5].last[0]  = 2230;
    g_band[5].step[0]  = 1;
    g_band[5].SCS[1] = 30;
    g_band[5].first[1] = 2183;
    g_band[5].last[1]  = 2224;
    g_band[5].step[1]  = 1;
    /* n7 */
    g_band[7].F_UL_low  = 2500;
    g_band[7].F_UL_high = 2570;
    g_band[7].F_DL_low  = 2620;
    g_band[7].F_DL_high = 2690;
    g_band[7].SCS[0] = 15;
    g_band[7].first[0] = 6554;
    g_band[7].last[0]  = 6718;
    g_band[7].step[0]  = 1;
    /* n8 */
    g_band[8].F_UL_low  = 880;
    g_band[8].F_UL_high = 915;
    g_band[8].F_DL_low  = 925;
    g_band[8].F_DL_high = 960;
    g_band[8].SCS[0] = 15;
    g_band[8].first[0] = 2318;
    g_band[8].last[0]  = 2395;
    g_band[8].step[0]  = 1;
    /* n12 */
    g_band[12].F_UL_low  = 699;
    g_band[12].F_UL_high = 716;
    g_band[12].F_DL_low  = 729;
    g_band[12].F_DL_high = 746;
    g_band[12].SCS[0] = 15;
    g_band[12].first[0] = 1828;
    g_band[12].last[0]  = 1858;
    g_band[12].step[0]  = 1;
    /* n20 */
    g_band[20].F_UL_low  = 832;
    g_band[20].F_UL_high = 862;
    g_band[20].F_DL_low  = 791;
    g_band[20].F_DL_high = 821;
    g_band[20].SCS[0] = 15;
    g_band[20].first[0] = 1982;
    g_band[20].last[0]  = 2047;
    g_band[20].step[0]  = 1;
    /* n25 */
    g_band[25].F_UL_low  = 1850;
    g_band[25].F_UL_high = 1915;
    g_band[25].F_DL_low  = 1930;
    g_band[25].F_DL_high = 1995;
    g_band[25].SCS[0] = 15;
    g_band[25].first[0] = 4829;
    g_band[25].last[0]  = 4981;
    g_band[25].step[0]  = 1;
    /* n28 */
    g_band[28].F_UL_low  = 703;
    g_band[28].F_UL_high = 748;
    g_band[28].F_DL_low  = 758;
    g_band[28].F_DL_high = 803;
    g_band[28].SCS[0] = 15;
    g_band[28].first[0] = 1901;
    g_band[28].last[0]  = 2002;
    g_band[28].step[0]  = 1;
    /* n34 */
    g_band[34].F_UL_low  = 2010;
    g_band[34].F_UL_high = 2025;
    g_band[34].F_DL_low  = 2010;
    g_band[34].F_DL_high = 2025;
    g_band[34].SCS[0] = 15;
    g_band[34].first[0] = 5030;
    g_band[34].last[0]  = 5060;
    g_band[34].step[0]  = 1;
    /* n38 */
    g_band[38].F_UL_low  = 2570;
    g_band[38].F_UL_high = 2620;
    g_band[38].F_DL_low  = 2570;
    g_band[38].F_DL_high = 2620;
    g_band[38].SCS[0] = 15;
    g_band[38].first[0] = 6431;
    g_band[38].last[0]  = 6544;
    g_band[38].step[0]  = 1;
    /* n39 */
    g_band[39].F_UL_low  = 1880;
    g_band[39].F_UL_high = 1920;
    g_band[39].F_DL_low  = 1880;
    g_band[39].F_DL_high = 1920;
    g_band[39].SCS[0] = 15;
    g_band[39].first[0] = 4706;
    g_band[39].last[0]  = 4795;
    g_band[39].step[0]  = 1;
    /* n40 */
    g_band[40].F_UL_low  = 2300;
    g_band[40].F_UL_high = 2400;
    g_band[40].F_DL_low  = 2300;
    g_band[40].F_DL_high = 2400;
    g_band[40].SCS[0] = 15;
    g_band[40].first[0] = 5756;
    g_band[40].last[0]  = 5995;
    g_band[40].step[0]  = 1;
    /* n41 */
    g_band[41].F_UL_low  = 2496;
    g_band[41].F_UL_high = 2690;
    g_band[41].F_DL_low  = 2496;
    g_band[41].F_DL_high = 2690;
    g_band[41].SCS[0] = 15;
    g_band[41].first[0] = 6246;
    g_band[41].last[0]  = 6717;
    g_band[41].step[0]  = 3;
    g_band[41].SCS[1] = 30;
    g_band[41].first[1] = 6252;
    g_band[41].last[1]  = 6714;
    g_band[41].step[1]  = 3;
    /* n50 */
    g_band[50].F_UL_low  = 1432;
    g_band[50].F_UL_high = 1517;
    g_band[50].F_DL_low  = 1432;
    g_band[50].F_DL_high = 1517;
    g_band[50].SCS[0] = 15;
    g_band[50].first[0] = 3584;
    g_band[50].last[0]  = 3787;
    g_band[50].step[0]  = 1;
    /* n51 */
    g_band[51].F_UL_low  = 1427;
    g_band[51].F_UL_high = 1432;
    g_band[51].F_DL_low  = 1427;
    g_band[51].F_DL_high = 1432;
    g_band[51].SCS[0] = 15;
    g_band[51].first[0] = 3572;
    g_band[51].last[0]  = 3574;
    g_band[51].step[0]  = 1;
    /* n65 */
    g_band[65].F_UL_low  = 1920;
    g_band[65].F_UL_high = 2010;
    g_band[65].F_DL_low  = 2110;
    g_band[65].F_DL_high = 2200;
    g_band[65].SCS[0] = 15;
    g_band[65].first[0] = 5279;
    g_band[65].last[0]  = 5494;
    g_band[65].step[0]  = 1;
    /* n66 */
    g_band[66].F_UL_low  = 1710;
    g_band[66].F_UL_high = 1780;
    g_band[66].F_DL_low  = 2110;
    g_band[66].F_DL_high = 2200;
    g_band[66].SCS[0] = 15;
    g_band[66].first[0] = 5279;
    g_band[66].last[0]  = 5494;
    g_band[66].step[0]  = 1;
    g_band[66].SCS[1] = 30;
    g_band[66].first[1] = 5285;
    g_band[66].last[1]  = 5488;
    g_band[66].step[1]  = 1;
    /* n70 */
    g_band[70].F_UL_low  = 1695;
    g_band[70].F_UL_high = 1710;
    g_band[70].F_DL_low  = 1995;
    g_band[70].F_DL_high = 2020;
    g_band[70].SCS[0] = 15;
    g_band[70].first[0] = 4993;
    g_band[70].last[0]  = 5044;
    g_band[70].step[0]  = 1;
    /* n71 */
    g_band[71].F_UL_low  = 663;
    g_band[71].F_UL_high = 698;
    g_band[71].F_DL_low  = 617;
    g_band[71].F_DL_high = 652;
    g_band[71].SCS[0] = 15;
    g_band[71].first[0] = 1547;
    g_band[71].last[0]  = 1624;
    g_band[71].step[0]  = 1;
    /* n74 */
    g_band[74].F_UL_low  = 1427;
    g_band[74].F_UL_high = 1470;
    g_band[74].F_DL_low  = 1475;
    g_band[74].F_DL_high = 1518;
    g_band[74].SCS[0] = 15;
    g_band[74].first[0] = 3692;
    g_band[74].last[0]  = 3790;
    g_band[74].step[0]  = 1;
    /* n75 */
    g_band[75].F_DL_low  = 1432;
    g_band[75].F_DL_high = 1517;
    g_band[75].SCS[0] = 15;
    g_band[75].first[0] = 3584;
    g_band[75].last[0]  = 3787;
    g_band[75].step[0]  = 1;
    /* n76 */
    g_band[76].F_DL_low  = 1427;
    g_band[76].F_DL_high = 1432;
    g_band[76].SCS[0] = 15;
    g_band[76].first[0] = 3572;
    g_band[76].last[0]  = 3574;
    g_band[76].step[0]  = 1;
    /* n77 */
    g_band[77].F_UL_low  = 3300;
    g_band[77].F_UL_high = 4200;
    g_band[77].F_DL_low  = 3300;
    g_band[77].F_DL_high = 4200;
    g_band[77].SCS[0] = 30;
    g_band[77].first[0] = 7711;
    g_band[77].last[0]  = 8329;
    g_band[77].step[0]  = 1;
    /* n78 */
    g_band[78].F_UL_low  = 3300;
    g_band[78].F_UL_high = 3800;
    g_band[78].F_DL_low  = 3300;
    g_band[78].F_DL_high = 3800;
    g_band[78].SCS[0] = 30;
    g_band[78].first[0] = 7711;
    g_band[78].last[0]  = 8051;
    g_band[78].step[0]  = 1;
    /* n79 */
    g_band[79].F_UL_low  = 4400;
    g_band[79].F_UL_high = 5000;
    g_band[79].F_DL_low  = 4400;
    g_band[79].F_DL_high = 5000;
    g_band[79].SCS[0] = 30;
    g_band[79].first[0] = 8480;
    g_band[79].last[0]  = 8880;
    g_band[79].step[0]  = 16;
    /* n80 */
    g_band[80].F_UL_low  = 1710;
    g_band[80].F_UL_high = 1785;
    /* n81 */
    g_band[81].F_UL_low  = 880;
    g_band[81].F_UL_high = 915;
    /* n82 */
    g_band[82].F_UL_low  = 832;
    g_band[82].F_UL_high = 862;
    /* n83 */
    g_band[83].F_UL_low  = 703;
    g_band[83].F_UL_high = 748;
    /* n84 */
    g_band[84].F_UL_low  = 1920;
    g_band[84].F_UL_high = 1980;
    /* n86 */
    g_band[86].F_UL_low  = 1710;
    g_band[86].F_UL_high = 1780;
    /* n257 */
    g_band[257].F_UL_low  = 26500;
    g_band[257].F_UL_high = 29500;
    g_band[257].F_DL_low  = 26500;
    g_band[257].F_DL_high = 29500;
    g_band[257].SCS[0] = 120;
    g_band[257].SCS[1] = 240;
    g_band[257].first[0] = 22388;
    g_band[257].first[1] = 22390;
    g_band[257].last[0]  = 22558;
    g_band[257].last[1]  = 22556;
    g_band[257].step[0]  = 1;
    g_band[257].step[1]  = 2;
    /* n258 */
    g_band[258].F_UL_low  = 24250;
    g_band[258].F_UL_high = 27500;
    g_band[258].F_DL_low  = 24250;
    g_band[258].F_DL_high = 27500;
    g_band[258].SCS[0] = 120;
    g_band[258].SCS[1] = 240;
    g_band[258].first[0] = 22257;
    g_band[258].first[1] = 22258;
    g_band[258].last[0]  = 22443;
    g_band[258].last[1]  = 22442;
    g_band[258].step[0]  = 1;
    g_band[258].step[1]  = 2;
    /* n260 */
    g_band[260].F_UL_low  = 37000;
    g_band[260].F_UL_high = 40000;
    g_band[260].F_DL_low  = 37000;
    g_band[260].F_DL_high = 40000;
    g_band[260].SCS[0] = 120;
    g_band[260].SCS[1] = 240;
    g_band[260].first[0] = 22995;
    g_band[260].first[1] = 22996;
    g_band[260].last[0]  = 23166;
    g_band[260].last[1]  = 23164;
    g_band[260].step[0]  = 1;
    g_band[260].step[1]  = 2;
    /* n261 */
    g_band[261].F_UL_low  = 27500;
    g_band[261].F_UL_high = 28350;
    g_band[261].F_DL_low  = 27500;
    g_band[261].F_DL_high = 28350;
    g_band[261].SCS[0] = 120;
    g_band[261].SCS[1] = 240;
    g_band[261].first[0] = 22446;
    g_band[261].first[1] = 22446;
    g_band[261].last[0]  = 22492;
    g_band[261].last[1]  = 22490;
    g_band[261].step[0]  = 1;
    g_band[261].step[1]  = 2;
   


    n = band;
    switch ( band )
    {
        case 1:
        case 2:
        case 3:
        case 5:
        case 7:
        case 8:
        case 12:
        case 20:
        case 25:
        case 28:
        case 34:
        case 38:
        case 39:
        case 40:
        case 41:
        case 50:
        case 51:
        case 65:
        case 66:
        case 70:
        case 71:
        case 74:
        case 75:
        case 76:
            /* 0 ~ 3000 MHz */
            for (M=1; M<=5; M+=2)
            {
                printf("------+--------+--------------+-----------+----------\n");
                printf("    N |   GSCN | SS_REF (KHz) | SCS (KHz) | M = %d\n", M);
                printf("------+--------+--------------+-----------+----------\n");
                SS_raster_1(
                    g_band[n].SCS[0],
                    g_band[n].first[0],
                    g_band[n].last[0],
                    g_band[n].step[0],
                    M
                );
                if (g_band[n].SCS[1] > 0)
                {
                    SS_raster_1(
                        g_band[n].SCS[1],
                        g_band[n].first[1],
                        g_band[n].last[1],
                        g_band[n].step[1],
                        M
                    );
                }
                printf("\n");
            }
            printf(
                "Band [1;35m%d[0m (%d ~ %d, %d MHz)\n",
                n,
                g_band[n].F_DL_low,
                g_band[n].F_DL_high, 
                (g_band[n].F_DL_high - g_band[n].F_DL_low)
            );
            break;

        case 77:
        case 78:
        case 79:
            /* 3000 ~ 24250 MHz */
            printf("------+--------+--------------+----------------------\n");
            printf("    N |   GSCN | SS_REF (MHz) | SCS (KHz)\n");
            printf("------+--------+--------------+----------------------\n");
            SS_raster_2(
                g_band[n].SCS[0],
                g_band[n].first[0],
                g_band[n].last[0],
                g_band[n].step[0]
            );
            printf("\n");
            printf(
                "Band [1;35m%d[0m (%d ~ %d, %d MHz)\n",
                n,
                g_band[n].F_DL_low,
                g_band[n].F_DL_high, 
                (g_band[n].F_DL_high - g_band[n].F_DL_low)
            );
            break;

        case 257:
        case 258:
        case 260:
        case 261:
            /* 24250 ~ 100000 MHz */
            printf("------+--------+--------------+----------------------\n");
            printf("    N |   GSCN | SS_REF (MHz) | SCS (KHz)\n");
            printf("------+--------+--------------+----------------------\n");
            SS_raster_3(
                g_band[n].SCS[0],
                g_band[n].first[0],
                g_band[n].last[0],
                g_band[n].step[0]
            );
            SS_raster_3(
                g_band[n].SCS[1],
                g_band[n].first[1],
                g_band[n].last[1],
                g_band[n].step[1]
            );
            printf("\n");
            printf(
                "Band [1;35m%d[0m (%d ~ %d, %d MHz)\n",
                n,
                g_band[n].F_DL_low,
                g_band[n].F_DL_high, 
                (g_band[n].F_DL_high - g_band[n].F_DL_low)
            );
            break;

        default:
            printf("Incorrect band %d\n\n", band);
    }


    return 0;
}

