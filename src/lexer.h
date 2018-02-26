#include <stdio.h>
#include "langDef.h"

#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

tokenDesc getToken(FILE *fp,char **fileBuff,char *lexeme,int *begin);
char *getTokenFromId(int id,char * name);
void removeComments(char *testFile,char *cleanFile);


#endif
