#ifndef _TEST_VECTOR_H_
#define _TEST_VECTOR_H_

#undef _USING_3GPP_AKA_

/*
 * response="6629fae49393a05397450978507c4ef1"
 */
char  pszAlg[] = "";
char  pszUser[] = "Mufasa";
char  pszRealm[] = "testrealm@host.com";
uint8 pszPass[] = "Circle Of Life";
int   pszPassLen = 14;
char  pszCNonce[] = "0a4f113b";
char  pszNonce[] = "dcd98b7102dd2f0e8b11d0f600bfb0c093";
char  pszNonceCount[] = "00000001";
char  pszQop[] = "auth";
char  pszMethod[] = "GET";
char  pszURI[] = "/dir/index.html";

char  SIP_BODY[] = "";

#endif
