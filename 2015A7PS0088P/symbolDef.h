/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/

#ifndef SYMBOLDEFS_H_INCLUDED
#define SYMBOLDEFS_H_INCLUDED


#define MAX_SYMBOLS 50
#define EPSILON "_eps_" 
#define ENDSYMBOL "$" 
#define MAINFUNCTION "<mainFunction>" 

typedef struct SymbolDef SymbolDef;

/*
* Data Structure to store the symbol and its rules list,follow and first list
*/
struct SymbolDef{
	int isTerminal;
	char value[30];
	struct RuleList *rules;
	struct SymbolList *first;
	struct SymbolList *follow;
};


typedef struct RuleList RuleList;

/*
* Data Structure to store the rule tokens
*/
struct RuleList {
	struct SymbolList * rule_tokens;
	struct RuleList *next;
};


typedef struct SymbolList SymbolList;

/*
* Data Structure to store the symbol list
*/
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
void emptyList(SymbolList ** list);
int mergeList(SymbolList ** first_list, SymbolList * first,int includeEps);
void printSymbolList(SymbolList * symbols,FILE *fp);
int hasEpsilon(SymbolList * symbols);
int allEpsilon(SymbolList * begin,SymbolList * end);
void printSymbolList(SymbolList * symbols,FILE *fp);
SymbolList *getRuleFromIndex(SymbolDef *symbol,int ruleNo);
int checkInList(SymbolList *symbols,SymbolDef *symbol);
void loadGrammar(char const *f_name);
void loadFirst(char const *first_file);
void loadFollow(char const *follow_file);

#endif
