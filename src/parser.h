#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED


#include "symbolStack.h"
#include "parserDef.h"

void createParseTree(FILE *fp);
void saveParseTable(char const *f_name);
void createParseTable();
void saveParseTree(FILE *fp);

ParseTree *pTree,*pTreeHead;
SymbolStack *symStack;


#endif