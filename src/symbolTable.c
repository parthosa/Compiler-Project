#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "symbolTable.h"

int offset;

SymbolTable* newTable(int scopeLevel){
	SymbolTable *newT = (SymbolTable*) malloc(sizeof(SymbolTable));
	newT->scopeLevel = scopeLevel;
	newT->symbolsHead = NULL;
	newT->parent = NULL;
	newT->firstChild = NULL;
	newT->sibling = NULL;

	return newT;
}

SymbolTableEntry* newEntry(char *value,char *type,int width,int lineNo){
	SymbolTableEntry *newSymEntry = (SymbolTableEntry*) malloc(sizeof(SymbolTableEntry));
	strcpy(newSymEntry->value,value);
	strcpy(newSymEntry->type,type);
	// newSymEntry->val = val;
	newSymEntry->width = width;
	newSymEntry->lineNo = lineNo;

	return newSymEntry;
}


SymbolTableEntry* lookup(SymbolTable* sTable,char *value){

	SymbolTableEntry *tempHead;
	while (sTable!=NULL){
		tempHead = sTable->symbolsHead;
		while(tempHead!=NULL){
			if(strcmp(tempHead->value,value) == 0)
				return tempHead;
			tempHead = tempHead->next;
		}
		sTable = sTable->parent;
	}

	return NULL;
}

void printSymbolTable(SymbolTable *sTable){
	if(sTable == NULL)
		return;

	printf("\n***********************************************\n");	
	SymbolTableEntry *symHeadTemp = sTable->symbolsHead;
	while(symHeadTemp!=NULL){
		printf("%-5d%-25s%-10s%-5d\n",symHeadTemp->lineNo,symHeadTemp->value,symHeadTemp->type,symHeadTemp->width);
		symHeadTemp=symHeadTemp->next;
	}

	SymbolTable *child = sTable->firstChild;

	while(child!=NULL){
		printSymbolTable(child);
		child=child->sibling;
	}

	return;
}


void insertEntry(SymbolTableEntry **symHead,char *type,ASTNode *id){
	int width = 0,offset = 0;
	// union initVal val;
	if(strcmp(type,"INT") == 0){
		width = 2;
	}
	if(strcmp(type,"REAL") == 0){
		width = 4;
	}
	if(strcmp(type,"STRING") == 0){
		width = strlen(id->token.name);
	}
	// MATRIX
	SymbolTableEntry *newSym = newEntry(id->token.name,type,width,id->token.line);

	
	if((*symHead)==NULL){
		*symHead = newSym;

	}else{
		SymbolTableEntry *symHeadTemp = *symHead;
		while(symHeadTemp->next!=NULL)
			symHeadTemp = symHeadTemp->next;

		symHeadTemp->next = newSym;
		newSym->prev = symHeadTemp;
	}


}

void insertList(SymbolTableEntry **symHead,char *type,ASTNode *idList){

	ASTNode *idListTemp = idList;
	while(idListTemp!=NULL){
		insertEntry(symHead,type,idListTemp);
		idListTemp = idListTemp->sibling;
	}
	
}

void insertTable(SymbolTable *parent,SymbolTable *child){
	SymbolTable *childTemp = parent->firstChild;
	if(childTemp==NULL){
		parent->firstChild = child;
	}else{
		while(childTemp->sibling!=NULL){
			childTemp=childTemp->sibling;
		}
		childTemp->sibling = child;
	}

	child->parent = parent;
}


void buildSymbolTableRec(ASTNode *root,SymbolTable *sTable){
	SymbolTable *sTableCurr = sTable;
    if(root==NULL)
        return;


    ASTNode *child = root->firstChild;
    while(child!=NULL){
    	if(strcmp(child->label,"DECLARATION")==0){
    		char *type = child->firstChild->label;
    		ASTNode *idList = child->firstChild->sibling;
			insertList(&(sTableCurr->symbolsHead),type,idList);
    	}

    	if(strcmp(child->label,"FUNCTIONDEF")==0){
    		ASTNode *funId = child->firstChild;
    		insertEntry(&(sTableCurr->symbolsHead),"FUNCTION",funId);
    		SymbolTable *currTemp = newTable((sTableCurr->scopeLevel)+1);
    		insertTable(sTableCurr,currTemp);
    		buildSymbolTableRec(child,currTemp);
    	}

    	if(strcmp(child->label,"IFSTATEMENT")==0){
    		buildSymbolTableRec(child,sTableCurr);
    	}

    	if((strcmp(child->label,"IFSTMTS") == 0) || (strcmp(child->label,"ELSESTMTS") == 0)){
    		SymbolTable *currTemp = newTable((sTableCurr->scopeLevel)+1);
    		insertTable(sTableCurr,currTemp);
    		buildSymbolTableRec(child,currTemp);
    	}

        child=child->sibling;
    }
        
    return;
}

void buildSymbolTable(ASTNode *root,int scopeLevel){
	symTable = newTable(scopeLevel);
	buildSymbolTableRec(root,symTable);
	printf("%-5s%-25s%-10s%-5s\n","Line","Lexeme","Type","Width");
	printSymbolTable(symTable);
}


