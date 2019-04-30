#ifndef _INTEL_HEX_H_
#define _INTEL_HEX_H_

#include <stdio.h>

#define uint8   unsigned char
#define uint16  unsigned short
#define uint32  unsigned int

#define BYTE_ARRAY_TO_UINT16(p, n) \
    do { \
        n  = (*((uint8*)p  ) <<  8); \
        n |= (*((uint8*)p+1)      ); \
    } while(0)

#define UINT16_TO_BYTE_ARRAY(n, p) \
    do { \
        *((uint8*)p  ) = ((n >>  8) & 0xFF); \
        *((uint8*)p+1) = ((n      ) & 0xFF); \
    } while (0)

#define BYTE_ARRAY_TO_UINT32(p, n) \
    do { \
        n  = (*((uint8*)p  ) << 24); \
        n |= (*((uint8*)p+1) << 16); \
        n |= (*((uint8*)p+2) <<  8); \
        n |= (*((uint8*)p+3)      ); \
    } while (0)

#define UINT32_TO_BYTE_ARRAY(n, p) \
    do { \
        *((uint8*)p  ) = ((n >> 24) & 0xFF); \
        *((uint8*)p+1) = ((n >> 16) & 0xFF); \
        *((uint8*)p+2) = ((n >>  8) & 0xFF); \
        *((uint8*)p+3) = ((n      ) & 0xFF); \
    } while (0)

#endif
