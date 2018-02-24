
#ifndef PARSERDEF_H_INCLUDED
#define PARSERDEF_H_INCLUDED


typedef struct ParseTable ParseTable;

struct ParseTable {
	SymbolDef * symbol;
	int ruleNo;
};

ParseTable ptable[45][45];

// typedef struct ParseTree ParseTree;

// struct ParseTree {
// 	SymbolDef * symbol;
// 	struct ParseTree *;
// };

#endif