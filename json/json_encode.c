#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json/json.h>
#include "json_demo.h"


json_object *EncodeJsonObject(tJsonDemo *pJson)
{
    json_object *pTopObj = NULL;
    json_object *pObj1 = NULL;
    json_object *pObj2 = NULL;
    json_object *pObj3 = NULL;
    int  i;


    /* new a base object */
    pTopObj = json_object_new_object();
    if (NULL == pTopObj)
    {
        printf("ERR: fail to create object\n");
        goto _EXIT;
    }

    /* new a boolean */
    pObj1 = json_object_new_boolean( pJson->boolean );
    if (NULL == pObj1)
    {
        printf(
            "ERR: fail to create boolean object for \"%s\"\n",
            JSON_BOOLEAN
        );
        goto _EXIT;
    }
    json_object_object_add(pTopObj, JSON_BOOLEAN, pObj1);
    pObj1 = NULL;

    /* new a string */
    pObj1 = json_object_new_string( pJson->string );
    if (NULL == pObj1)
    {
        printf(
            "ERR: fail to create string object for \"%s\"\n",
            JSON_STRING
        );
        goto _EXIT;
    }
    json_object_object_add(pTopObj, JSON_STRING, pObj1);
    pObj1 = NULL;

    /* new a integer */
    pObj1 = json_object_new_int( pJson->integer );
    if (NULL == pObj1)
    {
        printf(
            "ERR: fail to create integer object for \"%s\"\n",
            JSON_INTEGER
        );
        goto _EXIT;
    }
    json_object_object_add(pTopObj, JSON_INTEGER, pObj1);
    pObj1 = NULL;

    /* new a array */
    pObj1 = json_object_new_array();
    if (NULL == pObj1)
    {
        printf("ERR: fail to create array object\n");
        goto _EXIT;
    }

    /* new a string and add to the array */
    for (i=0; i<pJson->arrayNum; i++)
    {
        pObj2 = json_object_new_string( pJson->array[i] );
        if (NULL == pObj2)
        {
            printf(
                "ERR: fail to create string object for \"%s\"\n",
                JSON_ARRAY
            );
            goto _EXIT;
        }
        json_object_array_add(pObj1, pObj2);
        pObj2 = NULL;
    }

    /* add array to the base object */
    json_object_object_add(pTopObj, JSON_ARRAY, pObj1);
    pObj1 = NULL;

    if ( pJson->pObject )
    {
        /* new an object */
        pObj1 = json_object_new_object();
        if (NULL == pObj1)
        {
            printf(
                "ERR: fail to create object for \"%s\"\n",
                JSON_OBJECT
            );
            goto _EXIT;
        }

        /* new a boolean and add to the object */
        pObj2 = json_object_new_boolean( pJson->pObject->memberBoolean );
        if (NULL == pObj2)
        {
            printf(
                "ERR: fail to create boolean object for \"%s\"\n",
                JSON_MEMBER_BOOLEAN
            );
            goto _EXIT;
        }
        json_object_object_add(pObj1, JSON_MEMBER_BOOLEAN, pObj2);
        pObj2 = NULL;

        /* new a string and add to the object */
        pObj2 = json_object_new_string( pJson->pObject->memberString );
        if (NULL == pObj2)
        {
            printf(
                "ERR: fail to create string object for \"%s\"\n",
                JSON_MEMBER_STRING
            );
            goto _EXIT;
        }
        json_object_object_add(pObj1, JSON_MEMBER_STRING, pObj2);
        pObj2 = NULL;

        /* new a integer and add to the object */
        pObj2 = json_object_new_int( pJson->pObject->memberInteger );
        if (NULL == pObj2)
        {
            printf(
                "ERR: fail to create integer object for \"%s\"\n",
                JSON_MEMBER_INTEGER
            );
            goto _EXIT;
        }
        json_object_object_add(pObj1, JSON_MEMBER_INTEGER, pObj2);
        pObj2 = NULL;

        /* new a array */
        pObj2 = json_object_new_array();
        if (NULL == pObj2)
        {
            printf("ERR: fail to create array object\n");
            goto _EXIT;
        }

        /* new a integer and add to array */
        for (i=0; i<pJson->arrayNum; i++)
        {
            pObj3 = json_object_new_int( pJson->pObject->memberArray[i] );
            if (NULL == pObj3)
            {
                printf(
                    "ERR: fail to create string object for \"%s\"\n",
                    JSON_MEMBER_ARRAY
                   );
                goto _EXIT;
            }
            json_object_array_add(pObj2, pObj3);
            pObj3 = NULL;
        }

        /* add array to the object */
        json_object_object_add(pObj1, JSON_MEMBER_ARRAY, pObj2);
        pObj2 = NULL;

        /* add the object to base object */
        json_object_object_add(pTopObj, JSON_OBJECT, pObj1);
        pObj1 = NULL;
    }

    return pTopObj;


_EXIT:
    json_object_put( pObj3 );
    json_object_put( pObj2 );
    json_object_put( pObj1 );
    json_object_put( pTopObj );
    return NULL;
}

int main(int argc, char *argv[])
{
    json_object *pObj;
    tJsonDemo  json;


    json.boolean = TRUE;
    sprintf(json.string, "Hello");
    json.integer = 100;
    sprintf(json.array[0], "alpha");
    sprintf(json.array[1], "bravo");
    sprintf(json.array[2], "charlie");
    json.arrayNum = 3;
    json.pObject = malloc( sizeof( tObject ));
    if ( json.pObject )
    {
        json.pObject->memberBoolean = FALSE;
        sprintf(json.pObject->memberString, "Goodbye");
        json.pObject->memberInteger = 200;
        json.pObject->memberArray[0] = 1;
        json.pObject->memberArray[1] = 6;
        json.pObject->memberArray[2] = 8;
        json.pObject->memberArrayNum = 3;
    }


    pObj = EncodeJsonObject( &json );
    if ( pObj )
    {
        printf("%s\n", json_object_to_json_string( pObj ));

        json_object_to_file("json_encode.txt", pObj);

        json_object_put( pObj );
    }

    if ( json.pObject )
    {
        free( json.pObject );
    }

    return 0;
}

