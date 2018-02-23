
#ifndef FIRSTANDFOLLOW_H_INCLUDED
#define FIRSTANDFOLLOW_H_INCLUDED


#include <stdbool.h>

bool isEpsilon(SymbolDef * symbol);
bool isTerminal(SymbolDef * symbol);
SymbolDef * getSymbolIndex(SymbolList ** symbols,char * sym_string);
SymbolDef * insertSymbol(SymbolList ** symbols,SymbolDef *new_symbol);
SymbolDef *  insertSymbolFromToken(SymbolList ** symbols,char * leftSide);
void insertRule(SymbolDef **symbol,char * rightSide);
void loadGrammar(char const *f_name);


#endif
