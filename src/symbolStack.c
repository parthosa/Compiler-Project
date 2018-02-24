#include <stdio.h>
#include <stdlib.h>
#include "symbolStack.h"

SymbolStack *newSymbol(SymbolDef *symbol){
	SymbolStack *newSym = (SymbolStack*) malloc(sizeof(SymbolStack));
	newSym->symbol = symbol;
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
		if(temp==NULL)
			return sz;
	}
	return sz;
}

void printStack(SymbolStack *root){
	SymbolStack *tmp = root;
	printf("\nstack\n****\n");
	while(tmp!=NULL){
		// printf("%s at %p\n",tmp->symbol->value,tmp->symbol);
		printf("y %p ",tmp);
		fflush(stdout);
		printf("sym %p ",tmp->symbol);
		fflush(stdout);
		printf("v: %s\n",tmp->symbol->value);
		tmp=tmp->next;
	}
	printf("y %p\n", tmp);
	printf("****\n");
}

void push(SymbolStack **root,SymbolDef *symbol){
	// printf("Pushed: %s\n",symbol->value);
	SymbolStack *newSym = newSymbol(symbol);
	newSym->next = *root;
	*root = newSym;
	// printf("New Top : %p and sym: %s\n",(*root),(*root)->symbol->value);
}

SymbolDef *pop(SymbolStack **root){
	if(*root==NULL)
		return NULL;

	SymbolStack *temp = *root;
	SymbolDef *temp2 = temp->symbol;

	printf("Popped: %s n next : %p\n",temp2->value,((*root)->next->symbol));
	*root = (*root) ->next;
	printf("New Top : %p and sym: %s\n",(*root),(*root)->symbol->value);
	free(temp);
	return temp2;
}

void pushAll(SymbolStack **root,SymbolList *symbols){
	SymbolList *temp = symbols;
	while(temp->next!=NULL){
		// push(root,temp->symbol);
		temp=temp->next;
	}

	while(temp!=NULL){
		if(!isEpsilon(temp->symbol)){
			push(root,temp->symbol);
		}
		temp=temp->prev;
	}
}