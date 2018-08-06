#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gsm7bit.h"


#if 0
uint8  pSmsString[] = "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message string ! "
                      "Hello, short message stringXY";
uint16 smsStringLen;
#endif

#if 0
uint8  pSmsString[] = "0123456789.X";
uint16 smsStringLen;
#endif

#if 0
uint8  pSmsString[] = "Svc[123";
uint16 smsStringLen;
#endif

#if 0
uint8  pSmsString[] = "123{}[]\\$~1234";
uint16 smsStringLen;
#endif

#if 1
uint8  pSmsString[] = "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "0123456789"
                      "abcdefghij";
uint16 smsStringLen;
#endif

int main(void)
{
    uint16  messageLen = 0;
    uint8   message[1250];

    smsStringLen = strlen( (char *)pSmsString );
    printf("String length = %u\n", smsStringLen);

    memset(message, 0x00, 1250);
    {
        /* GSM 7-bit default alphabet */
        messageLen = sms_encodeGsm7bit(
                         message,
                         1250,
                         pSmsString,
                         &smsStringLen
                     );
    }

    printf("Encoded length = %u / %u\n", messageLen, smsStringLen);
    mem_dump(message, messageLen);

    return 0;
}

