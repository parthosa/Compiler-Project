/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/

#ifndef LANGDEF_H_INCLUDED
#define LANGDEF_H_INCLUDED

#include <stdlib.h>
#define MAX_LENGTH 20
#define BUFF_SIZE 64

typedef struct
{
    int id;
    char name[MAX_LENGTH+1];
    int line;
} TokenInfo;

int line;

#endif