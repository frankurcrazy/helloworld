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

/*
*   On 32-bit machine, sizeof( long ) is 4
*   On 64-bit machine, sizeof( long ) is 8
*   Using %lu to print intptr
*/
typedef unsigned long   intptr;

typedef unsigned char   bool;

#define BOOL_FALSE      0
#define BOOL_TRUE       1


#define FLOOR_DIV(X, Y)  ( (X) / (Y) )


/* Bandwidth */
typedef enum
{
    LTE_BW_INVALID = 0x00,

    LTE_BW_1_4M    = 0x01,
    LTE_BW_3M      = 0x02,
    LTE_BW_5M      = 0x04,
    LTE_BW_10M     = 0x08,
    LTE_BW_15M     = 0x10,
    LTE_BW_20M     = 0x20,

    LTE_BW_ALL     = 0x3F
} tLteBw;

/* Resource block index */
typedef enum
{
    LTE_6_RB_IDX = 0,
    LTE_15_RB_IDX,
    LTE_25_RB_IDX,
    LTE_50_RB_IDX,
    LTE_75_RB_IDX,
    LTE_100_RB_IDX,

    LTE_MAX_RB_IDX
} tLteRbIdx;

/* DCI format */
typedef enum
{
    LTE_DCI_FORMAT_0   = 0,
    LTE_DCI_FORMAT_1   = 1,
    LTE_DCI_FORMAT_1A  = 2,
    LTE_DCI_FORMAT_1B  = 3,
    LTE_DCI_FORMAT_1C  = 4,
    LTE_DCI_FORMAT_1D  = 5,
    LTE_DCI_FORMAT_2   = 6,
    LTE_DCI_FORMAT_2A  = 7,
    LTE_DCI_FORMAT_3   = 8,
    LTE_DCI_FORMAT_3A  = 9,

    LTE_DCI_FORMAT_INVALID = 0xFF
} tLteDciFormat;

/* Resource allocation type */
typedef enum
{
    LTE_RES_ALLOC_TYPE_0    = 0,
    LTE_RES_ALLOC_TYPE_1    = 1,
    LTE_RES_ALLOC_TYPE_2    = 2,
    LTE_RES_ALLOC_TYPE_2_1C = 3
} tLteResAllocType;


static char *_bandwidth[] = {
    "1.4m",
    "3m",
    "5m",
    "10m",
    "15m",
    "20m"
};

static char *_dciFormat[] = {
    "0",
    "1",
    "1a",
    "1b",
    "1c",
    "1d",
    "2",
    "2a",
    "3",
    "3a"
};

/* Resource allocation mask for DCI format 1A */
//static uint32 _dciResAllocMask1A[6] = {
//    /*  6RB      15RB     25RB     50RB     75RB     100RB  */
//        0x001F,  0x007F,  0x01FF,  0x07FF,  0x0FFF,  0x1FFF
//};

/* Resource allocation bits */
static uint8 _dciResAllocBitSize[10][6] = {
    /* 6RB   15RB   25RB  50RB  75RB  100RB */
    {   5,     7,     9,   11,   12,    13  },  /* format 0  */
    {   6,     8,    13,   17,   19,    25  },  /* format 1  */
    {   5,     7,     9,   11,   12,    13  },  /* format 1A */
    {   5,     7,     9,   11,   12,    13  },  /* format 1B */
    {   3,     5,     7,    7,    8,     9  },  /* format 1C */
    {   5,     7,     9,   11,   12,    13  },  /* format 1D */
    {   6,     8,    13,   17,   19,    25  },  /* format 2  */
    {   6,     8,    13,   17,   19,    25  },  /* format 2A */
    {   0,     0,     0,    0,    0,     0  },  /* format 3  */
    {   0,     0,     0,    0,    0,     0  }   /* format 3A */
};



static int _convertBw2Enum(char *pBwStr)
{
    int bw = LTE_BW_INVALID;
    int i;

    for (i=0; i<6; i++)
    {
        if (0 == strcmp(pBwStr, _bandwidth[i]))
        {
            bw = (0x1 << i);
            break;
        }
    }

    //printf("%s => %d\n", pBwStr, bw);

    return bw;
}

static int _convertFormat2Enum(char *pFormatStr)
{
    int format = LTE_DCI_FORMAT_INVALID;
    int i;

    for (i=0; i<10; i++)
    {
        if (0 == strcmp(pFormatStr, _dciFormat[i]))
        {
            format = i;
            break;
        }
    }

    //printf("%s => %d\n", pFormatStr, format);

    return format;
}

static uint8 _convertBw2RbNum(tLteBw bw)
{
    uint8 rb = 0;

    switch ( bw )
    {
        case LTE_BW_1_4M:
            rb = 6;
            break;
        case LTE_BW_3M:
            rb = 15;
            break;
        case LTE_BW_5M:
            rb = 25;
            break;
        case LTE_BW_10M:
            rb = 50;
            break;
        case LTE_BW_15M:
            rb = 75;
            break;
        case LTE_BW_20M:
            rb = 100;
            break;
        case LTE_BW_INVALID:
        default:
            printf("invalid bandwidth(%d)\n", bw);
    }

    return rb;
}

static inline tLteRbIdx _convertBw2RbIndex(tLteBw bw)
{
    tLteRbIdx rbIndex = 0;

    switch ( bw )
    {
        case LTE_BW_1_4M:
            rbIndex = LTE_6_RB_IDX;
            break;
        case LTE_BW_3M:
            rbIndex = LTE_15_RB_IDX;
            break;
        case LTE_BW_5M:
            rbIndex = LTE_25_RB_IDX;
            break;
        case LTE_BW_10M:
            rbIndex = LTE_50_RB_IDX;
            break;
        case LTE_BW_15M:
            rbIndex = LTE_75_RB_IDX;
            break;
        case LTE_BW_20M:
            rbIndex = LTE_100_RB_IDX;
            break;
        case LTE_BW_INVALID:
        default:
            printf("invalid bandwidth(%d)\n", bw);
    }

    return rbIndex;
}

static uint8 _getResAllocBits(
    tLteResAllocType  type,
    tLteBw            bw
)
{
    uint8  rbIndex;
    uint8  dciFmt;

    rbIndex = _convertBw2RbIndex( bw );

    switch ( type )
    {
        case LTE_RES_ALLOC_TYPE_0:
        case LTE_RES_ALLOC_TYPE_1:
            dciFmt = LTE_DCI_FORMAT_1;
            break;
        case LTE_RES_ALLOC_TYPE_2_1C:
            dciFmt = LTE_DCI_FORMAT_1C;
            break;
        case LTE_RES_ALLOC_TYPE_2:
        default:
            dciFmt = LTE_DCI_FORMAT_1A;
            break;
    }
    return _dciResAllocBitSize[dciFmt][rbIndex];
}

static int32 _encodeRiv(
    uint8          rbStart,
    uint8          lcrbs,
    tLteDciFormat  dciFmt,
    tLteBw         bw
)
{
    int32  riv = -1;
    uint8  rb;

    rb = _convertBw2RbNum( bw );

    switch ( dciFmt )
    {
        case LTE_DCI_FORMAT_0:
        case LTE_DCI_FORMAT_1A:
        case LTE_DCI_FORMAT_1B:
        case LTE_DCI_FORMAT_1D:
            if ((lcrbs - 1) < FLOOR_DIV(rb, 2))
            {
                riv = ((rb * (lcrbs - 1)) + rbStart);
            }
            else
            {
                riv = ((rb * (rb - lcrbs + 1)) + (rb - 1 - rbStart));
            }
            break;

        case LTE_DCI_FORMAT_1C:
            printf("%s: not yet support for DCI format 1C\n\n", __func__);
            break;

        default:
            printf("%s: unknown DCI format(%d)\n\n", __func__, dciFmt);
    }

    return riv;
}

static void _decodeRiv(
    int32          riv,
    tLteDciFormat  dciFmt,
    tLteBw         bw,
    uint8         *pRbStart,
    uint8         *pLcrbs
)
{
    int  quotient,  remainder;
    int  rbStart1,  rbStart2;
    int  lCRBs1,    lCRBs2;
    int  resAlloc1, resAlloc2;
    uint8  rb;

    rb = _convertBw2RbNum( bw );

    switch ( dciFmt )
    {
        case LTE_DCI_FORMAT_0:
        case LTE_DCI_FORMAT_1A:
        case LTE_DCI_FORMAT_1B:
        case LTE_DCI_FORMAT_1D:
            quotient  = FLOOR_DIV(riv, rb);
            remainder = (riv % rb);

            rbStart1 = (remainder);
            rbStart2 = (rb - 1 - remainder);
            lCRBs1   = (quotient + 1);
            lCRBs2   = (rb - quotient + 1);

            resAlloc1 = (rbStart1 + lCRBs1 - 1);
            resAlloc2 = (rbStart2 + lCRBs2 - 1);

            if ((resAlloc1 < rb) && (resAlloc2 >= rb))
            {
                *pRbStart = rbStart1;
                *pLcrbs   = lCRBs1;
            }
            else if ((resAlloc1 >= rb) && (resAlloc2 < rb))
            {
                *pRbStart = rbStart2;
                *pLcrbs   = lCRBs2;
            }
            else
            {
                printf("un-identify resource allocation type 2 (RIV=%d)\n", riv);
                exit(0);
            }
            break;

        case LTE_DCI_FORMAT_1C:
            printf("%s: not yet support for DCI format 1C\n\n", __func__);
            *pRbStart = 0;
            *pLcrbs   = 0;
            break;

        default:
            printf("%s: unknown DCI format(%d)\n\n", __func__, dciFmt);
            *pRbStart = 0;
            *pLcrbs   = 0;
    }
}


void help(void)
{
    printf("Usage: res_alloc_riv [OPTION]...\n");
    printf("\n");
    printf("  -b bw       Bandwidth: 1.4m, 3m, 5m, 10m, 15m or 20m.\n");
    //printf("  -f format   DCI format: 0, 1, 1a, 1b, 1c, 1d, 2, 2a, 3 or 3a.\n");
    printf("  -f format   DCI format: 0, 1a, 1b, 1d.\n");
    printf("\n");
    printf("  -e          RIV encoding.\n");
    printf("  -s start    RB start: 0 ~ 99.\n");
    printf("  -l length   RB length: 1 ~ 100.\n");
    printf("\n");
    printf("  -d          RIV decoding.\n");
    printf("  -r riv      RIV raw data.\n");
    printf("\n");
    printf("  -h          Show the help message.\n");
    printf("\n");
}

/*
 * R10 36.213 Section 7.1.6.3 Resource allocation type 2
 *                    8.1.1 Uplink Resource allocation type 0
 */
int main(int argc, char *argv[])
{
    int     dciFmt  = LTE_DCI_FORMAT_INVALID;
    int     bw      = LTE_BW_INVALID;
    uint8   rbStart = 0;
    uint8   lcrbs   = 0;
    int32   riv     = 0;
    uint8   bits    = 0;
    int     eFlag   = 0;
    int     dFlag   = 0;
    int     ch;


    if (1 == argc)
    {
        help();
        return -1;
    }

    opterr = 0;
    while ((ch=getopt(argc, argv, "edb:f:s:l:r:h")) != -1)
    {
        switch ( ch )
        {
            case 'e':
                /* encode RIV */
                eFlag = 1;
                break;

            case 'd':
                /* decode RIV */
                dFlag = 1;
                break;

            case 'b':
                bw = _convertBw2Enum( optarg );
                if (LTE_BW_INVALID == bw)
                {
                    printf("ERR: wrong bandwidth '%s'\n", optarg);
                }
                break;

            case 'f':
                dciFmt = _convertFormat2Enum( optarg );
                if (LTE_DCI_FORMAT_INVALID == dciFmt)
                {
                    printf("ERR: wrong format '%s'\n", optarg);
                }
                break;

            case 's':
                rbStart = atoi( optarg );
                break;

            case 'l':
                lcrbs = atoi( optarg );
                break;

            case 'r':
                if (0 == strncmp("0x", optarg, 2))
                {
                    sscanf(optarg, "0x%x", &riv);
                }
                else
                {
                    riv = atoi( optarg );
                }
                break;

            case 'h':
            default:
                help();
                return 0;
        }
    }


    bits = _getResAllocBits(LTE_RES_ALLOC_TYPE_2, bw);

    if ( eFlag )
    {
        uint32 rivInDci = 0;

        riv = _encodeRiv(
                  rbStart,
                  lcrbs,
                  dciFmt,
                  bw
              );
        if (riv >= 0)
        {
            rivInDci = (riv << (32 - bits));

            printf("\n");
            printf("RB Assignment Bits = %d\n", bits);
            printf("RB Start           = %d\n", rbStart);
            printf("RB Number          = %d\n", lcrbs);
            printf("RIV                = %d (0x%04X)\n", riv, riv);
            switch ( dciFmt )
            {
                case LTE_DCI_FORMAT_0:
                case LTE_DCI_FORMAT_1A:
                    rivInDci >>= 2;
                    break;
                case LTE_DCI_FORMAT_1B:
                case LTE_DCI_FORMAT_1D:
                    rivInDci >>= 1;
                    break;
                default:
                    ;
            }
            printf("RIV in DCI         = %02X %02X\n",
                ((rivInDci >> 24) & 0xFF),
                ((rivInDci >> 16) & 0xFF)
            );
            printf("\n");
        }
    }
    else if ( dFlag )
    {
        if (riv >= 0)
        {
            _decodeRiv(
                riv,
                dciFmt,
                bw,
                &rbStart,
                &lcrbs
            );

            printf("\n");
            printf("RB Assignment Bits = %d\n", bits);
            printf("RIV                = %d (0x%04X)\n", riv, riv);
            printf("RB Start           = %d\n", rbStart);
            printf("RB Number          = %d\n", lcrbs);
            printf("\n");
        }
    }


    return 0;
}
