/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/

#ifndef PARSERDEF_H_INCLUDED
#define PARSERDEF_H_INCLUDED


#include "symbolDef.h"
#include "langDef.h"

typedef struct ParseTable ParseTable;

/*
* Data Structure to store the parse table
*/
struct ParseTable {
	SymbolDef * symbol;
	int ruleNo;
};

ParseTable ptable[45][45];

typedef struct ParseTree ParseTree;

/*
* Data Structure to store the parse tree
*/
struct ParseTree {
	SymbolDef * symbol;
	TokenInfo token;
	struct ParseTree *firstChild;
	struct ParseTree *sibling;
	struct ParseTree *parent;
};


ParseTree *addChild(ParseTree *root,SymbolDef *symbol);
void addChildren(ParseTree **root,SymbolList *symbols);
void printTree(ParseTree *root,FILE *fp);
#endif