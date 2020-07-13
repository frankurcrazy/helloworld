#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIV_CEIL(X, Y)  (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y) ((X) / (Y))
#define MAX(A, B)       (((A) > (B)) ? (A) : (B))
#define MIN(A, B)       (((A) < (B)) ? (A) : (B))


typedef enum
{
    RESTRICTED_SET_NONE = 0,
    RESTRICTED_SET_A    = 1,
    RESTRICTED_SET_B    = 2
} tRestrictedSetType;


/* Mapping from logical index i to sequence number u for preamble formats with L_RA = 839 */
int Table_6_3_3_1_3[838] = {
 129, 710, 140, 699, 120, 719, 210, 629, 168, 671,  84, 755, 105, 734,  93, 746,  70, 769,  60, 779,
   2, 837,   1, 838,  56, 783, 112, 727, 148, 691,  80, 759,  42, 797,  40, 799,  35, 804,  73, 766,
 146, 693,  31, 808,  28, 811,  30, 809,  27, 812,  29, 810,  24, 815,  48, 791,  68, 771,  74, 765,
 178, 661, 136, 703,  86, 753,  78, 761,  43, 796,  39, 800,  20, 819,  21, 818,  95, 744, 202, 637,
 190, 649, 181, 658, 137, 702, 125, 714, 151, 688, 217, 622, 128, 711, 142, 697, 122, 717, 203, 636,
 118, 721, 110, 729,  89, 750, 103, 736,  61, 778,  55, 784,  15, 824,  14, 825,  12, 827,  23, 816,
  34, 805,  37, 802,  46, 793, 207, 632, 179, 660, 145, 694, 130, 709, 223, 616, 228, 611, 227, 612,
 132, 707, 133, 706, 143, 696, 135, 704, 161, 678, 201, 638, 173, 666, 106, 733,  83, 756,  91, 748,
  66, 773,  53, 786,  10, 829,   9, 830,   7, 832,   8, 831,  16, 823,  47, 792,  64, 775,  57, 782,
 104, 735, 101, 738, 108, 731, 208, 631, 184, 655, 197, 642, 191, 648, 121, 718, 141, 698, 149, 690,
 216, 623, 218, 621, 152, 687, 144, 695, 134, 705, 138, 701, 199, 640, 162, 677, 176, 663, 119, 720,
 158, 681, 164, 675, 174, 665, 171, 668, 170, 669,  87, 752, 169, 670,  88, 751, 107, 732,  81, 758,
  82, 757, 100, 739,  98, 741,  71, 768,  59, 780,  65, 774,  50, 789,  49, 790,  26, 813,  17, 822,
  13, 826,   6, 833,   5, 834,  33, 806,  51, 788,  75, 764,  99, 740,  96, 743,  97, 742, 166, 673,
 172, 667, 175, 664, 187, 652, 163, 676, 185, 654, 200, 639, 114, 725, 189, 650, 115, 724, 194, 645,
 195, 644, 192, 647, 182, 657, 157, 682, 156, 683, 211, 628, 154, 685, 123, 716, 139, 700, 212, 627,
 153, 686, 213, 626, 215, 624, 150, 689, 225, 614, 224, 615, 221, 618, 220, 619, 127, 712, 147, 692,
 124, 715, 193, 646, 205, 634, 206, 633, 116, 723, 160, 679, 186, 653, 167, 672,  79, 760,  85, 754,
  77, 762,  92, 747,  58, 781,  62, 777,  69, 770,  54, 785,  36, 803,  32, 807,  25, 814,  18, 821,
  11, 828,   4, 835,   3, 836,  19, 820,  22, 817,  41, 798,  38, 801,  44, 795,  52, 787,  45, 794,
  63, 776,  67, 772,  72, 767,  76, 763,  94, 745, 102, 737,  90, 749, 109, 730, 165, 674, 111, 728,
 209, 630, 204, 635, 117, 722, 188, 651, 159, 680, 198, 641, 113, 726, 183, 656, 180, 659, 177, 662,
 196, 643, 155, 684, 214, 625, 126, 713, 131, 708, 219, 620, 222, 617, 226, 613, 230, 609, 232, 607,
 262, 577, 252, 587, 418, 421, 416, 423, 413, 426, 411, 428, 376, 463, 395, 444, 283, 556, 285, 554,
 379, 460, 390, 449, 363, 476, 384, 455, 388, 451, 386, 453, 361, 478, 387, 452, 360, 479, 310, 529,
 354, 485, 328, 511, 315, 524, 337, 502, 349, 490, 335, 504, 324, 515, 323, 516, 320, 519, 334, 505,
 359, 480, 295, 544, 385, 454, 292, 547, 291, 548, 381, 458, 399, 440, 380, 459, 397, 442, 369, 470,
 377, 462, 410, 429, 407, 432, 281, 558, 414, 425, 247, 592, 277, 562, 271, 568, 272, 567, 264, 575,
 259, 580, 237, 602, 239, 600, 244, 595, 243, 596, 275, 564, 278, 561, 250, 589, 246, 593, 417, 422,
 248, 591, 394, 445, 393, 446, 370, 469, 365, 474, 300, 539, 299, 540, 364, 475, 362, 477, 298, 541,
 312, 527, 313, 526, 314, 525, 353, 486, 352, 487, 343, 496, 327, 512, 350, 489, 326, 513, 319, 520,
 332, 507, 333, 506, 348, 491, 347, 492, 322, 517, 330, 509, 338, 501, 341, 498, 340, 499, 342, 497,
 301, 538, 366, 473, 401, 438, 371, 468, 408, 431, 375, 464, 249, 590, 269, 570, 238, 601, 234, 605,
 257, 582, 273, 566, 255, 584, 254, 585, 245, 594, 251, 588, 412, 427, 372, 467, 282, 557, 403, 436,
 396, 443, 392, 447, 391, 448, 382, 457, 389, 450, 294, 545, 297, 542, 311, 528, 344, 495, 345, 494,
 318, 521, 331, 508, 325, 514, 321, 518, 346, 493, 339, 500, 351, 488, 306, 533, 289, 550, 400, 439,
 378, 461, 374, 465, 415, 424, 270, 569, 241, 598, 231, 608, 260, 579, 268, 571, 276, 563, 409, 430,
 398, 441, 290, 549, 304, 535, 308, 531, 358, 481, 316, 523, 293, 546, 288, 551, 284, 555, 368, 471,
 253, 586, 256, 583, 263, 576, 242, 597, 274, 565, 402, 437, 383, 456, 357, 482, 329, 510, 317, 522,
 307, 532, 286, 553, 287, 552, 266, 573, 261, 578, 236, 603, 303, 536, 356, 483, 355, 484, 405, 434,
 404, 435, 406, 433, 235, 604, 267, 572, 302, 537, 309, 530, 265, 574, 233, 606, 367, 472, 296, 543,
 336, 503, 305, 534, 373, 466, 280, 559, 279, 560, 419, 420, 240, 599, 258, 581, 229, 610
};

/* Mapping from logical index i to sequence number u for preamble formats with L_RA = 139 */
int Table_6_3_3_1_4[138] = {
 1, 138,  2, 137,  3, 136,  4, 135,  5, 134,  6, 133,  7, 132,  8, 131,  9, 130, 10, 129,
11, 128, 12, 127, 13, 126, 14, 125, 15, 124, 16, 123, 17, 122, 18, 121, 19, 120, 20, 119,
21, 118, 22, 117, 23, 116, 24, 115, 25, 114, 26, 113, 27, 112, 28, 111, 29, 110, 30, 109,
31, 108, 32, 107, 33, 106, 34, 105, 35, 104, 36, 103, 37, 102, 38, 101, 39, 100, 40,  99,
41,  98, 42,  97, 43,  96, 44,  95, 45,  94, 46,  93, 47,  92, 48,  91, 49,  90, 50,  89,
51,  88, 52,  87, 53,  86, 54,  85, 55,  84, 56,  83, 57,  82, 58,  81, 59,  80, 60,  79,
61,  78, 62,  77, 63,  76, 64,  75, 65,  74, 66,  73, 67,  72, 68,  71, 69,  70
};

/* N_CS for preamble formats with £Gf_RA = 1.25 KHz */
int Table_6_3_3_1_5[16][3] = {
 {   0,  15,  15 },
 {  13,  18,  18 },
 {  15,  22,  22 },
 {  18,  26,  26 },
 {  22,  32,  32 },
 {  26,  38,  38 },
 {  32,  46,  46 },
 {  38,  55,  55 },
 {  46,  68,  68 },
 {  59,  82,  82 },
 {  76, 100, 100 },
 {  93, 128, 118 },
 { 119, 158, 137 },
 { 167, 202,   0 },
 { 279, 237,   0 },
 { 419,   0,   0 }
};

/* N_CS for preamble formats with £Gf_RA = 5 KHz */
int Table_6_3_3_1_6[16][3] = {
 {   0,  36,  36 },
 {  13,  57,  57 },
 {  26,  72,  60 },
 {  33,  81,  63 },
 {  38,  89,  65 },
 {  41,  94,  68 },
 {  49, 103,  71 },
 {  55, 112,  77 },
 {  64, 121,  81 },
 {  76, 132,  85 },
 {  93, 137,  97 },
 { 119, 152, 109 },
 { 139, 173, 122 },
 { 209, 195, 137 },
 { 279, 216,   0 },
 { 419, 237,   0 }
};

/* N_CS for preamble formats with £Gf_RA = 15*2^u KHz */
int Table_6_3_3_1_7[16] = {
  0,
  2,
  4,
  6,
  8,
 10,
 12,
 13,
 15,
 17,
 19,
 23,
 27,
 34,
 46,
 69
};


void help(void)
{
    printf("Usage: cyclic_shift [OPTION]...\n");
    printf("\n");
    printf("  -f value   Preamble format (0, 1, 2, 3 for L839).\n");
    printf("                             (A1, A2, A3, B1, B2, B3, B4, C0, C2 for L139).\n");
    printf("  -s value   Restricted set (0: none, 1: type A, 2: type B).\n");
    printf("  -i value   Logical root squence index (0 ~ 137, 0 ~ 837).\n");
    printf("  -z value   Zero correlation zone config (0 ~ 15).\n");
    printf("  -h         Show the help message.\n");
    printf("\n");
}

/* 38.211 Section 6.3.3 Physical random-access channel */
int main(int argc, char *argv[])
{
    char *format = "A2";
    int logicalIndex = 0;
    int restrictedSet = 0;
    int zeroCorrZone = 0;

    int Cv = 0;
    int L_RA;
    int N_CS;
    int q;
    int u;
    int du;
    int w;
    int b_n_RA_shift;
    int bb_n_RA_shift;
    int bbb_n_RA_shift;
    int n_RA_shift;
    int n_RA_group;
    int bb_d_start;
    int bbb_d_start;
    int d_start;
    int temp;
    int v_last;
    int v;

    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "f:s:i:z:h")) != -1)
    {
        switch ( ch )
        {
            case 'f':
                format = optarg;
                break;
            case 's':
                restrictedSet = atoi( optarg );
                if ((restrictedSet != RESTRICTED_SET_NONE) &&
                    (restrictedSet != RESTRICTED_SET_A) &&
                    (restrictedSet != RESTRICTED_SET_B))
                {
                    printf("ERR: wrong restricted set %s\n\n", optarg);
                    return 0;
                }
                break;
            case 'i':
                logicalIndex = atoi( optarg );
                break;
            case 'z':
                zeroCorrZone = atoi( optarg );
                if ((zeroCorrZone < 0) || (zeroCorrZone > 15))
                {
                    printf("ERR: wrong zeroCorrZone %s\n\n", optarg);
                    return 0;
                }
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    switch ( format[0] )
    {
        case '0':
        case '1':
        case '2':
            L_RA = 839;
            if ((logicalIndex < 0) || (logicalIndex > 837))
            {
                printf("ERR: wrong logicalIndex %d\n\n", logicalIndex);
                return 0;
            }
            N_CS = Table_6_3_3_1_5[ zeroCorrZone ][ restrictedSet ];
            u = Table_6_3_3_1_3[ logicalIndex ];
            break;
        case '3':
            L_RA = 839;
            if ((logicalIndex < 0) || (logicalIndex > 837))
            {
                printf("ERR: wrong logicalIndex %d\n\n", logicalIndex);
                return 0;
            }
            N_CS = Table_6_3_3_1_6[ zeroCorrZone ][ restrictedSet ];
            u = Table_6_3_3_1_3[ logicalIndex ];
            break;
        case 'A':
        case 'B':
        case 'C':
            L_RA = 139;
            if ((logicalIndex < 0) || (logicalIndex > 137))
            {
                printf("ERR: wrong logicalIndex %d\n\n", logicalIndex);
                return 0;
            }
            N_CS = Table_6_3_3_1_7[ zeroCorrZone ];
            u = Table_6_3_3_1_4[ logicalIndex ];
            break;
        default:
            printf("ERR: wrong preamble format %s\n\n", format);
            return 0;
    }

    q = 0;
    for (;;)
    {
        if (1 == ((q * u) % L_RA)) break;
        q++;
    }

    if ((0 <= q) && (q < (L_RA / 2)))
    {
        du = q;
    }
    else
    {
        du = (L_RA - q);
    }

    if (RESTRICTED_SET_A == restrictedSet)
    {
        if ((N_CS <= du) && (du < (L_RA / 3)))
        {
            n_RA_shift = DIV_FLOOR(du, N_CS);
            d_start = ((2 * du) + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(L_RA, d_start);
            temp = DIV_FLOOR((L_RA - (2 * du) - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MAX(temp, 0);
        }
        else if (((L_RA / 3) <= du) && (du <= ((L_RA - N_CS) / 2)))
        {
            n_RA_shift = DIV_FLOOR((L_RA - (2 * du)), N_CS);
            d_start = (L_RA - (2 * du) + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(du, d_start);
            temp = DIV_FLOOR((du - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MIN(MAX(temp, 0), n_RA_shift);
        }
        else
        {
            printf("ERR: wrong du value %d (A)\n\n", du);
            return 0;
        }

        w = (n_RA_shift * n_RA_group * b_n_RA_shift);

        printf("[1;33mCv[0m =\n");
        v_last = w;
        for (v=0; v<v_last; v++)
        {
            Cv = ((d_start * DIV_FLOOR(v, n_RA_shift)) + ((v % n_RA_shift) * N_CS));
            printf(" %d", Cv);
        }
        printf("\n");
        printf("([1;35m%d[0m cyclic shifts)\n", v_last);
        printf("\n");
    }
    else if (RESTRICTED_SET_B == restrictedSet)
    {
        if ((N_CS <= du) && (du < (L_RA / 5)))
        {
            n_RA_shift = DIV_FLOOR(du, N_CS);
            d_start = ((4 * du) + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(L_RA, d_start);
            temp = DIV_FLOOR((L_RA - (4 * du) - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MAX(temp, 0);
        }
        else if (((L_RA / 5) <= du) && (du <= ((L_RA - N_CS) / 4)))
        {
            n_RA_shift = DIV_FLOOR((L_RA - (4 * du)), N_CS);
            d_start = (L_RA - (4 * du) + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(du, d_start);
            temp = DIV_FLOOR((du - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MIN(MAX(temp, 0), n_RA_shift);
        }
        else if ((((L_RA + N_CS) / 4) <= du) && (du < ((2 * L_RA) / 7)))
        {
            n_RA_shift = DIV_FLOOR(((4 * du) - L_RA), N_CS);
            d_start = ((4 * du) - L_RA + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(du, d_start);
            temp = DIV_FLOOR((L_RA - (3 * du) - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MAX(temp, 0);
            temp = MIN((du - (n_RA_group * d_start)), ((4 * du) - L_RA - (b_n_RA_shift * N_CS)));
            bb_n_RA_shift = DIV_FLOOR(temp, N_CS);
            temp = (((1 - MIN(1, b_n_RA_shift)) * (du - (n_RA_group * d_start))) +
                    (MIN(1, b_n_RA_shift) * ((4 * du) - L_RA - (b_n_RA_shift * N_CS))));
            bbb_n_RA_shift = (DIV_FLOOR(temp, N_CS) - bb_n_RA_shift);
            bb_d_start = (L_RA - (3 * du) + (n_RA_group * d_start) + (b_n_RA_shift * N_CS));
            bbb_d_start = (L_RA - (2 * du) + (n_RA_group * d_start) + (bb_n_RA_shift * N_CS));
        }
        else if ((((2 * L_RA) / 7) <= du) && (du <= ((L_RA - N_CS) / 3)))
        {
            n_RA_shift = DIV_FLOOR((L_RA - (3 * du)), N_CS);
            d_start = (L_RA - (3 * du) + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(du, d_start);
            temp = DIV_FLOOR(((4 * du) - L_RA - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MAX(temp, 0);
            bb_d_start = (du + (n_RA_group * d_start) + (b_n_RA_shift * N_CS));
            bbb_d_start = 0;
            temp = MIN((du - (n_RA_group * d_start)), (L_RA - (3 * du) - (b_n_RA_shift * N_CS)));
            bb_n_RA_shift = DIV_FLOOR(temp, N_CS);
            bbb_n_RA_shift = 0;
        }
        else if ((((L_RA + N_CS) / 3) <= du) && (du < ((2 * L_RA) / 5)))
        {
            n_RA_shift = DIV_FLOOR(((3 * du) - L_RA), N_CS);
            d_start = ((3 * du) - L_RA + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR(du, d_start);
            temp = DIV_FLOOR((L_RA - (2 * du) - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MAX(temp, 0);
            bb_d_start = 0;
            bbb_d_start = 0;
            bb_n_RA_shift = 0;
            bbb_n_RA_shift = 0;
        }
        else if ((((2 * L_RA) / 5) <= du) && (du <= ((L_RA - N_CS) / 2)))
        {
            n_RA_shift = DIV_FLOOR((L_RA - (2 * du)), N_CS);
            d_start = ((2 * (L_RA - (2 * du))) + (n_RA_shift * N_CS));
            n_RA_group = DIV_FLOOR((L_RA - du), d_start);
            temp = DIV_FLOOR(((3 * du) - L_RA - (n_RA_group * d_start)), N_CS);
            b_n_RA_shift = MAX(temp, 0);
            bb_d_start = 0;
            bbb_d_start = 0;
            bb_n_RA_shift = 0;
            bbb_n_RA_shift = 0;
        }
        else
        {
            printf("ERR: wrong du value %d (B)\n\n", du);
            return 0;
        }

        w = (n_RA_shift * n_RA_group * b_n_RA_shift);

        printf("[1;33mCv[0m =\n");
        v_last = w;
        for (v=0; v<v_last; v++)
        {
            Cv = ((d_start * DIV_FLOOR(v, n_RA_shift)) + ((v % n_RA_shift) * N_CS));
            printf(" %d", Cv);
        }
        printf("\n");
        printf("([1;35m%d[0m cyclic shifts)\n", v_last);

        v_last = (w + bb_n_RA_shift);
        for (v=w; v<v_last; v++)
        {
            Cv = ((d_start * DIV_FLOOR(v, n_RA_shift)) + ((v % n_RA_shift) * N_CS));
            printf(" %d", Cv);
        }
        printf("\n");
        printf("([1;35m%d[0m cyclic shifts)\n", (v_last - w));

        v_last = (w + bb_n_RA_shift + bbb_n_RA_shift);
        for (v=(w + bb_n_RA_shift); v<v_last; v++)
        {
            Cv = ((d_start * DIV_FLOOR(v, n_RA_shift)) + ((v % n_RA_shift) * N_CS));
            printf(" %d", Cv);
        }
        printf("\n");
        printf("([1;35m%d[0m cyclic shifts)\n", (v_last - (w + bb_n_RA_shift)));
        printf("\n");
    }
    else
    {
        w = 0;

        if (0 == N_CS)
        {
            Cv = 0;
            printf("[1;33mCv[0m = %d\n", Cv);
            printf("([1;35m1[0m cyclic shift)\n");
            printf("\n");
        }
        else
        {
            printf("[1;33mCv[0m =\n");
            v_last = DIV_FLOOR(L_RA, N_CS);
            for (v=0; v<v_last; v++)
            {
                Cv = (v * N_CS);
                printf(" %d", Cv);
            }
            printf("\n");
            printf("([1;35m%d[0m cyclic shifts)\n", v_last);
            printf("\n");
        }
    }


    printf("format = %s\n", format);
    printf("logicalIndex = %d\n", logicalIndex);
    printf("restrictedSet = %s\n",
        (restrictedSet == RESTRICTED_SET_A) ? "type A" :
        (restrictedSet == RESTRICTED_SET_B) ? "type B" :
        "none"
    );
    printf("zeroCorrZone = %d\n", zeroCorrZone);
    printf("L_RA = %d\n", L_RA);
    printf("N_CS = %d\n", N_CS);
    printf("u = %d\n", u);
    printf("w = %d\n", w);
    printf("\n");

    return 0;
}

