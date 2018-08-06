#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include "kdf.h"


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

void padding ( unsigned char *lastb, unsigned char *pad, uint32 dataSize )
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


void cmacGenerate( uint8 *pKey, uint8 *pData, uint32 dataSize, uint8 *pDigest)
{
    unsigned char  X[16],Y[16], M_last[16], padded[16];  //X,Y for temporary usage
    unsigned char  K1[16], K2[16];
    int  n, i, flag;  // flag => if the message is a possitive multiple of the block size
    
    AES_KEY cmacKey;
    

    AES_set_encrypt_key(pKey, 128, &cmacKey);


    generate_subkey(&cmacKey, K1, K2);
    //mem_dump("K1", K1, 16);
    //mem_dump("K2", K2, 16);
  
    n = (dataSize+15) / 16;  /* n is number of rounds */

    if ( n == 0 ) {
        n = 1;
        flag = 0;
    }
    else {
        if ( (dataSize%16) == 0 ) { 
            /* last block is a complete block */
            flag = 1;
        } 
        else { 
            /* last block is not complete block */
            flag = 0;
        }
    }
    if ( flag ) { 
        /* last block is complete block */
        xor_128(&pData[16*(n-1)],K1,M_last);
    } 
    else {
        padding(&pData[16*(n-1)],padded,dataSize%16);
        xor_128(padded,K2,M_last);
    }

    for ( i=0; i<16; i++ ) X[i] = 0;
    for ( i=0; i<n-1; i++ ) {  // AES-CBC 
        xor_128(X,&pData[16*i],Y); /* Y := Mi (+) X  */
        //AES_128(pKey,Y,X);      /* X := AES-128(KEY, Y); */  
        //AES_128(key, input, output)
        AES_encrypt(Y, X, &cmacKey);
    }

    xor_128(X,M_last,Y);   
    //AES_128(pKey,Y,X);    //  Generate last chiper block ==> CMAC
    AES_encrypt(Y, X, &cmacKey);

    for ( i=0; i<16; i++ ) {
        pDigest[i] = X[i];
    }
}


int main(int argc, char *argv[])
{
    //----------------------------------------------------------------
    #if (0) /* golden sample 33.401-C.2.2 */
    uint8   key[16] = {
                0xD3, 0xC5, 0xD5, 0x92, 0x32, 0x7F, 0xB1, 0x1C,
                0x40, 0x35, 0xC6, 0x68, 0x0A, 0xF8, 0xC6, 0xD1
            };
    uint32  count = 0x398A59B4;
    uint8   bearer = 0x1A;
    uint8   direction = 1;

    uint8   data[] = {
                0x48, 0x45, 0x83, 0xD5, 0xAF, 0xE0, 0x82, 0xAE
            };
    uint16  data_len = sizeof(data);
    #else
    uint8   key[16] = {
                0xA6, 0x89, 0x08, 0x91, 0x65, 0xC4, 0x68, 0xFF,
                0xB8, 0x31, 0x25, 0x86, 0xF3, 0x8C, 0x36, 0xE6
            };
    uint32  count = 1;
    uint8   bearer = 0;
    uint8   direction = 1; /* downlink */

    uint8   data[] = {
                0x00,
                /* NAS attach accept */
                0x07, 0x42, 0x01, 0x19, 0x06, 0x00, 0x64, 0xF6,
                0x66, 0x00, 0x01, 0x00, 0x14, 0x52, 0x00, 0xC1,
                0x01, 0x06, 0x08, 0x4D, 0x4D, 0x45, 0x5F, 0x45,
                0x4D, 0x55, 0x00, 0x05, 0x01, 0x0A, 0x0A, 0x0A,
                0x02, 0x50, 0x0B, 0xF6, 0x64, 0xF6, 0x66, 0x04,
                0xD2, 0x38, 0x00, 0x00, 0x00, 0x00
            };
    uint16  data_len = sizeof(data);
    #endif
    //----------------------------------------------------------------
    uint8   msg[256];
    uint16  msg_len;
    uint8   digest[16];


    UINT32_TO_BYTE_ARRAY(count, msg);
    msg[4]  = ((bearer    & 0x1F) << 3);
    msg[4] |= ((direction & 0x01) << 2);
    msg[5]  = 0x0;
    msg[6]  = 0x0;
    msg[7]  = 0x0;
    memcpy(msg+8, data, data_len);
    msg_len = data_len + 8;

    cmacGenerate(key, msg, msg_len, digest);
    printf("cmac generate ...\n\n");
    mem_dump("key", key, 16);
    mem_dump("msg", msg, msg_len);

    //mem_dump("CMAC", digest, 16);
    printf(
        "MAC: %02X %02X %02X %02X\n",
        digest[0],
        digest[1],
        digest[2],
        digest[3]
    );


    return 0;
}

