#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int is_case_A_symbol(int slot, int symb, int under3ghz)
{
#define CHECK_INDEX_A(slot, index) \
    ((((2 + (14 * slot)) <= index) && (index <= ( 5 + (14 * slot)))) || \
     (((8 + (14 * slot)) <= index) && (index <= (11 + (14 * slot)))))

    int index = ((slot * 14) + symb);
    int retval = 0;

    if ( under3ghz )
    {
        if ((0 == slot) || (1 == slot))
        {
            retval = CHECK_INDEX_A(slot, index);
        }
    }
    else
    {
        if ((0 == slot) || (1 == slot) || (2 == slot) || (3 == slot))
        {
            retval = CHECK_INDEX_A(slot, index);
        }
    }

    return retval;
}

void show_case_A(void)
{
    int Lmax = 0;
    int slot;
    int symb;
    int i;

    printf("[1;36mCase A[0m: 15KHz (1 slot per 1ms)\n\n");

    printf("f <= 3GHz\n");
    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 15KHz: 1ms has 1 slot */
        slot = i;
        printf(" [slot %d]", slot);
        for (symb=0; symb<14; symb++)
        {
            if ( is_case_A_symbol(slot, symb, 1) )
            {
                printf(" [1;31m%2d[0m", symb);
                Lmax++;
            }
            else
            {
                printf(" %2d", symb);
            }
        }
        printf("\n");
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");

    Lmax = 0;

    printf("3GHz < f <= 6GHz\n");
    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 15KHz: 1ms has 1 slot */
        slot = i;
        printf(" [slot %d]", slot);
        for (symb=0; symb<14; symb++)
        {
            if ( is_case_A_symbol(slot, symb, 0) )
            {
                printf(" [1;31m%2d[0m", symb);
                Lmax++;
            }
            else
            {
                printf(" %2d", symb);
            }
        }
        printf("\n");
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");
}


int is_case_B_symbol(int slot, int symb, int under3ghz)
{
#define CHECK_INDEX_B(slot, index) \
    (((( 4 + (28 * slot)) <= index) && (index <= ( 7 + (28 * slot)))) || \
     ((( 8 + (28 * slot)) <= index) && (index <= (11 + (28 * slot)))) || \
     (((16 + (28 * slot)) <= index) && (index <= (19 + (28 * slot)))) || \
     (((20 + (28 * slot)) <= index) && (index <= (23 + (28 * slot)))))

    int index = ((slot * 14) + symb);
    int retval = 0;

    slot >>= 1;
    if ( under3ghz )
    {
        if (0 == slot)
        {
            retval = CHECK_INDEX_B(slot, index);
        }
    }
    else
    {
        if ((0 == slot) || (1 == slot))
        {
            retval = CHECK_INDEX_B(slot, index);
        }
    }

    return retval;
}

void show_case_B(void)
{
    int Lmax = 0;
    int slot;
    int symb;
    int i;
    int j;

    printf("[1;36mCase B[0m: 30KHz (2 slots per 1ms)\n\n");

    printf("f <= 3GHz\n");
    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 30KHz: 1ms has 2 slots */
        for (j=0; j<2; j++)
        {
            slot = ((i * 2) + j);
            printf(" [slot %d]", slot);
            for (symb=0; symb<14; symb++)
            {
                if ( is_case_B_symbol(slot, symb, 1) )
                {
                    printf(" [1;31m%2d[0m", symb);
                    Lmax++;
                }
                else
                {
                    printf(" %2d", symb);
                }
            }
            printf("\n");
        }
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");

    Lmax = 0;

    printf("3GHz < f <= 6GHz\n");
    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 30KHz: 1ms has 2 slots */
        for (j=0; j<2; j++)
        {
            slot = ((i * 2) + j);
            printf(" [slot %d]", slot);
            for (symb=0; symb<14; symb++)
            {
                if ( is_case_B_symbol(slot, symb, 0) )
                {
                    printf(" [1;31m%2d[0m", symb);
                    Lmax++;
                }
                else
                {
                    printf(" %2d", symb);
                }
            }
            printf("\n");
        }
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");
}


int is_case_C_symbol(int slot, int symb, int under3ghz)
{
#define CHECK_INDEX_C(slot, index) \
    (((( 2 + (14 * slot)) <= index) && (index <= ( 5 + (14 * slot)))) || \
     ((( 8 + (14 * slot)) <= index) && (index <= (11 + (14 * slot)))))

    int index = ((slot * 14) + symb);
    int retval = 0;

    if ( under3ghz )
    {
        if ((0 == slot) || (1 == slot))
        {
            retval = CHECK_INDEX_C(slot, index);
        }
    }
    else
    {
        if ((0 == slot) || (1 == slot) || (2 == slot) || (3 == slot))
        {
            retval = CHECK_INDEX_C(slot, index);
        }
    }

    return retval;
}

void show_case_C(void)
{
    int Lmax = 0;
    int slot;
    int symb;
    int i;
    int j;

    printf("[1;36mCase C[0m: 30KHz (2 slots per 1ms)\n\n");

    printf("f <= 3GHz\n");
    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 30KHz: 1ms has 2 slots */
        for (j=0; j<2; j++)
        {
            slot = ((i * 2) + j);
            printf(" [slot %d]", slot);
            for (symb=0; symb<14; symb++)
            {
                if ( is_case_C_symbol(slot, symb, 1) )
                {
                    printf(" [1;31m%2d[0m", symb);
                    Lmax++;
                }
                else
                {
                    printf(" %2d", symb);
                }
            }
            printf("\n");
        }
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");

    Lmax = 0;

    printf("3GHz < f <= 6GHz\n");
    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 30KHz: 1ms has 2 slots */
        for (j=0; j<2; j++)
        {
            slot = ((i * 2) + j);
            printf(" [slot %d]", slot);
            for (symb=0; symb<14; symb++)
            {
                if ( is_case_C_symbol(slot, symb, 0) )
                {
                    printf(" [1;31m%2d[0m", symb);
                    Lmax++;
                }
                else
                {
                    printf(" %2d", symb);
                }
            }
            printf("\n");
        }
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");
}


int is_case_D_symbol(int slot, int symb)
{
#define CHECK_INDEX_D(slot, index) \
    (((( 4 + (28 * slot)) <= index) && (index <= ( 7 + (28 * slot)))) || \
     ((( 8 + (28 * slot)) <= index) && (index <= (11 + (28 * slot)))) || \
     (((16 + (28 * slot)) <= index) && (index <= (19 + (28 * slot)))) || \
     (((20 + (28 * slot)) <= index) && (index <= (23 + (28 * slot)))))

    int index = ((slot * 14) + symb);
    int retval = 0;

    slot >>= 1;
    if ((0  == slot) || (1  == slot) || (2  == slot) || (3  == slot) ||
        (5  == slot) || (6  == slot) || (7  == slot) || (8  == slot) ||
        (10 == slot) || (11 == slot) || (12 == slot) || (13 == slot) ||
        (15 == slot) || (16 == slot) || (17 == slot) || (18 == slot))
    {
        retval = CHECK_INDEX_D(slot, index);
    }

    return retval;
}

void show_case_D(void)
{
    int Lmax = 0;
    int slot;
    int symb;
    int i;
    int j;

    printf("[1;36mCase D[0m: 120KHz (8 slots per 1ms)\n\n");

    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 120KHz: 1ms has 8 slots */
        for (j=0; j<8; j++)
        {
            slot = ((i * 8) + j);
            printf(" [slot %2d]", slot);
            for (symb=0; symb<14; symb++)
            {
                if ( is_case_D_symbol(slot, symb) )
                {
                    printf(" [1;31m%2d[0m", symb);
                    Lmax++;
                }
                else
                {
                    printf(" %2d", symb);
                }
            }
            printf("\n");
        }
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");
}


int is_case_E_symbol(int slot, int symb)
{
#define CHECK_INDEX_E(slot, index) \
    (((( 8 + (56 * slot)) <= index) && (index <= (11 + (56 * slot)))) || \
     (((12 + (56 * slot)) <= index) && (index <= (15 + (56 * slot)))) || \
     (((16 + (56 * slot)) <= index) && (index <= (19 + (56 * slot)))) || \
     (((20 + (56 * slot)) <= index) && (index <= (23 + (56 * slot)))) || \
     (((32 + (56 * slot)) <= index) && (index <= (35 + (56 * slot)))) || \
     (((36 + (56 * slot)) <= index) && (index <= (39 + (56 * slot)))) || \
     (((40 + (56 * slot)) <= index) && (index <= (43 + (56 * slot)))) || \
     (((44 + (56 * slot)) <= index) && (index <= (47 + (56 * slot)))))

    int index = ((slot * 14) + symb);
    int retval = 0;

    slot >>= 2;
    if ((0 == slot) || (1 == slot) || (2 == slot) || (3 == slot) ||
        (5 == slot) || (6 == slot) || (7 == slot) || (8 == slot))
    {
        retval = CHECK_INDEX_E(slot, index);
    }

    return retval;
}

void show_case_E(void)
{
    int Lmax = 0;
    int slot;
    int symb;
    int i;
    int j;

    printf("[1;36mCase E[0m: 240KHz (16 slots per 1ms)\n\n");

    for (i=0; i<5; i++)
    {
        printf("+\n");
        /* 240KHz: 1ms has 16 slots */
        for (j=0; j<16; j++)
        {
            slot = ((i * 16) + j);
            printf(" [slot %2d]", slot);
            for (symb=0; symb<14; symb++)
            {
                if ( is_case_E_symbol(slot, symb) )
                {
                    printf(" [1;31m%2d[0m", symb);
                    Lmax++;
                }
                else
                {
                    printf(" %2d", symb);
                }
            }
            printf("\n");
        }
    }
    printf("+\n");
    printf("Lmax = [1;33m%d[0m\n", (Lmax / 4));
    printf("\n");
}


void help(void)
{
    printf("Usage: time_loc [OPTION]...\n");
    printf("\n");
    printf("  -a   Case A ( 15KHz SCS).\n");
    printf("  -b   Case B ( 30KHz SCS).\n");
    printf("  -c   Case C ( 30KHz SCS).\n");
    printf("  -d   Case D (120KHz SCS).\n");
    printf("  -e   Case E (240KHz SCS).\n");
    printf("  -h   Show the help message.\n");
    printf("\n");
}

/* 38.213 Section 4.1 Cell search */
int main(int argc, char *argv[])
{
    int aFlag = 0;
    int bFlag = 0;
    int cFlag = 0;
    int dFlag = 0;
    int eFlag = 0;
    int ch;


    opterr = 0;
    while ((ch=getopt(argc, argv, "abcdeh")) != -1)
    {
        switch ( ch )
        {
            case 'a':
                aFlag = 1;
                break;
            case 'b':
                bFlag = 1;
                break;
            case 'c':
                cFlag = 1;
                break;
            case 'd':
                dFlag = 1;
                break;
            case 'e':
                eFlag = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    if ( aFlag ) show_case_A();
    else if ( bFlag ) show_case_B();
    else if ( cFlag ) show_case_C();
    else if ( dFlag ) show_case_D();
    else if ( eFlag ) show_case_E();
    else show_case_A();


    return 0;
}

