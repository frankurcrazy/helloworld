#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gsm7bit.h"


#define DEBUG  (0)


/* ASCII to GSM 7-bit table */
static uint8 _gsm7bit[128] = {
    /* 00 ~ 0F */
    0x40, 0x01, 0x24, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    /* 10 ~ 1F */
    0x10, 0x5F, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    /* 20 ~ 2F */
    0x20, 0x21, 0x22, 0x23, 0x02, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    /* 30 ~ 3F */
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    /* 40 ~ 4F */
    0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    /* 50 ~ 5F */
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    /* 60 ~ 6F */
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    /* 70 ~ 7F */
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F
};

/* ASCII to GSM 7-bit extension table:
 *   ^, {, }, \, [, ~, ], | 
 */
static uint8 _gsm7bitExt[128] = {
    /* 00 ~ 0F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 10 ~ 1F */
    0x00, 0x00, 0x00, 0x00, 0x5E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 20 ~ 2F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7B, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C,
    /* 30 ~ 3F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x5B, 0x7E, 0x5D, 0x00,
    /* 40 ~ 4F */
    0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 50 ~ 5F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x2F, 0x3E, 0x14, 0x00,
    /* 60 ~ 6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 70 ~ 7F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x28, 0x40, 0x29, 0x3D, 0x00
};


uint16 asciiToGsm7bit(uint8 *pIn, uint16 inLen, uint8 *pOut, uint16 outLenMax)
{
    uint16  outLen = 0;
    uint8   ch;
    int     i;

    /* 23.038 section 6.2.1 */
    for (i=0; i<inLen; i++)
    {
        ch = pIn[i];
        if (('^'  == ch) ||
            ('{'  == ch) ||
            ('}'  == ch) ||
            ('\\' == ch) ||
            ('['  == ch) ||
            ('~'  == ch) ||
            (']'  == ch) ||
            ('|'  == ch)
           )
        {
            /* escape to the extension table */
            pOut[outLen++] = 0x1B;
            pOut[outLen++] = _gsm7bitExt[ ch ];
        }
        else
        {
            pOut[outLen++] = _gsm7bit[ ch ];
        }

        if (outLen >= outLenMax)
        {
            printf(
                "%s: output buffer size is too small (%d <-> %d)\n",
                __func__,
                outLenMax,
                outLen
            );
            outLen = 0;
            break;
        }
    }

    return outLen;
}

uint16 gsm7bitToAscii(uint8 *pIn, uint16 inLen, uint8 *pOut, uint16 outLenMax)
{
    uint16  outLen = 0;
    uint8   ch;
    int     i;

    /* 23.038 section 6.2.1 */
    for (i=0; i<inLen; i++)
    {
        ch = pIn[i];
        if (0x1B == ch)
        {
            /* escape to the extension table */
            ch = pIn[ ++i ];
            pOut[outLen++] = _gsm7bitExt[ ch ];
        }
        else
        {
            pOut[outLen++] = _gsm7bit[ ch ];
        }

        if (outLen >= outLenMax)
        {
            printf(
                "%s: output buffer size is too small (%d <-> %d)\n",
                __func__,
                outLenMax,
                outLen
            );
            outLen = 0;
            break;
        }
    }

    return outLen;
}


uint16 sms_encodeGsm7bit(
    uint8  *pOut,
    uint16  outSizeMax,
    uint8  *pIn,
    uint16 *pInSize
)
{
    uint8   mask[] = {
                0x80,  // 0
                0xC0,  // 1
                0xE0,  // 2
                0xF0,  // 3
                0xF8,  // 4
                0xFC,  // 5
                0xFE   // 6
            };
    uint16  inSize = (*pInSize);
    uint16  outSize = 0;
    uint8   byte;
    uint8  *pBuf;
    int m;
    int j;
    int k;

    /* convert ASCII to GSM 7-bit character */
    pBuf = malloc(inSize * 2);
    if (NULL == pBuf)
    {
        printf("%s: out of memory\n", __func__);
        return 0;
    }
    inSize = asciiToGsm7bit(pIn, inSize, pBuf, (inSize * 2));
    pIn = pBuf;
    (*pInSize) = inSize;

    outSize = CEIL_DIV((inSize * 7), 8);
    if (outSize > outSizeMax)
    {
        printf("GSM 7-bit alphabet length(%u) is too large\n", inSize);
        free( pBuf );
        return 0;
    }

    /*
    *         0 1 2 3 4 5 6 0  1 2  3  4  5  6  0  1   2  3  4  5  6  0  1  2   3
    *  in[k]  0 1 2 3 4 5 6,7  8 9 10 11 12 13 14,15  16 17 18 19 20 21 22,23  24
    *
    *  out[j] 0 1 2 3 4 5 6    7 8  9 10 11 12 13     14 15 16 17 18 19 20     21
    *  mask   0 1 2 3 4 5 6    0 1  2  3  4  5  6      0  1  2  3  4  5  6      0
    */
    for (j=0, k=0; j<outSize; j++)
    {
        #if (DEBUG)
        printf("> pIn[%d] = %02X\n", k, pIn[k]);
        #endif
        m = (j % 7);
        if (k == (inSize - 1))
        {
            byte = (((pIn[k] & 0x7F) >> m) |
                    (pOut[j] & mask[m]));
        }
        else
        {
            byte = (((pIn[k] & 0x7F) >> m) |
                    ((pIn[k + 1] << (7 - m)) & mask[m]));
        }
        pOut[j] = byte;

        if (m == 6) k += 2;
        else        k ++;
    }

    free( pBuf );

    return outSize;
}

uint16 sms_decodeGsm7bit(
    uint8  *pIn,
    uint16  inSize,
    uint8  *pOut,
    uint16  outSizeMax
)
{
    uint8   maskH[] = {
                0x7F,  // 0
                0x3F,  // 1
                0x1F,  // 2
                0x0F,  // 3
                0x07,  // 4
                0x03,  // 5
                0x01   // 6
            };
    uint8   maskL[] = {
                0x80,  // 0
                0xC0,  // 1
                0xE0,  // 2
                0xF0,  // 3
                0xF8,  // 4
                0xFC,  // 5
                0xFE   // 6
            };
    uint16  outSize = 0;
    uint8   byteH;
    uint8   byteL;
    uint8   temp;
    uint8  *pBuf;
    int     m;
    int     j;
    int     k;

    outSize = FLOOR_DIV((inSize * 8), 7);
    if ((outSize + 1) > outSizeMax)
    {
        printf("GSM 7-bit alphabet length(%u) is too large\n", inSize);
        pOut[0] = 0x00;
        return 0;
    }

    pBuf = malloc( outSize );
    if (NULL == pBuf)
    {
        printf("%s: out of memory\n", __func__);
        pOut[0] = 0x00;
        return 0;
    }

    /*
    *      k  0 1 2 3 4 5 6 7  8 9 10 11 12 13 14 15  16 17 18 19 20 21 22 23  24
    *  byteH  7 6 5 4 3 2 1 7  6 5  4  3  2  1  7  6   5  4  3  2  1  7  6  5   4
    *  byteL  0 1 2 3 4 5 6 0  1 2  3  4  5  6  0  1   2  3  4  5  6  0  1  2   3
    *
    *   m(h)  0 1 2 3 4 5 6 0  1 2  3  4  5  6  0  1   2  3  4  5  6  0  1  2   3
    *   m(l)  7 6 5 4 3 2 1 7  6 5  4  3  2  1  7  6   5  4  3  2  1  7  6  5   4
    */
    temp = 0x00;
    for (j=0, k=0; k<inSize; k++)
    {
        m = (k % 7);
        if (m == 6)
        {
            byteH = ((pIn[k] & maskH[m]) << m);
            byteL = temp;
            temp  = ((pIn[k] & maskL[m]) >> (7 - m));

            pBuf[j] = (byteH | byteL);
            #if (DEBUG)
            printf("> pBuf[%d] = %02X\n", j, pBuf[j]);
            #endif
            j++;

            pBuf[j] = temp;
            #if (DEBUG)
            printf("> pBuf[%d] = %02X\n", j, pBuf[j]);
            #endif
            j++;

            temp = 0x00;
        }
        else
        {
            byteH = ((pIn[k] & maskH[m]) << m);
            byteL = temp;
            temp  = ((pIn[k] & maskL[m]) >> (7 - m));

            pBuf[j] = (byteH | byteL);
            #if (DEBUG)
            printf("> pBuf[%d] = %02X\n", j, pBuf[j]);
            #endif
            j++;
        }
    }

    outSize = gsm7bitToAscii(pBuf, inSize, pOut, outSizeMax);
    pOut[outSize] = 0x00;

    free( pBuf );

    return outSize;
}


void mem_dump(void *addr, int size)
{
    uint8 *p = (uint8 *)addr;
    int i;

    if (addr == NULL) return;

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i%16) == 0)) printf("\n");
        printf("%02X ", p[i]);
    }
    printf("\n");
    printf("(%d bytes)\n", size);
    printf("\n");
}

