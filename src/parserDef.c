/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parserDef.h"

ParseTree *newNode(SymbolDef * symbol){
	ParseTree *newSym = (ParseTree*) malloc(sizeof(ParseTree));
	newSym->symbol=symbol;
	// newSym->token = NULL;
	// memset(&newSym->token, '\0', sizeof(newSym->token));
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




// char *getNameFromToken(TokenInfo token){
// 	// if(token==0)
// 	// 	return "---";
// 	// else
// 	return token.name;
// }

char *getParentValue(ParseTree *node){
	if(node->parent==NULL)
		return "ROOT";
	else
		return node->parent->symbol->value;
}


char *isLeaf(ParseTree *node){
	if(node->symbol->isTerminal)
		return "Yes";
	else
		return "No";
}



void printTree(ParseTree *root,FILE *fp){
	if(root==NULL)
		return;

	if(root->symbol->isTerminal==1){
		if(!isEpsilon(root->symbol)){
			if(strcmp(root->symbol->value,"NUM")==0 || strcmp(root->symbol->value,"RNUM")==0)
				fprintf(fp,"%-25s%-10d%-20s%-20s%-30s%-10s%-30s\n",root->token.name,root->token.line,root->symbol->value,root->token.name,getParentValue(root),isLeaf(root),root->symbol->value);
			else
				fprintf(fp,"%-25s%-10d%-20s%-20s%-30s%-10s%-30s\n",root->token.name,root->token.line,root->symbol->value,"---",getParentValue(root),isLeaf(root),root->symbol->value);
		}
		else
			fprintf(fp,"%-25s%-10s%-20s%-20s%-30s%-10s%-30s\n","---","---",root->symbol->value,"---",getParentValue(root),isLeaf(root),root->symbol->value);
		return;
	}

	ParseTree *child = root->firstChild;
	printTree(child,fp);
	fprintf(fp,"%-25s%-10s%-20s%-20s%-30s%-10s%-30s\n","---","---","---","---",getParentValue(root),isLeaf(root),root->symbol->value);
	
	if(child){
		child=child->sibling;
		while(child!=NULL){
			printTree(child,fp);
			child=child->sibling;
		}
		
	}
	return;
}

