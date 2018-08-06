#include <stdio.h>
#include <openssl/md5.h>


unsigned char _input[] = {
    // ID
    0x00,
    // Secret
    't', 'o', 'm', '1', '2', '3', '@',
    // Random
    0xe2, 0xb6, 0xea, 0xea, 0xe2, 0xb6, 0xea, 0xea, 0xe2, 0xb6, 0xea, 0xea, 0xe2, 0xb6, 0xea, 0xea
};
int _inputSize = sizeof( _input );


int main(void)
{
    MD5_CTX context;
    unsigned char digest[16];

    printf("Input %d bytes\n\n", _inputSize);

    MD5_Init(&context);
    MD5_Update(&context, _input, _inputSize);
    MD5_Final(digest, &context);


    printf("Digest:\n");
    printf(
        "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n\n",
        digest[0],
        digest[1],
        digest[2],
        digest[3],
        digest[4],
        digest[5],
        digest[6],
        digest[7],
        digest[8],
        digest[9],
        digest[10],
        digest[11],
        digest[12],
        digest[13],
        digest[14],
        digest[15]
    );


    return 0;
}

