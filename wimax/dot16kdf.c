#include "utility.h"

#define _GEN_AK_       1
#define _GEN_AKID_     1
#define _GEN_MAC_KEY_  1


/*
*
* PMK : EA 84 87 A0 F6 F4 B3 5F
*       BD DB 2F 23 2B 77 1D 2F 
*       71 FD DA 94 
*
* AK : D7 EA A9 68 3C 37 7B 76
*      A2 38 5B 84 54 B5 7A E7 
*      C9 AD 32 24 
*
* AKID : 3B 7E 35 4A 44 B2 58 06 
*
* CMAC_KEY_U : F6 EE D1 2D 93 C3 72 71
*              BD B3 84 44 9C B6 24 67 
*
* CMAC_KEY_D : C0 F5 E6 7C A3 B7 E3 C3
*              1D E4 22 2A 25 83 6B 2B 
*
* KEK : E0 93 89 B8 F3 39 1D FE
*       C8 A9 9A F4 1D A1 47 B9
*/

//- output --------------------------------------------------------------------------------
uint8 ak[20] = {
  0xCA, 0x00, 0xB7, 0x70, 0xEB, 0xFD, 0x33, 0xFD,
  0xD9, 0xDA, 0x19, 0xAD, 0x90, 0x1C, 0xC2, 0x86,
  0x38, 0x10, 0x12, 0x90
};
//-----------------------------------------------------------------------------------------

//- input ---------------------------------------------------------------------------------
#if _GEN_AK_
//MS-MPPE-Recv-Key
uint8 msk[64] = {
  0x3D, 0x8B, 0xFC, 0x43, 0xC8, 0x00, 0x4A, 0xFB,
  0x0F, 0xF8, 0x3B, 0x6A, 0x30, 0xEE, 0x7D, 0x6C,
  0xBF, 0x93, 0xB0, 0x76, 0xB9, 0x36, 0x53, 0xE9,
  0x62, 0x7F, 0xE4, 0x7A, 0xEF, 0x9C, 0xB3, 0x84,
  0xCA, 0xA5, 0x5B, 0x81, 0x2A, 0xAD, 0x2D, 0x7D,
  0xE5, 0x54, 0xCF, 0x9E, 0xEA, 0x84, 0x87, 0xA0,
  0xF6, 0xF4, 0xB3, 0x5F, 0xBD, 0xDB, 0x2F, 0x23,
  0x2B, 0x77, 0x1D, 0x2F, 0x71, 0xFD, 0xDA, 0x94
};
#endif
uint8 ms_mac_addr[6] = {
  0x00, 0x1b, 0x11, 0xc9, 0xd5, 0x0c
};
uint8 bs_id[6] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x05
};
uint8 mac_mode = DOT16KDF_CMAC;
//-----------------------------------------------------------------------------------------

int main(void)
{
    uint8 astring[256];
    int   astring_len = 0;


    #if _GEN_AK_
    {
        uint8 pmk[20];

        TRUNCATE_R(msk, 64, pmk, 20);
        mem_dump("PMK", pmk, 20);

        sprintf(
           (char*)astring,
           "%02X:%02X:%02X:%02X:%02X:%02X %02X:%02X:%02X:%02X:%02X:%02X AK",
           ms_mac_addr[0],
           ms_mac_addr[1],
           ms_mac_addr[2],
           ms_mac_addr[3],
           ms_mac_addr[4],
           ms_mac_addr[5],
           bs_id[0],
           bs_id[1],
           bs_id[2],
           bs_id[3],
           bs_id[4],
           bs_id[5]
        );
        printf("Dot16KDF AK: mac_mode=%d, astring=%s\n", mac_mode, astring);

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

        dot16kdf(pmk, astring, astring_len, ak, 20, mac_mode);
        mem_dump("AK", ak, 20);
    }
    #endif


    #if _GEN_AKID_
    {
        //- input --------
        uint8 ak_sn = 0;
        //----------------
        uint8 akid[8];

        sprintf(
           (char*)astring,
           "%d %02X:%02X:%02X:%02X:%02X:%02X %02X:%02X:%02X:%02X:%02X:%02X AK",
           ak_sn,
           ms_mac_addr[0],
           ms_mac_addr[1],
           ms_mac_addr[2],
           ms_mac_addr[3],
           ms_mac_addr[4],
           ms_mac_addr[5],
           bs_id[0],
           bs_id[1],
           bs_id[2],
           bs_id[3],
           bs_id[4],
           bs_id[5]
        );
        printf("Dot16KDF AKID: mac_mode=%d, astring=%s\n", mac_mode, astring);

        astring[0]  = ak_sn;
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

        dot16kdf(ak, astring, astring_len, akid, 8, mac_mode);
        mem_dump("AKID", akid, 8);
    }
    #endif


    #if _GEN_MAC_KEY_
    {
        //- input -------------------
        uint16  cmac_key_count = 0;
        //---------------------------
        uint8   buf1[48];
        uint8   buf2[16];

        uint8   macUp[16];
        uint8   macDown[16];
        uint8   kek[16];

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
        astring[12] = 'C';
        astring[13] = 'M';
        astring[14] = 'A';
        astring[15] = 'C';
        astring[16] = '_';
        astring[17] = 'K';
        astring[18] = 'E';
        astring[19] = 'Y';
        astring[20] = 'S';
        astring[21] = '+';
        astring[22] = 'K';
        astring[23] = 'E';
        astring[24] = 'K';
        astring_len = 25;
        dot16kdf(ak, astring, astring_len, buf1, 48, mac_mode);

        memset(buf2, 0x00, 14);
        buf2[14] = (cmac_key_count >> 8) & 0xFF;
        buf2[15] = (cmac_key_count     ) & 0xFF;
        aesEcbEncrypt(buf1,    16, macUp,   buf2);
        aesEcbEncrypt(buf1+16, 16, macDown, buf2);
        memcpy(kek, buf1+32, 16);

        mem_dump("CMAC_KEY_U", macUp,   16);
        mem_dump("CMAC_KEY_D", macDown, 16);
        mem_dump("KEK",        kek,     16);
    }
    #endif

    return 0;
}

