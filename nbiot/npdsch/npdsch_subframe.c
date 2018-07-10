#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/* Integers */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;


/* 36.214 Table 16.4.1.3-3 */
static uint16  _sib1NbRepNum[16] = {
   4, 8, 16,
   4, 8, 16,
   4, 8, 16,
   4, 8, 16,
   0, 0, 0, 0
};

/* 36.214 Table 16.4.1.3-4 */
static int  _sib1NbStartFrm[16][4] = {
   {  0, 16, 32, 48  },  //0
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   {  0, 16, 32, 48  },  //3
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   {  0, 16, 32, 48  },  //6
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   {  0, 16, 32, 48  },  //9
   {  0, 16,  0, 16  },
   {  0,  1,  0,  1  },
   { -1, -1, -1, -1  },  //12
   { -1, -1, -1, -1  },
   { -1, -1, -1, -1  },
   { -1, -1, -1, -1  }
};

/* SIB1-NB repetition control */
uint16  g_repIndex;

/* DL un-available mask */
uint16  g_subframeMask[1024] = {0};


void npss(void)
{
    int i;

    for (i=0; i<1024; i++)
    {
        /* NPSS is at subframe 5 */
        g_subframeMask[i] |= 0x020;
    }
}

void nsss(void)
{
    int i;

    for (i=0; i<1024; i++)
    {
        /* NSSS is at subframe 9 of every even frame */
        if ((i & 0x1) == 0)
        {
            g_subframeMask[i] |= 0x200;
        }
    }
}

void mib(void)
{
    int i;

    for (i=0; i<1024; i++)
    {
        /* MIB-NB is at subframe 0 */
        g_subframeMask[i] |= 0x001;
    }
}

void sib(uint16 cellId, uint16 repNum)
{
    uint16 frame = 0;
    int i;
    int j;
    int k;

    /* SIB-NB is at subframe 4 */
    if (4 == repNum)
    {
        for (k=0; k<4; k++)
        {
            /* 256 frames period */
            for (j=0; j<16; j++)
            {
                /* repetition (16 x 4) = 64 frames */
                for (i=0; i<16; i++)
                {
                    if ((cellId & 0x3) == (j & 0x3))
                    {
                        g_subframeMask[frame] |= 0x010;
                    }
                    frame++;
                }
            }
        }
    }
    else if (8 == repNum)
    {
        for (k=0; k<4; k++)
        {
            /* 256 frames period */
            for (j=0; j<16; j++)
            {
                /* repetition (16 x 8) = 128 frames */
                for (i=0; i<16; i++)
                {
                    if ((cellId & 0x1) == (j & 0x1))
                    {
                        g_subframeMask[frame] |= 0x010;
                    }
                    frame++;
                }
            }
        }
    }
    else
    {
        for (i=0; i<1024; i++)
        {
            /* repetition on every frame */
            g_subframeMask[frame] |= 0x010;
            frame++;
        }
    }
}

void availableSubframe(void)
{
    int i, j;

    printf("Frame\n");
    for (i=0; i<1024; i++)
    {
        printf("%4d : ", i);
        for (j=0; j<10; j++)
        {
            if (0 == (g_subframeMask[i] & (0x1 << j)))
            {
                printf(" [1;34m%d[0m ", j);
            }
            else
            {
                printf(" x ");
            }
        }
        printf("\n");
    }
    printf("\n");
}


void help(void)
{
    printf("Usage: npdsch_subframe [OPTION]...\n");
    printf("\n");
    printf("  -c cellId   Cell ID.\n");
    printf("  -s schInfo  SIB1-NB scheduling info (0 ~ 11).\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int cellId = 0;
    int schInfo = 0;

    int cellIdLsb;
    int repNum;
    int startFrm;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "c:s:h")) != -1)
    {
        switch ( ch )
        {
            case 'c':
                cellId = atoi( optarg );
                break;
            case 's':
                schInfo = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    cellIdLsb = (cellId & 0x3);
    repNum    = _sib1NbRepNum[ schInfo ];
    startFrm  = _sib1NbStartFrm[ schInfo ][ cellIdLsb ];

    npss();
    nsss();
    mib();
    sib(cellId, repNum);

    availableSubframe();

    printf("Cell ID = %d\n", cellId);
    printf("SIB1-NB start frame = %d\n", startFrm);
    printf("SIB1-NB repetition number = %d\n", repNum);
    printf("\n");

    return 0;
}

