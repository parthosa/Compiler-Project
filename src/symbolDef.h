#ifndef SYMBOLDEF_H_INCLUDED
#define SYMBOLDEF_H_INCLUDED


#define MAX_SYMBOLS 50
#define EPSILON "_eps_" 

typedef struct {
	int isTerminal;
	char value[100];
	struct RuleList *rules;
	struct SymbolList *first;
	struct SymbolList *follow;
} SymbolDef ;


typedef struct RuleList RuleList;

struct RuleList {
	struct SymbolList * rule_tokens;
	struct RuleList *next;
};


typedef struct SymbolList SymbolList;

struct SymbolList {
	SymbolDef * symbol;
	struct SymbolList *next;
	struct SymbolList *prev;
};

SymbolList * symbols;


bool isEpsilon(SymbolDef * symbol);
bool isTerminal(SymbolDef * symbol);
SymbolDef * getSymbolIndex(SymbolList ** symbols,char * sym_string);
SymbolDef * insertSymbol(SymbolList ** symbols,SymbolDef *new_symbol);
SymbolDef *  insertSymbolFromToken(SymbolList ** symbols,char * leftSide);
void insertRule(SymbolDef **symbol,char * rightSide);
int sizeOfList(SymbolList * symbols);
int mergeList(SymbolList ** first_list, SymbolList * first,int includeEps);
void printSymbolList(SymbolList * symbols,FILE *fp);
bool hasEpsilon(SymbolList * symbols);
int allEpsilon(SymbolList * begin,SymbolList * end);

#endif
