/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/


#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED


#include "symbolStack.h"
#include "parserDef.h"

void createParseTree(FILE *fp);
void saveParseTable(char const *f_name);
void createParseTable();
void saveParseTree(ParseTree *parseTree,FILE *fp);

ParseTree *pTree,*pTreeHead;
SymbolStack *symStack;
SymbolList *synchronizationSet;

#endif