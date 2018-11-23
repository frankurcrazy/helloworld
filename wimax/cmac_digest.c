#include "utility.h"


int calcCmacDigest(uint8 *pKey, uint8 *pAkid, uint32 cmacPn, uint16 cid, uint8 *pMacMsg, uint16 macMsgLen, uint8 *pCmacDigest)
{
    int status;
    uint8  cmac_digest[16];
    uint8  *cmac_buf = NULL;
    uint16 cmac_len;

    cmac_buf = (uint8 *)malloc( 2048 );
    if (cmac_buf == NULL)
    {
        printf("[ERROR] cmac_buf kmalloc failure\n");
        return -1;
    }

    cmac_len  = 0;
    //[1] AKID
    if (pAkid == NULL) {
        memset(cmac_buf, 0x7E, 8);
    }
    else {
        memcpy(cmac_buf, pAkid, 8);
    }
    cmac_len += 8;
    //[2] CMAC_PN
    cmac_buf[cmac_len++] = (cmacPn >> 24) & 0xFF;
    cmac_buf[cmac_len++] = (cmacPn >> 16) & 0xFF;
    cmac_buf[cmac_len++] = (cmacPn >>  8) & 0xFF;
    cmac_buf[cmac_len++] = (cmacPn      ) & 0xFF;
    //[3] CID
    cmac_buf[cmac_len++] = (cid >>  8) & 0xFF;
    cmac_buf[cmac_len++] = (cid      ) & 0xFF;
    //[4] 16-bit zero padding
    cmac_buf[cmac_len++] = 0;
    cmac_buf[cmac_len++] = 0;
    //[5] MAC Management Message
    memcpy(cmac_buf+cmac_len, pMacMsg, macMsgLen);
    cmac_len += macMsgLen;

    status = cmacGenerate(pKey, cmac_buf, cmac_len, cmac_digest);


    memcpy(pCmacDigest, cmac_digest+8, 8); //cmac digest -> truncate to 8bytes

    free( cmac_buf );

    return status;
}


uint8   key[16] = {
  0xCB, 0xFB, 0xA3, 0x7F, 0xF8, 0xF9, 0xB7, 0x0E,
  0x80, 0xE6, 0xD4, 0x50, 0xC9, 0x7C, 0x19, 0x9E
};
uint8   akid[8] = {
  0x3B, 0x7E, 0x35, 0x4A, 0x44, 0xB2, 0x58, 0x06
};
uint32  pn = 1;
uint16  cid = 0;

uint8   msg[] = {
  0x04, 0x00, 0x01, 0x01, 0x00, 0x02, 0x06, 0x00,
  0x1B, 0x11, 0xC9, 0xD5, 0x0C, 0x94, 0x01, 0x06,
  0x05, 0x06, 0x00, 0x02, 0x03, 0x00, 0x00, 0x06,
  0x06, 0x01, 0x01, 0x0C, 0x01, 0x00, 0x0D, 0x02,
  0x00, 0x01
};
uint16  len = sizeof(msg);

int main(void)
{
    uint8  digest[32];
    int    status;

    memset(digest, 0, 32);
    status = calcCmacDigest(key, akid, pn, cid, msg, len, digest);
    printf("calcCmacDigest ... %d\n", status);
    mem_dump("CMAC Digest", digest, 16);

    return 0;
}

