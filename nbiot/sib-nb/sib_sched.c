#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/* Integers */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

#define DIV_FLOOR(X, Y)  ( (X) / (Y) )


/* 36.214 Table 16.4.1.3-3 */
static uint16  _sib1NbRepNum[16] = {
   4, 8, 16,
   4, 8, 16,
   4, 8, 16,
   4, 8, 16,
   0, 0, 0, 0
};

/* 36.214 Table 16.4.1.5.2-1 */
static uint16  _sib1NbTbSize[16] = {
   208, 208, 208,
   328, 328, 328,
   440, 440, 440,
   680, 680, 680,
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

/* SIB1-NB available mask */
uint16  g_subframeMask[1024];


void setSubframe(uint16 cellIdLsb, uint16 repNum)
{
    uint16 frame = 0;
    int i;
    int j;
    int k;


    if (4 == repNum)
    {
        for (k=0; k<4; k++)
        {
            /* 256 frames period */
            for (j=0; j<16; j++)
            {
                /* repetition 64 frames */
                for (i=0; i<16; i++)
                {
                    if (cellIdLsb == (j & 0x3))
                    {
                        g_subframeMask[frame] = 0x010;
                    }
                    else
                    {
                        g_subframeMask[frame] = 0;
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
                /* repetition 128 frames */
                for (i=0; i<16; i++)
                {
                    if (cellIdLsb == (j & 0x1))
                    {
                        g_subframeMask[frame] = 0x010;
                    }
                    else
                    {
                        g_subframeMask[frame] = 0;
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
            /* repetition every frame */
            g_subframeMask[frame] = 0x010;
            frame++;
        }
    }
}

void schedSib1Nb(uint16 cellIdLsb, int startFrm)
{
    int blockIndex;
    int i, j;


    for (i=0; i<1024; i++)
    {
        for (j=0; j<10; j++)
        {
            if (g_subframeMask[i] & (0x1 << j))
            {
                if ((i & 0xFF) == startFrm)
                {
                    g_repIndex = 0;
                }

                if (1 == startFrm)
                {
                    blockIndex = ((i + 15) & 0xF);
                }
                else
                {
                    blockIndex = (i & 0xF);
                }

                /* start a new SIB1-NB at this subframe */
                if (0 == blockIndex)
                {
                    if (0 == g_repIndex)
                    {
                        printf("\n");
                        printf("SIB1-NB repetition\n");
                    }
                    printf("  %2d:", g_repIndex);
                    g_repIndex++;
                }

                printf(" %4d", i);
                if (15 == blockIndex) printf("\n");
            }
        }
    }
    printf("\n");
}


void help(void)
{
    printf("Usage: scheduling [OPTION]...\n");
    printf("\n");
    printf("  -c cellId   Cell ID.\n");
    printf("  -s schInfo  SIB1-NB scheduling info (0 ~ 11).\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

/*
*  R13 36.331 Section 5.2.1.2a Scheduling for NB-IoT
*/
int main(int argc, char *argv[])
{
    int cellId = 0;
    int schInfo = 0;

    int cellIdLsb;
    int repNum;
    int tbSize;
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
    repNum = _sib1NbRepNum[ schInfo ];
    tbSize = _sib1NbTbSize[ schInfo ];
    startFrm = _sib1NbStartFrm[ schInfo ][ cellIdLsb ];

    setSubframe(cellIdLsb, repNum);

    schedSib1Nb(cellIdLsb, startFrm);

    printf("TB size = %d bits\n\n", tbSize);

    return 0;
}
