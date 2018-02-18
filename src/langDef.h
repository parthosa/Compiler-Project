#ifndef LANGDEF_H_INCLUDED
#define LANGDEF_H_INCLUDED

#include <stdlib.h>
#define MAX_LENGTH 20
#define BUFF_SIZE 64

typedef struct
{
    int id;
    char name[MAX_LENGTH];
    int line;
} tokenDesc;

int line;

#endif