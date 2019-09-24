#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DIV_FLOOR(X, Y)  ((X) / (Y))


typedef struct _tBand
{
    /* Operating frequency in MHz */
    int  F_UL_low;
    int  F_UL_high;
    int  F_DL_low;
    int  F_DL_high;

    /* Subcarrier spacing in KHz */
    int  F_Raster[2];
    int  SSB_SCS[2];

    /* GSCN range */
    int  SSB_first[2];
    int  SSB_last[2];
    int  SSB_step[2];
} tBand;

tBand g_band[280];

void band_init(void)
{
    memset(&(g_band[0]), 0, (sizeof( tBand ) * 100));
    /* n1 */
    g_band[1].F_UL_low  = 1920;
    g_band[1].F_UL_high = 1980;
    g_band[1].F_DL_low  = 2110;
    g_band[1].F_DL_high = 2170;
    g_band[1].F_Raster[0] = 100;
    g_band[1].F_Raster[1] = 0;
    g_band[1].SSB_SCS[0] = 15;
    g_band[1].SSB_SCS[1] = 0;
    g_band[1].SSB_first[0] = 5279;
    g_band[1].SSB_last[0]  = 5419;
    g_band[1].SSB_step[0]  = 1;
    /* n2 */
    g_band[2].F_UL_low  = 1850;
    g_band[2].F_UL_high = 1910;
    g_band[2].F_DL_low  = 1930;
    g_band[2].F_DL_high = 1990;
    g_band[2].F_Raster[0] = 100;
    g_band[2].F_Raster[1] = 0;
    g_band[2].SSB_SCS[0] = 15;
    g_band[2].SSB_SCS[1] = 0;
    g_band[2].SSB_first[0] = 4829;
    g_band[2].SSB_last[0]  = 4969;
    g_band[2].SSB_step[0]  = 1;
    /* n3 */
    g_band[3].F_UL_low  = 1710;
    g_band[3].F_UL_high = 1785;
    g_band[3].F_DL_low  = 1805;
    g_band[3].F_DL_high = 1880;
    g_band[3].F_Raster[0] = 100;
    g_band[3].F_Raster[1] = 0;
    g_band[3].SSB_SCS[0] = 15;
    g_band[3].SSB_SCS[1] = 0;
    g_band[3].SSB_first[0] = 4517;
    g_band[3].SSB_last[0]  = 4693;
    g_band[3].SSB_step[0]  = 1;
    /* n5 */
    g_band[5].F_UL_low  = 824;
    g_band[5].F_UL_high = 849;
    g_band[5].F_DL_low  = 869;
    g_band[5].F_DL_high = 894;
    g_band[5].F_Raster[0] = 100;
    g_band[5].F_Raster[1] = 0;
    g_band[5].SSB_SCS[0] = 15;
    g_band[5].SSB_SCS[1] = 30;
    g_band[5].SSB_first[0] = 2177;
    g_band[5].SSB_last[0]  = 2230;
    g_band[5].SSB_step[0]  = 1;
    g_band[5].SSB_first[1] = 2183;
    g_band[5].SSB_last[1]  = 2224;
    g_band[5].SSB_step[1]  = 1;
    /* n7 */
    g_band[7].F_UL_low  = 2500;
    g_band[7].F_UL_high = 2570;
    g_band[7].F_DL_low  = 2620;
    g_band[7].F_DL_high = 2690;
    g_band[7].F_Raster[0] = 100;
    g_band[7].F_Raster[1] = 0;
    g_band[7].SSB_SCS[0] = 15;
    g_band[7].SSB_SCS[1] = 0;
    g_band[7].SSB_first[0] = 6554;
    g_band[7].SSB_last[0]  = 6718;
    g_band[7].SSB_step[0]  = 1;
    /* n8 */
    g_band[8].F_UL_low  = 880;
    g_band[8].F_UL_high = 915;
    g_band[8].F_DL_low  = 925;
    g_band[8].F_DL_high = 960;
    g_band[8].F_Raster[0] = 100;
    g_band[8].F_Raster[1] = 0;
    g_band[8].SSB_SCS[0] = 15;
    g_band[8].SSB_SCS[1] = 0;
    g_band[8].SSB_first[0] = 2318;
    g_band[8].SSB_last[0]  = 2395;
    g_band[8].SSB_step[0]  = 1;
    /* n12 */
    g_band[12].F_UL_low  = 699;
    g_band[12].F_UL_high = 716;
    g_band[12].F_DL_low  = 729;
    g_band[12].F_DL_high = 746;
    g_band[12].F_Raster[0] = 100;
    g_band[12].F_Raster[1] = 0;
    g_band[12].SSB_SCS[0] = 15;
    g_band[12].SSB_SCS[1] = 0;
    g_band[12].SSB_first[0] = 1828;
    g_band[12].SSB_last[0]  = 1858;
    g_band[12].SSB_step[0]  = 1;
    /* n20 */
    g_band[20].F_UL_low  = 832;
    g_band[20].F_UL_high = 862;
    g_band[20].F_DL_low  = 791;
    g_band[20].F_DL_high = 821;
    g_band[20].F_Raster[0] = 100;
    g_band[20].F_Raster[1] = 0;
    g_band[20].SSB_SCS[0] = 15;
    g_band[20].SSB_SCS[1] = 0;
    g_band[20].SSB_first[0] = 1982;
    g_band[20].SSB_last[0]  = 2047;
    g_band[20].SSB_step[0]  = 1;
    /* n25 */
    g_band[25].F_UL_low  = 1850;
    g_band[25].F_UL_high = 1915;
    g_band[25].F_DL_low  = 1930;
    g_band[25].F_DL_high = 1995;
    g_band[25].F_Raster[0] = 100;
    g_band[25].F_Raster[1] = 0;
    g_band[25].SSB_SCS[0] = 15;
    g_band[25].SSB_SCS[1] = 0;
    g_band[25].SSB_first[0] = 4829;
    g_band[25].SSB_last[0]  = 4981;
    g_band[25].SSB_step[0]  = 1;
    /* n28 */
    g_band[28].F_UL_low  = 703;
    g_band[28].F_UL_high = 748;
    g_band[28].F_DL_low  = 758;
    g_band[28].F_DL_high = 803;
    g_band[28].F_Raster[0] = 100;
    g_band[28].F_Raster[1] = 0;
    g_band[28].SSB_SCS[0] = 15;
    g_band[28].SSB_SCS[1] = 0;
    g_band[28].SSB_first[0] = 1901;
    g_band[28].SSB_last[0]  = 2002;
    g_band[28].SSB_step[0]  = 1;
    /* n34 */
    g_band[34].F_UL_low  = 2010;
    g_band[34].F_UL_high = 2025;
    g_band[34].F_DL_low  = 2010;
    g_band[34].F_DL_high = 2025;
    g_band[34].F_Raster[0] = 100;
    g_band[34].F_Raster[1] = 0;
    g_band[34].SSB_SCS[0] = 15;
    g_band[34].SSB_SCS[1] = 0;
    g_band[34].SSB_first[0] = 5030;
    g_band[34].SSB_last[0]  = 5060;
    g_band[34].SSB_step[0]  = 1;
    /* n38 */
    g_band[38].F_UL_low  = 2570;
    g_band[38].F_UL_high = 2620;
    g_band[38].F_DL_low  = 2570;
    g_band[38].F_DL_high = 2620;
    g_band[38].F_Raster[0] = 100;
    g_band[38].F_Raster[1] = 0;
    g_band[38].SSB_SCS[0] = 15;
    g_band[38].SSB_SCS[1] = 0;
    g_band[38].SSB_first[0] = 6431;
    g_band[38].SSB_last[0]  = 6544;
    g_band[38].SSB_step[0]  = 1;
    /* n39 */
    g_band[39].F_UL_low  = 1880;
    g_band[39].F_UL_high = 1920;
    g_band[39].F_DL_low  = 1880;
    g_band[39].F_DL_high = 1920;
    g_band[39].F_Raster[0] = 100;
    g_band[39].F_Raster[1] = 0;
    g_band[39].SSB_SCS[0] = 15;
    g_band[39].SSB_SCS[1] = 0;
    g_band[39].SSB_first[0] = 4706;
    g_band[39].SSB_last[0]  = 4795;
    g_band[39].SSB_step[0]  = 1;
    /* n40 */
    g_band[40].F_UL_low  = 2300;
    g_band[40].F_UL_high = 2400;
    g_band[40].F_DL_low  = 2300;
    g_band[40].F_DL_high = 2400;
    g_band[40].F_Raster[0] = 100;
    g_band[40].F_Raster[1] = 0;
    g_band[40].SSB_SCS[0] = 15;
    g_band[40].SSB_SCS[1] = 0;
    g_band[40].SSB_first[0] = 5756;
    g_band[40].SSB_last[0]  = 5995;
    g_band[40].SSB_step[0]  = 1;
    /* n41 */
    g_band[41].F_UL_low  = 2496;
    g_band[41].F_UL_high = 2690;
    g_band[41].F_DL_low  = 2496;
    g_band[41].F_DL_high = 2690;
    g_band[41].F_Raster[0] = 15;
    g_band[41].F_Raster[1] = 30;
    g_band[41].SSB_SCS[0] = 15;
    g_band[41].SSB_first[0] = 6246;
    g_band[41].SSB_last[0]  = 6717;
    g_band[41].SSB_step[0]  = 3;
    g_band[41].SSB_SCS[1] = 30;
    g_band[41].SSB_first[1] = 6252;
    g_band[41].SSB_last[1]  = 6714;
    g_band[41].SSB_step[1]  = 3;
    /* n50 */
    g_band[50].F_UL_low  = 1432;
    g_band[50].F_UL_high = 1517;
    g_band[50].F_DL_low  = 1432;
    g_band[50].F_DL_high = 1517;
    g_band[50].F_Raster[0] = 100;
    g_band[50].F_Raster[1] = 0;
    g_band[50].SSB_SCS[0] = 15;
    g_band[50].SSB_SCS[1] = 0;
    g_band[50].SSB_first[0] = 3584;
    g_band[50].SSB_last[0]  = 3787;
    g_band[50].SSB_step[0]  = 1;
    /* n51 */
    g_band[51].F_UL_low  = 1427;
    g_band[51].F_UL_high = 1432;
    g_band[51].F_DL_low  = 1427;
    g_band[51].F_DL_high = 1432;
    g_band[51].F_Raster[0] = 100;
    g_band[51].F_Raster[1] = 0;
    g_band[51].SSB_SCS[0] = 15;
    g_band[51].SSB_SCS[1] = 0;
    g_band[51].SSB_first[0] = 3572;
    g_band[51].SSB_last[0]  = 3574;
    g_band[51].SSB_step[0]  = 1;
    /* n65 */
    g_band[65].F_UL_low  = 1920;
    g_band[65].F_UL_high = 2010;
    g_band[65].F_DL_low  = 2110;
    g_band[65].F_DL_high = 2200;
    g_band[65].F_Raster[0] = 100;
    g_band[65].F_Raster[1] = 0;
    g_band[65].SSB_SCS[0] = 15;
    g_band[65].SSB_SCS[1] = 0;
    g_band[65].SSB_first[0] = 5279;
    g_band[65].SSB_last[0]  = 5494;
    g_band[65].SSB_step[0]  = 1;
    /* n66 */
    g_band[66].F_UL_low  = 1710;
    g_band[66].F_UL_high = 1780;
    g_band[66].F_DL_low  = 2110;
    g_band[66].F_DL_high = 2200;
    g_band[66].F_Raster[0] = 100;
    g_band[66].F_Raster[1] = 0;
    g_band[66].SSB_SCS[0] = 15;
    g_band[66].SSB_first[0] = 5279;
    g_band[66].SSB_last[0]  = 5494;
    g_band[66].SSB_step[0]  = 1;
    g_band[66].SSB_SCS[1] = 30;
    g_band[66].SSB_first[1] = 5285;
    g_band[66].SSB_last[1]  = 5488;
    g_band[66].SSB_step[1]  = 1;
    /* n70 */
    g_band[70].F_UL_low  = 1695;
    g_band[70].F_UL_high = 1710;
    g_band[70].F_DL_low  = 1995;
    g_band[70].F_DL_high = 2020;
    g_band[70].F_Raster[0] = 100;
    g_band[70].F_Raster[1] = 0;
    g_band[70].SSB_SCS[0] = 15;
    g_band[70].SSB_SCS[1] = 0;
    g_band[70].SSB_first[0] = 4993;
    g_band[70].SSB_last[0]  = 5044;
    g_band[70].SSB_step[0]  = 1;
    /* n71 */
    g_band[71].F_UL_low  = 663;
    g_band[71].F_UL_high = 698;
    g_band[71].F_DL_low  = 617;
    g_band[71].F_DL_high = 652;
    g_band[71].F_Raster[0] = 100;
    g_band[71].F_Raster[1] = 0;
    g_band[71].SSB_SCS[0] = 15;
    g_band[71].SSB_SCS[1] = 0;
    g_band[71].SSB_first[0] = 1547;
    g_band[71].SSB_last[0]  = 1624;
    g_band[71].SSB_step[0]  = 1;
    /* n74 */
    g_band[74].F_UL_low  = 1427;
    g_band[74].F_UL_high = 1470;
    g_band[74].F_DL_low  = 1475;
    g_band[74].F_DL_high = 1518;
    g_band[74].F_Raster[0] = 100;
    g_band[74].F_Raster[1] = 0;
    g_band[74].SSB_SCS[0] = 15;
    g_band[74].SSB_SCS[1] = 0;
    g_band[74].SSB_first[0] = 3692;
    g_band[74].SSB_last[0]  = 3790;
    g_band[74].SSB_step[0]  = 1;
    /* n75 */
    g_band[75].F_DL_low  = 1432;
    g_band[75].F_DL_high = 1517;
    g_band[75].F_Raster[0] = 100;
    g_band[75].F_Raster[1] = 0;
    g_band[75].SSB_SCS[0] = 15;
    g_band[75].SSB_SCS[1] = 0;
    g_band[75].SSB_first[0] = 3584;
    g_band[75].SSB_last[0]  = 3787;
    g_band[75].SSB_step[0]  = 1;
    /* n76 */
    g_band[76].F_DL_low  = 1427;
    g_band[76].F_DL_high = 1432;
    g_band[76].F_Raster[0] = 100;
    g_band[76].F_Raster[1] = 0;
    g_band[76].SSB_SCS[0] = 15;
    g_band[76].SSB_SCS[1] = 0;
    g_band[76].SSB_first[0] = 3572;
    g_band[76].SSB_last[0]  = 3574;
    g_band[76].SSB_step[0]  = 1;
    /* n77 */
    g_band[77].F_UL_low  = 3300;
    g_band[77].F_UL_high = 4200;
    g_band[77].F_DL_low  = 3300;
    g_band[77].F_DL_high = 4200;
    g_band[77].F_Raster[0] = 15;
    g_band[77].F_Raster[1] = 30;
    g_band[77].SSB_SCS[0] = 30;
    g_band[77].SSB_SCS[1] = 0;
    g_band[77].SSB_first[0] = 7711;
    g_band[77].SSB_last[0]  = 8329;
    g_band[77].SSB_step[0]  = 1;
    /* n78 */
    g_band[78].F_UL_low  = 3300;
    g_band[78].F_UL_high = 3800;
    g_band[78].F_DL_low  = 3300;
    g_band[78].F_DL_high = 3800;
    g_band[78].F_Raster[0] = 15;
    g_band[78].F_Raster[1] = 30;
    g_band[78].SSB_SCS[0] = 30;
    g_band[78].SSB_SCS[1] = 0;
    g_band[78].SSB_first[0] = 7711;
    g_band[78].SSB_last[0]  = 8051;
    g_band[78].SSB_step[0]  = 1;
    /* n79 */
    g_band[79].F_UL_low  = 4400;
    g_band[79].F_UL_high = 5000;
    g_band[79].F_DL_low  = 4400;
    g_band[79].F_DL_high = 5000;
    g_band[79].F_Raster[0] = 15;
    g_band[79].F_Raster[1] = 30;
    g_band[79].SSB_SCS[0] = 30;
    g_band[79].SSB_SCS[1] = 0;
    g_band[79].SSB_first[0] = 8480;
    g_band[79].SSB_last[0]  = 8880;
    g_band[79].SSB_step[0]  = 16;
    /* n80 */
    g_band[80].F_UL_low  = 1710;
    g_band[80].F_UL_high = 1785;
    g_band[80].F_Raster[0] = 100;
    g_band[80].F_Raster[1] = 0;
    /* n81 */
    g_band[81].F_UL_low  = 880;
    g_band[81].F_UL_high = 915;
    g_band[81].F_Raster[0] = 100;
    g_band[81].F_Raster[1] = 0;
    /* n82 */
    g_band[82].F_UL_low  = 832;
    g_band[82].F_UL_high = 862;
    g_band[82].F_Raster[0] = 100;
    g_band[82].F_Raster[1] = 0;
    /* n83 */
    g_band[83].F_UL_low  = 703;
    g_band[83].F_UL_high = 748;
    g_band[83].F_Raster[0] = 100;
    g_band[83].F_Raster[1] = 0;
    /* n84 */
    g_band[84].F_UL_low  = 1920;
    g_band[84].F_UL_high = 1980;
    g_band[84].F_Raster[0] = 100;
    g_band[84].F_Raster[1] = 0;
    /* n86 */
    g_band[86].F_UL_low  = 1710;
    g_band[86].F_UL_high = 1780;
    g_band[86].F_Raster[0] = 100;
    g_band[86].F_Raster[1] = 0;
}

int bw2rb(int band, int bw, int scs)
{
    int rb = 0;

    if (15 == scs)
    {
        if (5 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 12) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 34) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 50) ||
                (band == 51) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 76))
            {
                rb = 25;
            }
        }
        else if (10 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 12) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 52;
            }
        }
        else if (15 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 12) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 79;
            }
        }
        else if (20 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 106;
            }
        }
        else if (25 == bw)
        {
            if ((band ==  3) ||
                (band == 39) ||
                (band == 40) ||
                (band == 70))
            {
                rb = 133;
            }
        }
        else if (30 == bw)
        {
            if ((band ==  3) ||
                (band == 39) ||
                (band == 40) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 160;
            }
        }
        else if (40 == bw)
        {
            if ((band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 66) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 216;
            }
        }
        else if (50 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 270;
            }
        }
    }
    if (30 == scs)
    {
        if (5 == bw)
        {
            //rb = 11;
            rb = 0;
        }
        else if (10 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 12) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 24;
            }
        }
        else if (15 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 12) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 38;
            }
        }
        else if (20 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  5) ||
                (band ==  7) ||
                (band ==  8) ||
                (band == 20) ||
                (band == 25) ||
                (band == 28) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 71) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 51;
            }
        }
        else if (25 == bw)
        {
            if ((band ==  3) ||
                (band == 39) ||
                (band == 40) ||
                (band == 70))
            {
                rb = 65;
            }
        }
        else if (30 == bw)
        {
            if ((band ==  3) ||
                (band == 39) ||
                (band == 40) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 78;
            }
        }
        else if (40 == bw)
        {
            if ((band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 66) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 106;
            }
        }
        else if (50 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 133;
            }
        }
        else if (60 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 162;
            }
        }
        else if (70 == bw)
        {
            if ((band == 41) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 189;
            }
        }
        else if (80 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 217;
            }
        }
        else if (90 == bw)
        {
            if ((band == 41) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 245;
            }
        }
        else if (100 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 273;
            }
        }
    }
    if (60 == scs)
    {
        if (10 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  7) ||
                (band == 25) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 11;
            }
        }
        else if (15 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  7) ||
                (band == 25) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 18;
            }
        }
        else if (20 == bw)
        {
            if ((band ==  1) ||
                (band ==  2) ||
                (band ==  3) ||
                (band ==  7) ||
                (band == 25) ||
                (band == 38) ||
                (band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 65) ||
                (band == 66) ||
                (band == 70) ||
                (band == 74) ||
                (band == 75) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 24;
            }
        }
        else if (25 == bw)
        {
            if ((band ==  3) ||
                (band == 39) ||
                (band == 40) ||
                (band == 70))
            {
                rb = 31;
            }
        }
        else if (30 == bw)
        {
            if ((band ==  3) ||
                (band == 39) ||
                (band == 40) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 38;
            }
        }
        else if (40 == bw)
        {
            if ((band == 39) ||
                (band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 66) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 51;
            }
        }
        else if (50 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 65;
            }
        }
        else if (60 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 79;
            }
        }
        else if (70 == bw)
        {
            if ((band == 41) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 93;
            }
        }
        else if (80 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 50) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 107;
            }
        }
        else if (90 == bw)
        {
            if ((band == 41) ||
                (band == 77) ||
                (band == 78))
            {
                rb = 121;
            }
        }
        else if (100 == bw)
        {
            if ((band == 40) ||
                (band == 41) ||
                (band == 77) ||
                (band == 78) ||
                (band == 79))
            {
                rb = 135;
            }
        }
    }

    return rb;
}

void show_frequency(
    double SS_REF,    /* KHz */
    double ss_low,    /* KHz */
    double ss_high,   /* KHz */
    int    ss_raster, /* KHz */
    int    F_DL_low,  /* MHz */
    int    F_DL_high, /* MHz */
    int    F_Raster[2],
    int    BW,
    int    N_RB,
    int    SCS
)
{
    char raster[20];
    double F_REF;
    double f_low;
    double f_high;
    double rb_low;
    double rb_high;
    int N_SSB_CRB;
    int k_SSB;
    int count;
    int i;


    printf("======================================================================\n");
    printf("             Frequency (KHz)         | N_SSB_CRB | k_SSB | Raster \n");
    printf("======================================================================\n");
    printf(
        "[1;36mSSB[0m %.2f [1;33m%.2f[0m %.2f |           |       | %d KHz\n",
        ss_low,
        SS_REF,
        ss_high,
        ss_raster
    );
    printf("-------------------------------------+-----------+-------+------------\n");
    for (i=0; i<2; i++)
    {
        if (0 == F_Raster[i]) continue;

        count = 0;
        sprintf(raster, "%d KHz", F_Raster[i]);

        for (F_REF  = (F_DL_low * 1000);
             F_REF <= (F_DL_high * 1000);
             F_REF += F_Raster[i])
        {
            f_low  = (F_REF - ((BW * 1000) / 2));
            f_high = (F_REF + ((BW * 1000) / 2));
            if (1 == (N_RB % 2))
            {
                rb_low  = (F_REF - (((DIV_FLOOR(N_RB, 2) * 12) + 6) * SCS));
                rb_high = (F_REF + (((DIV_FLOOR(N_RB, 2) * 12) + 6) * SCS));
            }
            else
            {
                rb_low  = (F_REF - ((DIV_FLOOR(N_RB, 2) * 12) * SCS));
                rb_high = (F_REF + ((DIV_FLOOR(N_RB, 2) * 12) * SCS));
            }
            if ((rb_low <= ss_low) && (ss_high <= rb_high))
            {
                #if 0
                printf(
                    "    %.2f %.2f %.2f\n",
                    rb_low,
                    F_REF,
                    rb_high
                );
                continue;
                #endif
                if (0 == ((int)(ss_low - rb_low) % 15))
                {
                    N_SSB_CRB = (((int)(ss_low - rb_low) / 15) / 12);
                    k_SSB = (((int)(ss_low - rb_low) / 15) % 12);
                    printf(
                        "[1;36m%s[0m %.2f %.2f %.2f |    %4d   |   %2d  | %s\n",
                        ((count == 0) ? "CRB" : "   "),
                        rb_low,
                        F_REF,
                        rb_high,
                        N_SSB_CRB,
                        k_SSB,
                        ((count == 0) ? raster : "")
                    );
                    count++;
                }
            }
        }
        if (count > 0)
        {
            printf("-------------------------------------+-----------+-------+------------\n");
        }
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: band_FR1 [OPTION]...\n");
    printf("\n");
    printf("  -b   NR operating band.\n");
    printf("       (1, 2, 3, 5, 7, 8, 12, 20, 25, 28, 34, 38, 39, 40, 41\n");
    printf("        50, 51, 65, 66, 70, 71, 74, 77, 78, 79)\n");
    printf("  -w   Bandwidth in MHz.\n");
    printf("       (5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100)\n");
    printf("  -s   Subcarrier spacing (15, 30, 60 KHz).\n");
    printf("  -g   GSCN (2 ~ 7498, 7499 ~ 22255).\n");
    printf("  -r   SS Block raster (15, 30 KHz).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.3 Synchronization raster */
int main(int argc, char *argv[])
{
    double SS_REF;
    double ss_low;
    double ss_high;
    int band = 1;
    int GSCN = 5280;
    int BW = 5;
    int SCS = 15;  // subCarrierSpacingCommon
    int ss_raster = 15;
    int g = 0;
    int N_RB;
    int N;
    int M;
    int n;
    int i;
    int ch;


    band_init();

    opterr = 0;
    while ((ch=getopt(argc, argv, "b:w:s:g:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'b':
                band = atoi( optarg );
                break;
            case 'w':
                BW = atoi( optarg );
                break;
            case 's':
                SCS = atoi( optarg );
                break;
            case 'g':
                g = atoi( optarg );
                break;
            case 'r':
                ss_raster = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }

    n = band;
    if ((n != 1) &&
        (n != 2) &&
        (n != 3) &&
        (n != 5) &&
        (n != 7) &&
        (n != 8) &&
        (n != 12) &&
        (n != 20) &&
        (n != 25) &&
        (n != 28) &&
        (n != 34) &&
        (n != 38) &&
        (n != 39) &&
        (n != 40) &&
        (n != 41) &&
        (n != 50) &&
        (n != 51) &&
        (n != 65) &&
        (n != 66) &&
        (n != 70) &&
        (n != 71) &&
        (n != 74) &&
        (n != 77) &&
        (n != 78) &&
        (n != 79))
    {
        printf("ERR: wrong operating band %d\n", n);
        return 0;
    }

    for (i=0; i<2; i++)
    {
        //printf("g_band[%d].SSB_SCS[%d] = %d\n", n, i, g_band[n].SSB_SCS[i]);
        if ((g_band[n].SSB_SCS[i] > 0) &&
            (g_band[n].SSB_SCS[i] == ss_raster))
        {
            break;
        }
    }
    if (i > 1)
    {
        printf("ERR: wrong SS Block raster %d KHz\n", ss_raster);
        return 0;
    }

    GSCN = ((g > 0) ? g : g_band[n].SSB_first[i]);
    if ((GSCN < g_band[n].SSB_first[i]) || (GSCN > g_band[n].SSB_last[i]))
    {
        printf("ERR: wrong GSCN %d in band %d\n", GSCN, band);
        return 0;
    }

    N_RB = bw2rb(band, BW, SCS);
    if (0 == N_RB)
    {
        printf("ERR: wrong bandwidth %d\n", BW);
        return 0;
    }

    printf("[ Band [1;31m%d[0m ]\n", band);
    printf("  Range: %d ~ %d MHz (%d MHz)\n",
        g_band[n].F_DL_low,
        g_band[n].F_DL_high,
        (g_band[n].F_DL_high - g_band[n].F_DL_low)
    );
    printf("  BW   : %d MHz (%d RBs)\n", BW, N_RB);
    printf("  SCS  : %d KHz\n", SCS);
    printf("  GSCN : %d\n", GSCN);
    printf("\n");

    if ((GSCN >= 2) && (GSCN <= 7498))
    {
        if ((GSCN % 3) == 2)
        {
            M = 1;
        }
        else if ((GSCN % 3) == 1)
        {
            M = 5;
        }
        else
        {
            M = 3;
        }
        N = ((GSCN - ((M - 3) / 2)) / 3);
        SS_REF  = ((N * 1200) + (M * 50));  /* KHz */
        ss_low  = (SS_REF - (ss_raster * 120));
        ss_high = (SS_REF + (ss_raster * 120));
        show_frequency(
            SS_REF,
            ss_low,
            ss_high,
            ss_raster,
            g_band[n].F_DL_low,
            g_band[n].F_DL_high,
            g_band[n].F_Raster,
            BW,
            N_RB,
            SCS
        );
    }
    else if ((GSCN >= 7499) && (GSCN <= 22255))
    {
        N = (GSCN - 7499);
        SS_REF  = ((3000 + (N * 1.44)) * 1000);  /* KHz */
        ss_low  = (SS_REF - (ss_raster * 120));
        ss_high = (SS_REF + (ss_raster * 120));
        show_frequency(
            SS_REF,
            ss_low,
            ss_high,
            ss_raster,
            g_band[n].F_DL_low,
            g_band[n].F_DL_high,
            g_band[n].F_Raster,
            BW,
            N_RB,
            SCS
        );
    }
    else
    {
        printf("ERR: wrong GSCN %d\n", GSCN);
        return 0;
    }


    return 0;
}

