#include <stdio.h>
#include <sys/stat.h>
#include "crc32.h"

#define TEST_PATTERN  (1)

#define MAX_FILE_SIZE (0x1000000)  // 16MB


static long _getFileSize(char *pFileName)
{
    struct stat stat_buf;

    if ( !stat(pFileName, &stat_buf) )
    {
        return stat_buf.st_size;
    }

    return 0;
}

int main(int argc, char *argv[])
{
#if (TEST_PATTERN)
    uint8   data[] = "123456789";
    uint32  len = 9;
    uint32  crc = 0; /* 0xcbf43926 */

    crc = crc32(0, data, len);

    printf("CRC 0x%08x (size %d)\n\n", crc, len);

    return 0;
#else
    FILE *pInput  = NULL;
    int   fileSize;
    uint8  *pData = NULL;
    uint32  len;
    uint32  crc = 0;


    if (argc < 2)
    {
        printf("Usage: crc_test file_name\n");
        printf("\n");
        goto _EXIT;
    }

    fileSize = _getFileSize( argv[1] );
    if ((fileSize == 0) || (fileSize > MAX_FILE_SIZE))
    {
        printf("Exit: illegal file size %d\n", fileSize);
        printf("\n");
        goto _EXIT;
    }

    if ((pInput=fopen(argv[1], "rb")) == NULL)
    {
        printf("Exit: cannot open file '%s'\n", argv[1]);
        printf("\n");
        goto _EXIT;
    }

    len = fileSize;
    pData = malloc( len );
    if ( pData )
    {
        memset(pData, 0x00, len);

        fread(pData, fileSize, 1, pInput);

        crc = crc32(0, pData, len);

        printf("CRC 0x%08x (size %d)\n\n", crc, len);
    }

_EXIT:
    if ( pInput  ) fclose( pInput );
    if ( pData ) free( pData );
    return 0;
#endif
}

