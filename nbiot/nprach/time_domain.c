#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/* ms x 10 */
int preamble[2] = {
    56,
    64
};

void help(void)
{
    printf("Usage: time_loc [OPTION]...\n");
    printf("\n");
    printf("  -s N_NPRACH_period  (40, 80, 160, 240, 320, 640, 1280, 2560).\n");
    printf("  -o N_NPRACH_start   (8, 16, 32, 64, 128, 256, 512, 1024).\n");
    printf("  -r N_NPRACH_rep     (1, 2, 4, 8, 16, 32, 64, 128).\n");
    printf("  -f format           (0 or 1).\n");
    printf("  -d                  Enable the debug log.\n");
    printf("  -h                  Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.211 Section 10.1.6 Narrowband physical random access channel
*/
int main(int argc, char *argv[])
{
    int   N_NPRACH_period = 80; /* 40, 80, 160, 240, 320, 640, 1280, 2560 ms */
    int   N_NPRACH_start = 16; /* 8, 16, 32, 64, 128, 256, 512, 1024 ms */
    int   N_NPRACH_rep = 4; /* 1, 2, 4, 8, 16, 32, 64, 128 */
    int   format = 0;

    int   debug = 0;
    int   count = 0;
    int   ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "p:s:r:f:dh")) != -1)
    {
        switch ( ch )
        {
            case 'p':
                N_NPRACH_period = atoi( optarg );
                break;
            case 's':
                N_NPRACH_start = atoi( optarg );
                break;
            case 'r':
                N_NPRACH_rep = atoi( optarg );
                break;
            case 'f':
                format = atoi( optarg );
                break;
            case 'd':
                debug = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ((N_NPRACH_start + (preamble[format] * N_NPRACH_rep)) > (N_NPRACH_period * 10))
    {
        printf(
            "incorrect N_NPRACH_period %d, N_NPRACH_start %d, N_NPRACH_rep %d\n",
            N_NPRACH_period,
            N_NPRACH_start,
            N_NPRACH_rep
        );
        exit(0);
    }

    printf("N_NPRACH_period = %d ms\n", N_NPRACH_period);
    printf("N_NPRACH_start  = %d ms\n", N_NPRACH_start);
    printf("N_NPRACH_rep    = %d\n", N_NPRACH_rep);
    printf("format          = %d\n", format);
    printf("\n");

    {
        int nf;
        int slot;

        slot = (N_NPRACH_start / 2);

        printf("Radio frame:\n");
        for (nf=0; nf<1024; nf++)
        {
            if ((nf % (N_NPRACH_period / 10)) == 0)
            {
                if (((count % 10) == 0) && (count != 0))
                {
                    printf("\n");
                }
                printf(" %4d", nf);
                count++;
            }
        }
        printf("\n");
        printf("\n");

        if ( debug )
        {
            if (0 == format)
            {
                printf("format[0] preamble length\n");
                printf("  = (2048 + 5 * 8192) * 4 / 30720 ms\n");
                printf("  = 5.6 ms\n");
            }
            else
            {
                printf("format[1] preamble length\n");
                printf("  = (8192 + 5 * 8192) * 4 / 30720 ms\n");
                printf("  = 6.4 ms\n");
            }
            printf("\n");

            printf("+%d slots [%d.%d x %d = %d.%d ms]\n",
                slot,
                (preamble[format] / 10),
                (preamble[format] % 10),
                N_NPRACH_rep,
                ((preamble[format] * N_NPRACH_rep) / 10),
                ((preamble[format] * N_NPRACH_rep) % 10)
            );
            printf("\n");
        }

        printf("  start   preamble   remain\n");
        printf(
            "[ %4d ][ %4d.%d  ][ %4d.%d ] ms\n",
            N_NPRACH_start,
            ((preamble[format] * N_NPRACH_rep) / 10),
            ((preamble[format] * N_NPRACH_rep) % 10),
            ((((N_NPRACH_period - N_NPRACH_start) * 10) - (preamble[format] * N_NPRACH_rep)) / 10),
            ((((N_NPRACH_period - N_NPRACH_start) * 10) - (preamble[format] * N_NPRACH_rep)) % 10)
        );
        printf("\n");
    }

    return 0;
}

