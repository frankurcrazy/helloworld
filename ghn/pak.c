#include <gmp.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHA256_BYTES  32   /*  256 bits */
#define POWM_BYTES    128  /* 1024 bits */
#define A_B_PW_BYTES  14

#define H1_BYTES      144  /* 1152 bits */
#define H2_BYTES      16   /*  128 bits */
#define H3_BYTES      16   /*  128 bits */
#define H4_BYTES      16   /*  128 bits */
#define H5_BYTES      16   /*  128 bits */

#define RA_BYTES      48   /*  384 bits */
#define RB_BYTES      48   /*  384 bits */
#define X_BYTES       272  /* 2176 bits */
#define Y_BYTES       144  /* 1152 bits */
#define S1_BYTES      16   /*  128 bits */
#define S2_BYTES      16   /*  128 bits */
#define K_BYTES       16   /*  128 bits */

#define X_1035_HEX_G  "2"
#define X_1035_HEX_P  "FFFFFFFFFFFFFFFFC90FDAA22168C234" \
                      "C4C6628B80DC1CD129024E088A67CC74" \
                      "020BBEA63B139B22514A08798E3404DD" \
                      "EF9519B3CD3A431B302B0A6DF25F1437" \
                      "4FE1356D6D51C245E485B576625E7EC6" \
                      "F44C42E9A637ED6B0BFF5CB6F406B7ED" \
                      "EE386BFB5A899FA5AE9F24117C4B1FE6" \
                      "49286651ECE65381FFFFFFFFFFFFFFFF"


static unsigned char _pABPW[A_B_PW_BYTES] = {
    0x1,
    0x2,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB
};
static unsigned char _pRa[RA_BYTES] = {
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48,
    0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48
};
static unsigned char _pRb[RB_BYTES] = {
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59,
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59,
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59,
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59,
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59,
    0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59
};
static unsigned char _pH1[H1_BYTES]  = { 0 };
static unsigned char _pH2[H2_BYTES]  = { 0 };
static unsigned char _pX[X_BYTES]    = { 0 };
static unsigned char _pY[Y_BYTES]    = { 0 };
static unsigned char _pS1[S1_BYTES]  = { 0 };
static unsigned char _pS2[S2_BYTES]  = { 0 };
static unsigned char _pS1A[S1_BYTES] = { 0 };
static unsigned char _pS2B[S2_BYTES] = { 0 };
static unsigned char _pKA[K_BYTES]   = { 0 };
static unsigned char _pKB[K_BYTES]   = { 0 };

static unsigned char _pBuf[1024];


void mem_dump(char *pName, unsigned char *pData, int size)
{
    int i;

    printf("[1;33m%s[0m: %d bytes\n", pName, size);

    if (NULL == pData)
    {
        printf("NULL pointer\n\n");
    }

    for (i=0; i<size; i++)
    {
        if ((i != 0) && ((i % 16) == 0))
        {
            printf("\n");
        }
        printf("%02X ", pData[i]);
    }
    printf("\n\n");
}

void genHash(unsigned char *pMsg, int msgLen, unsigned char *pHash, int hashLen)
{
    unsigned char  _digest[SHA256_BYTES];
    int offset = 0;
    int Q;
    int R;
    int i;

    SHA256(pMsg, msgLen, _digest);

    Q = (hashLen / SHA256_BYTES);
    R = (hashLen % SHA256_BYTES);

    for (i=0; i<Q; i++)
    {
        memcpy(pHash+offset, _digest, SHA256_BYTES);
        offset += SHA256_BYTES;
    }

    if (R > 0)
    {
        memcpy(pHash+offset, _digest, R);
    }
}

void print_mpz(char *name, mpz_t z)
{
    char *str = mpz_get_str(0, 16, z);

    printf("%s = %s\n\n", name, str);
    free( str );
}

/*
* ITU-T X.1035
*
* Password-authenticated key exchange (PAK) protocol
*/
int main(int argc, char **argv)
{
    mpz_t g, p;
    mpz_t H1, X_H1, X_H1_POWM;
    mpz_t H2, Y_H2, Y_H2_POWM;
    mpz_t RA, A_POWM;
    mpz_t RB, B_POWM;
    mpz_t X, Y;
    mpz_t S1, S2;
    mpz_t S1A, S2B;
    mpz_t KA, KB;
    unsigned int offset;
    unsigned int count;


    /*
    * A: select a random number RA and calculate X
    * B: select a random number RB and calculate Y
    *
    * A  ------- X -------> B
    *
    * B: get A_POWN from X and calculate S1
    *
    * A <---- S1 and Y ---> B
    *
    * A: get B_POWN from Y and calculate S2
    *
    * A  ------- S2 ------> B
    *
    * A: verify S1 and calculate K
    * B: verify S2 and calculate K
    */
    printf("Initialization ...\n");
    mpz_init_set_str(g, X_1035_HEX_G, 16);
    mpz_init_set_str(p, X_1035_HEX_P, 16);
    printf("\n");


    /* Begin of calculate X */
    /* [ */
    /* X = H1(A|B|PW) * (g^RA mod p) */
    printf("[1] calculate X\n");

    mpz_init( H1 );
    mpz_init( RA );
    mpz_init( A_POWM );
    mpz_init( X );

    genHash(_pABPW, A_B_PW_BYTES, _pH1, H1_BYTES);

    mpz_import(
        H1,
        H1_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH1
    );

    mpz_import(
        RA,
        RA_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRa
    );

    mpz_powm(A_POWM, g, RA, p);
    //print_mpz("A_POWM", A_POWM);

    mpz_mul(X, H1, A_POWM);
    //print_mpz("X", X);

    count = X_BYTES;
    mpz_export(
        _pX,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X
    );

    mem_dump("X", _pX, X_BYTES);

    mpz_clear( H1 );
    mpz_clear( RA );
    mpz_clear( A_POWM );
    mpz_clear( X );
    /* ] */
    /* End   of calculate X */


    /* Begin of calculate S1 */
    /* [ */
    /* S1 = H3(A|B|PW|X/H1(A|B|PW)|(g^RB mod p)|(X/H1(A|B|PW)^RB mod p)) */
    printf("[2] calculate S1\n");

    mpz_init( H1 );
    mpz_init( X_H1 );
    mpz_init( X_H1_POWM );
    mpz_init( RB );
    mpz_init( B_POWM );
    mpz_init( X );
    mpz_init( S1 );

    genHash(_pABPW, A_B_PW_BYTES, _pH1, H1_BYTES);

    mpz_import(
        H1,
        H1_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH1
    );

    mpz_import(
        RB,
        RB_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRb
    );

    mpz_import(
        X,
        X_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pX
    );

    mpz_powm(B_POWM, g, RB, p);
    //print_mpz("B_POWM", B_POWM);

    mpz_fdiv_q(X_H1, X, H1);
    //print_mpz("X / H1", X_H1);

    mpz_powm(X_H1_POWM, X_H1, RB, p);
    //print_mpz("(X / H1)^RB mod p", X_H1_POWM);

    offset = 0;
    memcpy(_pBuf+offset, _pABPW, A_B_PW_BYTES);
    offset += A_B_PW_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X_H1
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        B_POWM
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X_H1_POWM
    );
    offset += POWM_BYTES;

    genHash(_pBuf, offset, _pS1, H3_BYTES);

    count = S1_BYTES;
    mpz_export(
        _pS1,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        S1
    );

    mem_dump("S1", _pS1, S1_BYTES);

    mpz_clear( H1 );
    mpz_clear( X_H1 );
    mpz_clear( X_H1_POWM );
    mpz_clear( RB );
    mpz_clear( B_POWM );
    mpz_clear( X );
    mpz_clear( S1 );
    /* ] */
    /* End   of calculate S1 */


    /* Begin of calculate Y */
    /* [ */
    /* Y = H2(A|B|PW) * (g^RB mod p) */
    printf("[3] calculate Y\n");

    mpz_init( H2 );
    mpz_init( RB );
    mpz_init( B_POWM );
    mpz_init( Y );

    genHash(_pABPW, A_B_PW_BYTES, _pH2, H2_BYTES);

    mpz_import(
        H2,
        H2_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH2
    );

    mpz_import(
        RB,
        RB_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRb
    );

    mpz_powm(B_POWM, g, RB, p);
    //print_mpz("B_POWM", B_POWM);

    mpz_mul(Y, H2, B_POWM);
    //print_mpz("Y", Y);

    count = Y_BYTES;
    mpz_export(
        _pY,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y
    );

    mem_dump("Y", _pY, Y_BYTES);

    mpz_clear( H2 );
    mpz_clear( RB );
    mpz_clear( B_POWM );
    mpz_clear( Y );
    /* ] */
    /* End   of calculate Y */


    /* Begin of calculate S2 */
    /* [ */
    /* S2 = H4(A|B|PW|(g^RA mod p)|Y/H2(A|B|PW)|(Y/H2(A|B|PW)^RA mod p)) */
    printf("[4] calculate S2\n");

    mpz_init( H2 );
    mpz_init( Y_H2 );
    mpz_init( Y_H2_POWM );
    mpz_init( RA );
    mpz_init( A_POWM );
    mpz_init( Y );
    mpz_init( S2 );

    genHash(_pABPW, A_B_PW_BYTES, _pH2, H2_BYTES);

    mpz_import(
        H2,
        H2_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH2
    );

    mpz_import(
        RA,
        RA_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRa
    );

    mpz_import(
        Y,
        Y_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pY
    );

    mpz_powm(A_POWM, g, RA, p);
    //print_mpz("A_POWM", A_POWM);

    mpz_fdiv_q(Y_H2, Y, H2);
    //print_mpz("Y / H2", Y_H2);

    mpz_powm(Y_H2_POWM, Y_H2, RA, p);
    //print_mpz("(Y / H2)^RA mod p", Y_H2_POWM);

    offset = 0;
    memcpy(_pBuf+offset, _pABPW, A_B_PW_BYTES);
    offset += A_B_PW_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        A_POWM
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y_H2
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y_H2_POWM
    );
    offset += POWM_BYTES;

    genHash(_pBuf, offset, _pS2, H4_BYTES);

    count = S2_BYTES;
    mpz_export(
        _pS2,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        S2
    );

    mem_dump("S2", _pS2, S2_BYTES);

    mpz_clear( H2 );
    mpz_clear( Y_H2 );
    mpz_clear( Y_H2_POWM );
    mpz_clear( RA );
    mpz_clear( A_POWM );
    mpz_clear( Y );
    mpz_clear( S2 );
    /* ] */
    /* End   of calculate S2 */


    /* Begin of verify S1 */
    /* [ */
    /* S1 = H3(A|B|PW|(g^RA mod p)|(g^RB mod p)|((g^RB mod p)^RA mod p)) */
    printf("[5] A verify B's S1\n");

    mpz_init( H2 );
    mpz_init( Y_H2 );
    mpz_init( Y_H2_POWM );
    mpz_init( RA );
    mpz_init( A_POWM );
    mpz_init( Y );
    mpz_init( S1A );

    genHash(_pABPW, A_B_PW_BYTES, _pH2, H2_BYTES);

    mpz_import(
        H2,
        H2_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH2
    );

    mpz_import(
        RA,
        RA_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRa
    );

    mpz_import(
        Y,
        Y_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pY
    );

    mpz_powm(A_POWM, g, RA, p);
    //print_mpz("A_POWM", A_POWM);

    mpz_fdiv_q(Y_H2, Y, H2);
    //print_mpz("Y / H2", Y_H2);

    mpz_powm(Y_H2_POWM, Y_H2, RA, p);
    //print_mpz("(Y / H2)^RA mod p", Y_H2_POWM);

    offset = 0;
    memcpy(_pBuf+offset, _pABPW, A_B_PW_BYTES);
    offset += A_B_PW_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        A_POWM
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y_H2
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y_H2_POWM
    );
    offset += POWM_BYTES;

    genHash(_pBuf, offset, _pS1A, H3_BYTES);

    count = S1_BYTES;
    mpz_export(
        _pS1A,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        S1A
    );

    mem_dump("S1-A", _pS1A, S1_BYTES);

    mpz_clear( H2 );
    mpz_clear( Y_H2 );
    mpz_clear( Y_H2_POWM );
    mpz_clear( RA );
    mpz_clear( A_POWM );
    mpz_clear( Y );
    mpz_clear( S1A );
    /* ] */
    /* End   of verify S1 */


    /* Begin of verify S2 */
    /* [ */
    /* S2 = H4(A|B|PW|(g^RA mod p)|(g^RB mod p)|((g^RA mod p)^RB mod p)) */
    printf("[6] B verify A's S2\n");

    mpz_init( H1 );
    mpz_init( X_H1 );
    mpz_init( X_H1_POWM );
    mpz_init( RB );
    mpz_init( B_POWM );
    mpz_init( X );
    mpz_init( S2B );

    genHash(_pABPW, A_B_PW_BYTES, _pH1, H1_BYTES);

    mpz_import(
        H1,
        H1_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH1
    );

    mpz_import(
        RB,
        RB_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRb
    );

    mpz_import(
        X,
        X_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pX
    );

    mpz_powm(B_POWM, g, RB, p);
    //print_mpz("B_POWM", B_POWM);

    mpz_fdiv_q(X_H1, X, H1);
    //print_mpz("X / H1", X_H1);

    mpz_powm(X_H1_POWM, X_H1, RB, p);
    //print_mpz("(X / H1)^RB mod p", X_H1_POWM);

    offset = 0;
    memcpy(_pBuf+offset, _pABPW, A_B_PW_BYTES);
    offset += A_B_PW_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X_H1
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        B_POWM
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X_H1_POWM
    );
    offset += POWM_BYTES;

    genHash(_pBuf, offset, _pS2B, H5_BYTES);

    count = S2_BYTES;
    mpz_export(
        _pS2B,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        S2B
    );

    mem_dump("S2-B", _pS2B, S2_BYTES);

    mpz_clear( H1 );
    mpz_clear( X_H1 );
    mpz_clear( X_H1_POWM );
    mpz_clear( RB );
    mpz_clear( B_POWM );
    mpz_clear( X );
    mpz_clear( S2B );
    /* ] */
    /* End   of verify S2 */


    /* Begin of calculate K-A */
    /* [ */
    /* K-A = H5(A|B|PW|(g^RA mod p)|Y/H2(A|B|PW)|(Y/H2(A|B|PW)^RA mod p)) */
    printf("[7] calculate K-A\n");

    mpz_init( H2 );
    mpz_init( Y_H2 );
    mpz_init( Y_H2_POWM );
    mpz_init( RA );
    mpz_init( A_POWM );
    mpz_init( Y );
    mpz_init( KA );

    genHash(_pABPW, A_B_PW_BYTES, _pH2, H2_BYTES);

    mpz_import(
        H2,
        H2_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH2
    );

    mpz_import(
        RA,
        RA_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRa
    );

    mpz_import(
        Y,
        Y_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pY
    );

    mpz_powm(A_POWM, g, RA, p);
    //print_mpz("A_POWM", A_POWM);

    mpz_fdiv_q(Y_H2, Y, H2);
    //print_mpz("Y / H2", Y_H2);

    mpz_powm(Y_H2_POWM, Y_H2, RA, p);
    //print_mpz("(Y / H2)^RA mod p", Y_H2_POWM);

    offset = 0;
    memcpy(_pBuf+offset, _pABPW, A_B_PW_BYTES);
    offset += A_B_PW_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        A_POWM
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y_H2
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        Y_H2_POWM
    );
    offset += POWM_BYTES;

    genHash(_pBuf, offset, _pKA, H5_BYTES);

    count = K_BYTES;
    mpz_export(
        _pKA,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        KA
    );

    mem_dump("K-A", _pKA, K_BYTES);

    mpz_clear( H2 );
    mpz_clear( Y_H2 );
    mpz_clear( Y_H2_POWM );
    mpz_clear( RA );
    mpz_clear( A_POWM );
    mpz_clear( Y );
    mpz_clear( KA );
    /* ] */
    /* End   of calculate K-A */


    /* Begin of calculate K-B */
    /* [ */
    /* K-B = H5(A|B|PW|X/H1(A|B|PW)|(g^RB mod p)|(X/H1(A|B|PW)^RB mod p)) */
    printf("[8] calculate K-B\n");

    mpz_init( H1 );
    mpz_init( X_H1 );
    mpz_init( X_H1_POWM );
    mpz_init( RB );
    mpz_init( B_POWM );
    mpz_init( X );
    mpz_init( KB );

    genHash(_pABPW, A_B_PW_BYTES, _pH1, H1_BYTES);

    mpz_import(
        H1,
        H1_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pH1
    );

    mpz_import(
        RB,
        RB_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pRb
    );

    mpz_import(
        X,
        X_BYTES,  /* count */
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        _pX
    );

    mpz_powm(B_POWM, g, RB, p);
    //print_mpz("B_POWM", B_POWM);

    mpz_fdiv_q(X_H1, X, H1);
    //print_mpz("X / H1", X_H1);

    mpz_powm(X_H1_POWM, X_H1, RB, p);
    //print_mpz("(X / H1)^RB mod p", X_H1_POWM);

    offset = 0;
    memcpy(_pBuf+offset, _pABPW, A_B_PW_BYTES);
    offset += A_B_PW_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X_H1
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        B_POWM
    );
    offset += POWM_BYTES;

    count = POWM_BYTES;
    mpz_export(
        _pBuf+offset,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        X_H1_POWM
    );
    offset += POWM_BYTES;

    genHash(_pBuf, offset, _pKB, H5_BYTES);

    count = K_BYTES;
    mpz_export(
        _pKB,
        &count,
        1,  /* order */
        1,  /* size */
        1,  /* endian */
        0,  /* nail */
        KB
    );

    mem_dump("K-B", _pKB, K_BYTES);

    mpz_clear( H1 );
    mpz_clear( X_H1 );
    mpz_clear( X_H1_POWM );
    mpz_clear( RB );
    mpz_clear( B_POWM );
    mpz_clear( X );
    mpz_clear( KB );
    /* ] */
    /* End   of calculate K-B */


    mpz_clear( g );
    mpz_clear( p );

    return 0;
}
