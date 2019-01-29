#ifndef _JSON_DEMO_H_
#define _JSON_DEMO_H_


#define JSON_BOOLEAN "BOOLEAN"
#define JSON_STRING  "STRING"
#define JSON_INTEGER "INTEGER"
#define JSON_ARRAY   "ARRAY"
#define JSON_OBJECT  "OBJECT"
#define JSON_MEMBER_BOOLEAN "MEMBER_BOOLEAN"
#define JSON_MEMBER_STRING  "MEMBER_STRING"
#define JSON_MEMBER_INTEGER "MEMBER_INTEGER"
#define JSON_MEMBER_ARRAY   "MEMBER_ARRAY"


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef int  bool;

typedef struct _tObject
{
    bool  memberBoolean;
    char  memberString[80];
    int   memberInteger;
    int   memberArray[3];
    int   memberArrayNum;
} tObject;

typedef struct _tJsonDemo
{
    bool     boolean;
    char     string[80];
    int      integer;
    char     array[3][80];
    int      arrayNum;
    tObject *pObject;
} tJsonDemo;


#endif
