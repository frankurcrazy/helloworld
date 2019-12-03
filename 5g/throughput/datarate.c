#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


void help(void)
{
    printf("Usage: datarate [OPTION]...\n");
    printf("\n");
    printf("  -j J       Number of aggregated component carriers.\n");
    printf("  -v layer   Number of supported layers (1, 2, 4, 8).\n");
    printf("  -r R       Target code rate x 1024.\n");
    printf("  -q Qm      Max. supported modulation order.\n");
    printf("             (1: BPSK, 2: QPSB, 4: 16QAM, 6: 64QAM, 8: 256QAM)\n");
    printf("  -f factor  Scaling factor (1, 0.8, 0.75, 0.4).\n");
    printf("  -u SCS     Subcarrier spacing (15, 30, 60 KHz).\n");
    printf("  -n RB      Max. RB allocation.\n");
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/* 38.306 Section 4.1.2 Supported max data rate */
int main(int argc, char *argv[])
{
    char   modulation[8] = { '2', '5', '6', 'Q', 'A', 'M', 0, 0 };
    double dlRate = 0;
    double ulRate = 0;
    double OH[2] = { 0.14, 0.08 };
    double Ts;
    int J = 1;
    int vLayers = 2;
    int R = 948;
    int Qm = 8;
    double f = 1;
    int N_BW_PRB = 273;
    int scs = 30;
    int u = 1;
    int j;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "j:v:r:q:f:u:n:h")) != -1)
    {
        switch ( ch )
        {
            case 'j':
                J = atoi( optarg );
                if (J <= 0) J = 1;
                break;
            case 'v':
                vLayers = atoi( optarg );
                if ((vLayers != 1) &&
                    (vLayers != 2) &&
                    (vLayers != 4) &&
                    (vLayers != 8))
                {
                    printf("ERR: wrong number of layers (%d)", vLayers);
                    return 0;
                }
                break;
            case 'r':
                R = atoi( optarg );
                if (R < 30) R = 30;
                break;
            case 'q':
                Qm = atoi( optarg );
                if (1 == Qm)
                {
                    sprintf(modulation, "BPSK");
                }
                else if (2 == Qm)
                {
                    sprintf(modulation, "QPSK");
                }
                else if (4 == Qm)
                {
                    sprintf(modulation, "16QAM");
                }
                else if (6 == Qm)
                {
                    sprintf(modulation, "64QAM");
                }
                else if (8 == Qm)
                {
                    sprintf(modulation, "256QAM");
                }
                else
                {
                    printf("ERR: wrong modulation order (%d)", Qm);
                    return 0;
                }
                break;
            case 'f':
                f = atof( optarg );
                if ((f != 1.0) &&
                    (f != 0.8) &&
                    (f != 0.75) &&
                    (f != 0.4))
                {
                    printf("ERR: wrong scaling factor (%s)", optarg);
                    return 0;
                }
                break;
            case 'u':
                scs = atoi( optarg );
                if (15 == scs)
                {
                    u = 0;
                }
                else if (30 == scs)
                {
                    u = 1;
                }
                else if (60 == scs)
                {
                    u = 2;
                }
                else
                {
                    printf("ERR: wrong subcarrier spacing (%d)", scs);
                    return 0;
                }
                break;
            case 'n':
                N_BW_PRB = atoi( optarg );
                if (N_BW_PRB <= 0) N_BW_PRB = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    Ts = (1 / ((14 << u) * 1000));

    for (j=1; j<=J; j++)
    {
        dlRate += (vLayers * Qm * f * R * N_BW_PRB * 12 * (1 - OH[0]) * (14 << u) / 1024);
        ulRate += (vLayers * Qm * f * R * N_BW_PRB * 12 * (1 - OH[1]) * (14 << u) / 1024);
    }
    dlRate = (dlRate / 1000);
    ulRate = (ulRate / 1000);

    printf("\n");
    printf("J = %d (CCs)\n", J);
    printf("R = %d (code rate %.3f)\n", R, ((float)R / 1024));
    printf("vLayers = %d (layers)\n", vLayers);
    printf("Qm = %d (%s)\n", Qm, modulation);
    printf("f = %.2f\n", f);
    printf("u = %d (%dKHz)\n", u, scs);
    printf("N_BW_PRB = %d (RBs)\n", N_BW_PRB);
    printf("\n");

    printf("[1;36mDL[0m data rate:\n");
    printf("   [1;33m%.2f Mbpps[0m\n", dlRate);
    printf(
        " = (%d * %d * %d * %.2f * (%d / 1024) * (%d * 12) * (14 * 2^%d) * 10^3 * (1 - %.2f)) / 10^6\n",
        J,
        vLayers,
        Qm,
        f,
        R,
        N_BW_PRB,
        u,
        OH[0]
    );
    printf("\n");
    printf("[1;36mUL[0m data rate:\n");
    printf("   [1;33m%.2f Mbps[0m\n", ulRate);
    printf(
        " = (%d * %d * %d * %.2f * (%d / 1024) * (%d * 12) * (14 * 2^%d) * 10^3 * (1 - %.2f)) / 10^6\n",
        J,
        vLayers,
        Qm,
        f,
        R,
        N_BW_PRB,
        u,
        OH[1]
    );
    printf("\n");

    return 0;
}

