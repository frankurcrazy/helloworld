#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[])
{
   FILE *pFile = NULL;
   unsigned char  byte;
   unsigned int   size;
   unsigned int   i;


   if (argc != 3)
   {
      printf("Usage: binmega FILE_OUT MEGA_BYTES\n");
      printf("\n");
      return 0;
   }

   if ((pFile=fopen(argv[1], "wb")) == NULL)
   {
      printf("Exit: cannot open file %s\n", argv[1]);
      printf("\n");
      return -1;
   }

   size = (atoi( argv[2] ) << 20);

   srand( (int)time(0) );
   for (i=0; i<size; i++)
   {
      byte = (unsigned char)(256.0 * rand() / (RAND_MAX));
      fwrite(&byte, 1, 1, pFile);
   }

   fclose( pFile );

   printf("Create binary file with %u bytes\n", size);

   return 0;
}

