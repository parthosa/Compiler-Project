
#ifndef PARSERDEF_H_INCLUDED
#define PARSERDEF_H_INCLUDED


typedef struct ParseTable ParseTable;

struct ParseTable {
	SymbolDef * symbol;
	int ruleNo;
};

ParseTable ptable[50][50];

// typedef struct ParseTree ParseTree;

// struct ParseTree {
// 	SymbolDef * symbol;
// 	struct ParseTree *;
// };

#endif