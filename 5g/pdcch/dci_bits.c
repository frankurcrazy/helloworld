#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


#define DIV_CEIL(X, Y)   (((X) + ((Y) - 1)) / (Y))
#define DIV_FLOOR(X, Y)  ((X) / (Y))


typedef struct _tDCI_0_0
{
    int N_UL_BWP_RB;
    int SUL_flag;
    int r_bits;
} tDCI_0_0;

int format_0_0(tDCI_0_0 *pDci)
{
    int bits;

    pDci->r_bits = (int)ceil( log2((pDci->N_UL_BWP_RB * (pDci->N_UL_BWP_RB + 1)) / 2) );

    bits = (1 + pDci->r_bits + 4 + 1 + 5 + 1 + 2 + 4 + 2);

    return bits;
}


typedef struct _tDCI_0_1
{
    int CI_flag;
    int SUL_flag;
    int n_BWP; /* 0 ~ 4 */
    int N_UL_BWP_RB;
    int res_type; /* 0, 1, 2 */
    int I; /* 1 ~ 16 */
    int hop_flag;
    int HARQ_codebook[2];
    int SRS_flag;
    int TPMI_bits; /* 0 ~ 6 */
    int ant_num; /* 2 ~ 5 */
    int CSI_req; /* 0 ~ 6 */
    int CBGTI_bits; /* 0, 2, 4, 6, 8 */
    int PTRS_flag;
    int beta_flag;
    int TP_flag;
    int r_bits;
} tDCI_0_1;

int format_0_1(tDCI_0_1 *pDci)
{
    int bits;

    {
        int P;
        int N_size_BWP = pDci->N_UL_BWP_RB;
        int N_start_BWP = 0;
        int N_RBG;
        int N_RIV;

        if ((N_size_BWP >= 145) && (N_size_BWP <= 275))
        {
            P = 16;
        }
        else if ((N_size_BWP >= 73) && (N_size_BWP <= 144))
        {
            P = 8;
        }
        else if ((N_size_BWP >= 37) && (N_size_BWP <= 72))
        {
            P = 4;
        }
        else
        {
            P = 2;
        }
        N_RBG = DIV_CEIL((N_size_BWP + (N_start_BWP % P)), P);
        N_RIV = (int)ceil( log2((N_size_BWP * (N_size_BWP + 1)) / 2) );

        if (0 == pDci->res_type)
        {
            pDci->r_bits = N_RBG;
        }
        else if (1 == pDci->res_type)
        {
            pDci->r_bits = N_RIV;
        }
        else
        {
            pDci->r_bits = (((N_RIV > N_RBG) ? N_RIV : N_RBG) + 1);
        }
    }

    bits  = 1;
    bits += (( pDci->CI_flag ) ? 3 : 0);
    bits += (( pDci->SUL_flag ) ? 1 : 0);
    if (pDci->n_BWP > 0)
    {
        bits += ((pDci->n_BWP < 4) ? (int)ceil( log2(pDci->n_BWP + 1) ) : 2);
    }
    bits += pDci->r_bits;
    bits += (int)ceil( log2(pDci->I) );
    bits += ((( pDci->hop_flag ) && (pDci->res_type > 0)) ? 1 : 0);
    bits += 5;
    bits += 1;
    bits += 2;
    bits += 4;
    bits += (( pDci->HARQ_codebook[0] ) ? 2 : 1);
    bits += (( pDci->HARQ_codebook[1] ) ? 2 : 0);
    bits += 2;
    bits += (( pDci->SRS_flag ) ? 1 : 0);
    bits += pDci->TPMI_bits;
    bits += pDci->ant_num;
    bits += 2;
    bits += pDci->CSI_req;
    bits += pDci->CBGTI_bits;
    bits += (( pDci->PTRS_flag ) ? 2 : 0);
    bits += (( pDci->beta_flag ) ? 2 : 0);
    bits += (( pDci->TP_flag ) ? 0 : 1);
    bits += 1;

    return bits;
}


typedef struct _tDCI_1_0
{
    char RNTI[16];
    int PDCCH_order;
    int N_DL_BWP_RB;
    int r_bits;
} tDCI_1_0;

int format_1_0(tDCI_1_0 *pDci)
{
    int bits;

    pDci->r_bits = (int)ceil( log2((pDci->N_DL_BWP_RB * (pDci->N_DL_BWP_RB + 1)) / 2) );

    /* format 1_0 has the same length regardless of RNTI */
    if (0 == strcmp("C", pDci->RNTI))
    {
        if ( pDci->PDCCH_order )
        {
            bits = (1 + pDci->r_bits + 6 + 1 + 6 + 4 + 10);
        }
        else
        {
            bits = (1 + pDci->r_bits + 4 + 1 + 5 + 1 + 2 + 4 + 2 + 2 + 3 + 3);
        }
    }
    else if (0 == strcmp("P", pDci->RNTI))
    {
        bits = (2 + 8 + pDci->r_bits + 4 + 1 + 5 + 2 + 6);
    }
    else if (0 == strcmp("SI", pDci->RNTI))
    {
        bits = (pDci->r_bits + 4 + 1 + 5 + 2 + 1 + 15);
    }
    else if (0 == strcmp("RA", pDci->RNTI))
    {
        bits = (pDci->r_bits + 4 + 1 + 5 + 2 + 16);
    }
    else if (0 == strcmp("TC", pDci->RNTI))
    {
        bits = (1 + pDci->r_bits + 4 + 1 + 5 + 1 + 2 + 4 + 2 + 2 + 3 +3);
    }
    else
    {
        printf("ERR: wrong RNTI '%s'\n\n", pDci->RNTI);
        bits = 0;
    }

    return bits;
}


typedef struct _tDCI_1_1
{
    int CI_flag;
    int n_BWP; /* 0 ~ 4 */
    int N_DL_BWP_RB;
    int res_type; /* 0, 1, 2 */
    int I; /* 1 ~ 16 */
    int VRB_flag;
    int bundle_flag;
    int rate_match_ind; /* 0, 1, 2 */
    int n_ZP; /* 0 ~ 16 */
    int mimo_flag;
    int DAI_bits; /* 0, 2, 4 */
    int n_UL_ACK; /* 0 ~ 8 */
    int ant_port_bits; /* 4, 5, 6 */
    int TCI_flag;
    int CBGTI_bits; /* 0, 2, 4, 6, 8 */
    int CBGFI_bits; /* 0, 1 */
    int r_bits;
} tDCI_1_1;

int format_1_1(tDCI_1_1 *pDci)
{
    int bits;

    {
        int P;
        int N_size_BWP = pDci->N_DL_BWP_RB;
        int N_start_BWP = 0;
        int N_RBG;
        int N_RIV;

        if ((N_size_BWP >= 145) && (N_size_BWP <= 275))
        {
            P = 16;
        }
        else if ((N_size_BWP >= 73) && (N_size_BWP <= 144))
        {
            P = 8;
        }
        else if ((N_size_BWP >= 37) && (N_size_BWP <= 72))
        {
            P = 4;
        }
        else
        {
            P = 2;
        }
        N_RBG = DIV_CEIL((N_size_BWP + (N_start_BWP % P)), P);
        N_RIV = (int)ceil( log2((N_size_BWP * (N_size_BWP + 1)) / 2) );

        if (0 == pDci->res_type)
        {
            pDci->r_bits = N_RBG;
        }
        else if (1 == pDci->res_type)
        {
            pDci->r_bits = N_RIV;
        }
        else
        {
            pDci->r_bits = (((N_RIV > N_RBG) ? N_RIV : N_RBG) + 1);
        }
    }

    bits  = 1;
    bits += (( pDci->CI_flag ) ? 3 : 0);
    if (pDci->n_BWP > 0)
    {
        bits += ((pDci->n_BWP < 4) ? (int)ceil( log2(pDci->n_BWP + 1) ) : 2);
    }
    bits += pDci->r_bits;
    bits += (int)ceil( log2(pDci->I) );
    bits += ((( pDci->VRB_flag ) && (pDci->res_type > 0)) ? 1 : 0);
    bits += (( pDci->bundle_flag ) ? 1 : 0);
    bits += pDci->rate_match_ind;
    bits += (int)ceil( log2(pDci->n_ZP + 1) );
    bits += 5;
    bits += 1;
    bits += 2;
    bits += (( pDci->mimo_flag ) ? 5 : 0);
    bits += (( pDci->mimo_flag ) ? 1 : 0);
    bits += (( pDci->mimo_flag ) ? 2 : 0);
    bits += 4;
    bits += pDci->DAI_bits;
    bits += 2;
    bits += 3;
    if (pDci->n_UL_ACK > 0)
    {
        bits += (int)ceil( log2(pDci->n_UL_ACK) );
    }
    bits += pDci->ant_port_bits;
    bits += (( pDci->TCI_flag ) ? 3 : 0);
    bits += 2;
    bits += pDci->CBGTI_bits;
    bits += pDci->CBGFI_bits;
    bits += 1;

    return bits;
}


void help(void)
{
    printf("Usage: dci_bits 0_0 N_UL_BWP_RB\n");
    printf("  0_0            DCI format 0_0.\n");
    printf("  N_UL_BWP_RB    RB number of initial UL BWP (1 ~ 273).\n");
    printf("\n");
    printf("Usage: dci_bits 0_1 N_UL_BWP_RB type \n");
    printf("  0_1            DCI format 0_1.\n");
    printf("  N_UL_BWP_RB    RB number of initial UL BWP (1 ~ 273).\n");
    printf("  type           Resource allocation type (0, 1, 2).\n");
    printf("\n");
    printf("Usage: dci_bits 1_0 N_DL_BWP_RB RNTI [RACH]\n");
    printf("  1_0            DCI format 1_0.\n");
    printf("  N_DL_BWP_RB    RB number of CORESET 0 or initial DL BWP (1 ~ 273).\n");
    printf("  RNTI           RNTI (C, P, SI, RA, TC).\n");
    printf("  RACH           PDCCH order (0, 1).\n");
    printf("\n");
    printf("Usage: dci_bits 1_1 N_DL_BWP_RB type \n");
    printf("  1_1            DCI format 1_1.\n");
    printf("  N_DL_BWP_RB    RB number of initial DL BWP (1 ~ 273).\n");
    printf("  type           Resource allocation type (0, 1, 2).\n");
    printf("\n");
}

/* 38.213 Section 7.3.1 DCI formats */
int main(int argc, char *argv[])
{
    int bits;


    if (argc > 1)
    {
        if (0 == strcmp("0_0", argv[1]))
        {
            tDCI_0_0 dci;

            dci.N_UL_BWP_RB = 100;
            dci.SUL_flag = 0;
            if (argc > 2) dci.N_UL_BWP_RB = atoi( argv[2] );
            dci.r_bits = 0;

            bits = format_0_0( &dci );
            printf("DCI: [1;31m%d[0m bits + padding\n", bits);
            printf("RES: %d bits\n\n", dci.r_bits);
        }
        else if (0 == strcmp("0_1", argv[1]))
        {
            tDCI_0_1 dci;

            dci.CI_flag = 0;
            dci.SUL_flag = 0;
            dci.n_BWP = 1;
            dci.N_UL_BWP_RB = 100;
            dci.res_type = 0;
            dci.I = 16;
            dci.hop_flag = 0;
            dci.HARQ_codebook[0] = 0;
            dci.HARQ_codebook[1] = 0;
            dci.SRS_flag = 0;
            dci.TPMI_bits = 0;
            dci.ant_num = 2;
            dci.CSI_req = 0;
            dci.CBGTI_bits = 0;
            dci.PTRS_flag = 0;
            dci.beta_flag = 0;
            dci.TP_flag = 0;
            if (argc > 2) dci.N_UL_BWP_RB = atoi( argv[2] );
            if (argc > 3) dci.res_type = atoi( argv[3] );
            dci.r_bits = 0;

            bits = format_0_1( &dci );
            printf("DCI: [1;31m%d[0m bits\n", bits);
            printf("RES: %d bits\n\n", dci.r_bits);
        }
        else if (0 == strcmp("1_0", argv[1]))
        {
            tDCI_1_0 dci;

            sprintf(dci.RNTI, "C");
            dci.PDCCH_order = 0;
            dci.N_DL_BWP_RB = 100;
            if (argc > 2) dci.N_DL_BWP_RB = atoi( argv[2] );
            if (argc > 3) strcpy(dci.RNTI, argv[3]);
            if (argc > 4) dci.PDCCH_order = atoi( argv[4] );
            dci.r_bits = 0;

            bits = format_1_0( &dci );
            printf("DCI: [1;31m%d[0m bits (%s-RNTI)\n", bits, dci.RNTI);
            printf("RES: %d bits\n\n", dci.r_bits);
        }
        else if (0 == strcmp("1_1", argv[1]))
        {
            tDCI_1_1 dci;

            dci.CI_flag = 0;
            dci.n_BWP = 1;
            dci.N_DL_BWP_RB = 100;
            dci.res_type = 0;
            dci.I = 16;
            dci.VRB_flag = 0;
            dci.bundle_flag = 0;
            dci.rate_match_ind = 0;
            dci.n_ZP = 0;
            dci.mimo_flag = 0;
            dci.DAI_bits = 0;
            dci.n_UL_ACK = 0;
            dci.ant_port_bits = 4;
            dci.TCI_flag = 0;
            dci.CBGTI_bits = 0;
            dci.CBGFI_bits = 0;
            if (argc > 2) dci.N_DL_BWP_RB = atoi( argv[2] );
            if (argc > 3) dci.res_type = atoi( argv[3] );
            dci.r_bits = 0;

            bits = format_1_1( &dci );
            printf("DCI: [1;31m%d[0m bits\n", bits);
            printf("RES: %d bits\n\n", dci.r_bits);
        }
        else
        {
            printf("ERR: wrong format '%s'\n\n", argv[1]);
            return 0;
        }
    }
    else
    {
        help();
    }

    return 0;
}

