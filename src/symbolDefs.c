#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <ctype.h>

#include "symbolDefs.h"

int isEpsilon(SymbolDef * symbol){
	if(!symbol)
		return 0;
	if(strcmp(symbol->value,EPSILON)==0)
		return 1;
	else
		return 0;
}

int isEndSymbol(SymbolDef * symbol){
	if(!symbol)
		return 0;
	if(strcmp(symbol->value,ENDSYMBOL)==0)
		return 1;
	else
		return 0;
}

int isTerminal(SymbolDef * symbol){
	int i=0;
	
	if(isEpsilon(symbol) || isEndSymbol(symbol))
		return 1;

	while(symbol->value[i]){
		if(!isupper(symbol->value[i]))
			return 0;
		i++;
	}
	return 1;
}


int hasEpsilon(SymbolList * symbols){
	SymbolList * temp = symbols;
	
	while(temp!=NULL){
		if(isEpsilon(temp->symbol)){
			return 1;
		}
		temp=temp->next;
	}
	return 0;
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

SymbolDef * makeSymbol(char * str){
	SymbolDef* new_symbol = (SymbolDef*) malloc(sizeof(SymbolDef));
	strcpy(new_symbol->value,str);
	if(isTerminal(new_symbol)){
		new_symbol->isTerminal=1;
	}
	else
		new_symbol->isTerminal=0;
	new_symbol->rules = NULL;
	new_symbol->first = NULL;
	new_symbol->follow = NULL;
	return new_symbol;
}

SymbolDef *  insertSymbolFromToken(SymbolList ** symbols,char * str){
	
	return insertSymbol(symbols,makeSymbol(str));
	
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


SymbolList *getRuleFromIndex(SymbolDef *symbol,int ruleNo){
	RuleList *rules = symbol->rules;
	int rNo = 0;
	while(rules!=NULL){
		if(rNo == ruleNo)
			return rules->rule_tokens;
		rules=rules->next;
		rNo++;
	}

	return NULL;
}

int checkInList(SymbolList *symbols,SymbolDef *symbol){
	SymbolList *temp =  symbols;
	while(temp!=NULL){
		if(temp->symbol==symbol)
			return 1;
		temp=temp->next;
	}
	return 0;
}


void loadGrammar(char const *f_name){
	if(symbols!=NULL)
		return;
	FILE * fp;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	SymbolDef * symbol;
	fp = fopen(f_name,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}
	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s ===> %[^\n\t]",leftSide,rightSide);
		symbol = getSymbolIndex(&symbols,leftSide);
		if(symbol==NULL){
			symbol = insertSymbolFromToken(&symbols,leftSide);
		}
		insertRule(&symbol,rightSide);
	}
	

	fclose(fp);
}


void loadFirst(char const *first_file){
	FILE * fp;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	fp = fopen(first_file,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	SymbolList * temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==1){
			SymbolList *temp2 = NULL;
			insertSymbol(&temp2,temp->symbol);
			mergeList(&temp->symbol->first,temp2,1);
		}
		temp=temp->next;
	}

	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s --> %[^\n\t]",leftSide,rightSide);
		SymbolDef * symbol = getSymbolIndex(&symbols,leftSide);
		char * token = strtok(rightSide , ", ");
		while(token!=NULL){
	    	SymbolDef * symbol2 = getSymbolIndex(&symbols,token);
	        insertSymbol(&symbol->first,symbol2);
	        token = strtok(NULL, ", ");
	    }
	}

}

void loadFollow(char const *follow_file){
	FILE * fp;
	char leftSide[100],rightSide[200];
	char *line = NULL;
	size_t len=0;
	fp = fopen(follow_file,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s --> %[^\n\t]",leftSide,rightSide);
		SymbolDef * symbol = getSymbolIndex(&symbols,leftSide);
		char * token = strtok(rightSide , ", ");
		while(token!=NULL){
	    	SymbolDef * symbol2 = getSymbolIndex(&symbols,token);
	        insertSymbol(&symbol->follow,symbol2);
	        token = strtok(NULL, ", ");
	    }
	}

	fclose(fp);
	
}
