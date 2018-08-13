#ifndef _TEST_VECTOR_H_
#define _TEST_VECTOR_H_

#define _USING_3GPP_AKA_

bool milenage_algorithm = 0;

uint8 AKA_K[16] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

uint8 AKA_OPC[16] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AKA_RAND[16] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AKA_SQN[6] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8 AKA_AMF[2] = {
0x00, 0x00
};

uint8 AKA_MAC[8];
uint8 AKA_XRES[16];
uint8 AKA_CK[16];
uint8 AKA_IK[16];
uint8 AKA_AK[6];
uint8 AKA_AUTN[16];

/*
 * response="c76c2c281a21c0d6131e667d61a6bdf5"
 */
char  pszAlg[] = "AKAv1-MD5";
char  pszUser[] = "466058041131504@ims.mnc005.mcc466.3gppnetwork.org";
char  pszRealm[] = "ims.mnc005.mcc466.3gppnetwork.org";
uint8 pszPass[20];
int   pszPassLen = 16;
char  pszCNonce[] = "YD05duxy";
char  pszNonce[1024];
char  pszNonceCount[] = "00000001";
char  pszQop[] = "auth";
char  pszMethod[] = "REGISTER";
char  pszURI[] = "sip:ims.mnc005.mcc466.3gppnetwork.org";

char  SIP_BODY[] = "";

#endif
