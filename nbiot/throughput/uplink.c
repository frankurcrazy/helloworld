#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define CEIL(x, y)   (((x) + ((y) - 1)) / (y))
#define FLOOR(x, y)  ((x) / (y))


/*
* TS 36.213 Table 16.5.1.1-2: Number of resource units (N_RU) for NPUSCH
*/
int N_RU[] = {
    0,
    0, 1, 2, 3, 4, 5, 0, 6, 0, 7
};

/* 36.213 Table 16.5.1.2-2 */
static int tb_size[13][8] = {
    /*  0    1    2     3     4     5     6     7 */
    {  16,  32,  56,   88,  120,  152,  208,  256 },  // 0
    {  24,  56,  88,  144,  176,  208,  256,  344 },  // 1
    {  32,  72, 144,  176,  208,  256,  328,  424 },  // 2
    {  40, 104, 176,  208,  256,  328,  440,  568 },  // 3
    {  56, 120, 208,  256,  328,  408,  552,  680 },  // 4
    {  72, 144, 224,  328,  424,  504,  680,  872 },  // 5
    {  88, 176, 256,  392,  504,  600,  808, 1000 },  // 6
    { 104, 224, 328,  472,  584,  712, 1000,    0 },  // 7
    { 120, 256, 392,  536,  680,  808,    0,    0 },  // 8
    { 136, 296, 456,  616,  776,  936,    0,    0 },  // 9
    { 144, 328, 504,  680,  872, 1000,    0,    0 },  //10
    { 176, 376, 584,  776, 1000,    0,    0,    0 },  //11
    { 208, 440, 680, 1000,    0,    0,    0,    0 }   //12
};


void help(void)
{
    printf("Usage: uplink [OPTION]...\n");
    printf("\n");
    printf("  -m MCS   MCS index: 0 ~ 12.\n");
    printf("  -r N_RU  RU nummber: 1, 2, 3, 4, 5, 6, 8, 10.\n");
    printf("\n");
    printf("  -h       Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.213 Section 16.5.1.2 Modulation order, redundancy version and transport block size determination
 */
int main(int argc, char *argv[])
{
    double dataRate = 0;
    double mbps;
    int mcs = 12;
    int ru = 4;
    int I_TBS;
    int I_RU;
    int N_RU_sc;
    int N_UL_slot;
    int Qm;
    int bits;
    int duration;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "m:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'm':
                mcs = atoi( optarg );
                break;
            case 'r':
                ru = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((mcs < 0) || (mcs > 12))
    {
        printf("ERR: incorrect MCS %d\n\n", mcs);
        return -1;
    }

    if ((ru != 1) && (ru != 2) && (ru != 3) && (ru != 4) &&
        (ru != 5) && (ru != 6) && (ru != 8) && (ru != 10))
    {
        printf("ERR: incorrect RU number %d\n\n", ru);
        return -1;
    }

    I_TBS = mcs;
    I_RU = N_RU[ru];
    N_RU_sc = 12;
    N_UL_slot = 2;
    Qm = 2;
    bits = tb_size[I_TBS][I_RU];
    printf("N_RU_sc   = %d\n", N_RU_sc);
    printf("N_RU      = %d\n", ru);
    printf("N_UL_slot = %d\n", N_UL_slot);
    printf("TB size   = %d bits\n", bits);
    printf("Modulaton = %s\n", ((Qm == 1) ? "BPSK"  :
                                (Qm == 2) ? "QPSK"  :
                                (Qm == 4) ? "16QAM" : "64QAM"));

    /*
     *   1     8     N_RU       3  
     * +---+       +--------+       +---+
     * |DCI|<----->| NPUSCH |<----->|DCI|
     * +---+       +--------+       +---+
     * |<-------------------------->|
     *   (TB bits) per (1 + 8 + N_RU + 3) ms
     */
    duration = (1 + 8 + ru + 3);
    mbps = (((double)1000 / duration) * bits);
    if (mbps > 1000000)
    {
        dataRate = (mbps / 1000000);
        printf("Data rate = [1;33m%.4f Mbps[0m\n", dataRate);
    }
    else if (mbps > 1000)
    {
        dataRate = (mbps / 1000);
        printf("Data rate = [1;33m%.4f Kbps[0m\n", dataRate);
    }
    else
    {
        printf("Data rate = [1;33m%.4f bps[0m\n", dataRate);
    }
    printf("\n");

    return 0;
}

