[ Test Algorithm ]

K
 ==> CK, IK
      ==> K_AUSF
           ==> K_SEAF
                ==> K_AMF
                     ==> K_NAS_int, K_NAS_enc
                     ==> K_gNB, NH
                          ==> K_RRC_int, K_RRC_enc, K_UP_int, K_UP_enc

K      : 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
RAND   : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
SQN    : 00 00 00 00 00 FF
AMF    : 90 01

CK     : 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 00
IK     : 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 00 01
AK     : 03 04 05 06 07 08

PLMN ID: 001.01
K_AUSF : 20 DE 4E B2 B7 5C 8D AC 30 1A D5 27 73 E2 E3 7B
         D7 B4 B0 C2 FD B1 C3 75 1B 98 DD 0D F8 46 AE D5

K_SEAF : D2 BB C6 3B 0C 14 12 47 34 1C 0F CA 3D C3 BF FD
       : C6 BF C1 81 34 89 34 2D E8 48 7E 7E 4D C3 72 8B

IMSI   : 001.01.0123456789
K_AMF  : 7E 9F 97 70 7C EF 48 31 8D F1 82 A7 F9 E7 BA 83
       : 8A 6F B3 6A B8 3A 7D 1B AF 26 10 B9 7A 5B 9D CF

UL NAS count: 0
K_gNB  : 61 7C 2F 3D B7 14 A5 A1 05 97 59 1A 25 FE B2 1A
       : 15 9E A7 79 61 09 29 2B 0B 2C A5 79 D9 00 2B 0C

