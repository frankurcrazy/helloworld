#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/des.h>
#include <openssl/aes.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/*
*   NULL value
*/
#undef  NULL
#define NULL ((void*)0)


#define TRUNCATE_L(src, src_len, dst, dst_len)  memcpy(dst, src, dst_len)
#define TRUNCATE_R(src, src_len, dst, dst_len)  memcpy(dst, src+(src_len-dst_len), dst_len)
#define DOT16KDF_HMAC           0
#define DOT16KDF_CMAC           1


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/*
*   integers
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

void mem_dump(char *name, const void *addr, unsigned int len);

void reverse_pmk(uint8 *input, uint8 *output);

int hmacGenerate( uint8 *pKey, uint8 *pData, uint32 dataSize, uint8 *pDigest);
int cmacGenerate( uint8 *pKey, uint8 *pData, uint32 dataSize, uint8 *pDigest);

void aesEcbEncrypt(uint8 *pKey, uint16 keyLen, uint8 *pOut, uint8 *pIn);

void dot16kdf(
    uint8  *key,
    uint8  *astring,
    uint32  astringlen,
    uint8  *output,
    uint16  outlen,
    uint8   macMode
);


#endif  // _UTILITY_H_
