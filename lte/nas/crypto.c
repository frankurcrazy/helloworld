#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "crypto.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#define MAX_LOG_BUFFER  (255)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void crypto_shiftLeft(uint8 *pArray, unsigned int arrayLen, unsigned int bits)
{
    unsigned int i, j;
    unsigned int byteOffset = bits / 8;
    unsigned int bitOffset = bits % 8;

    uint8 temp;

    if ((arrayLen << 3) <= bits)
    {
        memset(pArray, 0, arrayLen);
        return;
    }

    for (i = 0, j = byteOffset; j < (arrayLen - 1); i++, j++)
    {
        temp  = (pArray[j]     << (bitOffset));
        temp |= (pArray[j + 1] >> (8 - bitOffset));
        pArray[i] = temp;
    }
    pArray[i] = pArray[j] << bitOffset;

    while (++i < arrayLen)
    {
        pArray[i] = 0;
    }
}

void crypto_xor(uint8 *pOut, const uint8 *pX1, const uint8 *pX2, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {
        pOut[i] = pX1[i] ^ pX2[i];
    }
}


uint8 _get_bits(uint8* pByte, uint32 bit, uint8 num)
{
    uint8 ret = pByte[BIT_ADDRESS(bit)];

    ret >>= (8 - BIT_OFFSET(bit) - num);
    ret  &= (0xFF >> (8 - num));
    return ret;
}

void _put_bits(uint8* pByte, uint32 bit, uint8 num, uint8 value)
{
    uint8 mask = 0xFF << (8 - num);
    mask = mask >> BIT_OFFSET(bit);

    pByte    += BIT_ADDRESS(bit);
    value   <<= (8 - BIT_OFFSET(bit) - num);
    pByte[0] &= ~mask;
    pByte[0] |= (value & mask);
}

uint32 crypto_getBits(void *pByte, uint32 bit, uint8 num)
{
    uint8* start = ((uint8*)pByte) + BIT_ADDRESS(bit);
    uint8* stop  = ((uint8*)pByte) + BIT_ADDRESS(bit+num);
    uint8  bitNum = num;
    uint32 tmp    = 0;
    uint32 ret    = 0;

    if ( num == 0 )
    {
        LOG_ERROR( "Get 0 bit!!\n" );
    }
    else if ( num > 32 )
    {
        LOG_ERROR( "Get > 32 bits!!\n" );
    }
    else if ((BIT_OFFSET(bit) + num) <= 8)
    {
        ret = _get_bits((uint8*)pByte, bit, num);
    }
    else if ((BIT_OFFSET(bit) + num) < 40)
    {
        bitNum -= (8 - BIT_OFFSET(bit));
        tmp  = _get_bits(start, BIT_OFFSET(bit), (8 - BIT_OFFSET(bit))) << bitNum;
        ret |= tmp;

        while (start++ < (stop - 1))
        {
            bitNum  -= 8;
            tmp      = *start << bitNum;
            ret     |= tmp;
        }

        bitNum -= BIT_OFFSET(bit+num);
        tmp  = _get_bits(stop, 0, BIT_OFFSET(bit+num)) << bitNum;
        ret |= tmp;
    }

    return ret;
}

void crypto_putBits(void *pByte, uint32 bit, uint8 num, uint32 value)
{
    uint8* start = ((uint8*)pByte) + BIT_ADDRESS(bit);
    uint8* stop  = ((uint8*)pByte) + BIT_ADDRESS(bit+num);
    uint8  bitNum = num;

    //if (value & (~(0xFFFFFFFF >> (32 - num))))
    //{
    //    LOG_WARN(
    //        "bit length of value (0x%08X) is greater than (%u), only put lower (%u) bit(s)\n",
    //        value,
    //        num,
    //        num
    //    );
    //}

    value = value & (0xFFFFFFFF >> (32 - num));

    if ( num == 0 )
    {
        LOG_ERROR( "Put 0 bit!!\n" );
    }
    else if ( num > 32 )
    {
        LOG_ERROR( "Put > 32 bits!!\n" );
    }
    else if ((BIT_OFFSET(bit) + num) <= 8)
    {
        _put_bits((uint8*)pByte, bit, num, (uint8)value);
    }
    else
    {
        bitNum -= (8 - BIT_OFFSET(bit));
        _put_bits(start, BIT_OFFSET(bit), (8 - BIT_OFFSET(bit)), value >> bitNum);

        while (start++ < (stop - 1))
        {
            bitNum -= 8;
            *start = (value >> bitNum) & 0xFF;
        }

        bitNum -= BIT_OFFSET(bit+num);
        _put_bits(stop, 0, BIT_OFFSET(bit+num), value & (0xFF >> (8 - BIT_OFFSET(bit+num))));
    }
}


void log_print(const char *prefix, const char *fmt, ...)
{
    char  msg[MAX_LOG_BUFFER+1];
    int   msgLen = 0;
    va_list args;


    /* append prefix to log message */
    msgLen = sprintf(msg, "%s ", prefix);

    va_start(args, fmt);

    /* log message copy */
    msgLen += vsnprintf((msg + msgLen),
                        (MAX_LOG_BUFFER - msgLen),
                        fmt,
                        args);

    va_end(args);

    printf("%s", msg);
}

void log_dump(const void *addr, unsigned int size)
{
    unsigned char *p = (unsigned char *)addr;
    unsigned int   i;

    if (p == NULL)
    {
        printf("NULL pointer\n");
        printf("\n");
        return;
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }

        printf(" %02X", p[i]);
    }
    printf("\n");
    printf(" size = %d\n", size);
    printf("\n");
}

