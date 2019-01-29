#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json/json.h>
#include "json_demo.h"


char g_jsonStr[] = 
"{"
"  \"BOOLEAN\": false,"
"  \"STRING\": \"JSON\","
"  \"INTEGER\": 123,"
"  \"ARRAY\": [ \"foo\", \"bar\", \"qux\" ],"
"  \"OBJECT\":"
"    {"
"      \"MEMBER_BOOLEAN\": true,"
"      \"MEMBER_STRING\": \"Happy new year !\","
"      \"MEMBER_INTEGER\": 456,"
"      \"MEMBER_ARRAY\": [ 2019, 1, 1 ]"
"    }"
"}";


int DecodeJsonObject(json_object *pTopObj, tJsonDemo *pJson)
{
    json_object *pObj1 = NULL;
    json_object *pObj2 = NULL;
    json_object *pObj3 = NULL;
    int  i;


    /* get boolean */
    pObj1 = json_object_object_get(pTopObj, JSON_BOOLEAN);
    if (NULL == pObj1)
    {
        printf("ERR: fail to get \"%s\" object\n", JSON_BOOLEAN);
        goto _EXIT;
    }
    pJson->boolean = json_object_get_boolean( pObj1 );

    /* get string */
    pObj1 = json_object_object_get(pTopObj, JSON_STRING);
    if (NULL == pObj1)
    {
        printf("ERR: fail to get \"%s\" object\n", JSON_STRING);
        goto _EXIT;
    }
    strcpy(pJson->string, json_object_get_string( pObj1 ));

    /* get integer */
    pObj1 = json_object_object_get(pTopObj, JSON_INTEGER);
    if (NULL == pObj1)
    {
        printf("ERR: fail to get \"%s\" object\n", JSON_INTEGER);
        goto _EXIT;
    }
    pJson->integer = json_object_get_int( pObj1 );

    /* get array */
    pObj1 = json_object_object_get(pTopObj, JSON_ARRAY);
    if (NULL == pObj1)
    {
        printf("ERR: fail to get \"%s\" object\n", JSON_ARRAY);
        goto _EXIT;
    }

    /* get the length of the array */
    pJson->arrayNum = json_object_array_length( pObj1 );

    /* get the value of array[] */
    for (i=0; i<pJson->arrayNum; i++)
    {
        pObj2 = json_object_array_get_idx(pObj1, i);
        strcpy(pJson->array[i], json_object_get_string( pObj2 ));
    }

    /* get object */
    pObj1 = json_object_object_get(pTopObj, JSON_OBJECT);
    if (NULL == pObj1)
    {
        printf("ERR: fail to get \"%s\" object\n", JSON_OBJECT);
        goto _EXIT;
    }

    /* get boolean within a object */
    pObj2 = json_object_object_get(pObj1, JSON_MEMBER_BOOLEAN);
    if (NULL == pObj2)
    {
        printf("ERR: fail to get \"%s\" object\n", JSON_MEMBER_BOOLEAN);
        goto _EXIT;
    }

    pJson->pObject = malloc( sizeof( tObject ));
    if ( pJson->pObject )
    {
        pJson->pObject->memberBoolean = json_object_get_boolean( pObj2 );

        /* get string within a object */
        pObj2 = json_object_object_get(pObj1, JSON_MEMBER_STRING);
        if (NULL == pObj2)
        {
            printf("ERR: fail to get \"%s\" object\n", JSON_MEMBER_STRING);
            goto _EXIT;
        }
        strcpy(pJson->pObject->memberString, json_object_get_string( pObj2 ));

        /* get integer within a object */
        pObj2 = json_object_object_get(pObj1, JSON_MEMBER_INTEGER);
        if (NULL == pObj2)
        {
            printf("ERR: fail to get \"%s\" object\n", JSON_MEMBER_INTEGER);
            goto _EXIT;
        }
        pJson->pObject->memberInteger = json_object_get_int( pObj2 );

        /* get array within a object */
        pObj2 = json_object_object_get(pObj1, JSON_MEMBER_ARRAY);
        if (NULL == pObj2)
        {
            printf("ERR: fail to get \"%s\" object\n", JSON_MEMBER_ARRAY);
            goto _EXIT;
        }

        /* get the length of the array within a object */
        pJson->pObject->memberArrayNum = json_object_array_length( pObj2 );

        /* get the value of array[] within a object */
        for (i=0; i<pJson->pObject->memberArrayNum; i++)
        {
            pObj3 = json_object_array_get_idx(pObj2, i);
            pJson->pObject->memberArray[i] = json_object_get_int( pObj3 );
        }
    }

    return 0;


_EXIT:
    if ( pJson->pObject )
    {
        free( pJson->pObject );
        pJson->pObject = NULL;
    }
    json_object_put( pTopObj );
    return -1;
}

void PrintJsonStrcut(json_object *pObj)
{
    tJsonDemo  json;
    int  i;

    memset(&json, 0, sizeof( tJsonDemo ));
    if (0 == DecodeJsonObject(pObj, &json))
    {
        printf("boolean  = %s\n", (json.boolean ? "true" : "false"));
        printf("string   = \"%s\"\n", json.string);
        printf("integer  = %d\n", json.integer);
        for (i=0; i<json.arrayNum; i++)
        {
            printf("array[%d] = \"%s\"\n", i, json.array[i]);
        }
        if ( json.pObject )
        {
            printf("pObject->memberBoolean  = %s\n", (json.pObject->memberBoolean ? "true" : "false"));
            printf("pObject->memberString   = \"%s\"\n", json.pObject->memberString);
            printf("pObject->memberInteger  = %d\n", json.pObject->memberInteger);
            for (i=0; i<json.pObject->memberArrayNum; i++)
            {
                printf("pObject->memberArray[%d] = %d\n", i, json.pObject->memberArray[i]);
            }
            free( json.pObject );
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    json_object *pObj;

    pObj = json_object_from_file( "json_demo.txt" );
    if ( pObj )
    {
        PrintJsonStrcut( pObj );

        json_object_put( pObj );
    }

    pObj = json_tokener_parse( g_jsonStr );
    if ( pObj )
    {
        PrintJsonStrcut( pObj );

        json_object_put( pObj );
    }

    return 0;
}

