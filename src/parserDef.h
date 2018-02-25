
#ifndef PARSERDEF_H_INCLUDED
#define PARSERDEF_H_INCLUDED


#include "symbolDef.h"
#include "langDef.h"

typedef struct ParseTable ParseTable;

struct ParseTable {
	SymbolDef * symbol;
	int ruleNo;
};

ParseTable ptable[45][45];

typedef struct ParseTree ParseTree;

struct ParseTree {
	SymbolDef * symbol;
	tokenDesc token;
	struct ParseTree *firstChild;
	struct ParseTree *sibling;
	struct ParseTree *parent;
};

// ParseTree pTree;

ParseTree *addChild(ParseTree *root,SymbolDef *symbol);
void addChildren(ParseTree **root,SymbolList *symbols);
void printTree(ParseTree *root,FILE *fp);
#endif