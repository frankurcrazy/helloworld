#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

/* Global Header */
typedef struct pcap_hdr_s {
    unsigned int    magic_number;   /* magic number */
    unsigned short  version_major;  /* major version number */
    unsigned short  version_minor;  /* minor version number */
    int             thiszone;       /* GMT to local correction */
    unsigned int    sigfigs;        /* accuracy of timestamps */
    unsigned int    snaplen;        /* max length of captured packets, in octets */
    unsigned int    network;        /* data link type */
} pcap_hdr_t;

typedef struct pcaprec_hdr_s {
    unsigned int    ts_sec;         /* timestamp seconds */
    unsigned int    ts_usec;        /* timestamp microseconds */
    unsigned int    incl_len;       /* number of octets of packet saved in file */
    unsigned int    orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

#define PCAP_HEADER_LEN      (sizeof(pcap_hdr_t))
#define PCAP_SUB_HEADER_LEN  (sizeof(pcaprec_hdr_t))

#define PCAP_MAGIC_NUM       (0xA1B2C3D4)
#define PCAP_MAGIC_NUM_NANO  (0xA1B23C4D)  /* ns resolution */


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    char *pName = "pcap_gen.pcap";
    FILE *pFile = NULL;

    unsigned char GlobalHdr[PCAP_HEADER_LEN];
    unsigned char PacketHdr[PCAP_SUB_HEADER_LEN];
    pcaprec_hdr_t *pPacketHdr = (pcaprec_hdr_t *)PacketHdr;

    unsigned char eth[14] = {
        0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
        0x08, 0x00  /* IP */
    };
    int ethLen = sizeof( eth );
    unsigned char ip[20] = {
        0x45, 0x00,
        0x00, 0x24,  /* total length (20 + 16) */
        0x04, 0xd2,  /* identification */
        0x00, 0x00,
        0x01, 0x11,  /* UDP */
        0x00, 0x00,  /* checksum */
        192, 168, 3, 100,  /* source IP address */
        192, 168, 3, 200   /* destination IP address */
    };
    int ipLen = sizeof( ip );
    unsigned char udp[8] = {
        0xd9, 0x03,  /* source port number */
        0xd9, 0x03,  /* destination port number */
        0x00, 0x10,  /* length (8 + 8) */
        0x00, 0x00   /* checksum */
    };
    int udpLen = sizeof( udp );
    unsigned char foo[8] = {
        0x01,
        0x01,
        0x03, 0xe7,
        0x0a, 0x0a, 0x0a, 0x02
    };
    int fooLen = sizeof( foo );
    int totalLen = (ethLen + ipLen + udpLen + fooLen);


    if (argc > 1)
    {
        pName = argv[1];
    }

    pFile = fopen(pName, "wb");
    if ( pFile )
    {
        pcap_hdr_t    *pGlobalHdr = (pcap_hdr_t *)GlobalHdr;        

        pGlobalHdr->magic_number  = PCAP_MAGIC_NUM;
        pGlobalHdr->version_major = 2;
        pGlobalHdr->version_minor = 4;
        pGlobalHdr->thiszone      = 0;
        pGlobalHdr->sigfigs       = 0;
        pGlobalHdr->snaplen       = 262144;
        pGlobalHdr->network       = 1;  //Ethernet

        pPacketHdr->ts_sec   = 0;
        pPacketHdr->ts_usec  = 0;
        pPacketHdr->incl_len = (totalLen);
        pPacketHdr->orig_len = (totalLen);

        fwrite(GlobalHdr, 1, PCAP_HEADER_LEN,     pFile);
        fwrite(PacketHdr, 1, PCAP_SUB_HEADER_LEN, pFile);
        fwrite(eth,       1, ethLen, pFile);
        fwrite(ip,        1, ipLen,  pFile);
        fwrite(udp,       1, udpLen, pFile);
        fwrite(foo,       1, fooLen, pFile);
        
        fclose( pFile );        
    }

    return 0;
}

