#ifndef _CRYPTO_H_
#define _CRYPTO_H_


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* Unit: Bytes */
#define CRYPTO_AES_128_KEY_SIZE (16)
#define CRYPTO_AES_192_KEY_SIZE (24)
#define CRYPTO_AES_256_KEY_SIZE (32)

#define CRYPTO_AES_MIN_KEY_SIZE CRYPTO_AES_128_KEY_SIZE
#define CRYPTO_AES_MAX_KEY_SIZE CRYPTO_AES_256_KEY_SIZE

#define CRYPTO_AES_BLOCK_SIZE   (16)

/* Unit: Bytes */
#define CRYPTO_EEA_KEY_SIZE         (16)
#define CRYPTO_EIA_KEY_SIZE         (16)
#define CRYPTO_KDF_KEY_SIZE         (32)

#define CRYPTO_KDF_CK_SIZE          (16)
#define CRYPTO_KDF_IK_SIZE          (16)
#define CRYPTO_KDF_KENB_SIZE        (32)
#define CRYPTO_KDF_KASME_SIZE       (32)
#define CRYPTO_KDF_SYNC_SIZE        (32)
#define CRYPTO_KDF_NH_SIZE          (32)
#define CRYPTO_KDF_SQN_SIZE         ( 6)
#define CRYPTO_KDF_AK_SIZE          ( 6)
#define CRYPTO_KDF_KEY_SIZE         (32)

#define CRYPTO_KDF_FC_KASME         (0x10)
#define CRYPTO_KDF_FC_KENB          (0x11)
#define CRYPTO_KDF_FC_NH            (0x12)
#define CRYPTO_KDF_FC_KENB_STAR     (0x13)
#define CRYPTO_KDF_FC_ALG           (0x15)
#define CRYPTO_KDF_FC_CKIK          (0x16)
#define CRYPTO_KDF_FC_NAS_TOKEN     (0x17)
#define CRYPTO_KDF_FC_KASME_HO      (0x18)
#define CRYPTO_KDF_FC_KASME_IDLE    (0x19)
#define CRYPTO_KDF_FC_CKIK_SRVCC    (0x1a)

#define CRYPTO_DIR_UPLINK   (0)
#define CRYPTO_DIR_DOWNLINK (1)

#define CRTPTO_MACI_SIZE            (4)


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


#define BYTE_ARRAY_GET_BITS(p, b, n)     crypto_getBits(p, b, n)
#define BYTE_ARRAY_PUT_BITS(p, b, n, d)  crypto_putBits(p, b, n, d)

#define BIT_ADDRESS(b)  ((b) >> 3)
#define BIT_OFFSET(b)   ((b) %  8)

#define ENABLE_DEBUG  (0)

#if 0  /* disable log function */
#define LOG_INFO( a... )
#define LOG_WARN( a... )
#define LOG_ERROR( a... )
#define LOG_DEBUG( a... )
#define LOG_DUMP(var, len)
#else
#define LOG_INFO( a... )    log_print("[INFO]", ##a)
#define LOG_WARN( a... )    log_print("[WARN]", ##a)
#define LOG_ERROR( a... )   log_print("[ERRO]", ##a)
#define LOG_DEBUG( a... )   log_print("[DEBU]", ##a)
#define LOG_DUMP(var, len)  log_dump(var, len)
#endif


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void crypto_shiftLeft(uint8 *pArray, unsigned int arrayLen, unsigned int bits);
void crypto_xor(uint8 *pOut, const uint8 *pX1, const uint8 *pX2, unsigned int len);

uint32 crypto_getBits(void *pByte, uint32 bit, uint8 num);
void crypto_putBits(void *pByte, uint32 bit, uint8 num, uint32 value);

void log_print(const char *prefix, const char *fmt, ...);
void log_dump(const void *addr, unsigned int size);


#endif //_CRYPTO_H_
