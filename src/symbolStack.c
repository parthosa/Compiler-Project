#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "symbolStack.h"


SymbolStack *newSymbol(SymbolDef * symbol){
	SymbolStack *newSym = (SymbolStack*) malloc(sizeof(SymbolStack));
	newSym->symbol=symbol;
	newSym->next = NULL;
	return newSym;
}

int isEmpty(SymbolStack  *root)
{
	if(root==NULL || isEndSymbol(root->symbol))
		return 1;
    return 0;;
}

int getSize(SymbolStack  *root){
	SymbolStack *temp = root;
	int sz=0;
	while(temp!=NULL){
		sz++;
		temp=temp->next;
	}
	return sz;
}

void printStack(SymbolStack *root){
	SymbolStack *tmp = root;
	printf("\tStack\n");
	while(tmp!=NULL){
		printf("\t%s\n",tmp->symbol->value);
		tmp=tmp->next;
	}
	printf("\n");
}

void push(SymbolStack **root,SymbolDef * symbol){
	SymbolStack *newSym = newSymbol(symbol);
	newSym->next = *root;
	*root = newSym;
}

SymbolDef *pop(SymbolStack **root){
	if(*root==NULL)
		return NULL;

	SymbolStack *temp = *root;
	SymbolDef *tmp = temp->symbol;
	*root = (*root) ->next;
	free(temp);
	return tmp;
}

void pushAll(SymbolStack **root,SymbolList *symbols){
	SymbolList *temp = symbols;
	while(temp->next!=NULL){
		temp=temp->next;
	}

	while(temp!=NULL){
		if(!isEpsilon(temp->symbol)){

			push(root,temp->symbol);
		}
		temp=temp->prev;
	}
}