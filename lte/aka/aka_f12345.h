#ifndef _AKA_F12345_H_
#define _AKA_F12345_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

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


#define BOOL_FALSE  (0)
#define BOOL_TRUE   (1)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/*  integers  */
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;

typedef unsigned char   bool;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void xor(uint8 *c, uint8 *a, uint8 *b, int size);
void dump(char *name, const void *addr, int len);

void compute_opc( uint8 k[16], uint8 op[16], uint8 op_c[16] );

void milenage_f1(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 sqn[6],
    uint8 amf[2], 
    uint8 mac_a[8]
);

void milenage_f2345(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 res[8],
    uint8 ck[16],
    uint8 ik[16],
    uint8 ak[6]
);

void milenage_f1star(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 sqn[6],
    uint8 amf[2], 
    uint8 mac_s[8]
);

void milenage_f5star(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 ak[6]
);


void test_f1 (
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 sqn[6],
    uint8 amf[2], 
    uint8 mac_a[8]
);

void test_f2345(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 res[16],
    uint8 ck[16],
    uint8 ik[16],
    uint8 ak[6]
);

void test_f1star(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 sqn[6],
    uint8 amf[2], 
    uint8 mac_s[8]
);

void test_f5star(
    uint8 k[16],
    uint8 op_c[16],
    uint8 rand[16],
    uint8 ak[6]
);


#endif /* _AKA_F12345_H_ */
