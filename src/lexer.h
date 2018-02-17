#include <stdio.h>
#include "langDef.h"

#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

tokenDesc getToken(FILE *fp);
char *getTokenFromId(int id,char * name);

int error_in_lexer;

#endif
