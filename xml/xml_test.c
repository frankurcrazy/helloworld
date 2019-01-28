#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>


// /////////////////////////////////////////////////////////////////////////////
//    Macro declarations
// /////////////////////////////////////////////////////////////////////////////

/* Tags */
#define TAG_NAME_FUNCTION  "FUNCTION"
#define TAG_NAME_ARGUMENT  "ARGUMENT"
#define TAG_NAME_RETURN    "RETURN"
#define TAG_NAME_DESCRIPT  "DESCRIPT"

/* Attributes */
#define ATT_NAME_NAME     "name"
#define ATT_NAME_TYPE     "type"
#define ATT_NAME_LAST     "last"
#define ATT_NAME_INCLUDE  "include"

/* Debug options */
#define DEBUG_SCAN_TAG   (0)
#define DEBUG_PARSE_XML  (0)


// /////////////////////////////////////////////////////////////////////////////
//    Type declarations
// /////////////////////////////////////////////////////////////////////////////

typedef int (*tProcTagCb)(xmlNode *, char *);


// /////////////////////////////////////////////////////////////////////////////
//    Variables declarations
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//    Functions
// /////////////////////////////////////////////////////////////////////////////

void ForEachTag(xmlNode *pParent, char *pTag, tProcTagCb pFunc);

#if (DEBUG_SCAN_TAG)
void ScanTag(xmlNode *pHead, int level)
{
    xmlNode *pNode;
    int  i;

    level++;
    for (pNode=pHead; pNode; pNode=pNode->next)
    {
        for (i=0; i<level; i++) printf("  ");
        printf("type: %d\n", pNode->type);

        for (i=0; i<level; i++) printf("  ");
        printf("name: %s\n", pNode->name);

        for (i=0; i<level; i++) printf("  ");
        if ( pNode->content )
        {
            printf("content: \"%s\"\n", pNode->content);
        }
        else
        {
            printf("content: (null)\n");
        }

        ScanTag(pNode->children, level);
    }
}
#endif

char *RemoveSpace(char *pIn)
{
    char *pOut = pIn;
    int i;

    for (i=0; i<strlen(pIn); i++)
    {
        if ((0x0A != *pOut) && (0x0D != *pOut) && (0x20 != *pOut))
        {
            break;
        }
        pOut++;
    }

    for (i=0; i<strlen(pOut); i++)
    {
        if ((0x0A == pOut[i]) || (0x0D == pOut[i]))
        {
            pOut[i] = 0x00;
            break;
        }
    }

    return pOut;
}

char *GetAttribute(
    xmlNode *pNode,
    char    *pTagName,
    char    *pAttrName
)
{
    xmlAttr *pAttr = NULL;
    int  found = 0;

    for (pAttr=pNode->properties; pAttr; pAttr=pAttr->next)
    {
        if (0 == strcmp((char *)pAttr->name, pAttrName))
        {
            found = 1;
            break;
        }
    }

    if ( !found )
    {
        printf(
            "<%s>'s attribute '%s' is missing\n",
            pTagName,
            pAttrName
        );
        return NULL;
    }

    if (NULL == pAttr->children)
    {
        printf(
            "<%s>'s attribute '%s' is empty\n",
            pTagName,
            pAttrName
        );
        return NULL;
    }

    return (char *)pAttr->children->content;
}

int ProcDescript(xmlNode *pNode, char *pName)
{
    char *pString;

    pString = (char *)xmlNodeGetContent( pNode );

    #if (DEBUG_PARSE_XML)
    printf("    + %s\n", pNode->name);
    printf("         \"%s\"\n", RemoveSpace( pString ));
    #else
    printf("%s\n\n", RemoveSpace( pString ));
    #endif

    xmlFree( pString );

    return 0;
}

int ProcReturn(xmlNode *pNode, char *pName)
{
    char *pString;

    pString = (char *)xmlNodeGetContent( pNode );

    #if (DEBUG_PARSE_XML)
    printf("    + %s\n", pNode->name);
    printf("         \"%s\"\n", RemoveSpace( pString ));
    #else
    printf(
        "%s  /* %s */\n",
        GetAttribute(pNode, pName, ATT_NAME_TYPE),
        RemoveSpace( pString )
    );
    #endif

    xmlFree( pString );

    return 0;
}

int ProcArgument(xmlNode *pNode, char *pName)
{
    char *pString;
    int last;

    pString = (char *)xmlNodeGetContent( pNode );

    last = atoi( GetAttribute(pNode, pName, ATT_NAME_LAST) );

    #if (DEBUG_PARSE_XML)
    printf("    + %s\n", pNode->name);
    printf("         \"%s\"\n", RemoveSpace( pString ));
    #else
    printf(
        "    %s %s%c  /* %s */\n",
        GetAttribute(pNode, pName, ATT_NAME_TYPE),
        GetAttribute(pNode, pName, ATT_NAME_NAME),
        (( last ) ? ' ' : ','),
        RemoveSpace( pString )
    );
    #endif

    xmlFree( pString );

    return 0;
}

int ProcFunction(xmlNode *pNode, char *pName)
{
    char *pString;


    pString = (char *)xmlNodeGetContent( pNode );

    #if (DEBUG_PARSE_XML)
    printf(" + %s\n", pNode->name);
    //printf("      \"%s\"\n", pString);
    #endif

    xmlFree( pString );


    ForEachTag(pNode, TAG_NAME_DESCRIPT, ProcDescript);

    #if !(DEBUG_PARSE_XML)
    printf("#include <libxml/%s>\n\n",
        GetAttribute(pNode, TAG_NAME_FUNCTION, ATT_NAME_INCLUDE)
    );
    #endif

    ForEachTag(pNode, TAG_NAME_RETURN, ProcReturn);

    #if !(DEBUG_PARSE_XML)
    printf("%s(\n",
        GetAttribute(pNode, pName, ATT_NAME_NAME)
    );
    #endif

    ForEachTag(pNode, TAG_NAME_ARGUMENT, ProcArgument);

    #if !(DEBUG_PARSE_XML)
    printf(");\n\n\n");
    #endif

    return 0;
}

void ForEachTag(xmlNode *pParent, char *pTag, tProcTagCb pFunc)
{
    xmlNode *pNode;

    for (pNode=pParent->children; pNode; pNode=pNode->next)
    {
        if (XML_ELEMENT_NODE == pNode->type)
        {
            if (0 == strcmp((char *)pNode->name, pTag))
            {
                pFunc(pNode, pTag);
            }
        }
    }
}

int ParseXML(xmlDoc *pXmlDoc)
{
    xmlNode *pNodeRoot = NULL;


    /* Get the root element node */
    pNodeRoot = xmlDocGetRootElement( pXmlDoc );
    if (NULL == pNodeRoot)
    {
        printf("ERR: could not get the root element node\n");
        return -1;
    }

    /*
    *  <LIBXML2>
    *    |-- <FUNCTION>
    *    |     |-- <ARGUMENT>
    *    |     `-- <RETURN>
    *    |     `-- <DESCRIPT>
    *    |-- <FUNCTION>
    *    `-- ...
    */


    #if (DEBUG_SCAN_TAG)

    printf("type: %d\n", pNodeRoot->type);
    printf("name: %s\n", pNodeRoot->name);
    if ( pNodeRoot->content )
    {
        printf("content: \"%s\"\n", pNodeRoot->content);
    }
    else
    {
        printf("content: (null)\n");
    }

    ScanTag(pNodeRoot->children, 0);

    #else


    #if (DEBUG_PARSE_XML)
    printf("%s\n", pNodeRoot->name);
    #endif

    ForEachTag(pNodeRoot, TAG_NAME_FUNCTION, ProcFunction);

    #endif

    return 0;
}

int main(int argc, char *argv[])
{
    xmlDoc *pXmlDoc = NULL;
    int  status;

    if (argc != 2)
    {
        printf("Usage: xml_test example.xml\n");
        printf("\n");
        return 0;
    }

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    pXmlDoc = xmlReadFile(argv[1], NULL, 0);
    if (NULL == pXmlDoc)
    {
        printf("ERR: cannot open file %s\n", argv[1]);
        printf("\n");
        return -1;
    }

    status = ParseXML( pXmlDoc );

    xmlFreeDoc( pXmlDoc );

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return 0;
}

