#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "symbolDef.h"

bool isEpsilon(SymbolDef * symbol){
	if(strcmp(symbol->value,EPSILON)==0)
		return true;
	else
		return false;
}

bool isTerminal(SymbolDef * symbol){
	int i=0;
	
	if(isEpsilon(symbol))
		return true;

	while(symbol->value[i]){
		if(!isupper(symbol->value[i]))
			return false;
		i++;
	}
	return true;
}


bool hasEpsilon(SymbolList * symbols){
	SymbolList * temp = symbols;
	
	while(temp!=NULL){
		if(isEpsilon(temp->symbol)){
			return true;
		}
		temp=temp->next;
	}
	return false;
}

int allEpsilon(SymbolList * begin,SymbolList * end){
	SymbolList * temp = begin;
	int flg;
	while(temp!=end){
		flg = 0;
		if(temp->symbol->isTerminal==0){
			RuleList * temp2 = temp->symbol->rules;
			while(temp2!=NULL){
				if(hasEpsilon(temp2->rule_tokens)){
					flg=1;
				}
				temp2=temp2->next;
			}
		}else if(!isEpsilon(temp->symbol))
			flg = 0;

		if(flg==0)
			return 0;
		temp=temp->next;
	}
	return 1;
}


SymbolDef * getSymbolIndex(SymbolList ** symbols,char * sym_string){
	SymbolList * temp = *symbols;
	while(temp!=NULL){
		if(strcmp(temp->symbol->value,sym_string)==0)
			return temp->symbol;
		temp = temp->next;
	}
	return NULL;
}

SymbolDef * insertSymbol(SymbolList ** symbols,SymbolDef *new_symbol){
	SymbolList *new_symbol_ls  = (SymbolList*) malloc(sizeof(SymbolList));
	SymbolList *last = *symbols;
	new_symbol_ls->symbol = new_symbol;
	new_symbol_ls->next = NULL;
	new_symbol_ls->prev = NULL;

	if (*symbols == NULL)
    {
       *symbols = new_symbol_ls;
       return new_symbol;
    } 

    
    while (last->next != NULL){
        last = last->next;
    }
    last->next = new_symbol_ls;
    new_symbol_ls->prev = last;
	return new_symbol;
}

SymbolDef *  insertSymbolFromToken(SymbolList ** symbols,char * leftSide){
	SymbolDef* new_symbol = (SymbolDef*) malloc(sizeof(SymbolDef));
	strcpy(new_symbol->value,leftSide);
	if(isTerminal(new_symbol)){
		new_symbol->isTerminal=1;
	}
	else
		new_symbol->isTerminal=0;
	new_symbol->rules = NULL;
	new_symbol->first = NULL;
	new_symbol->follow = NULL;

	return insertSymbol(symbols,new_symbol);
	
}

void insertRule(SymbolDef **symbol,char * rightSide){
	RuleList* new_rule  = (RuleList*) malloc(sizeof(RuleList));
	RuleList *last = (*symbol)->rules;
	char * token = strtok(rightSide , " ");

	// printf("u of: %s rhs: %s\n",(*symbol)->value,rightSide);
    while(token!=NULL){
    	SymbolDef * symbol2 = getSymbolIndex(&symbols,token);
		if(symbol2==NULL){
			symbol2 = insertSymbolFromToken(&symbols,token);
		}
        insertSymbol(&new_rule->rule_tokens,symbol2);
        token = strtok(NULL, " ");
    }

	new_rule->next = NULL;

	if ((*symbol)->rules == NULL)
    {
       (*symbol)->rules = new_rule;
       return;
    } 

    while (last->next != NULL)
        last = last->next;

    last->next = new_rule;
}


int sizeOfList(SymbolList * symbols){
	SymbolList * temp = symbols;
	int count=0;
	while(temp!=NULL){
		count++;
		temp=temp->next;
	}
	return count;
}

int mergeList(SymbolList ** first_list, SymbolList * first,int includeEps){
	SymbolList * temp = first;
	int init_sz = sizeOfList(*first_list);
	while(temp!=NULL){
		if(includeEps == 0 && isEpsilon(temp->symbol)){
			temp=temp->next;
			continue;
		}
		if(getSymbolIndex(first_list,temp->symbol->value)==NULL)
			insertSymbol(first_list,temp->symbol);
		temp=temp->next;
	}
	int final_sz = sizeOfList(*first_list);
	return final_sz != init_sz;
}


void printSymbolList(SymbolList * symbols,FILE *fp){
	SymbolList * temp2 = symbols;
	while(temp2!=NULL){
		if(fp)
			fprintf(fp,"%s ",temp2->symbol->value);
		else
			printf("%s ",temp2->symbol->value );

		// fflush(stdout);
		temp2=temp2->next;
	}
}

