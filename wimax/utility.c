#include "utility.h"


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void mem_dump(char *name, const void *addr, unsigned int len)
{
    unsigned char *p = (unsigned char *)addr;
    unsigned int   i;

    if (addr == NULL)
    {
        printf("MEM-DUMP> NULL pointer\n");
        printf("\n");
        return;
    }

    printf("MEM-DUMP> %s\n", name);
    for(i=0; i<len; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf("%02X ", p[i]);
    }
    printf("\n");
    printf("(%d bytes)\n", len);
    printf("MEM-DUMP>\n");
    printf("\n");
}


void reverse_pmk(uint8 *input, uint8 *output)
{
    int i, j;

    for (i=0, j=19; i<20; i++, j--)
    {
        output[i] = input[j];
    }
}


int hmacGenerate( uint8 *pKey, uint8 *pData, uint32 dataSize, uint8 *pDigest)
{
    uint8 *pBuffer;
    uint8 ipad[64];
    uint8 opad[64];
    uint32 ii;

    if (pData == NULL)
    {
        printf("[ERROR] secCryptoApi_Error point is NULL\n");
        return -1;
    }

    if (dataSize > (2048 - 64))
    {
        printf("[ERROR] secCryptoApi_Error dataSize is too big\n");
        return -1;
    }

    pBuffer = malloc( 2048 );
    if (pBuffer == NULL)
    {
        printf("[ERROR] secCryptoApi_Error malloc Failture\n");
        return -1;
    }

    memset(ipad, 0x36, 64);
    memset( opad, 0x5c, 64 );
    for ( ii=0; ii<20; ii++ )
    {
        ipad[ii] ^= pKey[ii];
        opad[ii] ^= pKey[ii];
    }
    memcpy( pBuffer, ipad, 64);
    memcpy( pBuffer+64, pData, dataSize );

    SHA1(pBuffer, 64+dataSize, pDigest);

    memcpy( pBuffer, opad, 64);
    memcpy( pBuffer+64, pDigest, 20 );

    SHA1(pBuffer, 64+20, pDigest);

    free( pBuffer );

    return 0;
}



/* For CMAC Calculation */
unsigned char const_Rb[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};
unsigned char const_Zero[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}; 
 
/* Basic Functions */ 
void xor_128(unsigned char *a, unsigned char *b, unsigned char *out)
{
    int i;
    for (i=0;i<16; i++)
    {
        out[i] = a[i] ^ b[i];
    } 
}

void leftshift_onebit(unsigned char *input,unsigned char *output)
{
    int         i;
    unsigned char overflow = 0;

    for ( i=15; i>=0; i-- ) {
        output[i] = input[i] << 1;
        output[i] |= overflow;
        overflow = (input[i] & 0x80)?1:0;
    }
    return;
}

void generate_subkey(AES_KEY *cmacKey, unsigned char *K1, unsigned char *K2)
{
    unsigned char L[16];
    unsigned char Z[16];
    unsigned char tmp[16];
    int i;

    
    for ( i=0; i<16; i++ ) Z[i] = 0;

    //AES_128(pKey,Z,L);
    AES_encrypt(Z, L, cmacKey);

    if ( (L[0] & 0x80) == 0 ) { 
        /* If MSB(L) = 0, then K1 = L << 1 */
        leftshift_onebit(L,K1);
    } 
    else {    
        /* Else K1 = ( L << 1 ) (+) Rb */
        leftshift_onebit(L,tmp);
        xor_128(tmp,const_Rb,K1);
    }

    if ( (K1[0] & 0x80) == 0 ) {
        leftshift_onebit(K1,K2);
    } 
    else {
        leftshift_onebit(K1,tmp);
        xor_128(tmp,const_Rb,K2);
    }
    return;
}

void padding(unsigned char *lastb, unsigned char *pad, uint32 dataSize)
{
    int j;

    /* original last block */
    for ( j=0; j<16; j++ ) {
        if ( j < dataSize ) {
            pad[j] = lastb[j];
        } 
        else if ( j == dataSize ) {
            pad[j] = 0x80;
        } 
        else {
            pad[j] = 0x00;
        }
    }
}


int cmacGenerate(uint8 *pKey, uint8 *pData, uint32 dataSize, uint8 *pDigest)
{
    unsigned char  X[16],Y[16], M_last[16], padded[16];  //X,Y for temporary usage
    unsigned char  K1[16], K2[16];
    int  n, i, flag;  // flag => if the message is a possitive multiple of the block size
    
    AES_KEY cmacKey;

    AES_set_encrypt_key(pKey, 128, &cmacKey);

    generate_subkey(&cmacKey, K1, K2);
  
    n = (dataSize+15) / 16;       /* n is number of rounds */

    if (n == 0)
    {
        n = 1;
        flag = 0;
    }
    else
    {
        if ((dataSize%16) == 0)
        { 
            /* last block is a complete block */
            flag = 1;
        } 
        else
        { 
            /* last block is not complete block */
            flag = 0;
        }
    }

    if ( flag )
    { 
        /* last block is complete block */
        xor_128(&pData[16*(n-1)],K1,M_last);
    } 
    else
    {
        padding(&pData[16*(n-1)],padded,dataSize%16);
        xor_128(padded,K2,M_last);
    }

    for (i=0; i<16; i++) X[i] = 0;
    for (i=0; i<n-1; i++)
    {
        // AES-CBC 
        xor_128(X,&pData[16*i],Y); /* Y := Mi (+) X  */
        //AES_128(pKey,Y,X);      /* X := AES-128(KEY, Y); */  
        //AES_128(key, input, output)
        AES_encrypt(Y, X, &cmacKey);
    }

    xor_128(X,M_last,Y);   
    //AES_128(pKey,Y,X);    //  Generate last chiper block ==> CMAC
    AES_encrypt(Y, X, &cmacKey);
    
    for (i=0; i<16; i++)
    {
        pDigest[i] = X[i];
    }

    return 0;
}

void aesEcbEncrypt(uint8 *pKey, uint16 keyLen, uint8 *pOut, uint8 *pIn)
{
    AES_KEY aesKey;

    AES_set_encrypt_key(pKey, keyLen*8, &aesKey);
    AES_encrypt(pIn, pOut, &aesKey);
}


void dot16kdf(
    uint8  *key,
    uint8  *astring,
    uint32  astring_len, 
    uint8  *output,
    uint16  output_len,
    uint8   mac_mode
)
{
    unsigned int   i, j;
    unsigned char  kin[20];
    unsigned char  result[1024];
    unsigned int   result_len = 0;
    unsigned char *p = result;
    unsigned char  plaintext[200];
    unsigned int   idx;
    unsigned int   keylength_bits = output_len*8;
    int block_size = 0;
    
    if (mac_mode == DOT16KDF_HMAC)
    {
        //SHA-1 base
        block_size = 20;
    }
    else
    {
        //AES base
        block_size = 16;
    }
    TRUNCATE_R(key, 20, kin, block_size);

    for (i=0; i<=((keylength_bits-1)/(block_size*8)); i++)
    {
        idx = 0;

        //[1] i
        plaintext[idx++] = (unsigned char)((i >> 24) & 0xff);  /* Put MSB of i first */
        plaintext[idx++] = (unsigned char)((i >> 16) & 0xff);
        plaintext[idx++] = (unsigned char)((i >> 8) & 0xff);
        plaintext[idx++] = (unsigned char)( i  & 0xff);        /* Put LSB of i last */

        //[2] astring
        for (j=0; j < astring_len; j++)
        {
            plaintext[idx++] = astring[j];
        }

        //[3] keylength in bits
        plaintext[idx++] = ((keylength_bits >> 24) & 0xff);
        plaintext[idx++] = ((keylength_bits >> 16) & 0xff);
        plaintext[idx++] = ((keylength_bits >> 8) & 0xff);
        plaintext[idx++] = (keylength_bits  & 0xff);
        
        if (mac_mode == DOT16KDF_HMAC)
        {
            //SHA-1 base

            for (j=0; j<block_size; j++)
            {
                plaintext[idx++] = kin[j];
            }
            
            // unsigned char *SHA1(
            //                    const unsigned char *d,
            //                    size_t               n,
            //                    unsigned char       *md
            //                );
            SHA1(plaintext, 4 + astring_len + 4 + block_size, p);
        }
        else
        {
            //AES base

            // int cmacGenerate(
            //         uint8 *pKey,
            //         uint8 *pData,
            //         uint32 dataSize,
            //         uint8 *pDigest
            //     );
            cmacGenerate(kin, plaintext, 4 + astring_len + 4, p);
        }
        
        p += block_size;
        result_len += block_size;

        if (result_len > 200)
        {
            printf("ERROR: no space to run dot16kdf()\n");
            break;
        }
    }

    TRUNCATE_R(result, result_len, output, output_len);
}

