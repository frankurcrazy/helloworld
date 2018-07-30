#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define CEIL_DIV(X, Y)  ( ( (X) + ((Y) - 1) ) / (Y) )

typedef struct _tNpuschRes
{
    int  npuschScNum;
    int  format;
    int  ruScNum;  /* N_RU_sc */
    int  ruSlot;   /* N_UL_slots */
    int  ruNum;    /* N_RU */
    int  repNum;   /* N_Rep */
    int  nSlots;   /* N_slots */
    int  mIdent;   /* M_NPUSCH_identical */

} tNpuschRes;


int  g_blockIndex = 0;
int  g_2msFlag = 0;
int  g_countSlot = 0;
int  g_countRu = 0;
int  g_countRep = 0;
int  g_countM = 0;


int npusch_sched(
    tNpuschRes *pNpusch,
    int         frmNum,
    int         subFrmNum,
    int         slotNum,
    int         startFlag
)
{

    if ( startFlag )
    {
        printf(
            "[1;36m%d[0m x [1;36m%d[0m x %d slots\n\n",
            pNpusch->ruSlot,
            pNpusch->ruNum,
            pNpusch->repNum
        );

        /* here has a NPUSCH start subframe */
        g_blockIndex = 0;
        g_2msFlag = 0;
        g_countSlot = pNpusch->ruSlot;
        g_countRu = pNpusch->ruNum;
        g_countRep = pNpusch->repNum;
        g_countM = pNpusch->mIdent;
        if (pNpusch->mIdent > 1)
        {
            printf("M_NPUSCH_identical - 1 = [1;35m%d[0m\n", (pNpusch->mIdent - 1));
        }
    }

    if (pNpusch->mIdent > 1)
    {
        if (g_countRep > 0)
        {
            if (g_countRu > 0)
            {
                if (g_countSlot > 0)
                {
                    if ((g_blockIndex == 0) && (g_countM == pNpusch->mIdent))
                    {
                        printf("(%d.%d) ", frmNum, subFrmNum);
                    }
                    if (g_countM == pNpusch->mIdent)
                    {
                        printf("[1;33m %d[0m", g_blockIndex);
                    }
                    else
                    {
                        printf(" %d", g_blockIndex);
                    }

                    g_countSlot -= 2;
                    if (g_countSlot <= 0)
                    {
                        g_countSlot = pNpusch->ruSlot;
                        g_countRu--;
                    }

                    g_countM--;
                    if (g_countM <= 0)
                    {
                        g_blockIndex += 2;
                        g_countM = pNpusch->mIdent;

                        if (g_blockIndex >= (pNpusch->ruSlot * pNpusch->ruNum))
                        {
                            g_blockIndex = 0;
                            printf("\n");
                        }
                    }
                }

                if (g_countRu <= 0)
                {
                    g_countRu = pNpusch->ruNum;
                    g_countRep--;
                }
            }

            if (g_countRep <= 0)
            {
                printf("\n");
                return 1;
            }
        }
    }
    else
    {
        if (g_countRep > 0)
        {
            if (g_countRu > 0)
            {
                if (g_countSlot > 0)
                {
                    if ((g_countSlot == pNpusch->ruSlot) && (0 == g_2msFlag))
                    {
                        printf("(%d.%d) ", frmNum, subFrmNum);
                    }
                    if (g_countRep == pNpusch->repNum)
                    {
                        printf("[1;33m %d[0m", g_blockIndex);
                    }
                    else
                    {
                        printf(" %d", g_blockIndex);
                    }

                    if (12 == pNpusch->npuschScNum)
                    {
                        g_countSlot -= 2;
                        g_blockIndex += 2;
                    }
                    else
                    {
                        g_countSlot -= g_2msFlag;
                        g_blockIndex += g_2msFlag;
                        g_2msFlag ^= 0x1;
                    }

                    if (g_countSlot <= 0)
                    {
                        g_countSlot = pNpusch->ruSlot;
                        g_countRu--;
                        printf("\n");
                    }
                }

                if (g_countRu <= 0)
                {
                    g_countRu = pNpusch->ruNum;
                    g_blockIndex = 0;
                    g_countRep--;
                }
            }

            if (g_countRep <= 0)
            {
                printf("\n");
                return 1;
            }
        }
    }

    return 0;
}


void help(void)
{
    printf("Usage: npusch_slot [OPTION]...\n");
    printf("\n");
    printf("  -s subcarrier   NPUSCH subcarrie number (12 or 48).\n");
    printf("  -f format       NPUSCH format (1 or 2).\n");
    printf("  -c N_RU_sc      Number of subcarriers (1, 3, 6, 12).\n");
    printf("  -u N_RU         Number of RUs (1, 2, 3, 4, 5, 6, 8, 10).\n");
    printf("  -r N_Rep        Number of repetitions (1, 2, 4, 8, 16, 32, 64, 128).\n");
    printf("  -a frame        Start frame (0 ~ 1023).\n");
    printf("  -b subframe     Start subframe (0 ~ 9).\n");
    printf("  -h              Show the help message.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    tNpuschRes npusch;
    int startFrmNum;
    int startSubFrmNum;
    int frmNum;
    int subFrmNum;
    int slotNum;
    int startFlag;
    int done;
    int ch;


    /* setup the default values */
    npusch.npuschScNum = 12;
    npusch.format = 1;
    npusch.ruScNum = 12;
    npusch.ruSlot = 2;
    npusch.ruNum = 1;
    npusch.repNum = 1;
    startFrmNum = 0;
    startSubFrmNum = 0;


    opterr = 0;
    while ((ch=getopt(argc, argv, "s:f:c:u:r:a:b:h")) != -1)
    {
        switch ( ch )
        {
            case 's':
                npusch.npuschScNum = atoi( optarg );
                break;
            case 'f':
                npusch.format = atoi( optarg );
                break;
            case 'c':
                npusch.ruScNum = atoi( optarg );
                break;
            case 'u':
                npusch.ruNum = atoi( optarg );
                break;
            case 'r':
                npusch.repNum = atoi( optarg );
                break;
            case 'a':
                startFrmNum = atoi( optarg );
                break;
            case 'b':
                startSubFrmNum = atoi( optarg );
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if (1 == npusch.format)
    {
        if (48 == npusch.npuschScNum)
        {
            npusch.ruScNum = 1;
            npusch.ruSlot = 16;
            npusch.nSlots = 1;
        }
        else if (12 == npusch.npuschScNum)
        {
            if (1 == npusch.ruScNum)
            {
                npusch.ruSlot = 16;
            }
            else if (3 == npusch.ruScNum)
            {
                npusch.ruSlot = 8;
            }
            else if (6 == npusch.ruScNum)
            {
                npusch.ruSlot = 12;
            }
            else if (12 == npusch.ruScNum)
            {
                npusch.ruSlot = 2;
            }
            else
            {
                printf("ERR: wrong RU subcarriers %d\n", npusch.ruScNum);
                return -1;
            }
            npusch.nSlots = 2;
        }
        else
        {
            printf("ERR: wrong NPUSCH subcarriers %d\n", npusch.npuschScNum);
            return -1;
        }
    }
    else if (2 == npusch.format)
    {
        if (48 == npusch.npuschScNum)
        {
            npusch.ruScNum = 1;
            npusch.ruSlot = 4;
            npusch.nSlots = 1;
        }
        else if (12 == npusch.npuschScNum)
        {
            npusch.ruScNum = 1;
            npusch.ruSlot = 4;
            npusch.nSlots = 2;
        }
        else
        {
            printf("ERR: wrong NPUSCH subcarriers %d\n", npusch.npuschScNum);
            return -1;
        }
    }
    else
    {
        printf("ERR: wrong NPUSCH format %d\n", npusch.format);
        return -1;
    }

    if (npusch.ruScNum > 1)
    {
        npusch.mIdent = CEIL_DIV(npusch.repNum, 2);
        if (npusch.mIdent > 4)
        {
            npusch.mIdent = 4;
        }
    }
    else
    {
        npusch.mIdent = 0;
    }

    frmNum = startFrmNum;
    subFrmNum = startSubFrmNum;
    while (1)
    {
        if (12 == npusch.npuschScNum)
        {
            slotNum = (subFrmNum << 1);
        }
        else
        {
            slotNum = (subFrmNum >> 1);
        }
        startFlag = ((frmNum == startFrmNum) && (subFrmNum == startSubFrmNum));

        done = npusch_sched(
                   &npusch,
                   frmNum,
                   subFrmNum,
                   slotNum,
                   startFlag
               );

        subFrmNum++;
        if (subFrmNum >= 10)
        {
            subFrmNum = 0;
            frmNum++;
            if (frmNum >= 1024)
            {
                frmNum = 0;
            }
        }

        if ( done ) return 0;
    }

    return 0;
}

