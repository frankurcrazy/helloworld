#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void http_post(int id)
{
    CURL *pCurl;
    CURLcode res;

    struct curl_slist *pHdr;
    char  *pName = "This is a movie";
    char  *pDirector = "Somebody";
    float  rating = 6.0;
    char   body[256];


    pCurl = curl_easy_init();
    if ( pCurl )
    {
        /* URL */
        curl_easy_setopt(
            pCurl,
            CURLOPT_URL,
            "http://localhost:3000/movies"
        );

        /* using POST method */
        curl_easy_setopt(pCurl, CURLOPT_POST, 1L);

        /* pass our list of custom made headers */
        pHdr = NULL;
        pHdr = curl_slist_append(pHdr, "Content-type: application/json");
        curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pHdr);

        /* Data */
        sprintf(
            body,
            "{ \"id\":%d, \"name\":\"%s\", \"director\":\"%s\", \"rating\":%f }",
            id,
            pName,
            pDirector,
            rating
        );
        curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, body);

        /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
        itself */
        curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, (long)strlen( body ));

        /* Perform the request, res will get the return code */
        res = curl_easy_perform( pCurl );
        /* Check for errors */
        if (res != CURLE_OK)
        {
            printf(
                "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res)
            );
        }

        if ( pHdr )
        {
            curl_slist_free_all( pHdr );
        }

        /* always cleanup */
        curl_easy_cleanup( pCurl );
    }
}


static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = (size * nmemb);
    char *pBuf;

    if (realsize > 0)
    {
        pBuf = malloc(realsize + 1);

        memcpy(pBuf, ptr, realsize);
        pBuf[realsize] = 0;

        printf("\n");
        printf("%s\n", pBuf);
        printf("\n");

        free( pBuf );
    }

    return realsize;
}

void http_get(int id)
{
    CURL *pCurl;
    CURLcode res;

    struct curl_slist *pHdr;
    char  url[256]; 


    pCurl = curl_easy_init();
    if ( pCurl )
    {
        curl_easy_setopt(
            pCurl,
            CURLOPT_WRITEFUNCTION,
            write_callback
        );
        /* pass user data to the callback function */
        curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, NULL);

        /* URL */
        sprintf(url, "http://localhost:3000/movies/%d", id);
        curl_easy_setopt(
            pCurl,
            CURLOPT_URL,
            url
        );

        /* using GET method */
        curl_easy_setopt(pCurl, CURLOPT_HTTPGET, 1L);

        /* pass our list of custom made headers */
        pHdr = NULL;
        pHdr = curl_slist_append(pHdr, "Content-type: application/json");
        curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pHdr);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform( pCurl );
        /* Check for errors */
        if (res != CURLE_OK)
        {
            printf(
                "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res)
            );
        }

        if ( pHdr )
        {
            curl_slist_free_all( pHdr );
        }

        /* always cleanup */
        curl_easy_cleanup( pCurl );
    }
}


void help(void)
{
    printf("Usage: curl_test [OPTION]...\n");
    printf("\n");
    printf("  -g id   GET method with a database ID.\n");
    printf("  -p id   POST method with a database ID.\n");
    printf("  -h      Show the help message.\n");
    printf("\n");
}

int main(int argc, char *argv[])
{
    int post = 0;
    int get  = 0;
    int id   = 1;
    int ch;

    opterr = 0;
    while ((ch=getopt(argc, argv, "p:g:h")) != -1)
    {
        switch ( ch )
        {
            case 'p':
                id = atoi( optarg );
                post = 1;
                break;
            case 'g':
                id = atoi( optarg );
                get = 1;
                break;
            case 'h':
            default:
                help();
                return 0;
        }
    }


    curl_global_init( CURL_GLOBAL_ALL );

    if ( get  ) http_get( id );
    if ( post ) http_post( id );

    curl_global_cleanup();

    return 0;
}


