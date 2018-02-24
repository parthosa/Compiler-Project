#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parserDef.h"

ParseTree *newNode(SymbolDef * symbol){
	ParseTree *newSym = (ParseTree*) malloc(sizeof(ParseTree));
	newSym->symbol=symbol;
	newSym->firstChild = NULL;
	newSym->parent = NULL;
	newSym->sibling = NULL;
	return newSym;
}


ParseTree *addChild(ParseTree *root,SymbolDef *symbol){
	ParseTree *newSym;
	newSym = newNode(symbol);
	newSym->parent = root;
	return newSym;
}

void addChildren(ParseTree **root,SymbolList *symbols){
	SymbolList *temp = symbols;
	ParseTree *newSym,*tmpSym,*headSym;

	if(temp==NULL)
		return;

	newSym = addChild(*root,temp->symbol);
	tmpSym = newSym;
	headSym = newSym;
	(*root)->firstChild = newSym;
	temp=temp->next;
	
	while(temp!=NULL){
		newSym = addChild(*root,temp->symbol);
		tmpSym->sibling = newSym;
		tmpSym = newSym;
		temp=temp->next;
	}

	newSym->sibling = NULL;
	*root = headSym;
}


void printTree(ParseTree *root){
	if(root==NULL)
		return;

	if(root->symbol->isTerminal==1){
		printf("%s ",root->symbol->value);
		return;
	}

	ParseTree *child = root->firstChild;
	while(child!=NULL){
		printTree(child);
		child=child->sibling;
	}
	return;

}