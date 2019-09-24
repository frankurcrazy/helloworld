#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


/* 38.211
 * Table 4.3.2-1:
 *   Number of OFDM symbols per slot, slots per frame, and slots
 *    per subframe for normal cyclic prefix.
 */
int N_frame_u_slot[5] = { 10, 20, 40, 80, 160 };

void help(void)
{
    printf("Usage: occasion [OPTION]...\n");
    printf("\n");
    printf("  -u   OFDM numerology (0, 1, 2, 3, 4).\n");
    printf("  -k   PDCCH monitoring periodicity (k_s slots).\n");
    printf("  -o   PDCCH monitoring offset (o_s slots).\n");
    printf("  -t   PDCCH monitoring duration (T_s slots).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.213 Section 10.1 UE procedure for determining physical downlink control channel assignment */
int main(int argc, char *argv[])
{
    int u = 0;
    int k_s = 8;
    int o_s = 0;
    int T_s = 2;

    int n_u_s_f[160];
    int n_f;
    int slot;
    int flag;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "u:k:o:t:h")) != -1)
    {
        switch ( ch )
        {
            case 'u':
                u = atoi( optarg );
                if ((u < 0) || (u > 4))
                {
                    printf("ERR: wrong OFDM numerology %d\n", u);
                    return -1;
                }
                break;
            case 'k':
                k_s = atoi( optarg );
                break;
            case 'o':
                o_s = atoi( optarg );
                break;
            case 't':
                T_s = atoi( optarg );
                break;
            default:
                help();
                return 0;
        }
    }


    if ((k_s != 1) &&
        (k_s != 2) &&
        (k_s != 4) &&
        (k_s != 5) &&
        (k_s != 8) &&
        (k_s != 10) &&
        (k_s != 16) &&
        (k_s != 20) &&
        (k_s != 40) &&
        (k_s != 80) &&
        (k_s != 160) &&
        (k_s != 320) &&
        (k_s != 640) &&
        (k_s != 1280) &&
        (k_s != 2560))
    {
        printf("ERR: wrong PDCCH monitoring periodicity %d\n", k_s);
        return -1;
    }
    if ((o_s < 0) || (o_s >= k_s))
    {
        printf("ERR: wrong PDCCH monitoring offset %d\n", o_s);
        return -1;
    }


    printf("PDCCH monitoring occasion (slot):\n");
    memset(n_u_s_f, 0, (160 * 4));
    for (n_f=0; n_f<1024; n_f++)
    {
        flag = 0;
        for (slot=0; slot<N_frame_u_slot[u]; slot++)
        {
            if (0 == n_u_s_f[slot])
            {
                if (0 == (((n_f * N_frame_u_slot[u]) + slot - o_s) % (k_s)))
                {
                    n_u_s_f[slot] = 1;
                    printf("  %d", slot);
                    flag = 1;
                }
            }
        }
        if ( flag ) printf("\n");
    }
    printf("\n");


    return 0;
}

