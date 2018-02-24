#ifndef SYMBOLDEF_H_INCLUDED
#define SYMBOLDEF_H_INCLUDED


#define MAX_SYMBOLS 50
#define EPSILON "_eps_" 
#define ENDMARKER "$" 

typedef struct SymbolDef SymbolDef;

struct SymbolDef{
	int isTerminal;
	char value[100];
	struct RuleList *rules;
	struct SymbolList *first;
	struct SymbolList *follow;
};


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


int isEpsilon(SymbolDef * symbol);
int isEndSymbol(SymbolDef * symbol);
int isTerminal(SymbolDef * symbol);
SymbolDef * getSymbolIndex(SymbolList ** symbols,char * sym_string);
SymbolDef * insertSymbol(SymbolList ** symbols,SymbolDef *new_symbol);
SymbolDef * makeSymbol(char * str);
SymbolDef *  insertSymbolFromToken(SymbolList ** symbols,char * str);
void insertRule(SymbolDef **symbol,char * rightSide);
int sizeOfList(SymbolList * symbols);
int mergeList(SymbolList ** first_list, SymbolList * first,int includeEps);
void printSymbolList(SymbolList * symbols,FILE *fp);
int hasEpsilon(SymbolList * symbols);
int allEpsilon(SymbolList * begin,SymbolList * end);
void printSymbolList(SymbolList * symbols,FILE *fp);
SymbolList *getRuleFromIndex(SymbolDef *symbol,int ruleNo);

#endif
