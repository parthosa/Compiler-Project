#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SYMBOLS 50

typedef struct {
	int isTerminal;
	char value[100];
	struct RuleList *rules;
	struct SymbolList *first;
	struct SymbolList *follow;
} SymbolDef ;


typedef struct RuleList RuleList;

struct RuleList {
	struct SymbolList * rule_tokens;
	struct RuleList *next;
};


typedef struct SymbolList SymbolList;

struct SymbolList {
	SymbolDef * symbol;
	struct SymbolList *next;
};

bool isEpsilon(SymbolDef * symbol){
	if(strcmp(symbol->value,"_eps_")==0)
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


SymbolList * symbols;

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

	if (*symbols == NULL)
    {
       *symbols = new_symbol_ls;
       return new_symbol;
    } 

    
    while (last->next != NULL){
        last = last->next;
    }
    last->next = new_symbol_ls;
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

void viewSymbols(){
	SymbolList * temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==0){
			printf("%s->",temp->symbol->value);
			RuleList * temp2 = temp->symbol->rules;
			while(temp2!=NULL){
				printf("{");
				SymbolList * temp3 = temp2->rule_tokens;
				while(temp3!=NULL){
					printf("%s,",temp3->symbol->value);
					temp3 = temp3->next;
				}
				printf("},");
				temp2 = temp2->next;
			}
			printf("\n");
		}
		temp = temp->next;
	}
}



void parseGrammar(char const *f_name){
	FILE * fp;
	symbols = NULL;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	fp = fopen(f_name,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s ===> %[^\n\t]",leftSide,rightSide);
		SymbolDef * symbol = getSymbolIndex(&symbols,leftSide);
		if(symbol==NULL){
			symbol = insertSymbolFromToken(&symbols,leftSide);
		}
		insertRule(&symbol,rightSide);
	}
	

	fclose(fp);
}


void mergeList(SymbolList ** first_list, SymbolList * first){
	SymbolList * temp = first;
	while(temp!=NULL){
		if(getSymbolIndex(first_list,temp->symbol->value)==NULL)
			insertSymbol(first_list,temp->symbol);
		temp=temp->next;
	}
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

SymbolList * findFirst_R(SymbolDef *symbol){
	if(symbol->first!=NULL){
		return symbol->first;
	}
	else{
		SymbolList *first_list = NULL;
	
		if(symbol->isTerminal==1){
			insertSymbol(&first_list,symbol);
			return first_list;
		}

		RuleList * temp2 = symbol->rules;
		while(temp2!=NULL){
			SymbolList * temp3 = temp2->rule_tokens;
			SymbolList * first = findFirst_R(temp3->symbol);
			if(temp3->symbol->first==NULL)
				mergeList(&temp3->symbol->first,first);
			mergeList(&first_list,first);
			while(hasEpsilon(first)){
				temp3 = temp3->next;
				if(temp3==NULL)
					break;
				SymbolList * first = findFirst_R(temp3->symbol);
				if(temp3->symbol->first==NULL)
					mergeList(&temp3->symbol->first,first);
				mergeList(&first_list,first);
			}
			temp2 = temp2->next;
		}
		
		return first_list;
		
	}
}



void findFirst(){
	SymbolList * temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==0){
			if(temp->symbol->first==NULL){
				SymbolList * first = findFirst_R(temp->symbol);
				mergeList(&temp->symbol->first,first);
			}
		}
		temp=temp->next;
	}

	// writing to a file
	FILE *fptr = fopen("language/first.txt","w");

	temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==0){
			fprintf(fptr,"%s -> ",temp->symbol->value);
			SymbolList * temp2 = temp->symbol->first;
			while(temp2->next!=NULL){
				fprintf(fptr,"%s, ",temp2->symbol->value);
				temp2=temp2->next;
			}
			fprintf(fptr,"%s",temp2->symbol->value);
			fprintf(fptr,"\n");
		}
		temp=temp->next;
	}
	fclose(fptr);

}

int main(int argc, char const *argv[])
{
	parseGrammar(argv[1]);
	findFirst();
	return 0;
}