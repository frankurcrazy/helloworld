#ifndef __UTILITY_H__
#define __UTILITY_H__


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

#if (1)
# define EC_INFO( fmt, a... )      printf( "[EC-I] " fmt, ##a )
# define EC_WARN( fmt, a... )      printf( "[EC-W] " fmt, ##a )
# define EC_ERROR( fmt, a... )     printf( "[EC-E] " fmt, ##a )
# define EC_DEBUG( fmt, a... )     printf( "[EC-D] " fmt, ##a )
# define EC_DUMP( str ,var, len )  hex_dump( str, var, len )
# define EC_TRACE()                printf( "[EC-T] %d\n", __LINE__ )
#else
# define EC_INFO( fmt, a... )   
# define EC_WARN( fmt, a... )   
# define EC_ERROR( fmt, a... )  
# define EC_DEBUG( fmt, a... )
# define EC_DUMP( str ,var, len ) 
# define EC_TRACE()
#endif


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


#define HEX_STRING_TO_BYTE_ARRAY(in, inLen, out, outLen) \
    hex_string_to_byte_array(in, inLen, out, outLen)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/*
*  integers
*/
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void mem_dump(void *pAddr, int len);

void hex_dump(char *pName, unsigned char *pByte, int len);

int hex_string_to_byte_array(
    char   *pString,
    uint16  stringLen,
    uint8  *pData,
    uint16  dataLen
);


#endif
