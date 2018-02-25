

#ifndef SYMBOLSTACK_H_INCLUDED
#define SYMBOLSTACK_H_INCLUDED

#include "symbolDef.h"

typedef struct SymbolStack SymbolStack;

struct SymbolStack{
	SymbolDef * symbol;
	struct SymbolStack *next;
};

int isEmpty(SymbolStack  *root);
void printStack(SymbolStack *root);
int getSize(SymbolStack  *root);
void push(SymbolStack **root,SymbolDef * symbol);
SymbolDef * pop(SymbolStack **root);
void pushAll(SymbolStack **root,SymbolList *symbols);

#endif