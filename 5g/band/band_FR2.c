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
    /* n257 */
    g_band[257].F_UL_low  = 26500;
    g_band[257].F_UL_high = 29500;
    g_band[257].F_DL_low  = 26500;
    g_band[257].F_DL_high = 29500;
    g_band[257].F_Raster[0] = 60;
    g_band[257].F_Raster[1] = 120;
    g_band[257].SSB_SCS[0] = 120;
    g_band[257].SSB_SCS[1] = 240;
    g_band[257].SSB_first[0] = 22388;
    g_band[257].SSB_first[1] = 22390;
    g_band[257].SSB_last[0]  = 22558;
    g_band[257].SSB_last[1]  = 22556;
    g_band[257].SSB_step[0]  = 1;
    g_band[257].SSB_step[1]  = 2;
    /* n258 */
    g_band[258].F_UL_low  = 24250;
    g_band[258].F_UL_high = 27500;
    g_band[258].F_DL_low  = 24250;
    g_band[258].F_DL_high = 27500;
    g_band[258].F_Raster[0] = 60;
    g_band[258].F_Raster[1] = 120;
    g_band[258].SSB_SCS[0] = 120;
    g_band[258].SSB_SCS[1] = 240;
    g_band[258].SSB_first[0] = 22257;
    g_band[258].SSB_first[1] = 22258;
    g_band[258].SSB_last[0]  = 22443;
    g_band[258].SSB_last[1]  = 22442;
    g_band[258].SSB_step[0]  = 1;
    g_band[258].SSB_step[1]  = 2;
    /* n260 */
    g_band[260].F_UL_low  = 37000;
    g_band[260].F_UL_high = 40000;
    g_band[260].F_DL_low  = 37000;
    g_band[260].F_DL_high = 40000;
    g_band[260].F_Raster[0] = 60;
    g_band[260].F_Raster[1] = 120;
    g_band[260].SSB_SCS[0] = 120;
    g_band[260].SSB_SCS[1] = 240;
    g_band[260].SSB_first[0] = 22995;
    g_band[260].SSB_first[1] = 22996;
    g_band[260].SSB_last[0]  = 23166;
    g_band[260].SSB_last[1]  = 23164;
    g_band[260].SSB_step[0]  = 1;
    g_band[260].SSB_step[1]  = 2;
    /* n261 */
    g_band[261].F_UL_low  = 27500;
    g_band[261].F_UL_high = 28350;
    g_band[261].F_DL_low  = 27500;
    g_band[261].F_DL_high = 28350;
    g_band[261].F_Raster[0] = 60;
    g_band[261].F_Raster[1] = 120;
    g_band[261].SSB_SCS[0] = 120;
    g_band[261].SSB_SCS[1] = 240;
    g_band[261].SSB_first[0] = 22446;
    g_band[261].SSB_first[1] = 22446;
    g_band[261].SSB_last[0]  = 22492;
    g_band[261].SSB_last[1]  = 22490;
    g_band[261].SSB_step[0]  = 1;
    g_band[261].SSB_step[1]  = 2;
}

int bw2rb(int band, int bw, int scs)
{
    int rb = 0;

    if (60 == scs)
    {
        if (50 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 66;
            }
        }
        else if (100 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 132;
            }
        }
        else if (200 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 264;
            }
        }
    }
    if (120 == scs)
    {
        if (50 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 32;
            }
        }
        else if (100 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 66;
            }
        }
        else if (200 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 132;
            }
        }
        else if (400 == bw)
        {
            if ((band == 257) ||
                (band == 258) ||
                (band == 260) ||
                (band == 261))
            {
                rb = 264;
            }
        }
    }

    return rb;
}

void show_frequency(
    double SS_REF,    /* MHz */
    double ss_low,    /* MHz */
    double ss_high,   /* MHz */
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


    printf("================================================================\n");
    printf("        Frequency (MHz)        | N_SSB_CRB | k_SSB | Raster\n");
    printf("================================================================\n");
    printf(
        "[1;36mSSB[0m %.2f [1;33m%.2f[0m %.2f |           |       | %d KHz\n",
        ss_low,
        SS_REF,
        ss_high,
        ss_raster
       );
    printf("-------------------------------+-----------+-------+------------\n");
    for (i=0; i<2; i++)
    {
        count = 0;
        sprintf(raster, "%d KHz", F_Raster[i]);

        for (F_REF  = F_DL_low;
             F_REF <= F_DL_high;
             F_REF += ((double)F_Raster[i] / 1000))
        {
            f_low  = (F_REF - (BW / 2));
            f_high = (F_REF + (BW / 2));
            if (1 == (N_RB % 2))
            {
                rb_low  = (F_REF - ((double)(((DIV_FLOOR(N_RB, 2) * 12) + 6) * SCS) / 1000));
                rb_high = (F_REF + ((double)(((DIV_FLOOR(N_RB, 2) * 12) + 6) * SCS) / 1000));
            }
            else
            {
                rb_low  = (F_REF - ((double)((DIV_FLOOR(N_RB, 2) * 12) * SCS) / 1000));
                rb_high = (F_REF + ((double)((DIV_FLOOR(N_RB, 2) * 12) * SCS) / 1000));
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
                if (0 == (((int)(ss_low - rb_low) * 1000) % SCS))
                {
                    N_SSB_CRB = ((((int)(ss_low - rb_low) * 1000) / 60) / 12);
                    k_SSB  = ((((int)(ss_low - rb_low) * 1000) / 60) % 12);
                    k_SSB /= (SCS / 60);
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
            printf("-------------------------------+-----------+-------+------------\n");
        }
    }
    printf("\n");
}

void help(void)
{
    printf("Usage: band_FR2 [OPTION]...\n");
    printf("\n");
    printf("  -b   NR operating band (257, 258, 260, 261).\n");
    printf("  -w   Bandwidth in MHz (50, 100, 200, 400).\n");
    printf("  -s   Subcarrier spacing (60, 120 KHz).\n");
    printf("  -g   GSCN (22256 ~ 26639).\n");
    printf("  -r   SS Block raster (120, 240 KHz).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.104 Section 5.4.3 Synchronization raster */
int main(int argc, char *argv[])
{
    double SS_REF;
    double ss_low;
    double ss_high;
    int band = 257;
    int GSCN = 22388;
    int BW = 50;
    int SCS = 120;  // subCarrierSpacingCommon
    int ss_raster = 120;
    int g = 0;
    int N_RB;
    int N;
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
    if ((n != 257) && (n != 258) && (n != 260) && (n != 261))
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

    if ((GSCN >= 22256) && (GSCN <= 26639))
    {
        N = (GSCN - 22256);
        SS_REF  = (24250.08 + (N * 17.28));  /* MHz */
        ss_low  = (SS_REF - ((double)(ss_raster * 120) / 1000));
        ss_high = (SS_REF + ((double)(ss_raster * 120) / 1000));
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

