#include "utility.h"

/*
*
*         +--------+
* PMK  -->|        |
* MSID -->|Dot16KDF|--> AK
* BSID -->|        |
*         +--------+
* 
* PMK : F4 3C E2 CC B5 6B F2 F1
*       86 2C 2A AC 72 4C 20 DB 
*       43 7D 62 AD
*
* MSID : 00 ED 04 A5 10 5B
*
* BSID : 00 00 BB 02 00 03
*
* AK : CA E0 DE 08 E0 27 AC B5
*      19 8A E7 8C 2A 2D 78 AB 
*      99 45 CE 74
*
* AKID : C0 56 43 2D 94 07 49 94
*/

/* This is the answer ... */
uint8 ak[20] = {
  0xCA, 0xE0, 0xDE, 0x08, 0xE0, 0x27, 0xAC, 0xB5, 0x19, 0x8A,
  0xE7, 0x8C, 0x2A, 0x2D, 0x78, 0xAB, 0x99, 0x45, 0xCE, 0x74
};

/* MS-MPPE-Recv-Key */
uint8 msk[64] = {
  0xd1, 0x58, 0x87, 0xdb, 0x8e, 0x5c, 0x55, 0x50,
  0x2c, 0xf7, 0xe5, 0x18, 0xc6, 0x21, 0xaa, 0x87,
  0xbe, 0x93, 0xc4, 0x94, 0x19, 0x09, 0xf8, 0x53,
  0x8c, 0x49, 0xf7, 0x5e, 0x2e, 0xa4, 0x08, 0x61,
  0xa1, 0x90, 0xfd, 0x3a, 0xd8, 0xfc, 0x30, 0x39,
  0xaf, 0x55, 0x72, 0xdd, 0xf4, 0x3c, 0xe2, 0xcc,
  0xb5, 0x6b, 0xf2, 0xf1, 0x86, 0x2c, 0x2a, 0xac,
  0x72, 0x4c, 0x20, 0xdb, 0x43, 0x7d, 0x62, 0xad
};
uint8 pmk[20] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 ms_mac_addr[6] = {
  0x00, 0xED, 0x04, 0xA5, 0x10, 0x5B
};
uint8 bs_id[6] = {
  0x00, 0x00, 0xBB, 0x02, 0x00, 0x03
};

int main(void)
{
    uint8 astring[256];
    int astring_len = 0;
    uint8 output[20];


    /* [1] derive PMK from MSK */
    TRUNCATE_R(msk, 64, pmk, 20);
    mem_dump("PMK", pmk, 20);

    /* [2] derive AK from PMK */
    astring[0]  = ms_mac_addr[0];
    astring[1]  = ms_mac_addr[1];
    astring[2]  = ms_mac_addr[2];
    astring[3]  = ms_mac_addr[3];
    astring[4]  = ms_mac_addr[4];
    astring[5]  = ms_mac_addr[5];
    astring[6]  = bs_id[0];
    astring[7]  = bs_id[1];
    astring[8]  = bs_id[2];
    astring[9]  = bs_id[3];
    astring[10] = bs_id[4];
    astring[11] = bs_id[5];
    astring[12] = 'A';
    astring[13] = 'K';
    astring_len = 14;

    //mem_dump("astring", astring, astring_len);

    dot16kdf(pmk, astring, astring_len, output, 20, DOT16KDF_CMAC);
    if (memcmp(ak, output, 20) == 0)
    {
        printf("\nYou got correct AK!!!\n\n");
        mem_dump("AK", output, 20);
    }


    printf("\n");

    /* [3] derive AKID from AK */
    astring[0]  = 0;
    astring[1]  = ms_mac_addr[0];
    astring[2]  = ms_mac_addr[1];
    astring[3]  = ms_mac_addr[2];
    astring[4]  = ms_mac_addr[3];
    astring[5]  = ms_mac_addr[4];
    astring[6]  = ms_mac_addr[5];
    astring[7]  = bs_id[0];
    astring[8]  = bs_id[1];
    astring[9]  = bs_id[2];
    astring[10] = bs_id[3];
    astring[11] = bs_id[4];
    astring[12] = bs_id[5];
    astring[13] = 'A';
    astring[14] = 'K';
    astring_len = 15;

    //mem_dump("astring", astring, astring_len);

    dot16kdf(ak, astring, astring_len, output, 8, DOT16KDF_CMAC);
    mem_dump("AKID", output, 8);


    return 0;
}

