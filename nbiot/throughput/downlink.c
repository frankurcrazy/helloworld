#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define CEIL(x, y)   (((x) + ((y) - 1)) / (y))
#define FLOOR(x, y)  ((x) / (y))


/*
* TS 36.213 Table 16.4.1.3-1: Number of subframes for NPDSCH.
*/
int N_SF[] = {
    0,
    0, 1, 2, 3, 4, 5, 0, 6, 0, 7
};

/*
* TS 36.213 Table 16.4.1.5.1-1: Transport block size (TBS) table
*/
int tb_size[13][8] = {
    /*  0    1    2    3    4    5    6    7 */
    {  16,  32,  56,  88, 120, 152, 208, 256 },  // 0
    {  24,  56,  88, 144, 176, 208, 256, 344 },  // 1
    {  32,  72, 144, 176, 208, 256, 328, 424 },  // 2
    {  40, 104, 176, 208, 256, 328, 440, 568 },  // 3
    {  56, 120, 208, 256, 328, 408, 552, 680 },  // 4
    {  72, 144, 224, 328, 424, 504, 680,   0 },  // 5
    {  88, 176, 256, 392, 504, 600,   0,   0 },  // 6
    { 104, 224, 328, 472, 584, 680,   0,   0 },  // 7
    { 120, 256, 392, 536, 680,   0,   0,   0 },  // 8
    { 136, 296, 456, 616,   0,   0,   0,   0 },  // 9
    { 144, 328, 504, 680,   0,   0,   0,   0 },  //10
    { 176, 376, 584,   0,   0,   0,   0,   0 },  //11
    { 208, 440, 680,   0,   0,   0,   0,   0 }   //12
};


void help(void)
{
    printf("Usage: downlink [OPTION]...\n");
    printf("\n");
    printf("  -m MCS   MCS index: 0 ~ 12.\n");
    printf("  -s N_SF  Subframe nummber: 1, 2, 3, 4, 5, 6, 8, 10.\n");
    printf("\n");
    printf("  -h       Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.213 Section 16.4.1.5 Modulation order and transport block size determination
 */
int main(int argc, char *argv[])
{
    double dataRate = 0;
    double mbps;
    int mcs = 10;
    int sf = 4;
    int I_TBS;
    int I_SF;
    int Qm;
    int bits;
    int duration;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "m:s:h")) != -1)
    {
        switch ( ch )
        {
            case 'm':
                mcs = atoi( optarg );
                break;
            case 's':
                sf = atoi( optarg );
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

    if ((sf != 1) && (sf != 2) && (sf != 3) && (sf != 4) &&
        (sf != 5) && (sf != 6) && (sf != 8) && (sf != 10))
    {
        printf("ERR: incorrect subframe number %d\n\n", sf);
        return -1;
    }

    I_TBS = mcs;
    I_SF = N_SF[sf];
    Qm = 2;
    bits = tb_size[I_TBS][I_SF];
    if (0 == bits)
    {
        printf(
            "ERR: incorrect MCS %d or subframe number %d\n\n",
            mcs,
            sf
        );
        return -1;
    }
    printf("N_SF      = %d\n", sf);
    printf("TB size   = %d bits\n", bits);
    printf("Modulaton = %s\n", ((Qm == 2) ? "QPSK"  :
                                (Qm == 4) ? "16QAM" : "64QAM"));

    /*
     *   1     4     N_SF       12     2     3
     * +---+       +--------+        +---+       +---+
     * |DCI|<----->| NPDSCH |<------>|ACK|<----->|DCI|
     * +---+       +--------+        +---+       +---+
     * |<--------------------------------------->|
     *   (TB bits) per (1 + 4 + N_SF + 12 + 2 + 3) ms
     */
    duration = (1 + 4 + sf + 12 + 2 + 3);
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

