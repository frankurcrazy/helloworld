#include "utility.h"

#define TEK_64_SIZE  8

int computeEcbBlock( const uint8 *ecbBuf, uint8 *xorBuf, DES_key_schedule *sch)
{
    uint8  gpEcbBlock_src[16];
    uint8 *gpEcbBlock_dst = gpEcbBlock_src+8;
    uint32 i;

    memcpy( gpEcbBlock_src, ecbBuf, 8 );

    /* Encrypt the original IV */
    DES_ecb_encrypt(
        (const_DES_cblock *)gpEcbBlock_src,
        (DES_cblock *)gpEcbBlock_dst,
        sch,
        DES_ENCRYPT
    );


    /* Perform the xor on last bytes. */
    /* Offset of the first byte outside the boundary of eigth. */
    for ( i=0; i<8; i++ )
    {
        xorBuf[i] = gpEcbBlock_dst[i] ^ xorBuf[i];
    }

    return 0;
}

void computeIvFrameXor( uint8 *ivFrame, uint8 *iv, uint32 frame)
{
    unsigned char n = 0;
    /**
                    0   1   2   3   4   5   6   7
                  +---+---+---+---+---+---+---+---+
     TEK IV       | x | x | x | x | x | x | x | x |
                  +---+---+---+---+---+---+---+---+
                                 xor
                  +-------------------+---+---+---+
     Frame Number |                   | x | x | x |
                  +-------------------+---+---+---+
                                 |
                                 v
                  +---+---+---+---+---+---+---+---+
     CBC IV       | x | x | x | x | x | x | x | x |
                  +---+---+---+---+---+---+---+---+
    */
    memcpy(ivFrame, iv, 8);

    /* frame number is 24-bit */
    n = (frame >> 16) & 0xFF;
    ivFrame[5] = iv[5] ^ n;
    n = (frame >>  8) & 0xFF;
    ivFrame[6] = iv[6] ^ n;
    n = (frame      ) & 0xFF;
    ivFrame[7] = iv[7] ^ n;
}


int _DesCbcDecrypt (uint8 *pTek, uint8 *pIv, uint32 frameNo, uint8 *pData, uint16 dataSize)
{
    int status = 0;
    uint8 ivFrame[TEK_64_SIZE];  /* Store the result of the (iv XOR frame). */
    uint8 ecbBuffer[TEK_64_SIZE];
    uint32 ecbLen;
    uint32 ecbOff = 0;

    DES_key_schedule transportCBC;

    /* Source buffer cannot be null. */
    if ( pData == NULL )
    {
        printf("[ERROR] secCryptoApi_Error point is NULL\n");
        return -1;
    }


    DES_set_key((DES_cblock *)pTek, &transportCBC);

    /* Length less than 8. */
    if ( dataSize < TEK_64_SIZE )
    {
        /* Copy incoming data to ecbBuffer. */
        //mem_dump("C1", pData, dataSize);
        memcpy( ecbBuffer, pData, dataSize);

        /* Compute the value of the bytes using ECB with IV and xor. */
        status = computeEcbBlock(pIv, ecbBuffer, &transportCBC);

        /* Take back last byte(s). */
        memcpy( pData, ecbBuffer, dataSize);
        //mem_dump("P1", pData, dataSize);
    }
    else
    {
        /* iv XOR frame. The result is in ivFrame. */
        computeIvFrameXor( ivFrame, pIv, frameNo );

        /* len must be a multiple of the 8, otherwise the DES-ECB/XOR algo must be used. */
        ecbLen = dataSize % TEK_64_SIZE;
        /* If they have remainder, perform DES-ECB algo. */
        if ( ecbLen )
        {
            /* The decryption must be done in two steps. */
            ecbOff = dataSize - ecbLen;
            memcpy(ecbBuffer, pData+ecbOff, ecbLen);

            /* Compute the value of the remainder bytes using ECB and xor. */
            /* Src data is offset to get the last block of eigth bytes. */
            status = computeEcbBlock( pData+ecbOff-TEK_64_SIZE, ecbBuffer, &transportCBC );
        }
        /* Send data with len rounded to a multiple of eigth. */
        /* Send data and wait */
        /* Put the packet to the device. */
        //mem_dump("C0", pData, dataSize-ecbLen);
        DES_cbc_encrypt(
            pData,
            pData,
            dataSize-ecbLen,
            &transportCBC,
            (DES_cblock *)ivFrame,
            DES_DECRYPT
        );
        //mem_dump("P0", pData, dataSize-ecbLen);
        /* Did packet process ok? */

        /* modeECB, If the number of byte is not a multiple of eigth. */
        /* Use data compute in ECB mode earlier and add them to the caller data. */
        /* If no remaining byte means ecbLen = 0. */
        /* no effect. */
        if ( ecbLen )
        {
            //mem_dump("Cn", pData+ecbOff, ecbLen);
            memcpy( pData+ecbOff, ecbBuffer, ecbLen);
            //mem_dump("Pn", pData+ecbOff, ecbLen);
        }
    }

    return status;
}

int _DesCbcEncrypt (uint8 *pTek, uint8 *pIv, uint32 frameNo, uint8 *pData, uint16 dataSize)
{
    int status = 0;
    uint8 ivFrame[TEK_64_SIZE];  /* Store the result of the 'iv XOR frame'. */
    uint8 ecbBuffer[TEK_64_SIZE];
    uint32 ecbLen;
    uint32 ecbOff = 0;

    DES_key_schedule transportCBC;

    /* Source buffer cannot be null. */
    if ( pData == NULL )
    {
        printf("[ERROR] secCryptoApi_Error point is NULL\n");
        return -1;
    }

    DES_set_key((DES_cblock *)pTek, &transportCBC);

    /* Length less than 8. */
    if ( dataSize < TEK_64_SIZE )
    {
        /* Copy incoming data to ecbBuffer. */
        memcpy( ecbBuffer, pData, dataSize );

        /* Compute the value of the bytes using ECB with IV and xor. */
        status = computeEcbBlock(pIv, ecbBuffer, &transportCBC);

        /* Take back last byte(s). */
        memcpy( pData, ecbBuffer, dataSize);
    }
    else
    {
        /* iv XOR frame. The result is in ivFrame. */
        computeIvFrameXor( ivFrame, pIv, frameNo );
        //CipherCtxUpdateIv ( pChan->chanEncCBC, ivFrame );

        /* len must be a multiple of 8, otherwise the DES-ECB/XOR algo must be used. */
        ecbLen = dataSize % TEK_64_SIZE;

        /* Compute the remainder bytes with DES-ECB/XOR algo. */
        if ( ecbLen )
        {
            /* The encryption must be done in two steps. */
            /* Save data for later used. */
            ecbOff = dataSize - ecbLen;
            memcpy(ecbBuffer, pData+ecbOff, ecbLen);
            /* The second step is done after the encryption of the packet with DES-CBC. */
        }

        /* Send data with len rounded to a multiple of eigth. */
        /* Send data and wait */
        /* Put the packet to the device. */
        DES_cbc_encrypt(
            pData,
            pData,
            dataSize-ecbLen,
            &transportCBC,
            (DES_cblock *)ivFrame,
            DES_ENCRYPT
        );

        /* modeECB, If the number of byte is not a multiple of eigth. */
        /* Use ECB mode to compute the remaining bytes. */
        if ( ecbLen )
        {
            /* Compute the value of the remainder bytes using ECB and xor. */
            /* Src data is offset to get the last block of eigth bytes. */
            status = computeEcbBlock( pData+ecbOff-TEK_64_SIZE, ecbBuffer, &transportCBC );

            /* Take back last byte(s). */
            memcpy( pData+ecbOff, ecbBuffer, ecbLen);
        }
    }

    return status;
}


uint8   TEK[8] = {
  0x61, 0x04, 0xF1, 0x84, 0xB1, 0x14, 0x3D, 0xC9
//0x6F, 0xBD, 0xF6, 0x69, 0x6A, 0xAC, 0x99, 0x84
};
uint8   IV[8] = {
  0x2C, 0x2A, 0xEA, 0xE5, 0x81, 0x51, 0x34, 0xCF
//0x80, 0xC4, 0xF0, 0xCA, 0xE4, 0x35, 0x75, 0xC9
};
uint8   P_IN[] = {
  0x80, 0x00, 0x14, 0x45, 0x00, 0x00, 0x1d, 0x00,
  0x00, 0x40, 0x00, 0x40, 0x01, 0x12, 0xca, 0x0a,
  0x0a, 0x0a, 0x02, 0x0a
};
uint16  P_IN_LEN = sizeof(P_IN);
uint8   C_IN[] = {
  0xb2, 0x84, 0x5b, 0x1d, 0x61, 0x29, 0xcc, 0x22,
  0x8b, 0x41, 0x24, 0x86, 0xd8, 0x8e, 0x3d, 0x4f,
  0xe3, 0x5e, 0x23, 0x82
};
uint16  C_IN_LEN = sizeof(C_IN);

int main(void)
{
    uint8   key[8];
    uint8   iv[8];
    uint32  frameNo = 0;
    int     status;


    memcpy(key, TEK, 8);
    memcpy(iv,  IV,  8);
    frameNo = 507;

    mem_dump("Plaintext In", P_IN, P_IN_LEN);
    status = _DesCbcEncrypt (key, iv, frameNo, P_IN, P_IN_LEN);
    printf("_DesCbcEncrypt ... %d\n", status);
    mem_dump("Ciphertext Out", P_IN, P_IN_LEN);


    memcpy(key, TEK, 8);
    memcpy(iv,  IV,  8);
    frameNo = 508;

    mem_dump("Ciphertext In", C_IN, C_IN_LEN);
    status = _DesCbcDecrypt (key, iv, frameNo, C_IN, C_IN_LEN);
    printf("_DesCbcDecrypt ... %d\n", status);
    mem_dump("Plaintext Out", C_IN, C_IN_LEN);

    return 0;
}

