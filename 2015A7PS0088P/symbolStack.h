/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/


#ifndef SYMBOLSTACK_H_INCLUDED
#define SYMBOLSTACK_H_INCLUDED

#include "symbolDef.h"

typedef struct SymbolStack SymbolStack;

/*
* Data Structure to store the stack symbol 
*/
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