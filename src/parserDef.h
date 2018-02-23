
#ifndef PARSERDEF_H_INCLUDED
#define PARSERDEF_H_INCLUDED


typedef struct ParseTable ParseTable;

struct ParseTable {
	SymbolDef * symbol;
	RuleList *rule;
};

ParseTable ptable[50][50];

#endif