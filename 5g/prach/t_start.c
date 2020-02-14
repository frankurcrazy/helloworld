#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void help(void)
{
    printf("Usage: t_start [OPTION]...\n");
    printf("\n");
    printf("  -s value   RA subcarrier spacing (1.25, 5, 15, 30, 60, 120 KHz).\n");
    printf("  -l value   Symbol index (0 ~ 27).\n");
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/* 38.211 Section 5.3.2 OFDM baseband signal generation for PRACH */
int main(int argc, char *argv[])
{
    char *scs = "15";
    int u = 0;
    int l = 0;

    double t_RA_start_l[30];
    double t_RA_start;
    int N_u;
    int N_CP;
    int i;

    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "s:l:i:z:h")) != -1)
    {
        switch ( ch )
        {
            case 's':
                scs = optarg;
                if (0 == strcmp("1.25", scs))
                {
                    u = 0;
                }
                else if (0 == strcmp("5", scs))
                {
                    u = 0;
                }
                else if (0 == strcmp("15", scs))
                {
                    u = 0;
                }
                else if (0 == strcmp("30", scs))
                {
                    u = 1;
                }
                else if (0 == strcmp("60", scs))
                {
                    u = 2;
                }
                else if (0 == strcmp("120", scs))
                {
                    u = 3;
                }
                else
                {
                    printf("ERR: wrong subcarrier spacing %s\n\n", scs);
                    return 0;
                }
                break;
            case 'l':
                l = atoi( optarg );
                if ((l < 0) || (l > 27))
                {
                    printf("ERR: wrong symbol index %d\n\n", l);
                    return 0;
                }
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    N_u = ((2048 * 64) >> u);

    for (i=0; i<(l+1); i++)
    {
        if (i > 0)
        {
            if ((0 == i) || ((7 << u) == i))
            {
                N_CP = (((144 * 64) >> u) + (16 * 64));
            }
            else
            {
                N_CP = ((144 * 64) >> u);
            }
            t_RA_start_l[i] = (t_RA_start_l[i-1] + ((N_u + N_CP) * 1000 / (480 * 4096)));
        }
        else
        {
            t_RA_start_l[0] = 0;
        }
    }

    t_RA_start = t_RA_start_l[i-1];


    printf("SCS = %s KHz\n", scs);
    printf("u = %d\n", u);
    printf("l = %d\n", l);
    printf("t_RA_start = %.4f us\n", t_RA_start);
    printf("\n");

    return 0;
}

